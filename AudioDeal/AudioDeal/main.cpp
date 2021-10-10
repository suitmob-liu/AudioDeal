
#include "ClassAudioTool.h"
#include "ClassDebug.h"

#include <stdio.h>
#include <iostream>
#pragma warning(disable:4996)

int main()
{
//	printf("this is test\n");
	char tm1[128] = { 0 };
	char tm2[128] = { 0 };
	int num = 0;
#if 1
	printf("输入需截切的音频文件：");
	scanf("%s",tm1);
	printf("\n");

	printf("输入生成音频文件路径：");
	scanf("%s", tm2);
	printf("\n");

	printf("输入待截切音频的通道数：");
	scanf("%d", &num);
	printf("\n");

	ClassAudioTool at;
	int ret = 0;
	ret = at.audioCut(tm1, tm2, num);
	printf("audioCut ret is %d\n", ret);
#else

	ClassDebug &cd = ClassDebug::getInStance();
	ClassDebug &cd2 = ClassDebug::getInStance();
	string str = "main is run";
	cd.debugLog(str);
	ClassAudioTool at2;
	at2.audioJoint();
	cd.debugLog("run over");

#endif



	return 0;
}