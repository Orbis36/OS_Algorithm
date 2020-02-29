#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "OS_report_design.h"
#include "Data_structure.h"
using namespace std;

void OS_report_design::Optimal()//最佳置换算法 (已知未来时间内要使用的页面)
{
	float count = 0; //最后需要计算准确率
	vector<CPage> C_Page(PageWinodwSize);
	ui.textBrowser->clear();


	for (int i = 0; i < PageWinodwSize; i++)//初始化内存页面
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
			if (C_Page[j].ID == NeedProcess[i])   //命中的情况
			{
				count++;
				success = 1;
				C_Page[j].nextTime = 999;
				for (int k = i + 1; k < PageNeed; k++)
				{
					if (NeedProcess[k] == C_Page[j].ID)
					{
						C_Page[j].nextTime = k;  //更新下一次使用时间
						break;
					}
				}
				break;//不应该再寻找下次
			}
			if (C_Page[j].ID < 0)//如果为还有页面为空
			{
				unuse = j;  //纪录空页面的序号
			}
		}
		Page_Cout(NeedProcess[i], success, i);
		if (unuse != -1)   //还有空页面
		{
			C_Page[unuse].ID = NeedProcess[i];//放入
			for (int k = i + 1; k < PageNeed; k++)
			{
				if (NeedProcess[k] == C_Page[unuse].ID)
					C_Page[unuse].nextTime = k;
			}
		}
		else//没有命中且没有空页面
		{
			int next = 0;
			int nexttime = C_Page[0].nextTime;  //寻找最长的nexttime取代
			for (int j = 1; j < 4; j++)
			{
				if (C_Page[j].nextTime > nexttime)
				{
					next = j;
					nexttime = C_Page[j].nextTime;
				}
			}
			C_Page[next].ID = NeedProcess[i];//置换
			for (int k = i + 1; k < PageNeed; k++)
			{
				if (NeedProcess[k] == C_Page[next].ID)
				{
					C_Page[next].nextTime = k;//事先已知所有页面
					break;
				}
			}
		}
	   }
	cout << count;
	Page_Cout_ACC(count / PageNeed);
	}
	
void OS_report_design::FIFO()//先进先出置换算法
{
	vector<CPage> C_Page(PageWinodwSize);
	ui.textBrowser->clear();
	float count = 0.0;


	for (int i = 0; i < PageWinodwSize; i++)//初始化内存页面
	{
		C_Page[i].ID = -1;
		C_Page[i].stayTime = 0;
		C_Page[i].unUseTime = 0;
	}
	for (int i = 0; i < PageNeed; i++)
	{
		int success = 0, unuse = -1, unuse_t = 0, stay_t = 0, stay;

		for (int j = 0; j < PageWinodwSize; j++)//对页面进行访问，查看是否已存在
			{
				if (C_Page[j].ID == NeedProcess[i])//命中的情况
				{
					count++;
					C_Page[j].stayTime += 1;//每访问一次，页面滞留时间加1
					C_Page[j].unUseTime = 0;
					success = 1;//成功命中标志
					break;//不继续
				}
				else if (C_Page[j].ID < 0)//还有没用的页面
				{
					C_Page[j].unUseTime += 1;//已经多久没被使用
					if (C_Page[j].unUseTime > unuse_t)
					{
						unuse = j;//表明其未使用
						unuse_t = C_Page[j].unUseTime;//
					}
				}
				else							//页面驻留时间加1
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
		if (success == 0 && unuse >= 0)//存在空闲页面
		{
			C_Page[unuse].ID = NeedProcess[i];
			C_Page[unuse].stayTime = 1;
			C_Page[unuse].unUseTime = 0;
		}
		else				//没有命中且没有空闲页面
		{
			C_Page[stay].ID = NeedProcess[i];
			C_Page[stay].stayTime = 1;
			C_Page[stay].unUseTime = 0;
		}
	}
	Page_Cout_ACC(count / PageNeed);
}

void OS_report_design::LRU()//最近最久未使用置换算法
{
	vector<CPage> C_Page(PageWinodwSize);
	ui.textBrowser->clear();
	float count = 0.0;


	for (int i = 0; i < PageWinodwSize; i++)//初始化内存页面
	{
		C_Page[i].ID = -1;
		C_Page[i].stayTime = 0;
		C_Page[i].unUseTime = 0;
	}
	for (int i = 0; i < PageNeed; i++)
	{
		int success = 0, unuse = -1, unuse_t = 0, stay_t = 0, stay;
		for (int j = 0; j < PageWinodwSize; j++)//对页面进行访问，查看是否已存在
		{
			if (C_Page[j].ID == NeedProcess[i])//命中的情况
			{
				count += 1.0;
				C_Page[j].stayTime = 1;//命中后页面停留时间置为1
				C_Page[j].unUseTime = 0;
				success = 1;//成功命中标志
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
			else							//页面驻留时间加1
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
	    if (success == 0 && unuse >= 0)//存在空闲页面
		{
			C_Page[unuse].ID = NeedProcess[i];
			C_Page[unuse].stayTime = 1;
			C_Page[unuse].unUseTime = 0;
		}
		else				//没有命中且没有空闲页面
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

	for (int i = 0; i < PageWinodwSize; i++)//初始化内存页面,并把最早的四个页面装入内存中
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
		for (int j = 0; j < 4; j++)//查看是否命中
		{
			   if (C_Page[j].ID == NeedProcess[i])
			   {
					C_Page[j].stayTime = 1;
					count += 1;
					p = &C_Page[(j + 1) % 4];//如果成功就迭代p的指向页面
					success = 1;
					break;
					//printf("命中页面：%d", NeedProcess[i]);
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

	for (int i = 0; i < 4; i++)//初始化内存页面,并把最早的四个页面装入内存中
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
		int s = 1;//保证是第一次访问到的页面
		for (int j = 0; j < 4; j++)//查看是否命中
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


