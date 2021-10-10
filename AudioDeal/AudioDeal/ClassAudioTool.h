#pragma once

#include "audioType.h"
#include "generalType.h"
#include "ClassDebug.h"

#include <stdio.h>
#include <string>

using namespace std;

class ClassAudioTool
{
public:
	ClassAudioTool();
	~ClassAudioTool();

	u32_t audioCut(string inputName, string outPutPath, u16_t channel);//多声道拆解为单声道
	u32_t audioJoint();//单声道转多声道

private:
	/*
	* brief:	读取传入文件内容，创建buffer存储文件内容，返回buffer指针和文件大小
	* param:	filePath为传入文件路径，outRes为文件buffer指针，fileSize为文件大小
	* return：	AUDIO_OPEN_FILE_FAILURE：	文件不存在
	*			AUDIO_SUCCESS：				运行成功
	*/
	u32_t readFile(string& filePath, char** pRes, u64_t& fileSize); //私有成员函数。专门读取文件

private:
	FILE* fileIn;		//输入文件
	FILE* fileOut;		//输出文件
	char* m_pFileRes;	//输入文件句柄
};

