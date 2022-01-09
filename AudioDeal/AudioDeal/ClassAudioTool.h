#pragma once

#include "audioType.h"
#include "generalType.h"
#include "ClassDebug.h"

#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

class ClassAudioTool
{
public:
	ClassAudioTool();
	~ClassAudioTool();

	u32_t audioCut(string inputName, string outPutPath, u16_t channel);//多声道拆解为单声道
	u32_t audioJoint(u16_t nChannel);//单声道转多声道

	u32_t audioCompound(string inPath, string filter);//合成音频，多个单声道合成一个多声道音频

	u32_t audioSplicing(string inPath);//多个短音频按文件名称拼接成一个长音频

	u32_t audioCutForTime(string inputName, u16_t channel, u16_t nStartTime, u16_t nEndTime);//一个音频文件获取指定片段的音频

	void test();

private:
	/*
	* brief:	读取传入文件内容，创建buffer存储文件内容，返回buffer指针和文件大小
	* param:	filePath为传入文件路径，outRes为文件buffer指针，fileSize为文件大小
	* return：	AUDIO_OPEN_FILE_FAILURE：	文件不存在
	*			AUDIO_SUCCESS：				运行成功
	*/
	u32_t readFile(string& filePath, char** pRes, u64_t& fileSize); //私有成员函数。专门读取文件

	void getFiles(string path, vector<string>& files);
	void getFilesFilter (string path, vector<string>& files, string Filter);

private:
	FILE* fileIn;		//输入文件
	FILE* fileOut;		//输出文件
	char* m_pFileRes;	//输入文件句柄
};

