#ifndef __IGALIVE9_H__
#define __IGALIVE9_H__

#pragma once


//=================================================================================
/*!typdef
视频结束回调知函数 参数用户自定义
*/
typedef void (__stdcall *PFN_IGALiveVideoNotifyCB)(void* pParameters);

/**********************************************************************************/
/*!func
IGALive组件初始化函数,D3D初始后调用此函数
!param[in] 游戏程序D3D句柄,必须在CreateDevice调用前传入此参数才有效,否则设为NULL
!param[in] 游戏程序的D3D Device句柄 前一参数不为NULL,则参数可以为NULL
!return 返回TRUE 成功 FALSE 失败
*/
BOOL __stdcall IGALiveInitialize( IDirect3D9* pIDirect3D9, IDirect3DDevice9* pIDirect3DDevice9 );

/**********************************************************************************/
/*!func
开启IGALive组件显示视频广告功能
!param[in] 视频结束的回调通知函数
!param[in] 用户自定义数据会做为回调函数的参数传给用户
!return 返回TRUE成功 FALSE 此次没有视频广告或者用户机器环境不足以支持视频广告
*/
BOOL __stdcall IGALiveShowVideo( PFN_IGALiveVideoNotifyCB pfn, void* pParameters);

/**********************************************************************************/
/*!func
关闭IGALive组件显示视频广告功能
*/
void __stdcall IGALiveCloseVideo();

/**********************************************************************************/
/*!func
IGALive组件销毁函 游戏结束时D3D释放后调用此函数
*/
void __stdcall IGALiveDestroy();


#endif //__IGALIVE9_H__