#pragma once
/*
*类说明：ClassDebug专注于解决程序打印问题
*需添加程序的日志打印等级；日志打印对应的系统时间；日志内容等
*/

#include "generalType.h"

class ClassDebug
{
private:
	ClassDebug();

public:
	~ClassDebug();
	//获取单例函数接口
	static ClassDebug& getInStance();
	//传入字符串写入日志文件
	void debugLog(string& inputDebug, u16_t logLevel = 1);

private:
	//获取系统时间，当前精度为秒，后续需要修改为毫秒级
	string getCurrentTime();

private:
	FILE *m_pFileLog;

};

