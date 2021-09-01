#include "ClassAudioTool.h"
#pragma warning(disable:4996)

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
#if 1
	char* inRes = NULL;//读取的输入文件内存
	char* outRes = NULL;//输出文件内存
	u16_t channelNum = channel;//通道数
	string openFileName = inputName;


	//need to check file exist

#ifdef OLDLOGIC
	//参数检查
	fileIn = fopen(openFileName.c_str(), "w+");
	if (fileIn == NULL)
	{
		printf("open file failure\n");
		return AUDIO_OPEN_FILE_FAILURE;
	}

	if (channelNum >= 1 && channelNum <= 16)
	{
		if (channelNum != (u16_t)channelNum)
		{
			printf("The Param is invalid and Param is decimal!\n");
			return AUDIO_INVAIID_PARAM;
		}
	}
	else
	{
		printf("The Param is invalid\n");
		return AUDIO_INVAIID_PARAM;
	}

	u32_t size = 0;//文件大小
	fseek(fileIn, 0, SEEK_END);
	size = ftell(fileIn);
	fseek(fileIn, 0, SEEK_SET);

	u32_t sizeRes = size / channelNum;//单声道文件大小
	inRes = (char*)malloc(sizeof(char) * size);
	memset(inRes, 0, size * sizeof(char));
	fread(inRes, sizeof(char), size, fileIn);
	fclose(fileIn);
#else
	u32_t ret = AUDIO_SUCCESS;
	u32_t fileSize = 0;
	ret = readFile(openFileName, inRes, fileSize);
	u32_t sizeRes = fileSize / channelNum;//单声道文件大小
#endif // OLDLOGIC

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
		u32_t numD = numIndex * 2;
		u32_t j = 0;
		for (u32_t i = 0; i < fileSize; i += channelNum * 2)
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
#endif
	return AUDIO_SUCCESS;
}

u32_t ClassAudioTool::audioJoint()
{
	u32_t ret = AUDIO_SUCCESS;
	u16_t fileNum;//单声道文件个数。后期由UI判断
	fileNum = 4;
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

	ret = readFile(pathNameTmp1, fileResTmp1, fileSizeTmp1);
	ret = readFile(pathNameTmp2, fileResTmp2, fileSizeTmp2);
	ret = readFile(pathNameTmp3, fileResTmp3, fileSizeTmp3);
	ret = readFile(pathNameTmp4, fileResTmp4, fileSizeTmp4);

	u64_t fileSizeSum = fileSizeTmp1 + fileSizeTmp2 + fileSizeTmp3 + fileSizeTmp4;//当作参数透传出去
	char* fileResSum = NULL;//当作参数透传出去
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

	string outPath = "./pcm/outFile.pcm";
	FILE* fp = fopen(outPath.c_str(), "w+");
	fwrite(fileResSum, 1, fileSizeSum, fp);
	fclose(fp);

	if (fileResSum != NULL)
	{
		free(fileResSum);
		fileResSum = NULL;
	}

	return 0;
}

u32_t ClassAudioTool::readFile(string& filePath, char* outRes, u32_t fileSize)
{
	FILE* inputHandle = NULL;
	inputHandle = fopen(filePath.c_str(), "r");
	if (inputHandle == NULL)
	{
		printf("open file failure\n");
		return AUDIO_OPEN_FILE_FAILURE;
	}

	u32_t resSize = 0;
	fseek(inputHandle, 0, SEEK_END);
	resSize = ftell(inputHandle);
	fseek(inputHandle, 0, SEEK_SET);

	m_pFileRes = (char*)malloc(resSize);
	memset(m_pFileRes, 0, resSize);
	fread(m_pFileRes, 1, resSize, inputHandle);
	fclose(inputHandle);
	inputHandle = NULL;
	outRes = m_pFileRes;
	fileSize = resSize;
	return AUDIO_SUCCESS;
}
