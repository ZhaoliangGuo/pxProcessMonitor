#ifndef _PX_COMMON_DEF_H
#define _PX_COMMON_DEF_H

// Created by Zhaoliang Guo

extern CString g_strConfFile;
extern bool g_bThreadActive;

static HBRUSH brush_red = ::CreateSolidBrush(RGB(255,0,0));
static HBRUSH brush_green = ::CreateSolidBrush(RGB(0,255,0));

enum STATIC_BKCOLOR
{
	NULL_COLOR,
	RED_COLOR,
	GREEN_COLOR,
};

bool IsProcessExist(LPCTSTR in_lpstrClientName);
BOOL IsFileExist(LPCTSTR lpFile);

#endif