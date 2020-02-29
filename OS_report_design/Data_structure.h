#pragma once
#include <string.h>
#include <iostream>
using namespace std;

struct process {
	string Name;
	float ArriveTime;
	float RunTime;
	int PriorityNum;
	float StartTime;
	float EndTime;
	float TTime;
	double WTTime;
	float RTime;
};//进程结构体

typedef struct Page {
	int	 ID;				//页面号
	int nextTime;          //下次访问时间
	int  stayTime;          //内存中驻留时间
	int  unUseTime;   //已经多久未被使用
}CPage;//页面结构体
