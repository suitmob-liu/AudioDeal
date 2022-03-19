
#include "ClassAudioTool.h"
#include "ClassDebug.h"

#include <stdio.h>
#include <iostream>
#pragma warning(disable:4996)

#define key 5

int main()
{
	char tm1[128] = { 0 };
	char tm2[128] = { 0 };
	int num = 0;
	int startTime = 0, endTime = 0;
	ClassAudioTool testFunction;
	//ClassAudioTool at;
	int ret = 0;
	string str;
	int functionTest = 0;
	int times = 0;

	while (1)
	{
		printf("剪切音频******************************输入1\n"
			"合成音频******************************输入2\n"
			"拼接音频******************************输入3\n"
			"获取指定片段音频（根据时间）**********输入4\n"
			"剪切大文件音频（超过2G文件）**********输入5\n");
		functionTest = getchar();

		if (functionTest == '1')
		{
			break;
		}
		else if (functionTest == '2')
		{
			break;
		}
		else if (functionTest == '3')
		{
			break;
		}
		else if (functionTest == '4')
		{
			break;
		}
		else if (functionTest == '5')
		{
			break;
		}
		else
		{
			times++;
			printf("输入错误第%d次，请重新输入\n", times);
			if (times >= 4)
			{
				printf("输入任意键退出");
				system("pause");
				return 0;
			}
		}
	}
	
	memset(tm1, 0, 128);
	memset(tm2, 0, 128);
	switch (functionTest)
	{
	case '1':
		printf("输入需截切的音频文件：");
		scanf("%s", tm1);
		printf("\n");

		printf("输入生成音频文件路径：");
		scanf("%s", tm2);
		printf("\n");

		printf("输入待截切音频的通道数：");
		scanf("%d", &num);
		printf("\n");

		ret = testFunction.audioCut(tm1, tm2, num);
		printf("audioCut ret is %d\n", ret);
		break;
	case '2':
		printf("输入需合成的音频文件路径：");
		scanf("%s", tm1);
		printf("\n");

		str = "out";
		testFunction.audioCompound(tm1, str);

		printf("合成成功\n");
		break;
	case '3':
		printf("输入需拼接的音频文件路径：");
		scanf("%s", tm1);
		printf("\n");

		testFunction.audioSplicing(tm1);
		printf("拼接结束\n");
		break;
	case '4':
		printf("输入需截切的音频文件：");
		scanf("%s", tm1);
		printf("\n");

		printf("输入待截切音频的通道数：");
		scanf("%d", &num);
		printf("\n");

		printf("输入待截切音频的起始点：(单位秒)");
		scanf("%d", &startTime);
		printf("\n");

		printf("输入待截切音频的终点：(单位秒)");
		scanf("%d", &endTime);
		printf("\n");

		ret = testFunction.audioCutForTime(tm1, num, startTime, endTime);
		printf("audioCutForTime ret is %d\n", ret);
		break;
	case '5':
		printf("输入需截切的音频文件：");
		scanf("%s", tm1);
		printf("\n");

		printf("输入生成音频文件路径：");
		scanf("%s", tm2);
		printf("\n");

		printf("输入待截切音频的通道数：");
		scanf("%d", &num);
		printf("\n");

		ret = testFunction.audioCutForBigFile(tm1, tm2, num);
		printf("audioCut ret is %d\n", ret);
		break;
	default:
		break;
	}

	system("pause");

	return 0;
}