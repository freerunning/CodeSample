// fileSearch.cpp : Defines the entry point for the console application.
// ����ָ��Ŀ¼�������ļ�����ӡ�ļ���

#include "stdafx.h"
#include "stdio.h"
#include "io.h"

#define  FILE_PATH    ".\\��������\\*.*" 

char name[500];

int fileSearch()
{
	long Handle;
	struct _finddata_t FileInfo;

	Handle = _findfirst(FILE_PATH, &FileInfo);
	if (-1L == Handle)
	{
		printf("û���ҵ��ļ�\n");
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

