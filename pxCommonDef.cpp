// Created by gzl 20141130, sunday
#include "stdafx.h"
#include "pxCommonDef.h"
#include <afxdisp.h>
#include "tlhelp32.h"

bool g_bThreadActive  = true;
CString g_strConfFile = ".\\config.ini"; 

// To see if the process exist.
bool IsProcessExist(LPCTSTR in_lpstrClientName)
{
	HANDLE hSnapshot;   
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0); 
	if( hSnapshot == INVALID_HANDLE_VALUE )
	{
		return false;
	}

	PROCESSENTRY32 sProcessList;   
	sProcessList.dwSize = sizeof(PROCESSENTRY32);   

	BOOL bRet = FALSE;   
	bRet = Process32First(hSnapshot, &sProcessList);   

	bool bProcessExist = false;
	while(bRet)   
	{
		if (strcmp( sProcessList.szExeFile, in_lpstrClientName) == 0)     
		{   
			bProcessExist = true;
		}

		bRet = Process32Next(hSnapshot,&sProcessList);    
	}     

	CloseHandle(hSnapshot);

	return bProcessExist;
}

BOOL IsFileExist(LPCTSTR lpFile)
{
	HANDLE hFile = CreateFile( lpFile ,
		GENERIC_READ ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL , 
		OPEN_EXISTING ,
		FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED ,
		NULL ) ;

	if( hFile == INVALID_HANDLE_VALUE )
	{
		DWORD dwError = ::GetLastError() ;

		hFile = NULL ;
		return FALSE;
	}

	CloseHandle(hFile);
	hFile = NULL ;

	return TRUE;
}