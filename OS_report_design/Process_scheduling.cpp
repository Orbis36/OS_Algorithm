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
	return a.ArriveTime < b.ArriveTime;//������ʱ����ȵ�������
}
bool cmp2(process a, process b) {
	return a.RunTime < b.RunTime;//������ʱ��Ӷ̵�������
}
bool cmp3(process a, process b) {
	return a.PriorityNum > b.PriorityNum;//�����ȼ��Ӹߵ�������
}

int OS_report_design::FCFS()
{
	int NeedNum = ui.comboBox1->currentText().toInt();
	vector<process> v(NeedNum);
	v = Extract(v, m_gLayout,3,NeedNum,true);//��ȡ����


	sort(v.begin(), v.end(), cmp1);//����
	queue<process> q;
	for (int i = 0; i < NeedNum; i++) {
		q.push(v[i]);//�������ƽ�����
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
		q.pop();//������ɳ�����
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
	while (x != NeedNum || !vec.empty()) {//ִ�е����н��̽����Ҷӿ�
		if (vec.empty() && x < NeedNum) {//�ն�ʱ��һ�����̷������
			vec.push_back(v[x]);
			if (t <= v[x].ArriveTime)
				t = v[x].ArriveTime;
			x++;
		}
		for (int i = x; i < NeedNum; i++) {
			if (t >= v[i].ArriveTime) {//���̵���Ž����ƽ����У���ͬʱ�䵽��һͬ����
				vec.push_back(v[i]);
				x++;
			}
		}
		sort(vec.begin(), vec.end(), cmp2);//����
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
{//���������ҵ������ͬ

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
		sort(vec.begin(), vec.end(), cmp3);//����
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
	cout <<"ʱ��Ƭ��С��"<< T;
	sort(v.begin(), v.end(), cmp1);
	for (int i = 0; i < NeedNum; i++) {//���̳�ʼ��תʱ��Ϊ0
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
		if (q.front().RTime < q.front().RunTime&&q.front().RTime != 0 && flag == 0) {//������׽���δ����Ҳ����״���ת������β
			q.push(q.front());
			q.pop();
		}
		flag = 0;
		if (q.front().RTime == 0) {//�״���ת��ʼʱ��
			q.front().StartTime = t;
		}
		q.front().RTime = q.front().RTime + T;
		t = t + T;
		if (q.front().RTime >= q.front().RunTime) {
			if (q.front().RTime > q.front().RunTime)//ʱ��Ƭʣ��ʱ��
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
			flag = 1;//�н�����ɳ��ӾͲ�����β
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
	queue<process> q[3];//��������
	int x = 0, t = 0, k = 0;
	while (x < NeedNum) {//��������ȫ��ʱ�޽��̵�����һ�����̽���
		q[0].push(v[x]);
		t = v[x].ArriveTime;
		x++;
		while (!q[0].empty()) {//����һ������
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
				q[1].push(q[0].front());//����δ��ɷ����������
				q[0].pop();
			}
			for (int i = x; i < NeedNum; i++) {//�н��̵����һ������
				if (t >= v[i].ArriveTime) {
					q[0].push(v[i]);
					x++;
				}
			}
		}
		while (q[0].empty() && !q[1].empty()) {//һ�����п�ʱ�����������
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
		while (q[0].empty() && q[1].empty() && !q[2].empty()) {//�������п�ʱ������������
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
				q[2].push(q[2].front());//��������������ת
				q[2].pop();
			}
		}
	}
	Cout(v, 0, all);
	return 0;
}

