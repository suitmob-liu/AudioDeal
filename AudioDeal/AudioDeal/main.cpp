#include "ClassAudioTool.h"

#include <stdio.h>
#include <iostream>

int main()
{
	printf("this is test\n");

	ClassAudioTool at;
	int ret = 0;
	//ret = at.audioCut("./pcm/1.txt", "./pcm", 1);
	printf("audioCut ret is %d\n", ret);

	return 0;
}