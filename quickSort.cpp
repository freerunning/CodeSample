// quickSort.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>

int quickSort(int a[], int beg, int end)
{
	int piv, i, j;

	i = beg;
	j = end;
	piv = a[i];

	if (i >= j)
	{
		return 0;
	}
    while (i < j)
    {
		// �Ӻ���ǰ��С��piv����
		while (a[j] >= piv && i < j )
		{
			j--;
		}
		if(i < j)
		{
			a[i] = a[j];
		    i++;
		}

		// ��ǰ�����Ҵ���piv����
		while (a[i] <= piv && i < j)
		{
			i++;
		}
		if (i < j)
		{
			a[j] = a[i];
			j--;
		}
	}
	a[i] = piv;
	quickSort(a, beg, i - 1);
	quickSort(a, i + 1, end);
	return 0;
}

int main(int argc, char* argv[])
{
    int a[10];
	int i;
	int j = 20;
	while (j--)
	{
		printf("\n");
		for (i = 0; i < 10; i++)
		{
			a[i] = rand() % 100;
			printf("%d ", a[i]);
		}
		printf("\n");
		quickSort(a, 0, 9);
		for (i = 0; i < 10; i++)
		{
			printf("%d ", a[i]);
		}
        printf("\n");
	}
	
	return 0;
}

