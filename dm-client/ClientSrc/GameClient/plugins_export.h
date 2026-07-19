

#ifndef _EXPORT_H_CGP_
#define _EXPORT_H_CGP_

//////////////////////////////////////////////////////////////////////////////////////////////
//
/**
 * 初始化模块
 */

extern BOOL CgpInitialize();


/**
 * 释放模块资源
 */

extern void CgpUninitialize();

//焦点管理标志
#define FOCUS_HOOK		0
#define FOCUS_IGNORE		1

//////////////////////////////////////////////////////////////////////////////////////////////
// web 化接口


#define CGP_VARIANT_AUTHCODE		0x01		// 认证码变量标识
#define CGP_VARIANT_SERVINFO		0x02		// 服务器信息
#define CGP_VARIANT_MACHINECODE		0x03		// 唯一机器码
#define CGP_VARIANT_WEB_PARAMS		0x04		// web参数
#define CGP_VARIANT_MAX			0x04


typedef void (*CgpAppExitEvent_t)();

typedef void (*CgpFullscreenEvent_t)(BOOL fullScreen);

typedef void (*CgpWindowSizeEvent_t)(SIZE newSize);


/**
 *	获取插件窗口句柄
 *	返回: 窗口句柄
 */

extern HWND CgpGetActiveWindow();


/**
 *	获取插件窗口尺寸
 *	返回: 尺寸 
 */

extern SIZE CgpGetActiveWindowSize();


/**
 *	检查应用是否被下载完成
 *	返回:
 */

extern BOOL CgpAppDownComplete();


/**
 *	应用主窗口创建完成后，调用该接口
 *	参数: 应用主窗口句柄
*	参数: 焦点监控标志,默认为监控
 *	返回: 是否成功
 */

extern BOOL CgpSetupAppWindow(HWND hWnd, INT nFlags = FOCUS_HOOK);


/**
 *	设置全屏切换回调函数
 *	参数: 回调函数
 *	返回: 无
 */

extern void CgpListenFullscreenEvent(CgpFullscreenEvent_t cb);


/**
 *	设置窗口大小变化事件回调函数
 *	参数: 回调函数
 *	返回: 无
 */

extern void CgpListenWindowSizeEvent(CgpWindowSizeEvent_t cb);


/**
 *	设置应用退出事件回调函数
 *	参数: 回调函数
 *	返回: 无
 */

extern void CgpListenAppExitEvent(CgpAppExitEvent_t cb);


/**
*	获取制定的变量的内容
*	参数: UINT varId			变量 id, see CGP_VARIANT_AUTHCODE
*	参数: LPSTR pszBuf			缓冲区
*	参数: UINT nMax				缓冲区长度
*	返回: 获取的数据的长度
*/

extern UINT CgpQueryVariant(UINT varId, LPSTR pszBuf, UINT nMax);




//////////////////////////////////////////////////////////////////////////////////////////////
// 预下载接口

#pragma pack(push, 1)

struct CgpFileSegmentW {
	WCHAR filename[MAX_PATH];
	UINT bytes;
	UINT64 offset;	
};

#pragma pack(pop)
/**
 *	预加载批次完成回调
 *	参数: UINT nBatch		批次对象标示
 *	参数: UINT nFileSeg		文件片段标示，为 -1时表示批次对象中所有文件片段都已下载完成
 *	参数: UINT nBytes		文件片段中完成的字节数
 *	参数: void* ud 			用户自定义参数
 *	返回: 文件片段在批次对象中的下标，失败时返回 -1
 */
typedef void (*CgpPreloadComplete_t)(UINT nBatch, UINT nFileSeg, UINT nBytes, void* ud);



#define CGP_BATCH_STATIC	0x01



/**
 *	启用/关闭下载静态预下载批次
 *	参数:	BOOL bEnable		是否启用
			LPCWSTR pszGameName 游戏名称
 *	返回: 无
 */
extern void CgpEnablePreload(BOOL bEnable);
extern void CgpEnablePreloadEx(LPCWSTR pszGameName, BOOL bEnable);

/**
 *	创建预加载批次对象
 *	参数: LPCSTR pszBatchName	批次对象名称
 *	参数: UINT nFlags			批次类型，CGP_BATCH_STATIC 表示预定义的批处理，在云端
 *	返回: 批次对象标示，失败时返回 -1
 */
extern UINT CgpCreatePreloadBatch(LPCWSTR pszBatchName, UINT nFlags);
extern UINT CgpCreatePreloadBatchEx(LPCWSTR pszGameName, LPCWSTR pszBatchName, UINT nFlags);


/**
 *	获取批次的文件片段个数
 *	参数: UINT nBatch			批次对象标示
 *	返回: 文件片段个数
 */
extern UINT CgpGetFileSegCount(UINT nBatch);


/**
 *	添加文件片段到预加载批次对象中
 *	参数: UINT nBatch			批次对象标示
 *	参数: LPCWSTR pszPath		文件的绝对路径
 *	参数: UINT64 nOffset		文件偏移
 *	参数: UINT nBytes			数据长度
 *	返回: 文件片段在批次对象中的下标，失败时返回 -1
 */
extern UINT CgpAddFileSegmentW(UINT nBatch, LPCWSTR pszPath, UINT64 nOffset, UINT nBytes);


/**
 *	获取文件片段到预加载批次对象中
 *	参数: UINT nBatch			批次对象标示
 *	参数: UINT nFileSeg		文件片段标示
 *	参数: CgpFileSegmentW* pSegement 	返回文件片段信息
 *	返回: 获取成功返回 true，失败返回 false
 */
extern BOOL CgpGetFileSegmentW(UINT nBatch, UINT nFileSeg, CgpFileSegmentW* pSegment);


/**
 *	检测预加载批次是否已经下载完成
 *	参数: UINT nBatch			批次对象标示
 *	返回: 下载完成返回 ture，未完成返回 false
 */
extern BOOL CgpCheckBatch(UINT nBatch);


/**
 *	开始预加载批次
 *	参数: UINT nBatch			批次对象标示
 *	参数: CgpPreloadComplete_t fnComplete 完成回调函数
 *	参数: void* ud 			用户自定义参数
 *	返回: 操作被接受返回 ture，否则返回 false
 */
extern BOOL CgpPreloadBatch(UINT nBatch, CgpPreloadComplete_t fnComplete, void* ud);



/**
 *	预加载单一文件片段，简化调用
 *	参数: LPCWSTR pszPath		文件的绝对路径
 *	参数: UINT64 nOffset		文件偏移
 *	参数: UINT nBytes			数据长度
 *	参数: CgpPreloadComplete_t fnComplete 完成回调函数
 *	参数: void* ud 			用户自定义参数
 *	返回: 批次对象标示，失败时返回 -1
 *	注:   返回标示需要使用 CgpCloseBatch 关闭
 */
extern UINT CgpPreloadFileDataW(LPCWSTR pszPath, 
	UINT64 nOffset, UINT nBytes, CgpPreloadComplete_t fnComplete, void* ud);



/**
 *	取消预加载操作
 *	参数: UINT nBatch			批次对象标示
 *	返回: 操作被取消返回 ture，否则返回 false
 */
extern BOOL CgpCancelBatch(UINT nBatch);


/**
 *	关闭预加载批次对象/单文件
 *	参数: UINT nBatch			批次对象标示
 *	返回: 无
 */
extern void CgpCloseBatch(UINT nBatch);


/**
*	获取后台网络状况
*	参数: CgpFlowInfo* r		返回的网络状况信息
*	返回: 成功返回 ture，否则返回 false
*/
struct CgpFlowInfo {
	UINT totalSend;		// 总发送字节数
	UINT totalRecv;		// 总接收字节数
	UINT curSend;		// 发送速率
	UINT curRecv;		// 接收速率
};

extern BOOL CgpQueryFlow(CgpFlowInfo* r);

/**
*	等待子进程初始化完毕
*	参数: HANDLE hChildProcess 子进程句柄
*	返回: 成功返回 ture，否则返回 false
*/
extern BOOL CgpWaitChildsInited(HANDLE hChildProcess);

#endif

