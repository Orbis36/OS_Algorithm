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
};//���̽ṹ��

typedef struct Page {
	int	 ID;				//ҳ���
	int nextTime;          //�´η���ʱ��
	int  stayTime;          //�ڴ���פ��ʱ��
	int  unUseTime;   //�Ѿ����δ��ʹ��
}CPage;//ҳ��ṹ��
