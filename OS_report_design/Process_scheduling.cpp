#include "OS_report_design.h"
#include "Data_structure.h"
#include <QLineEdit>
#include <QPushButton>
#include <iostream>
#include <queue>
#include <QElapsedTimer>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;  

bool cmp1(process a, process b) {
	return a.ArriveTime < b.ArriveTime;//按到达时间从先到后排序
}
bool cmp2(process a, process b) {
	return a.RunTime < b.RunTime;//按运行时间从短到长排序
}
bool cmp3(process a, process b) {
	return a.PriorityNum > b.PriorityNum;//按优先级从高到低排序
}

int OS_report_design::FCFS()
{
	int NeedNum = ui.comboBox1->currentText().toInt();
	vector<process> v(NeedNum);
	v = Extract(v, m_gLayout,3,NeedNum,true);//获取数据


	sort(v.begin(), v.end(), cmp1);//排序
	queue<process> q;
	for (int i = 0; i < NeedNum; i++) {
		q.push(v[i]);//把数组推进队列
	}
	int t = 0, k = 0;
	while (!q.empty()) {
		if (t >= q.front().ArriveTime) {
			q.front().StartTime = t;
		}
		else {
			q.front().StartTime = q.front().ArriveTime;
		}
		q.front().EndTime = q.front().StartTime + q.front().RunTime;
		t = q.front().EndTime;
		q.front().TTime = q.front().EndTime - q.front().ArriveTime;
		q.front().WTTime = static_cast<double>(q.front().TTime) / q.front().RunTime;
		v[k].Name = q.front().Name;
		v[k].StartTime = q.front().StartTime;
		v[k].EndTime = q.front().EndTime;
		v[k].TTime = q.front().TTime;
		v[k].WTTime = q.front().WTTime;
		k++;
		q.pop();//进程完成出队列
	}
	Cout(v,0,NULL);
	return 0;

}

int OS_report_design::SJF()
{
	int NeedNum = ui.comboBox1->currentText().toInt();
	vector<process> v(NeedNum);
	v = Extract(v,m_gLayout,3,NeedNum,true);


	sort(v.begin(), v.end(), cmp1);
	vector<process> vec;
	int x = 0, t = 0, k = 0;
	while (x != NeedNum || !vec.empty()) {//执行到所有进程进队且队空
		if (vec.empty() && x < NeedNum) {//空队时将一个进程放入队列
			vec.push_back(v[x]);
			if (t <= v[x].ArriveTime)
				t = v[x].ArriveTime;
			x++;
		}
		for (int i = x; i < NeedNum; i++) {
			if (t >= v[i].ArriveTime) {//进程到达才将其推进队列，相同时间到达一同进队
				vec.push_back(v[i]);
				x++;
			}
		}
		sort(vec.begin(), vec.end(), cmp2);//排序
		vec[0].StartTime = t;
		vec[0].EndTime = vec[0].StartTime + vec[0].RunTime;
		t = vec[0].EndTime;
		vec[0].TTime = vec[0].EndTime - vec[0].ArriveTime;
		vec[0].WTTime = static_cast<double>(vec[0].TTime) / vec[0].RunTime;
		v[k].Name = vec[0].Name;
		v[k].StartTime = vec[0].StartTime;
		v[k].EndTime = vec[0].EndTime;
		v[k].TTime = vec[0].TTime;
		v[k].WTTime = vec[0].WTTime;
		k++;
		vec.erase(vec.begin());
	}
	Cout(v,0,NULL);
	return 0;
}

int OS_report_design::FPF() 
{//流程与短作业优先相同

	int NeedNum = ui.comboBox1->currentText().toInt();
	vector<process> v(NeedNum);
	v = Extract(v,m_gLayout,3, NeedNum,true);
	v = Get_extra_data(v);
	cout << v[0].PriorityNum << v[1].PriorityNum << v[2].PriorityNum;


	sort(v.begin(), v.end(), cmp1);
	vector<process> vec;
	int x = 0, t = 0, k = 0;
	while (x != NeedNum || !vec.empty()) {
		if (vec.empty() && x < NeedNum) {
			vec.push_back(v[x]);
			if (t <= v[x].ArriveTime)
				t = v[x].ArriveTime;
			x++;
		}
		for (int i = x; i < NeedNum; i++) {
			if (t >= v[i].ArriveTime) {
				vec.push_back(v[i]);
				x++;
			}
		}
		sort(vec.begin(), vec.end(), cmp3);//排序
		vec[0].StartTime = t;
		vec[0].EndTime = vec[0].StartTime + vec[0].RunTime;
		t = vec[0].EndTime;
		vec[0].TTime = vec[0].EndTime - vec[0].ArriveTime;
		vec[0].WTTime = static_cast<double>(vec[0].TTime) / vec[0].RunTime;
		v[k].Name = vec[0].Name;
		v[k].StartTime = vec[0].StartTime;
		v[k].EndTime = vec[0].EndTime;
		v[k].TTime = vec[0].TTime;
		v[k].WTTime = vec[0].WTTime;
		k++;
		vec.erase(vec.begin());
	}
	Cout(v,0,NULL);
	return 0;
}

int OS_report_design::RR()
{
	int NeedNum = ui.comboBox1->currentText().toInt();
	vector<process> v(NeedNum);
	
	v = Extract(v, m_gLayout, 3, NeedNum, true);
	Time_slice_GUI();
	while (!Go_on)
	{
		QElapsedTimer et;
		et.start();
		while (et.elapsed() < 500)
			QCoreApplication::processEvents();
	}
	Go_on = false;

	double T = Result_of_SpinBox;
	cout <<"时间片大小："<< T;
	sort(v.begin(), v.end(), cmp1);
	for (int i = 0; i < NeedNum; i++) {//进程初始轮转时间为0
		v[i].RTime = 0;
	}
	queue<process> q;
	int x = 0, t = 0, k = 0, flag = 0;
	while (x != NeedNum || !q.empty()) {
		if (q.empty() && x < NeedNum) {
			q.push(v[x]);
			if (t <= v[x].ArriveTime)
				t = v[x].ArriveTime;
			x++;
		}
		for (int i = x; i < NeedNum; i++) {
			if (t >= v[i].ArriveTime) {
				q.push(v[i]);
				x++;
			}
		}
		if (q.front().RTime < q.front().RunTime&&q.front().RTime != 0 && flag == 0) {//如果队首进程未完成且不是首次轮转，进队尾
			q.push(q.front());
			q.pop();
		}
		flag = 0;
		if (q.front().RTime == 0) {//首次轮转开始时间
			q.front().StartTime = t;
		}
		q.front().RTime = q.front().RTime + T;
		t = t + T;
		if (q.front().RTime >= q.front().RunTime) {
			if (q.front().RTime > q.front().RunTime)//时间片剩余时间
				t = t - (q.front().RTime - q.front().RunTime);
			q.front().EndTime = t;
			q.front().TTime = q.front().EndTime - q.front().ArriveTime;
			q.front().WTTime = static_cast<double>(q.front().TTime) / q.front().RunTime;
			v[k].Name = q.front().Name;
			v[k].StartTime = q.front().StartTime;
			v[k].EndTime = q.front().EndTime;
			v[k].TTime = q.front().TTime;
			v[k].WTTime = q.front().WTTime;
			k++;
			q.pop();
			flag = 1;//有进程完成出队就不进队尾
		}
	}
	Cout(v,T,NULL);
	return 0;
}

int OS_report_design::MFQ()
{
	int NeedNum = ui.comboBox1->currentText().toInt();
	int T1,T2,T3,*all;
	vector<process> v(NeedNum);
	v = Extract(v, m_gLayout, 3, NeedNum,true);
	Multi_Level_GUI();
	while (!Go_on)
	{
		QElapsedTimer et;
		et.start();
		while (et.elapsed() < 500)
			QCoreApplication::processEvents();
	}
	Go_on = false;
	
	all = muti_level_result_get();
	T1 = all[0];
	T2 = all[1];
	T3 = all[2];
	cout << T1 << T2 << T3;

	sort(v.begin(), v.end(), cmp1);
	for (int i = 0; i < NeedNum; i++) {
		v[i].RTime = 0;
	}
	queue<process> q[3];//三级队列
	int x = 0, t = 0, k = 0;
	while (x < NeedNum) {//三级队列全空时无进程到达让一个进程进队
		q[0].push(v[x]);
		t = v[x].ArriveTime;
		x++;
		while (!q[0].empty()) {//处理一级队列
			q[0].front().StartTime = t;
			q[0].front().RTime = q[0].front().RTime + T1;
			t = t + T1;
			if (q[0].front().RTime >= q[0].front().RunTime) {
				if (q[0].front().RTime > q[0].front().RunTime)
					t = t - (q[0].front().RTime - q[0].front().RunTime);
				q[0].front().EndTime = t;
				q[0].front().TTime = q[0].front().EndTime - q[0].front().ArriveTime;
				q[0].front().WTTime = static_cast<double>(q[0].front().TTime) / q[0].front().RunTime;
				v[k].Name = q[0].front().Name;
				v[k].StartTime = q[0].front().StartTime;
				v[k].EndTime = q[0].front().EndTime;
				v[k].TTime = q[0].front().TTime;
				v[k].WTTime = q[0].front().WTTime;
				k++;
				q[0].pop();
			}
			else {
				q[1].push(q[0].front());//进程未完成放入二级队列
				q[0].pop();
			}
			for (int i = x; i < NeedNum; i++) {//有进程到达进一级队列
				if (t >= v[i].ArriveTime) {
					q[0].push(v[i]);
					x++;
				}
			}
		}
		while (q[0].empty() && !q[1].empty()) {//一级队列空时处理二级队列
			q[1].front().RTime = q[1].front().RTime + T2;
			t = t + T2;
			if (q[1].front().RTime >= q[1].front().RunTime) {
				if (q[1].front().RTime > q[1].front().RunTime)
					t = t - (q[1].front().RTime - q[1].front().RunTime);
				q[1].front().EndTime = t;
				q[1].front().TTime = q[1].front().EndTime - q[1].front().ArriveTime;
				q[1].front().WTTime = static_cast<double>(q[1].front().TTime) / q[1].front().RunTime;
				v[k].Name = q[1].front().Name;
				v[k].StartTime = q[1].front().StartTime;
				v[k].EndTime = q[1].front().EndTime;
				v[k].TTime = q[1].front().TTime;
				v[k].WTTime = q[1].front().WTTime;
				k++;
				q[1].pop();
			}
			else {
				q[2].push(q[1].front());
				q[1].pop();
			}
		}
		while (q[0].empty() && q[1].empty() && !q[2].empty()) {//二级队列空时处理三级队列
			q[2].front().RTime = q[2].front().RTime + T3;
			t = t + T3;
			if (q[2].front().RTime >= q[2].front().RunTime) {
				if (q[2].front().RTime > q[2].front().RunTime)
					t = t - (q[2].front().RTime - q[2].front().RunTime);
				q[2].front().EndTime = t;
				q[2].front().TTime = q[2].front().EndTime - q[2].front().ArriveTime;
				q[2].front().WTTime = static_cast<double>(q[2].front().TTime) / q[2].front().RunTime;
				v[k].Name = q[2].front().Name;
				v[k].StartTime = q[2].front().StartTime;
				v[k].EndTime = q[2].front().EndTime;
				v[k].TTime = q[2].front().TTime;
				v[k].WTTime = q[2].front().WTTime;
				k++;
				q[2].pop();
			}
			else {
				q[2].push(q[2].front());//在三级队列中轮转
				q[2].pop();
			}
		}
	}
	Cout(v, 0, all);
	return 0;
}

