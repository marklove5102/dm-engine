#include "dsutil.h"
//#include "MusicInterface.h"
#ifdef _DEBUG
#include "Global/new.h"
#define new DEBUG_NEW
#endif 
#include "Global/DebugTry.h"
#include <string>
#include "Audio.inl" 

#define INC_STEP		3000

static char		a_szWorkingPath[MAX_PATH];		//工作路径“..\\data\\music”

//-----------------------------------------------------------------------------
// Name: CWaveFile::CWaveFile()
// Desc: Constructs the class.  Call Open() to open a wave file for reading.  
//       Then call Read() as needed.  Calling the destructor or Close() 
//       will close the file.  
//-----------------------------------------------------------------------------
CWaveFile::CWaveFile()
{
    m_pwfx				= NULL;
    m_hmmio				= NULL;
    m_pResourceBuffer	= NULL;
    m_dwSize			= 0;
    m_bIsReadingFromMemory = FALSE;
}

//-----------------------------------------------------------------------------
// Name: CWaveFile::~CWaveFile()
// Desc: Destructs the class
//-----------------------------------------------------------------------------
CWaveFile::~CWaveFile()
{
    Close();

    if( !m_bIsReadingFromMemory )
        SAFE_DELETE_ARRAY( m_pwfx );
}

//-----------------------------------------------------------------------------
// Name: CWaveFile::Open()
// Desc: Opens a wave file for reading
//-----------------------------------------------------------------------------
HRESULT CWaveFile::Open( LPTSTR strFileName, WAVEFORMATEX* pwfx, DWORD dwFlags )
{
    HRESULT hr;

	char      cFile[MAX_PATH] = {0};
	sprintf(cFile,"%s%s",a_szWorkingPath,strFileName);

    m_dwFlags = dwFlags;
    m_bIsReadingFromMemory = FALSE;
	//strFileName = NULL;
    if( m_dwFlags == WAVEFILE_READ )
	{
        if( strFileName == NULL )
            return E_INVALIDARG;
        SAFE_DELETE_ARRAY( m_pwfx );

		//使用stdio文件系统
		//!!!BUGBUGBUG,if file not exist,buffer will not be freed??
 //       m_hmmio = mmioOpen( cFile, NULL, MMIO_ALLOCBUF | MMIO_READ );
 //       if( NULL == m_hmmio )
        
		long	size	= 0;
		if(m_pResourceBuffer)
		{
			delete [] m_pResourceBuffer;
			m_pResourceBuffer = NULL;
		}

		if( (size = LoadFile(cFile,&m_pResourceBuffer,false) ) > 0  && m_pResourceBuffer)
		{
			MMIOINFO mmioInfo;
			ZeroMemory( &mmioInfo, sizeof(mmioInfo) );
			mmioInfo.fccIOProc = FOURCC_MEM;
			mmioInfo.cchBuffer = size;
			mmioInfo.pchBuffer = (CHAR*) m_pResourceBuffer;
			m_hmmio = mmioOpen( NULL, &mmioInfo, MMIO_READ );

			if( NULL == m_hmmio )
			{
				m_dwSize = 0;
				return E_FAIL;
			}
        }
		else
		{
			m_hmmio = NULL;
			m_dwSize = 0;
			return E_FAIL;
		}

        if( FAILED( hr = ReadMMIO() ) )
        {
            // ReadMMIO will fail if its an not a wave file
            mmioClose( m_hmmio, 0 );
            return E_FAIL;//DXTRACE_ERR_NOMSGBOX( TEXT("ReadMMIO"), hr );
        }

        if( FAILED( hr = ResetFile() ) )
            return E_FAIL;//DXTRACE_ERR( TEXT("ResetFile"), hr );

        // After the reset, the size of the wav file is m_ck.cksize so store it now
        m_dwSize = m_ck.cksize;
    }
    else
    {
        m_hmmio = mmioOpen( cFile, NULL, MMIO_ALLOCBUF  | 
                                                  MMIO_READWRITE | 
                                                  MMIO_CREATE );
        if( NULL == m_hmmio )
            return E_FAIL;//DXTRACE_ERR( TEXT("mmioOpen"), E_FAIL );

        if( FAILED( hr = WriteMMIO( pwfx ) ) )
        {
            mmioClose( m_hmmio, 0 );
            return E_FAIL;//DXTRACE_ERR( TEXT("WriteMMIO"), hr );
        }
                        
        if( FAILED( hr = ResetFile() ) )
            return E_FAIL;//DXTRACE_ERR( TEXT("ResetFile"), hr );
    }

    return hr;
}

//-----------------------------------------------------------------------------
// Name: CWaveFile::OpenFromMemory()
// Desc: copy data to CWaveFile member variable from memory
//-----------------------------------------------------------------------------
HRESULT CWaveFile::OpenFromMemory( BYTE* pbData, ULONG ulDataSize, 
                                   WAVEFORMATEX* pwfx, DWORD dwFlags )
{
    m_pwfx       = pwfx;
    m_ulDataSize = ulDataSize;
    m_pbData     = pbData;
    m_pbDataCur  = m_pbData;
    m_bIsReadingFromMemory = TRUE;
    
    if( dwFlags != WAVEFILE_READ )
        return E_NOTIMPL;       
    
    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: CWaveFile::Close()
// Desc: Closes the wave file 
//-----------------------------------------------------------------------------
HRESULT CWaveFile::Close()
{
    if( m_dwFlags == WAVEFILE_READ )
    {
		UINT fuClose = 0;
        MMRESULT res = 0;
		if(m_hmmio)
		{
			mmioClose( m_hmmio, fuClose );
		}
		if(res)
		{
#ifdef _DEBUG
			OutputDebugString("error :WaveFile::Close\n");
#endif
		}
        m_hmmio = NULL;

//		PkgFreeBuffer(m_pResourceBuffer);
		if(m_pResourceBuffer)
			delete [] m_pResourceBuffer;

    }
    else
    {
        m_mmioinfoOut.dwFlags |= MMIO_DIRTY;

        if( m_hmmio == NULL )
            return CO_E_NOTINITIALIZED;

        if( 0 != mmioSetInfo( m_hmmio, &m_mmioinfoOut, 0 ) )
            return E_FAIL;//DXTRACE_ERR( TEXT("mmioSetInfo"), E_FAIL );
    
        // Ascend the output file out of the 'data' chunk -- this will cause
        // the chunk size of the 'data' chunk to be written.
        if( 0 != mmioAscend( m_hmmio, &m_ck, 0 ) )
            return E_FAIL;//DXTRACE_ERR( TEXT("mmioAscend"), E_FAIL );
    
        // Do this here instead...
        if( 0 != mmioAscend( m_hmmio, &m_ckRiff, 0 ) )
            return E_FAIL;//DXTRACE_ERR( TEXT("mmioAscend"), E_FAIL );
        
        mmioSeek( m_hmmio, 0, SEEK_SET );

        if( 0 != (INT)mmioDescend( m_hmmio, &m_ckRiff, NULL, 0 ) )
            return E_FAIL;//DXTRACE_ERR( TEXT("mmioDescend"), E_FAIL );
    
        m_ck.ckid = mmioFOURCC('f', 'a', 'c', 't');

        if( 0 == mmioDescend( m_hmmio, &m_ck, &m_ckRiff, MMIO_FINDCHUNK ) ) 
        {
            DWORD dwSamples = 0;
            mmioWrite( m_hmmio, (HPSTR)&dwSamples, sizeof(DWORD) );
            mmioAscend( m_hmmio, &m_ck, 0 ); 
        }
    
        // Ascend the output file out of the 'RIFF' chunk -- this will cause
        // the chunk size of the 'RIFF' chunk to be written.
        if( 0 != mmioAscend( m_hmmio, &m_ckRiff, 0 ) )
            return E_FAIL;//DXTRACE_ERR( TEXT("mmioAscend"), E_FAIL );
    
        mmioClose( m_hmmio, 0 );
        m_hmmio = NULL;
    }

    return S_OK;
}



//-----------------------------------------------------------------------------
// Name: CWaveFile::ReadMMIO()
// Desc: Support function for reading from a multimedia I/O stream.
//       m_hmmio must be valid before calling.  This function uses it to
//       update m_ckRiff, and m_pwfx. 
//-----------------------------------------------------------------------------
HRESULT CWaveFile::ReadMMIO()
{
    MMCKINFO        ckIn;           // chunk info. for general use.
    PCMWAVEFORMAT   pcmWaveFormat;  // Temp PCM structure to load in.       

    m_pwfx = NULL;

	if (m_hmmio == NULL)
	{
		return E_FAIL;
	}

    if( ( 0 != mmioDescend( m_hmmio, &m_ckRiff, NULL, 0 ) ) )
        return E_FAIL;//DXTRACE_ERR( TEXT("mmioDescend"), E_FAIL );

    // Check to make sure this is a valid wave file
    if( (m_ckRiff.ckid != FOURCC_RIFF) ||
        (m_ckRiff.fccType != mmioFOURCC('W', 'A', 'V', 'E') ) )
        return E_FAIL;//DXTRACE_ERR_NOMSGBOX( TEXT("mmioFOURCC"), E_FAIL ); 

    // Search the input file for for the 'fmt ' chunk.
    ckIn.ckid = mmioFOURCC('f', 'm', 't', ' ');
    if( 0 != mmioDescend( m_hmmio, &ckIn, &m_ckRiff, MMIO_FINDCHUNK ) )
        return E_FAIL;//DXTRACE_ERR( TEXT("mmioDescend"), E_FAIL );

    // Expect the 'fmt' chunk to be at least as large as <PCMWAVEFORMAT>;
    // if there are extra parameters at the end, we'll ignore them
       if( ckIn.cksize < (LONG) sizeof(PCMWAVEFORMAT) )
           return E_FAIL;//DXTRACE_ERR( TEXT("sizeof(PCMWAVEFORMAT)"), E_FAIL );

    // Read the 'fmt ' chunk into <pcmWaveFormat>.
    if( mmioRead( m_hmmio, (HPSTR) &pcmWaveFormat, 
                  sizeof(pcmWaveFormat)) != sizeof(pcmWaveFormat) )
        return E_FAIL;//DXTRACE_ERR( TEXT("mmioRead"), E_FAIL );

    // Allocate the waveformatex, but if its not pcm format, read the next
    // word, and thats how many extra bytes to allocate.
    if( pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM )
    {
        m_pwfx = (WAVEFORMATEX*)new CHAR[ sizeof(WAVEFORMATEX) ];
        if( NULL == m_pwfx )
            return E_FAIL;//DXTRACE_ERR( TEXT("m_pwfx"), E_FAIL );

        // Copy the bytes from the pcm structure to the waveformatex structure
        memcpy( m_pwfx, &pcmWaveFormat, sizeof(pcmWaveFormat) );
        m_pwfx->cbSize = 0;
    }
    else
    {
        // Read in length of extra bytes.
        WORD cbExtraBytes = 0L;
        if( mmioRead( m_hmmio, (CHAR*)&cbExtraBytes, sizeof(WORD)) != sizeof(WORD) )
            return E_FAIL;//DXTRACE_ERR( TEXT("mmioRead"), E_FAIL );

        m_pwfx = (WAVEFORMATEX*)new CHAR[ sizeof(WAVEFORMATEX) + cbExtraBytes ];
        if( NULL == m_pwfx )
            return E_FAIL;//DXTRACE_ERR( TEXT("new"), E_FAIL );

        // Copy the bytes from the pcm structure to the waveformatex structure
        memcpy( m_pwfx, &pcmWaveFormat, sizeof(pcmWaveFormat) );
        m_pwfx->cbSize = cbExtraBytes;

        // Now, read those extra bytes into the structure, if cbExtraAlloc != 0.
        if( mmioRead( m_hmmio, (CHAR*)(((BYTE*)&(m_pwfx->cbSize))+sizeof(WORD)),
                      cbExtraBytes ) != cbExtraBytes )
        {
            SAFE_DELETE( m_pwfx );
            return E_FAIL;//DXTRACE_ERR( TEXT("mmioRead"), E_FAIL );
        }
    }

    // Ascend the input file out of the 'fmt ' chunk.
    if( 0 != mmioAscend( m_hmmio, &ckIn, 0 ) )
    {
        SAFE_DELETE( m_pwfx );
        return E_FAIL;//DXTRACE_ERR( TEXT("mmioAscend"), E_FAIL );
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::GetSize()
// Desc: Retuns the size of the read access wave file 
//-----------------------------------------------------------------------------
DWORD CWaveFile::GetSize()
{
    return m_dwSize;
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::ResetFile()
// Desc: Resets the internal m_ck pointer so reading starts from the 
//       beginning of the file again 
//-----------------------------------------------------------------------------
HRESULT CWaveFile::ResetFile()
{
    if( m_bIsReadingFromMemory )
    {
        m_pbDataCur = m_pbData;
    }
    else 
    {
        if( m_hmmio == NULL )
            return CO_E_NOTINITIALIZED;

        if( m_dwFlags == WAVEFILE_READ )
        {
            // Seek to the data
            if( -1 == mmioSeek( m_hmmio, m_ckRiff.dwDataOffset + sizeof(FOURCC),
                            SEEK_SET ) )
                return E_FAIL;//DXTRACE_ERR( TEXT("mmioSeek"), E_FAIL );

            // Search the input file for the 'data' chunk.
            m_ck.ckid = mmioFOURCC('d', 'a', 't', 'a');
            if( 0 != mmioDescend( m_hmmio, &m_ck, &m_ckRiff, MMIO_FINDCHUNK ) )
              return E_FAIL;//DXTRACE_ERR( TEXT("mmioDescend"), E_FAIL );
        }
        else
        {
            // Create the 'data' chunk that holds the waveform samples.  
            m_ck.ckid = mmioFOURCC('d', 'a', 't', 'a');
            m_ck.cksize = 0;

            if( 0 != mmioCreateChunk( m_hmmio, &m_ck, 0 ) ) 
                return E_FAIL;//DXTRACE_ERR( TEXT("mmioCreateChunk"), E_FAIL );

            if( 0 != mmioGetInfo( m_hmmio, &m_mmioinfoOut, 0 ) )
                return E_FAIL;//DXTRACE_ERR( TEXT("mmioGetInfo"), E_FAIL );
        }
    }
    
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::Read()
// Desc: Reads section of data from a wave file into pBuffer and returns 
//       how much read in pdwSizeRead, reading not more than dwSizeToRead.
//       This uses m_ck to determine where to start reading from.  So 
//       subsequent calls will be continue where the last left off unless 
//       Reset() is called.
//-----------------------------------------------------------------------------
HRESULT CWaveFile::Read( BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead )
{
    if( m_bIsReadingFromMemory )
    {
        if( m_pbDataCur == NULL )
            return CO_E_NOTINITIALIZED;
        if( pdwSizeRead != NULL )
            *pdwSizeRead = 0;

        if( (BYTE*)(m_pbDataCur + dwSizeToRead) > 
            (BYTE*)(m_pbData + m_ulDataSize) )
        {
            dwSizeToRead = m_ulDataSize - (DWORD)(m_pbDataCur - m_pbData);
        }
        
        CopyMemory( pBuffer, m_pbDataCur, dwSizeToRead );
        
        if( pdwSizeRead != NULL )
            *pdwSizeRead = dwSizeToRead;

        return S_OK;
    }
    else 
    {
        MMIOINFO mmioinfoIn; // current status of m_hmmio

        if( m_hmmio == NULL )
            return CO_E_NOTINITIALIZED;
        if( pBuffer == NULL || pdwSizeRead == NULL )
            return E_INVALIDARG;

        if( pdwSizeRead != NULL )
            *pdwSizeRead = 0;

        if( 0 != mmioGetInfo( m_hmmio, &mmioinfoIn, 0 ) )
            return E_FAIL;//DXTRACE_ERR( TEXT("mmioGetInfo"), E_FAIL );
                
        UINT cbDataIn = dwSizeToRead;
        if( cbDataIn > m_ck.cksize ) 
            cbDataIn = m_ck.cksize;       

        m_ck.cksize -= cbDataIn;
    
        for( DWORD cT = 0; cT < cbDataIn; cT++ )
        {
            // Copy the bytes from the io to the buffer.
            if( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead )
            {
                if( 0 != mmioAdvance( m_hmmio, &mmioinfoIn, MMIO_READ ) )
                    return E_FAIL;//DXTRACE_ERR( TEXT("mmioAdvance"), E_FAIL );

                if( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead )
                    return E_FAIL;//DXTRACE_ERR( TEXT("mmioinfoIn.pchNext"), E_FAIL );
            }

            // Actual copy.
            *((BYTE*)pBuffer+cT) = *((BYTE*)mmioinfoIn.pchNext);
            mmioinfoIn.pchNext++;
        }

        if( 0 != mmioSetInfo( m_hmmio, &mmioinfoIn, 0 ) )
            return E_FAIL;//XTRACE_ERR( TEXT("mmioSetInfo"), E_FAIL );

        if( pdwSizeRead != NULL )
            *pdwSizeRead = cbDataIn;

        return S_OK;
    }
}

//-----------------------------------------------------------------------------
// Name: CWaveFile::WriteMMIO()
// Desc: Support function for reading from a multimedia I/O stream
//       pwfxDest is the WAVEFORMATEX for this new wave file.  
//       m_hmmio must be valid before calling.  This function uses it to
//       update m_ckRiff, and m_ck.  
//-----------------------------------------------------------------------------
HRESULT CWaveFile::WriteMMIO( WAVEFORMATEX *pwfxDest )
{
    DWORD    dwFactChunk; // Contains the actual fact chunk. Garbage until WaveCloseWriteFile.
    MMCKINFO ckOut1;
    
    dwFactChunk = (DWORD)-1;

    // Create the output file RIFF chunk of form type 'WAVE'.
    m_ckRiff.fccType = mmioFOURCC('W', 'A', 'V', 'E');       
    m_ckRiff.cksize = 0;

    if( 0 != mmioCreateChunk( m_hmmio, &m_ckRiff, MMIO_CREATERIFF ) )
        return E_FAIL;//DXTRACE_ERR( TEXT("mmioCreateChunk"), E_FAIL );
    
    // We are now descended into the 'RIFF' chunk we just created.
    // Now create the 'fmt ' chunk. Since we know the size of this chunk,
    // specify it in the MMCKINFO structure so MMIO doesn't have to seek
    // back and set the chunk size after ascending from the chunk.
    m_ck.ckid = mmioFOURCC('f', 'm', 't', ' ');
    m_ck.cksize = sizeof(PCMWAVEFORMAT);   

    if( 0 != mmioCreateChunk( m_hmmio, &m_ck, 0 ) )
        return E_FAIL;//DXTRACE_ERR( TEXT("mmioCreateChunk"), E_FAIL );
    
    // Write the PCMWAVEFORMAT structure to the 'fmt ' chunk if its that type. 
    if( pwfxDest->wFormatTag == WAVE_FORMAT_PCM )
    {
        if( mmioWrite( m_hmmio, (HPSTR) pwfxDest, 
                       sizeof(PCMWAVEFORMAT)) != sizeof(PCMWAVEFORMAT))
            return E_FAIL;//DXTRACE_ERR( TEXT("mmioWrite"), E_FAIL );
    }   
    else 
    {
        // Write the variable length size.
        if( (UINT)mmioWrite( m_hmmio, (HPSTR) pwfxDest, 
                             sizeof(*pwfxDest) + pwfxDest->cbSize ) != 
                             ( sizeof(*pwfxDest) + pwfxDest->cbSize ) )
            return E_FAIL;//DXTRACE_ERR( TEXT("mmioWrite"), E_FAIL );
    }  
    
    // Ascend out of the 'fmt ' chunk, back into the 'RIFF' chunk.
    if( 0 != mmioAscend( m_hmmio, &m_ck, 0 ) )
        return E_FAIL;//DXTRACE_ERR( TEXT("mmioAscend"), E_FAIL );
    
    // Now create the fact chunk, not required for PCM but nice to have.  This is filled
    // in when the close routine is called.
    ckOut1.ckid = mmioFOURCC('f', 'a', 'c', 't');
    ckOut1.cksize = 0;

    if( 0 != mmioCreateChunk( m_hmmio, &ckOut1, 0 ) )
        return E_FAIL;//DXTRACE_ERR( TEXT("mmioCreateChunk"), E_FAIL );
    
    if( mmioWrite( m_hmmio, (HPSTR)&dwFactChunk, sizeof(dwFactChunk)) != 
                    sizeof(dwFactChunk) )
         return E_FAIL;//DXTRACE_ERR( TEXT("mmioWrite"), E_FAIL );
    
    // Now ascend out of the fact chunk...
    if( 0 != mmioAscend( m_hmmio, &ckOut1, 0 ) )
        return E_FAIL;//DXTRACE_ERR( TEXT("mmioAscend"), E_FAIL );
       
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::Write()
// Desc: Writes data to the open wave file
//-----------------------------------------------------------------------------
HRESULT CWaveFile::Write( UINT nSizeToWrite, BYTE* pbSrcData, UINT* pnSizeWrote )
{
    UINT cT;

    if( m_bIsReadingFromMemory )
        return E_NOTIMPL;
    if( m_hmmio == NULL )
        return CO_E_NOTINITIALIZED;
    if( pnSizeWrote == NULL || pbSrcData == NULL )
        return E_INVALIDARG;

    *pnSizeWrote = 0;
    
    for( cT = 0; cT < nSizeToWrite; cT++ )
    {       
        if( m_mmioinfoOut.pchNext == m_mmioinfoOut.pchEndWrite )
        {
            m_mmioinfoOut.dwFlags |= MMIO_DIRTY;
            if( 0 != mmioAdvance( m_hmmio, &m_mmioinfoOut, MMIO_WRITE ) )
                return E_FAIL;//DXTRACE_ERR( TEXT("mmioAdvance"), E_FAIL );
        }

        *((BYTE*)m_mmioinfoOut.pchNext) = *((BYTE*)pbSrcData+cT);
        (BYTE*)m_mmioinfoOut.pchNext++;

        (*pnSizeWrote)++;
    }

    return S_OK;
}



CSoundManager::CSoundManager(void)
{
	m_pDS = NULL;
	strcpy(a_szWorkingPath,"\0");
}

CSoundManager::~CSoundManager(void)
{
	SAFE_RELEASE( m_pDS );
}

//设置工作路径；保持pak和普通路径一致
void CSoundManager::SetPath(char *path)
{
	if(!path)return;
	strcpy(a_szWorkingPath,path);
};				

HRESULT CSoundManager::Initialize(HWND hWnd,DWORD dwCoopLevel,char *path)
{
    HRESULT       hr;
	
    DWORD dwPrimaryChannels		= 1;	//Mono
    DWORD dwPrimaryFreq			= 22050;//22k
    DWORD dwPrimaryBitRate		= 8;	//8bits

    SAFE_RELEASE( m_pDS );

	SetPath(path);
    // Create IDirectSound using the primary sound device
    if( FAILED( hr = DirectSoundCreate8( NULL, &m_pDS, NULL ) ) )
        return E_FAIL;//DXTRACE_ERR( TEXT("DirectSoundCreate8"), hr );

    // Set DirectSound coop level 
    if( FAILED( hr = m_pDS->SetCooperativeLevel( hWnd, dwCoopLevel ) ) )
        return E_FAIL;//DXTRACE_ERR( TEXT("SetCooperativeLevel"), hr );
	
	DSCAPS dsCaps;
	dsCaps.dwSize = sizeof(DSCAPS);
	if(FAILED(m_pDS->GetCaps(&dsCaps)))
		return E_FAIL;//;

	if(dsCaps.dwFlags & DSCAPS_SECONDARY16BIT)//支持 16 位
	{
		dwPrimaryBitRate = 16;
	}
	if(dsCaps.dwFlags & DSCAPS_PRIMARYSTEREO)//支持立体声
	{
		dwPrimaryChannels = 2;
	}
	dwPrimaryFreq = 44100 ; 
    
    // Set primary buffer format
    if( FAILED( hr = SetPrimaryBufferFormat( dwPrimaryChannels, dwPrimaryFreq, dwPrimaryBitRate ) ) )
        return E_FAIL;//DXTRACE_ERR( TEXT("SetPrimaryBufferFormat"), hr );

    return S_OK;
}


HRESULT CSoundManager::SetPrimaryBufferFormat( DWORD dwPrimaryChannels, 
                                               DWORD dwPrimaryFreq, 
                                               DWORD dwPrimaryBitRate )
{
    HRESULT             hr;
    LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;

    if( m_pDS == NULL )
        return CO_E_NOTINITIALIZED;

    // Get the primary buffer 
    DSBUFFERDESC dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize        = sizeof(DSBUFFERDESC);
    dsbd.dwFlags       = DSBCAPS_PRIMARYBUFFER;
    dsbd.dwBufferBytes = 0;
    dsbd.lpwfxFormat   = NULL;
       
    if( FAILED( hr = m_pDS->CreateSoundBuffer( &dsbd, &pDSBPrimary, NULL ) ) )
        return E_FAIL;//DXTRACE_ERR( TEXT("CreateSoundBuffer"), hr );

    WAVEFORMATEX wfx;
    ZeroMemory( &wfx, sizeof(WAVEFORMATEX) ); 
    wfx.wFormatTag      = WAVE_FORMAT_PCM; 
    wfx.nChannels       = (WORD) dwPrimaryChannels; 
    wfx.nSamplesPerSec  = dwPrimaryFreq; 
    wfx.wBitsPerSample  = (WORD) dwPrimaryBitRate; 
    wfx.nBlockAlign     = (WORD) (wfx.wBitsPerSample / 8 * wfx.nChannels);
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

    if( FAILED( hr = pDSBPrimary->SetFormat(&wfx) ) )
        return E_FAIL;//DXTRACE_ERR( TEXT("SetFormat"), hr );

    SAFE_RELEASE( pDSBPrimary );

    return S_OK;
}

HRESULT CSoundManager::Create( CSound* pSound, LPTSTR strWaveFileName, DWORD dwCreationFlags , GUID guid3DAlgorithm , DWORD dwNumBuffers)
{
    HRESULT hr;
    HRESULT hrRet = S_OK;
    LPDIRECTSOUNDBUFFER  apDSBuffer     = NULL;
    DWORD                dwDSBufferSize = NULL;
    CWaveFile*           pWaveFile      = NULL;

    if( m_pDS == NULL )
        return CO_E_NOTINITIALIZED;
    if( strWaveFileName == NULL || pSound == NULL || dwNumBuffers < 1 )
        return E_INVALIDARG;

     pWaveFile = new CWaveFile();
    if( pWaveFile == NULL )
    {
        hr = E_OUTOFMEMORY;
        goto LFail;
    }

    pWaveFile->Open( strWaveFileName, NULL, WAVEFILE_READ );

    if( pWaveFile->GetSize() == 0 )
    {
        hr = E_FAIL;
        goto LFail;
    }

    // Make the DirectSound buffer the same size as the wav file
    dwDSBufferSize = pWaveFile->GetSize();

    // Create the direct sound buffer, and only request the flags needed
    // since each requires some overhead and limits if the buffer can 
    // be hardware accelerated
    DSBUFFERDESC dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize          = sizeof(DSBUFFERDESC);
    dsbd.dwFlags         = dwCreationFlags;
    dsbd.dwBufferBytes   = dwDSBufferSize;
    dsbd.guid3DAlgorithm = guid3DAlgorithm;
    dsbd.lpwfxFormat     = pWaveFile->m_pwfx;

    // DirectSound is only guarenteed to play PCM data.  Other
    // formats may or may not work depending the sound card driver.
    hr = m_pDS->CreateSoundBuffer( &dsbd, &apDSBuffer, NULL );

    // Be sure to return this error code if it occurs so the
    // callers knows this happened.
    if( hr == DS_NO_VIRTUALIZATION )
        hrRet = DS_NO_VIRTUALIZATION;
            
    if( FAILED(hr) )
    {
        goto LFail;
    }

    // Create the sound
	pSound->Initialize( apDSBuffer, dwDSBufferSize, pWaveFile ,dwCreationFlags);
    return hrRet;

LFail:
    // Cleanup
    SAFE_DELETE( pWaveFile );
    return hr;
}

CSound::CSound()
{
	m_apDSBuffer = NULL;
	m_pWaveFile = NULL;
	m_dwCurPlayPos =0;
	m_pWaveFile     = NULL;
	m_dwCurWritePos = 0;
	m_nVol = -10000;
	m_nStep = DEC_STEP;
}


CSound::CSound( LPDIRECTSOUNDBUFFER apDSBuffer, DWORD dwDSBufferSize, 
                CWaveFile* pWaveFile ,DWORD dwCreationFlags)
{
    m_apDSBuffer = apDSBuffer;

    m_dwDSBufferSize = dwDSBufferSize;
    m_pWaveFile      = pWaveFile;
	m_dwCreationFlags = dwCreationFlags;
	m_dwCurPlayPos =0;
	m_dwCurWritePos = 0;
	m_nVol = -10000;
	m_nStep = DEC_STEP;
	m_nIncStep= INC_STEP;
    // Make DirectSound do pre-processing on sound effects
	if(m_apDSBuffer)
	{
	   FillBufferWithSound( m_apDSBuffer, FALSE );
	m_apDSBuffer->SetCurrentPosition(0);
	}
}


void CSound::Initialize( LPDIRECTSOUNDBUFFER apDSBuffer, DWORD dwDSBufferSize, CWaveFile* pWaveFile, DWORD dwCreationFlags )
{
//首先删除已经存在的缓冲.
	SAFE_RELEASE( m_apDSBuffer); 
	SAFE_DELETE( m_pWaveFile );

//更新新的缓冲.
	m_apDSBuffer = apDSBuffer;
    m_dwDSBufferSize = dwDSBufferSize;
    m_pWaveFile      = pWaveFile;
	m_dwCreationFlags = dwCreationFlags;
	m_dwCurPlayPos =0;
	m_dwCurWritePos = 0;
	m_nVol = -10000;
	m_nStep = DEC_STEP;
	m_nIncStep= INC_STEP;

	//use waiting thread ,do not first fill the play buffer!
    FillBufferWithSound( m_apDSBuffer, FALSE );
  
	m_apDSBuffer->SetCurrentPosition(0);
}

//-----------------------------------------------------------------------------
// Name: CSound::~CSound()
// Desc: Destroys the class
//-----------------------------------------------------------------------------
CSound::~CSound()
{	
	Free();	
}

void CSound::Free()
{
	SAFE_RELEASE(m_apDSBuffer);
	SAFE_DELETE(m_pWaveFile );
}

//-----------------------------------------------------------------------------
// Name: CSound::FillBufferWithSound()
// Desc: Fills a DirectSound buffer with a sound file 
//-----------------------------------------------------------------------------//0 wav 1 ogg
HRESULT CSound::FillBufferWithSound( LPDIRECTSOUNDBUFFER pDSB, BOOL bRepeatWavIfBufferLarger ,int i)
{
    HRESULT hr; 
    VOID*   pDSLockedBuffer      = NULL; // Pointer to locked buffer memory
    DWORD   dwDSLockedBufferSize = 0;    // Size of the locked DirectSound buffer
    DWORD   dwWavDataRead        = 0;    // Amount of data read from the wav file 

    if( pDSB == NULL )
        return CO_E_NOTINITIALIZED;

    // Make sure we have focus, and we didn't just switch in from
    // an app which had a DirectSound device
    if( FAILED( hr = RestoreBuffer( pDSB, NULL ) ) ) 
        return E_FAIL;//DXTRACE_ERR( TEXT("RestoreBuffer"), hr );

    // Lock the buffer down
    if( FAILED( hr = pDSB->Lock( 0, m_dwDSBufferSize, 
                                 &pDSLockedBuffer, &dwDSLockedBufferSize, 
                                 NULL, NULL, 0L ) ) )
        return E_FAIL;//DXTRACE_ERR( TEXT("Lock"), hr );

    // Reset the wave file to the beginning 
	if(i==0)
	{
		m_pWaveFile->ResetFile();
		if( FAILED( hr = m_pWaveFile->Read( (BYTE*) pDSLockedBuffer,
                                        dwDSLockedBufferSize, 
                                        &dwWavDataRead ) ) )           
        //return E_FAIL;
			goto LFAIL;
		if( dwWavDataRead == 0 )
		{
			// Wav is blank, so just fill with silence
			FillMemory( (BYTE*) pDSLockedBuffer, 
						dwDSLockedBufferSize, 
						(BYTE)(m_pWaveFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
		}
		else if( dwWavDataRead < dwDSLockedBufferSize )
		{
			// If the wav file was smaller than the DirectSound buffer, 
			// we need to fill the remainder of the buffer with data 
			if( bRepeatWavIfBufferLarger )
			{       
				// Reset the file and fill the buffer with wav data
				DWORD dwReadSoFar = dwWavDataRead;    // From previous call above.
				while( dwReadSoFar < dwDSLockedBufferSize )
				{  
					// This will keep reading in until the buffer is full 
					// for very short files
					if( FAILED( hr = m_pWaveFile->ResetFile() ) )
						goto LFAIL;//return E_FAIL;//DXTRACE_ERR( TEXT("ResetFile"), hr );

					hr = m_pWaveFile->Read( (BYTE*)pDSLockedBuffer + dwReadSoFar,
											dwDSLockedBufferSize - dwReadSoFar,
											&dwWavDataRead );
					if( FAILED(hr) )
						goto LFAIL;//return E_FAIL;//DXTRACE_ERR( TEXT("Read"), hr );

					dwReadSoFar += dwWavDataRead;
				} 
			}
			else
			{
				// Don't repeat the wav file, just fill in silence 
				FillMemory( (BYTE*) pDSLockedBuffer + dwWavDataRead, 
							dwDSLockedBufferSize - dwWavDataRead, 
							(BYTE)(m_pWaveFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
			}
		}
	}

    //DXTRACE_ERR( TEXT("Read"), hr );



    // Unlock the buffer, we don't need it anymore.
    pDSB->Unlock( pDSLockedBuffer, dwDSLockedBufferSize, NULL, 0 );

    return S_OK;
LFAIL:
	pDSB->Unlock( pDSLockedBuffer, dwDSLockedBufferSize, NULL, 0 );
	return E_FAIL;

}




//-----------------------------------------------------------------------------
// Name: CSound::RestoreBuffer()
// Desc: Restores the lost buffer. *pbWasRestored returns TRUE if the buffer was 
//       restored.  It can also be NULL if the information is not needed.
//-----------------------------------------------------------------------------
HRESULT CSound::RestoreBuffer( LPDIRECTSOUNDBUFFER pDSB, BOOL* pbWasRestored )
{
    HRESULT hr;

    if( pDSB == NULL )
        return CO_E_NOTINITIALIZED;
    if( pbWasRestored )
        *pbWasRestored = FALSE;

    DWORD dwStatus;
    if( FAILED( hr = pDSB->GetStatus( &dwStatus ) ) )
        return E_FAIL;//DXTRACE_ERR( TEXT("GetStatus"), hr );

    if( dwStatus & DSBSTATUS_BUFFERLOST )
    {
        // Since the app could have just been activated, then DirectSound 
        // may not be giving us control yet, so restoring the buffer may fail.  
        // If it does, sleep until DirectSound gives us control.
        do 
        {
            hr = pDSB->Restore();
            if( hr == DSERR_BUFFERLOST )
                Sleep( 10 );
        }
        while( hr != DS_OK );

        if( pbWasRestored != NULL )
            *pbWasRestored = TRUE;

        return S_OK;
    }
    else
    {
        return S_FALSE;
    }
}

//-----------------------------------------------------------------------------
// Name: CSound::GetFreeBuffer()
// Desc: Checks to see if a buffer is playing and returns TRUE if it is.
//-----------------------------------------------------------------------------
LPDIRECTSOUNDBUFFER CSound::GetFreeBuffer()
{
    if( m_apDSBuffer == NULL )
        return NULL; 

    if( m_apDSBuffer)
    {  
        DWORD dwStatus = 0;
        m_apDSBuffer->GetStatus( &dwStatus );
        if ( ( dwStatus & DSBSTATUS_PLAYING ) == 0 )
			return m_apDSBuffer;
		else
			return NULL;
            // break;
    }

	return NULL;
}

//-----------------------------------------------------------------------------
// Name: CSound::GetBuffer()
// Desc: 
//-----------------------------------------------------------------------------
LPDIRECTSOUNDBUFFER CSound::GetBuffer( DWORD dwIndex )
{
    if( m_apDSBuffer == NULL )
        return NULL;
    return m_apDSBuffer;
}

//-----------------------------------------------------------------------------
// Name: CSound::Get3DBufferInterface()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CSound::Get3DBufferInterface( DWORD dwIndex, LPDIRECTSOUND3DBUFFER* ppDS3DBuffer )
{
    if( m_apDSBuffer == NULL )
        return CO_E_NOTINITIALIZED;
  

    *ppDS3DBuffer = NULL;
	
    return m_apDSBuffer->QueryInterface( IID_IDirectSound3DBuffer, 
                                                  (VOID**)ppDS3DBuffer );
}

//-----------------------------------------------------------------------------
// Name: CSound::Play()
// Desc: Plays the sound using voice management flags.  Pass in DSBPLAY_LOOPING
//       in the dwFlags to loop the sound
//-----------------------------------------------------------------------------
HRESULT CSound::Play( DWORD dwPriority, DWORD dwFlags, LONG lVolume, LONG lFrequency, LONG lPan )
{
    HRESULT hr;
    BOOL    bRestored;

    if( m_apDSBuffer == NULL )
        return CO_E_NOTINITIALIZED;

	//revised by zhao zhi qiang
    LPDIRECTSOUNDBUFFER pDSB = m_apDSBuffer;//GetFreeBuffer();

    if( pDSB == NULL )
        return E_FAIL;

    // Restore the buffer if it was lost
    if( FAILED( hr = RestoreBuffer( pDSB, &bRestored ) ) )
        return E_FAIL;

    if( bRestored )
    {
        // The buffer was restored, so we need to fill it with new data
        if( FAILED( hr = FillBufferWithSound( pDSB, FALSE ) ) )
            return E_FAIL;

        // Make DirectSound do pre-processing on sound effects
        Reset();
    }

	if( m_dwCreationFlags & DSBCAPS_CTRLVOLUME )
    {
        pDSB->SetVolume( lVolume );//-5000
		m_nVol = lVolume;
    }
    
    if( m_dwCreationFlags & DSBCAPS_CTRLPAN )
    {
        pDSB->SetPan( lPan );
    }
	Reset();
	m_nStep = DEC_STEP;
	return pDSB->Play( 0, dwPriority, dwFlags );
}

//-----------------------------------------------------------------------------
// Name: CSound::Stop()
// Desc: Stops the sound from playing
//-----------------------------------------------------------------------------
HRESULT CSound::Stop()
{
    if( m_apDSBuffer == NULL )
        return CO_E_NOTINITIALIZED;

    HRESULT hr = 0;
  
	hr = m_apDSBuffer->Stop();
    return hr;
}


//当前不支持音效暂停
void CSound::Pause()
{
	if( m_apDSBuffer == NULL )
        return ;

	HRESULT hr = 0;
	hr = m_apDSBuffer->GetCurrentPosition(&m_dwCurPlayPos, &m_dwCurWritePos);

	if(SUCCEEDED(hr))
		hr = m_apDSBuffer->Stop();

	return;
}

void CSound::Restart(DWORD dwFlags, DWORD dwPriority)
{
	if( m_apDSBuffer == NULL )
        return ;

	HRESULT hr = 0;

	hr = m_apDSBuffer->SetCurrentPosition(m_dwCurPlayPos);


	if(FAILED(hr))
		return ;

	if(dwFlags == 0)
		return;

	m_apDSBuffer->Play(0, dwPriority, dwFlags);
}


//-----------------------------------------------------------------------------
// Name: CSound::Reset()
// Desc: Reset all of the sound buffers
//-----------------------------------------------------------------------------
HRESULT CSound::Reset()
{
    if( m_apDSBuffer == NULL )
        return CO_E_NOTINITIALIZED;

    HRESULT hr = 0;
   
	hr = m_apDSBuffer->SetCurrentPosition( 0 );
    return hr;
}

//-----------------------------------------------------------------------------
// Name: CSound::IsSoundPlaying()
// Desc: Checks to see if a buffer is playing and returns TRUE if it is.
// 返回值:1,停止;0xFFFFFFFF播放;0:异常
//-----------------------------------------------------------------------------
DWORD CSound::IsSoundPlaying()
{
    DWORD dwPlaying = 0xFFFFFFFF;//全都在放

    if( m_apDSBuffer )
    {  
        DWORD dwStatus = 0;
        m_apDSBuffer->GetStatus( &dwStatus );
		if( !(dwStatus & DSBSTATUS_PLAYING ))//
			return 1; //有没有放的buffer
		else
			return dwPlaying;
    }
	return 0;	
}


HRESULT CSound::SetPan(LONG lPan)
{
	HRESULT hr = S_OK;
	DSBCAPS dsbCaps;
	ZeroMemory(&dsbCaps, sizeof(DSBCAPS));
	dsbCaps.dwSize = sizeof(DSBCAPS);
	if(m_apDSBuffer)
	{
		if(FAILED( hr = m_apDSBuffer->GetCaps(&dsbCaps)))
			return hr;
	}
	if(dsbCaps.dwFlags & DSBCAPS_CTRLPAN )
		return m_apDSBuffer->SetPan(lPan);
	else
		return hr;
}

HRESULT CSound::SetVol(LONG lVol)
{
	HRESULT hr = S_OK;
	DSBCAPS dsbCaps;
	ZeroMemory(&dsbCaps, sizeof(DSBCAPS));
	dsbCaps.dwSize = sizeof(DSBCAPS);
	if(m_apDSBuffer)
	{
		if(FAILED( hr = m_apDSBuffer->GetCaps(&dsbCaps)))
			return hr;
	}
	if(dsbCaps.dwFlags & DSBCAPS_CTRLVOLUME )
	{
		m_nVol = lVol;	
		return m_apDSBuffer->SetVolume(lVol);
	}
	else
		return hr;

}

long CSound::DecVol()
{
	m_nVol += m_nStep;
	m_nStep *= 2;
	if(m_nVol <-5000)
	{
		m_nVol= -6000;
	}
	SetVol(m_nVol);
	return m_nVol;
}

bool CSound::IncVol()
{
	bool result = false ;
	m_nVol +=m_nIncStep;
	
	if(m_nIncStep <= 60)
		m_nIncStep = 30;
	else
		m_nIncStep /= 2;
	
	if(m_nVol >= m_nFinalVol)
	{
		m_nVol =  m_nFinalVol;
		result = true;
	}
	SetVol(m_nVol);
	return result;
}
