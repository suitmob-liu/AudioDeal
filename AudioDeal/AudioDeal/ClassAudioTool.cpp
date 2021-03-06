
#include "ClassAudioTool.h"
#include <io.h>

#pragma warning(disable:4996)

#define FIXSIZE (200*1024*1024)

ClassAudioTool::ClassAudioTool()
{
}

ClassAudioTool::~ClassAudioTool()
{
	if (m_pFileRes != NULL)
	{
		free(m_pFileRes);
		m_pFileRes = NULL;
	}
}

u32_t ClassAudioTool::audioCut(string inputName, string outPutPath, u16_t channel)
{
	char* inRes = NULL;//读取的输入文件内存
	char* outRes = NULL;//输出文件内存
	u16_t channelNum = channel;//通道数
	string openFileName = inputName;
	ClassDebug &nDebug = ClassDebug::getInStance();
	string strLog = "";

	u32_t ret = AUDIO_SUCCESS;
	u64_t fileSize = 0;
	bool isWavAudio = false;
	ret = readFile(openFileName, &inRes, fileSize);

	u64_t sizeRes = fileSize / channelNum;//单声道文件大小

	outRes = (char*)malloc(sizeof(char) * sizeRes);
	string outPath = outPutPath;
	char* fileName = NULL;
	fileName = (char*)malloc(sizeof(char) * 100);

	for (u16_t numIndex = 0; numIndex < channelNum; numIndex++)
	{
		memset(fileName, 0, 100 * sizeof(char));
		memset(outRes, 0, sizeRes * sizeof(char));
		unsigned char fileIndex = numIndex + 1;
		sprintf(fileName, "/outFile_%d.pcm", fileIndex);
		u16_t numD = numIndex * 2;
		u64_t j = 0;
		for (u64_t i = 0; i < fileSize; i += channelNum * 2)
		{
			outRes[j] = inRes[i + numD];
			outRes[j + 1] = inRes[i + 1 + numD];
			j += 2;
		}

		outPath = outPutPath;
		outPath += fileName;
		FILE* fp2 = NULL;
		fp2 = fopen(outPath.c_str(), "wb+");
		if (fp2 == NULL)
		{
			printf("open file failure\n");
			strLog = "open file failure\n";
			nDebug.debugLog(strLog);
			return AUDIO_OPEN_FILE_FAILURE;
		}
		int ret = fwrite(outRes, sizeof(char), sizeRes, fp2);
		fclose(fp2);
		fp2 = NULL;
	}
	if (fileName != NULL)
	{
		free(fileName);
		fileName = NULL;
	}
	if (inRes != NULL)
	{
		free(inRes);
		inRes = NULL;
	}
	if (outRes != NULL)
	{
		free(outRes);
		outRes = NULL;
	}
	return AUDIO_SUCCESS;
}

u32_t ClassAudioTool::audioJoint(u16_t nChannel)
{
	u32_t ret = AUDIO_SUCCESS;
	u16_t fileNum;//单声道文件个数。后期由UI判断
	fileNum = nChannel;
	string pathNameTmp1;//文件名称，后期通过UI加载
	string pathNameTmp2;
	string pathNameTmp3;
	string pathNameTmp4;

	char* fileResTmp1 = NULL;
	char* fileResTmp2 = NULL;
	char* fileResTmp3 = NULL;
	char* fileResTmp4 = NULL;

	u64_t fileSizeTmp1 = 0;//四个单声道文件组装的话，文件大小应该一致，此处定义一个变量即可，需对一致性做判断
	u64_t fileSizeTmp2 = 0;
	u64_t fileSizeTmp3 = 0;
	u64_t fileSizeTmp4 = 0;

	pathNameTmp1 = "D:\\SOFT\\audioCut\\out0.pcm";
	pathNameTmp2 = "D:\\SOFT\\audioCut\\out1.pcm";
	pathNameTmp3 = "D:\\SOFT\\audioCut\\out2.pcm";
	pathNameTmp4 = "D:\\SOFT\\audioCut\\out3.pcm";

	ret = readFile(pathNameTmp1, &fileResTmp1, fileSizeTmp1);
	ret = readFile(pathNameTmp2, &fileResTmp2, fileSizeTmp2);
	ret = readFile(pathNameTmp3, &fileResTmp3, fileSizeTmp3);
	ret = readFile(pathNameTmp4, &fileResTmp4, fileSizeTmp4);
	if (ret != 0)
	{
		printf("fail to readFile\n");
		return AUDIO_FAILE;
	}

	u64_t fileSizeSum = fileSizeTmp1 + fileSizeTmp2 + fileSizeTmp3 + fileSizeTmp4;//当作参数透传出去
	char* fileResSum = NULL;
	fileResSum = (char*)malloc(fileSizeSum);
	u64_t j = 0;
	for (u64_t i = 0; i < fileSizeSum; i += 8)
	{
		fileResSum[i] = fileResTmp1[j];
		fileResSum[i + 1] = fileResTmp1[j + 1];

		fileResSum[i + 2] = fileResTmp2[j];
		fileResSum[i + 3] = fileResTmp2[j + 1];

		fileResSum[i + 4] = fileResTmp3[j];
		fileResSum[i + 5] = fileResTmp3[j + 1];

		fileResSum[i + 6] = fileResTmp4[j];
		fileResSum[i + 7] = fileResTmp4[j + 1];
		j += 2;
	}

	string outPath = "./outFile.pcm";
	FILE* fp = fopen(outPath.c_str(), "wb+");
	fwrite(fileResSum, sizeof(char), fileSizeSum, fp);
	fclose(fp);

	if (fileResSum != NULL)
	{
		free(fileResSum);
		fileResSum = NULL;
	}

	if (fileResTmp1 != NULL)
	{
		free(fileResTmp1);
		fileSizeTmp1 = NULL;
	}
	if (fileResTmp2 != NULL)
	{
		free(fileResTmp2);
		fileResTmp2 = NULL;
	}
	if (fileResTmp3 != NULL)
	{
		free(fileResTmp3);
		fileResTmp3 = NULL;
	}
	if (fileResTmp4 != NULL)
	{
		free(fileResTmp4);
		fileSizeTmp4 = NULL;
	}

	return 0;
}

u32_t ClassAudioTool::audioCompound(string inPath, string filter)
{
	u32_t ret = AUDIO_SUCCESS;
	vector<string> files;
	bool fileAlready = false;
	filter = "pcm";
	getFilesFilter(inPath, files, filter);
	if (files.size() != 0)
	{
		fileAlready = true;
	}
	if (!fileAlready)
	{
		filter = "wav";
		getFilesFilter(inPath, files, filter);
	}
	//getFiles(inPath, files);
	int fileNum = files.size();
	if (fileNum == 0)
	{
		return AUDIO_FILE_NOT_FOUND;
	}

	string strLog;
	ClassDebug& nDebug = ClassDebug::getInStance();
	for (int ifileName = 0; ifileName < files.size(); ifileName++)
	{
		printf("file name is %s\n", files[ifileName].c_str());
		strLog = "file name is ";
		strLog += files[ifileName];
		nDebug.debugLog(strLog);
	}


	int j = 0;
	char* fileResIn = NULL;
	u64_t fileSizeIn = 0;
	u64_t fileSizeSum = 0;
	char* fileResSum = NULL;

	for (int i = 0; i < fileNum; i++)
	{
		j = 0;
		ret = readFile(files[i], &fileResIn, fileSizeIn);
		if (ret != AUDIO_SUCCESS)
		{
			printf("readFile ret %d", ret);
			if (fileResIn)
			{
				free(fileResIn);
				fileResIn = NULL;
			}
			return ret;
		}

		if (i == 0)
		{
			fileSizeSum = fileSizeIn * fileNum;
			fileResSum = (char*)malloc(fileSizeSum);
			memset(fileResSum, 0, fileSizeSum);
		}

		for (u64_t a = 0; a < fileSizeSum- fileNum*2; a += fileNum * 2)
		{
			fileResSum[a + i*2] = fileResIn[j];
			fileResSum[a + 1 + i*2] = fileResIn[j + 1];

			j += 2;
		}
		if (fileResIn)
		{
			free(fileResIn);
			fileResIn = NULL;
		}
	}

	string outPath = inPath;
	outPath += "/outPutFile.pcm";//需要多一层判断该文件是否已存在
	FILE* fp = fopen(outPath.c_str(), "wb+");
	fwrite(fileResSum, sizeof(char), fileSizeSum, fp);
	fclose(fp);
	fp = NULL;

	if (fileResSum)
	{
		free(fileResSum);
		fileResSum = NULL;
	}

	return ret;
}

u32_t ClassAudioTool::audioSplicing(string inPath)
{
	u32_t ret = AUDIO_SUCCESS;
	vector<string> files;
	ClassDebug& nDebug = ClassDebug::getInStance();
	string filter = "pcm";
	getFilesFilter(inPath, files, filter);
	filter = "wav";
	getFilesFilter(inPath, files, filter);
	//getFiles(inPath, files);
	string strLog;

	int fileNum = files.size();
	if (fileNum == 0)
		return AUDIO_FILE_NOT_FOUND;
	for (int i = 0; i < fileNum; i++)
	{
		printf("file name is %s\n", files[i].c_str());
		strLog = "file name is";
		strLog += files[i];
		nDebug.debugLog(strLog);
	}

	char* fileResIn = NULL;
	u64_t fileSizeIn = 0;
	char* fileResSum = NULL;
	
	string outPath = inPath;
	outPath += "/outPutFile.pcm";//需要多一层判断该文件是否已存在
	FILE* fp = fopen(outPath.c_str(), "ab");
	if (fp == NULL)
	{
		printf("open file %s failure\n", outPath.c_str());
	}

	for (int i = 0; i < fileNum; i++)
	{
		ret = readFile(files[i], &fileResIn, fileSizeIn);
		if (ret != AUDIO_SUCCESS)
		{
			printf("readFile ret %d", ret);
			if (fileResIn)
			{
				free(fileResIn);
				fileResIn = NULL;
			}
			return ret;
		}
		printf("fileSizeIn is %d\n", fileSizeIn);
		if (fileResIn != NULL && fp != NULL)
		{
			fwrite(fileResIn, sizeof(char), fileSizeIn, fp);
		}

		if (fileResIn)
		{
			free(fileResIn);
			fileResIn = NULL;
		}
	}
	if (fp)
	{
		fclose(fp);
	}

	return AUDIO_SUCCESS;
}

u32_t ClassAudioTool::audioCutForTime(string inputName, u16_t channel, u64_t nStartTime, u64_t nEndTime)
{
	u32_t ret = AUDIO_SUCCESS;
	
	string openFileName = inputName;

	char* fileResOrigin = NULL;
	u64_t fileSizeOrigin = 0;
	u64_t fileSizeOut = 0;
	u64_t nCurIndex = 0;

	fileSizeOut = (nEndTime - nStartTime) * 32000* channel;

	ret = readFile(openFileName, &fileResOrigin, fileSizeOrigin);
	if (ret != AUDIO_SUCCESS)
	{
		printf("readFile ret %d", ret);
		if (fileResOrigin)
		{
			free(fileResOrigin);
			fileResOrigin = NULL;
		}
		return ret;
	}
	if (fileSizeOut <= 0 || fileSizeOrigin < fileSizeOut)
	{
		printf("Invalid parameter!!!\nnStartTime is %lld,nEndTime is %lld\norigin file size is %lld\n"
			,nStartTime,nEndTime, fileSizeOrigin- fileSizeOut);
		if (fileResOrigin)
		{
			free(fileResOrigin);
			fileResOrigin = NULL;
		}
	}

	nCurIndex = nStartTime * 32000* channel;
	string outPath = inputName;
	outPath += "outPutTime.pcm";
	FILE* fp2 = NULL;
	fp2 = fopen(outPath.c_str(), "wb+");
	fwrite(fileResOrigin + nCurIndex, sizeof(char), fileSizeOut, fp2);
	fclose(fp2);
	fp2 = NULL;

	if (fileResOrigin)
	{
		free(fileResOrigin);
		fileResOrigin = NULL;
	}

	return ret;
}

u32_t ClassAudioTool::audioCutForBigFile(string inputName, string outPutPath, u16_t channel)
{
	u32_t ret = AUDIO_SUCCESS;

	ClassDebug& nDebug = ClassDebug::getInStance();
	u64_t resSize = 0,writeSize = 0;
	string outPutName;
	char* tmp = NULL, * outRes = NULL;
	FILE* inputHandle = NULL;
	inputHandle = fopen(inputName.c_str(), "rb+");
	if (inputHandle == NULL)
	{
		printf("open file failure\n");
		return AUDIO_OPEN_FILE_FAILURE;
	}
	fseek(inputHandle, 0, SEEK_SET);
	u64_t fileSize = 0;
	fileSize = channel * FIXSIZE;
	tmp = new char[fileSize];
	outRes = new char[fileSize / channel];
	u64_t outResIndex = 0;

	for(int fileIndex = 0; fileIndex < channel; fileIndex++)
	{
		outPutName = outPutPath;
		outPutName += "./LargeAudio_";
		outPutName += to_string(fileIndex);
		outPutName += ".pcm";
		fseek(inputHandle, 0, SEEK_SET);
		FILE* tempFP = fopen(outPutName.c_str(), "wb+");
		memset(tmp, 0, sizeof(tmp));
		memset(outRes, 0, sizeof(outRes));
		outResIndex = 0;
		nDebug.debugLog("CALL audioCutForBigFile begin every times");
		while (!feof(inputHandle))
		{
			nDebug.debugLog("CALL audioCutForBigFile begin fread begin");
			writeSize = fread(tmp, 100, fileSize / 100, inputHandle);
			writeSize *= 100;
			nDebug.debugLog("CALL audioCutForBigFile begin fread end");
			for (u64_t outIndex = 0; outIndex < writeSize; outIndex += (channel * 2))
			{
				outRes[outResIndex] = tmp[outIndex + fileIndex * 2];
				outRes[outResIndex + 1] = tmp[outIndex + 1 + fileIndex * 2];
				outResIndex += 2;
			}
			nDebug.debugLog("CALL audioCutForBigFile begin fwrite begin");
			fwrite(outRes, 1, writeSize / channel, tempFP);
			nDebug.debugLog("CALL audioCutForBigFile begin fwrite end");
			memset(outRes, 0, sizeof(outRes));
			outResIndex = 0;
		}
		fclose(tempFP);
		tempFP = NULL;
	}

	if (outRes != NULL)
	{
		delete[] outRes;
	}
	if (tmp != NULL)
	{
		delete[] tmp;
	}
	nDebug.debugLog("CALL audioCutForBigFile over");
	return ret;
}

u32_t ClassAudioTool::fileNameOut(string inPath, string filter)
{
	u32_t ret = AUDIO_SUCCESS;
	vector<string> files;
	bool fileAlready = false;
	filter = "pcm";
	getFilesFilter(inPath, files, filter);
	if (files.size() != 0)
	{
		fileAlready = true;
	}
	if (!fileAlready)
	{
		filter = "wav";
		getFilesFilter(inPath, files, filter);
	}
	int fileNum = files.size();
	if (fileNum == 0)
	{
		return AUDIO_FILE_NOT_FOUND;
	}

	FILE* outFileName = fopen("./fileName.txt", "ab+");
	
	string strLog1,strLog2;
	ClassDebug& nDebug = ClassDebug::getInStance();
	strLog2 = "======================================================\n";
	fwrite(strLog2.c_str(), 1, strLog2.size(), outFileName);
	fwrite(strLog2.c_str(), 1, strLog2.size(), outFileName);
	for (int ifileName = 0; ifileName < files.size(); ifileName++)
	{
		printf("file name is %s\n", files[ifileName].c_str());
		strLog1 = "file name is ";
		strLog1 += files[ifileName];
		strLog2 = files[ifileName];
		strLog2 += "\n";
		nDebug.debugLog(strLog1);
		fwrite(strLog2.c_str(), 1, strLog2.size(), outFileName);
	}

	fclose(outFileName);

	return ret;
}

void ClassAudioTool::test()
{
	string path;
	vector<string> files;
	string filter = "out";
	path = "D:\\CODE\\work\\Audio\\AudioDeal\\AudioDeal\\pcm";
	//getFiles(path, files);
	getFilesFilter(path, files, filter);
	for (int i = 0; i < files.size(); i++)
	{
		printf("file name is %s\n", files[i].c_str());
	}
}

u32_t ClassAudioTool::readFile(string& filePath, char** pRes, u64_t& fileSize)
{
	char* tmp = NULL;
	FILE* inputHandle = NULL;
	inputHandle = fopen(filePath.c_str(), "rb");
	if (inputHandle == NULL)
	{
		printf("open file failure\n");
		return AUDIO_OPEN_FILE_FAILURE;
	}

	u64_t resSize = 0;
	fseek(inputHandle, 0, SEEK_END);
	resSize = ftell(inputHandle);
	fseek(inputHandle, 0, SEEK_SET);

	tmp = (char*)malloc(resSize);
	memset(tmp, 0, resSize);
	fread(tmp, 1, resSize, inputHandle);
	fclose(inputHandle);
	inputHandle = NULL;

	if (memcmp(tmp, "RIFF", 4) == 0)
	{
		resSize = resSize - 44;
		memmove(tmp, tmp + 44, resSize);
	}
	*pRes = tmp;
	fileSize = resSize;
	return AUDIO_SUCCESS;
}

void ClassAudioTool::getFiles(string path, vector<string>& files)
{
	//文件句柄
	intptr_t   hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之
			//如果不是,加入列表
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void ClassAudioTool::getFilesFilter(string path, vector<string>& files, string Filter)
{
	//文件句柄
	intptr_t   hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	char temp[100] = { 0 };
	sprintf(temp,"\\*.%s", Filter.c_str());
	if ((hFile = _findfirst(p.assign(path).append(temp).c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之
			//如果不是,加入列表
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
