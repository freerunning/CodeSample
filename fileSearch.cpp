// fileSearch.cpp : Defines the entry point for the console application.
// 搜索指定目录下所有文件并打印文件名

#include "stdafx.h"
#include "stdio.h"
#include "io.h"

#define  FILE_PATH    ".\\语音测试\\*.*" 

char name[500];

int fileSearch()
{
	long Handle;
	struct _finddata_t FileInfo;

	Handle = _findfirst(FILE_PATH, &FileInfo);
	if (-1L == Handle)
	{
		printf("没有找到文件\n");
		return 0;
	}
	else
	{
		printf("%s\n", FileInfo.name);
		while (_findnext(Handle, &FileInfo) == 0)
		{
			printf("%s\n", FileInfo.name);
		}
		_findclose(Handle);
	}

	return 1;
}

int _tmain(int argc, _TCHAR* argv[])
{
	fileSearch();
	return 0;
}

