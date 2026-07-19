// GdiRaii.h - GDI 句柄 RAII 封装（替代 CFont/CBrush/CPen）
// 使用 std::unique_ptr + 自定义 deleter 管理 GDI 资源
#pragma once
#include "stdafx.h"

namespace gdi {

// ========== GDI 句柄删除器 ==========
struct FontDeleter   { void operator()(HFONT   h) const { if (h) ::DeleteObject(h); } };
struct BrushDeleter  { void operator()(HBRUSH  h) const { if (h) ::DeleteObject(h); } };
struct PenDeleter    { void operator()(HPEN    h) const { if (h) ::DeleteObject(h); } };
struct BitmapDeleter { void operator()(HBITMAP h) const { if (h) ::DeleteObject(h); } };
struct RegionDeleter { void operator()(HRGN    h) const { if (h) ::DeleteObject(h); } };

// ========== RAII 句柄类型 ==========
using FontHandle   = std::unique_ptr<std::remove_pointer<HFONT>::type,   FontDeleter>;
using BrushHandle  = std::unique_ptr<std::remove_pointer<HBRUSH>::type,  BrushDeleter>;
using PenHandle    = std::unique_ptr<std::remove_pointer<HPEN>::type,    PenDeleter>;
using BitmapHandle = std::unique_ptr<std::remove_pointer<HBITMAP>::type, BitmapDeleter>;
using RegionHandle = std::unique_ptr<std::remove_pointer<HRGN>::type,    RegionDeleter>;

// ========== 辅助创建函数 ==========

// 创建字体（替代 CFont::CreateFont）
inline HFONT CreateFontEx(
	int height, int width = 0, int escapement = 0, int orientation = 0,
	int weight = FW_NORMAL, bool italic = false, bool underline = false, bool strikeOut = false,
	const char* faceName = "Microsoft YaHei")
{
	return ::CreateFontA(
		height, width, escapement, orientation,
		weight,
		italic ? TRUE : FALSE,
		underline ? TRUE : FALSE,
		strikeOut ? TRUE : FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		faceName);
}

inline FontHandle MakeFont(
	int height, int weight = FW_NORMAL, const char* faceName = "Microsoft YaHei")
{
	return FontHandle(CreateFontEx(height, 0, 0, 0, weight, false, false, false, faceName));
}

// 创建实心画刷（替代 CBrush::CreateSolidBrush）
inline BrushHandle MakeSolidBrush(COLORREF color)
{
	return BrushHandle(::CreateSolidBrush(color));
}

// 创建画笔
inline PenHandle MakePen(int style, int width, COLORREF color)
{
	return PenHandle(::CreatePen(style, width, color));
}

// 获取系统 Stock 对象（不需要释放，返回 raw handle）
inline HBRUSH GetStockNullBrush()  { return (HBRUSH)::GetStockObject(NULL_BRUSH); }
inline HBRUSH GetStockWhiteBrush() { return (HBRUSH)::GetStockObject(WHITE_BRUSH); }
inline HPEN   GetStockBlackPen()   { return (HPEN)::GetStockObject(BLACK_PEN); }

} // namespace gdi
