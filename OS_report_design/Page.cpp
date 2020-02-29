#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "OS_report_design.h"
#include "Data_structure.h"
using namespace std;

void OS_report_design::Optimal()//����û��㷨 (��֪δ��ʱ����Ҫʹ�õ�ҳ��)
{
	float count = 0; //�����Ҫ����׼ȷ��
	vector<CPage> C_Page(PageWinodwSize);
	ui.textBrowser->clear();


	for (int i = 0; i < PageWinodwSize; i++)//��ʼ���ڴ�ҳ��
	{
		C_Page[i].ID = -1;
		C_Page[i].nextTime = 999;
		C_Page[i].stayTime = 0;
		C_Page[i].unUseTime = 0;
	}

	for (int i = 0; i < PageNeed; i++)
	{
		int success = 0, unuse = -1;
		//Page_Cout(NeedProcess[i], false);

		for (int j = 0; j < PageWinodwSize; j++)
		{
			if (C_Page[j].ID == NeedProcess[i])   //���е����
			{
				count++;
				success = 1;
				C_Page[j].nextTime = 999;
				for (int k = i + 1; k < PageNeed; k++)
				{
					if (NeedProcess[k] == C_Page[j].ID)
					{
						C_Page[j].nextTime = k;  //������һ��ʹ��ʱ��
						break;
					}
				}
				break;//��Ӧ����Ѱ���´�
			}
			if (C_Page[j].ID < 0)//���Ϊ����ҳ��Ϊ��
			{
				unuse = j;  //��¼��ҳ������
			}
		}
		Page_Cout(NeedProcess[i], success, i);
		if (unuse != -1)   //���п�ҳ��
		{
			C_Page[unuse].ID = NeedProcess[i];//����
			for (int k = i + 1; k < PageNeed; k++)
			{
				if (NeedProcess[k] == C_Page[unuse].ID)
					C_Page[unuse].nextTime = k;
			}
		}
		else//û��������û�п�ҳ��
		{
			int next = 0;
			int nexttime = C_Page[0].nextTime;  //Ѱ�����nexttimeȡ��
			for (int j = 1; j < 4; j++)
			{
				if (C_Page[j].nextTime > nexttime)
				{
					next = j;
					nexttime = C_Page[j].nextTime;
				}
			}
			C_Page[next].ID = NeedProcess[i];//�û�
			for (int k = i + 1; k < PageNeed; k++)
			{
				if (NeedProcess[k] == C_Page[next].ID)
				{
					C_Page[next].nextTime = k;//������֪����ҳ��
					break;
				}
			}
		}
	   }
	cout << count;
	Page_Cout_ACC(count / PageNeed);
	}
	
void OS_report_design::FIFO()//�Ƚ��ȳ��û��㷨
{
	vector<CPage> C_Page(PageWinodwSize);
	ui.textBrowser->clear();
	float count = 0.0;


	for (int i = 0; i < PageWinodwSize; i++)//��ʼ���ڴ�ҳ��
	{
		C_Page[i].ID = -1;
		C_Page[i].stayTime = 0;
		C_Page[i].unUseTime = 0;
	}
	for (int i = 0; i < PageNeed; i++)
	{
		int success = 0, unuse = -1, unuse_t = 0, stay_t = 0, stay;

		for (int j = 0; j < PageWinodwSize; j++)//��ҳ����з��ʣ��鿴�Ƿ��Ѵ���
			{
				if (C_Page[j].ID == NeedProcess[i])//���е����
				{
					count++;
					C_Page[j].stayTime += 1;//ÿ����һ�Σ�ҳ������ʱ���1
					C_Page[j].unUseTime = 0;
					success = 1;//�ɹ����б�־
					break;//������
				}
				else if (C_Page[j].ID < 0)//����û�õ�ҳ��
				{
					C_Page[j].unUseTime += 1;//�Ѿ����û��ʹ��
					if (C_Page[j].unUseTime > unuse_t)
					{
						unuse = j;//������δʹ��
						unuse_t = C_Page[j].unUseTime;//
					}
				}
				else							//ҳ��פ��ʱ���1
				{
					C_Page[j].stayTime += 1;
					if (C_Page[j].stayTime > stay_t)
					{
						stay_t = C_Page[j].stayTime;
						stay = j;
					}
				}
			}
		
		Page_Cout(NeedProcess[i],success,i);
		if (success == 0 && unuse >= 0)//���ڿ���ҳ��
		{
			C_Page[unuse].ID = NeedProcess[i];
			C_Page[unuse].stayTime = 1;
			C_Page[unuse].unUseTime = 0;
		}
		else				//û��������û�п���ҳ��
		{
			C_Page[stay].ID = NeedProcess[i];
			C_Page[stay].stayTime = 1;
			C_Page[stay].unUseTime = 0;
		}
	}
	Page_Cout_ACC(count / PageNeed);
}

void OS_report_design::LRU()//������δʹ���û��㷨
{
	vector<CPage> C_Page(PageWinodwSize);
	ui.textBrowser->clear();
	float count = 0.0;


	for (int i = 0; i < PageWinodwSize; i++)//��ʼ���ڴ�ҳ��
	{
		C_Page[i].ID = -1;
		C_Page[i].stayTime = 0;
		C_Page[i].unUseTime = 0;
	}
	for (int i = 0; i < PageNeed; i++)
	{
		int success = 0, unuse = -1, unuse_t = 0, stay_t = 0, stay;
		for (int j = 0; j < PageWinodwSize; j++)//��ҳ����з��ʣ��鿴�Ƿ��Ѵ���
		{
			if (C_Page[j].ID == NeedProcess[i])//���е����
			{
				count += 1.0;
				C_Page[j].stayTime = 1;//���к�ҳ��ͣ��ʱ����Ϊ1
				C_Page[j].unUseTime = 0;
				success = 1;//�ɹ����б�־
				break;
			}
			else if (C_Page[j].ID < 0)
			{
				C_Page[j].unUseTime += 1;
				if (C_Page[j].unUseTime > unuse_t)
				{
					unuse = j;
					unuse_t = C_Page[j].unUseTime;
				}
			}
			else							//ҳ��פ��ʱ���1
			{
				C_Page[j].stayTime += 1;
				if (C_Page[j].stayTime > stay_t)
				{
					stay_t = C_Page[j].stayTime;
					stay = j;
				}
			}
		}
		
		Page_Cout(NeedProcess[i], success, i);
	    if (success == 0 && unuse >= 0)//���ڿ���ҳ��
		{
			C_Page[unuse].ID = NeedProcess[i];
			C_Page[unuse].stayTime = 1;
			C_Page[unuse].unUseTime = 0;
		}
		else				//û��������û�п���ҳ��
		{
			C_Page[stay].ID = NeedProcess[i];
			C_Page[stay].stayTime = 1;
			C_Page[stay].unUseTime = 0;
		}
	}

	Page_Cout_ACC(count / PageNeed);
}

void OS_report_design::Clock()
{
	vector<CPage> C_Page(PageWinodwSize);
	ui.textBrowser->clear();
	float count = 0.0;
	CPage *p;

	for (int i = 0; i < PageWinodwSize; i++)//��ʼ���ڴ�ҳ��,����������ĸ�ҳ��װ���ڴ���
	{
		C_Page[i].ID = NeedProcess[i];
		C_Page[i].stayTime = 0;
		C_Page[i].unUseTime = 0;
		Page_Cout(NeedProcess[i], 0, i);
	}
	p = &C_Page[0];
	for (int i = 4; i < PageNeed; i++)
	{
		int success = 0;
		//printf("%d  ", NeedProcess[i]);
		for (int j = 0; j < 4; j++)//�鿴�Ƿ�����
		{
			   if (C_Page[j].ID == NeedProcess[i])
			   {
					C_Page[j].stayTime = 1;
					count += 1;
					p = &C_Page[(j + 1) % 4];//����ɹ��͵���p��ָ��ҳ��
					success = 1;
					break;
					//printf("����ҳ�棺%d", NeedProcess[i]);
			   }
		}
		Page_Cout(NeedProcess[i], success, i);
		while (!success)
		{
			if (p->stayTime == 0)
			{
				p->ID = NeedProcess[i];
				p->stayTime = 1;
				success = 1;
				p = &C_Page[(i + 1) % 4];
			}
			else
			{
				p->stayTime = 0;
				p = &C_Page[(i + 1) % 4];
			}
		}
		
	}
	Page_Cout_ACC(count / PageNeed);
}

void OS_report_design::Improved_Clock()
{
	vector<CPage> C_Page(PageWinodwSize);
	ui.textBrowser->clear();
	float count = 0;
	CPage* p;

	for (int i = 0; i < 4; i++)//��ʼ���ڴ�ҳ��,����������ĸ�ҳ��װ���ڴ���
	{
		C_Page[i].ID = NeedProcess[i];
		C_Page[i].stayTime = 0;
		C_Page[i].unUseTime = 0;
		Page_Cout(NeedProcess[i], 0, i);
	}
	p = &C_Page[0];
	for (int i = 4; i < 20; i++)
	{
		int success = 0;
		int s = 1;//��֤�ǵ�һ�η��ʵ���ҳ��
		for (int j = 0; j < 4; j++)//�鿴�Ƿ�����
			{
				if (C_Page[j].ID == NeedProcess[i])
				{
					C_Page[j].stayTime = 1;
					C_Page[j].unUseTime = 0;
					count += 1.0;
					p = &C_Page[(j + 1) % 4];
					success = 1;
					break;
				}
			}
		Page_Cout(NeedProcess[i], success, i);
		while (!success)
		{
			for (int j = 0; j < 4; j++)
				{
					if (C_Page[j].stayTime == 0 && C_Page[j].unUseTime == 0 && s == 1)
					{
						C_Page[j].ID = NeedProcess[i];
						C_Page[j].stayTime = 1;
						C_Page[j].unUseTime = 1;
						success = 1;
						s = 0;
						p = &C_Page[j];
					}
				}
			for (int j = 0; j < 4; j++)
			{
					if (C_Page[j].stayTime == 0 && C_Page[j].unUseTime == 1 && s == 1)
					{
						C_Page[j].ID = NeedProcess[i];
						C_Page[j].stayTime = 1;
						C_Page[j].unUseTime = 1;
						success = 1;
						s = 0;
						p = &C_Page[j];
					}
					else
						C_Page[j].stayTime = 0;
			}
		}
		
	}
	Page_Cout_ACC(count / PageNeed);
}


