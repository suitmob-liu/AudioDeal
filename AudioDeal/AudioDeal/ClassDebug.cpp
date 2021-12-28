#include "ClassDebug.h"

#include <Windows.h>

#pragma warning(disable:4996)

ClassDebug::ClassDebug()
{
	cout << "ClassDebug be create" << endl;
	m_pFileLog = fopen("./audioLog.log", "ab+");
}

ClassDebug::~ClassDebug()
{
	cout << "ClassDebug be destory" << endl;
	if (m_pFileLog != NULL)
	{
		fclose(m_pFileLog);
		m_pFileLog = NULL;
	}

}

ClassDebug& ClassDebug::getInStance()
{
	static ClassDebug m_debugInstance;
	return m_debugInstance;
}

void ClassDebug::debugLog(string& inputDebug, u16_t logLevel)
{
	string time = getCurrentTime();
	string writeData = "";
	writeData += "[";
	writeData += time;
	writeData += "]";
	writeData += "  ";
	writeData += inputDebug;
	writeData += "\n";
	//printf("write data is %s\n", writeData.c_str());

	fwrite(writeData.c_str(), 1, writeData.size(), m_pFileLog);
}

void ClassDebug::debugLog(const char* inputChar, u16_t logLevel)
{
	string time = getCurrentTime();
	string inputDebug = inputChar;
	string writeData = "";
	writeData += "[";
	writeData += time;
	writeData += "]";
	writeData += "  ";
	writeData += inputDebug;
	writeData += "\n";
	printf("write data is %s\n", writeData.c_str());

	fwrite(writeData.c_str(), 1, writeData.size(), m_pFileLog);
}

string ClassDebug::getCurrentTime()
{
#if 0 
	time_t nowTime;
	nowTime = time(NULL);
	struct tm* sysTime = localtime(&nowTime);
	/*
	  美国国家标准协会规定 C99(C++11)
	  tm_year:从1900年开始计算，所以如果直接打印tm_year，将得到1900至当前年份已过去的年数,如现在是2015年，则打印115,所以要加1900
	  tm_mon:是由下标为0开始计算的，即0代表1月份，所以要加1，比如，当前是三月份，如果不加1，则显示是2.
	  其它略，可参考C99或C++11标准规定。
	*/
	printf("系统时间：%d年%d月%d日%d时%d分\n", sysTime->tm_year + 1900, sysTime->tm_mon + 1, sysTime->tm_mday, sysTime->tm_hour, sysTime->tm_min);
	// TODO: 在此处插入 return 语句
	
	char tmpTime[128] = { 0 };
	sprintf(tmpTime, "%d-%d-%d_%d-%d-%d", sysTime->tm_year + 1900, sysTime->tm_mon + 1, sysTime->tm_mday, sysTime->tm_hour, sysTime->tm_min, sysTime->tm_sec);
	string str(tmpTime);
	printf("str size is %d\n", str.size());

#endif
	char str_time[50] = {0};
	SYSTEMTIME local_time, base_local_time;
	FILETIME file_time, base_file_time;
	ULARGE_INTEGER uli, base_uli;
	int usec;
#ifdef NEED_WEEK_TIME
	const char* week_day[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
#endif // NEED_WEEK_TIME
	GetLocalTime(&local_time);
	SystemTimeToFileTime(&local_time, &file_time);
	uli.LowPart = file_time.dwLowDateTime;
	uli.HighPart = file_time.dwHighDateTime;
	memset(&base_local_time, 0, sizeof(base_local_time));
	base_local_time.wYear = 1970;
	base_local_time.wMonth = 1;
	base_local_time.wDay = 1;
	base_local_time.wDayOfWeek = 5;
	SystemTimeToFileTime(&base_local_time, &base_file_time);
	base_uli.LowPart = base_file_time.dwLowDateTime;
	base_uli.HighPart = base_file_time.dwHighDateTime;
	usec = (int)((uli.QuadPart / 10 - base_uli.QuadPart / 10) % 1000000);
#ifdef NEED_WEEK_TIME
	sprintf_s(str_time, sizeof(str_time), "%04d-%02d-%02d.%s._%02d-%02d-%02d.%06d", local_time.wYear, local_time.wMonth,
		local_time.wDay, week_day[local_time.wDayOfWeek], local_time.wHour, local_time.wMinute, local_time.wSecond, usec);
#else
	sprintf_s(str_time, sizeof(str_time), "%04d-%02d-%02d_%02d-%02d-%02d.%06d", local_time.wYear, local_time.wMonth,
		local_time.wDay, local_time.wHour, local_time.wMinute, local_time.wSecond, usec);
#endif // NEED_WEEK_TIME

	return string(str_time);
}
