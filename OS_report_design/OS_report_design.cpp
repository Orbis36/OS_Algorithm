#include "OS_report_design.h"
#include <iostream>
#include <string>
#include <cstdlib>

#include <qdebug>
#include <QLineEdit>
#include <QMessageBox>
#include <QElapsedTimer>
#include <QSpinBox>
#include <QTextBrowser>
#include "Data_structure.h"
//#include "Process_scheduling.cpp"
using namespace std;


OS_report_design::OS_report_design(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//this->setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);//叠加以|形式
	//this->setWindowFlags(Qt::FramelessWindowHint);//去除框体
	//this->setWindowFlag(Qt::WindowMinimizeButtonHint)//禁用最小化按钮，具体括号内查手册
	//connect(ui.tests, SIGNAL(clicked()), this, SLOT(TestSlot()));
}

void OS_report_design::SelectProcessNumber(int index)
{
	m_gLayout = ui.Box_Grid;
	int itemindex = 0;
	int NeedNum = ui.comboBox1->currentText().toInt();
	m_lineEditcreate.clear();

	if (m_gLayout)
	{
		int widget_num = m_gLayout->count();
		for (int i = 0; i < widget_num; i++)
		{
			QWidget* widget = m_gLayout->itemAt(0)->widget();
			m_gLayout->removeWidget(widget);
			delete widget;
		}
	}
	for (int i = 0; i < NeedNum*3; i++)
		m_lineEditcreate.append(new QLineEdit());//lineedit为单行可输入文本控件

	for (int i = 0; i < NeedNum; i++)
		for (int j = 0; j < 3; j++)
		{
			m_gLayout->addWidget(m_lineEditcreate.at(itemindex),i,j);
			itemindex++;
		}
}

vector<process> OS_report_design::Extract(vector<process> v,QGridLayout* m_g, int col,int lines,bool status)
{
	
	//m_gLayout = ui.Box_Grid;
	//vector<process> v(NeedNum);

	for (int columns = 0; columns < col; columns++)
	{
		for (int line = 0; line < lines; line++)
		{
			QString text_q;
			QLayoutItem* p = m_g->itemAtPosition(line, columns);
			QLineEdit *s = qobject_cast<QLineEdit*>(p->widget());
			if (status)//main window
			{
				text_q = s->text();
				if (columns == 0 && !text_q.isEmpty())
					v[line].Name.assign(text_q.toStdString());
				else if (columns == 1 && !text_q.isEmpty())//arrive time can be zero
					v[line].ArriveTime = text_q.toFloat();
				else if (columns == 2 && text_q.toFloat())
					v[line].RunTime = text_q.toFloat();
				else if (text_q.isEmpty())
				{
					QMessageBox::information(NULL, "Error", "Please enter all information,\nwhere the second and third column should be numbers(Int or Float)");
					return v;
				}
			}
			else if (!status)
			{
				if(columns != 1)
					break;
				text_q = s->text();
				if(text_q.toFloat())
					v[line].PriorityNum = text_q.toFloat();
			}
             //when we use function in dialog, just extract second line, the turns of conditions is important here
	
		}
	}
	return v;
}

vector<process> OS_report_design::Get_extra_data(vector<process> v)
{
	QFont font;
	dialog = new QDialog(this);
	dialog->setModal(false);
	dialog->resize(200, 400);
	mainLayout1 = new QVBoxLayout(dialog);
	g_layer = new QGridLayout();//Initialize the variables already declared in .h
	font.setBold(false);
	font.setWeight(50);


	m_lineEditcreate.clear();
	m_labelcreate.clear();


	Label_1 = new QLabel();
	Push1 = new QPushButton("Over");
	Label_1->setObjectName(QStringLiteral("label_sub_1"));
	Label_1->setText("Please enter each process \n priority (as a number)");
	mainLayout1->addWidget(Label_1);

	for (int i = 0; i < v.size(); i++)
	{
		m_lineEditcreate.append(new QLineEdit());//lineedit为单行可输入文本控件
		m_labelcreate.append(new QLabel(v[i].Name.c_str()));
	}
	for (int i = 0; i < v.size(); i++)//只有两列，标签和输入框
	{
		for (int j = 0; j < 2; j++)
		{
			g_layer->addWidget(m_labelcreate.at(i), i, j);
			g_layer->addWidget(m_lineEditcreate.at(i), i, j + 1);
		}
	}
	
	QObject::connect(Push1, SIGNAL(clicked()), dialog, SLOT(close()));

	mainLayout1->addLayout(g_layer);
	mainLayout1->addWidget(Push1);
	mainLayout1->addStretch(1);
	dialog->show();

	QString text_q;
	for (int i = 0; i < m_lineEditcreate.size(); i++)//scan the editbox in this dialog,until they all haven written data.
	{
		QLayoutItem* p = g_layer->itemAtPosition(i, 1);
		QLineEdit *s = qobject_cast<QLineEdit*>(p->widget());
		text_q = s->text();
		while (text_q.isEmpty())
		{
			QElapsedTimer et;
			et.start();
			while (et.elapsed() < 1000)
				QCoreApplication::processEvents();
			text_q = s->text();//update
		}
	}
	int NeedNum = ui.comboBox1->currentText().toInt();
	v = Extract(v, g_layer,2,NeedNum,false);
	return v;
	
}

void OS_report_design::Time_slice_GUI()
{
	dialog2 = new QDialog(this);
	dialog2->setModal(true);
	dialog2->resize(300, 300);
	mainLayout2 = new QVBoxLayout(dialog2);

	pSpinBox = new QDoubleSpinBox(this);
	pSpinBox->setRange(0, 10);
	pSpinBox->setSingleStep(0.2);
	pSpinBox->setDecimals(1);
	pSpinBox->setValue(3);
	pSpinBox->setSuffix(" s");
	pSpinBox->setWrapping(true);

	
	Push2 = new QPushButton("Over");

	QObject::connect(Push2, SIGNAL(clicked()), this, SLOT(spinbox_result_get()));
	QObject::connect(Push2, SIGNAL(clicked()), dialog2, SLOT(close()));

	mainLayout2->addWidget(pSpinBox);
	mainLayout2->addWidget(Push2);
	mainLayout2->addStretch(1);
	dialog2->show();

}
void OS_report_design::spinbox_result_get()
{
	Result_of_SpinBox = pSpinBox->value();
	Go_on = true;
}


void OS_report_design::Multi_Level_GUI()
{
	dialog3 = new QDialog(this);
	dialog3->setModal(true);
	dialog3->resize(300, 300);

	mainLayout3 = new QVBoxLayout(dialog3);
	g_level_layer = new QGridLayout();

	Label_2 = new QLabel();
	Label_2->setObjectName(QStringLiteral("label_sub_2"));
	Label_2->setText("Please enter the time slice size for \nthe three-level queue.The size of the time \nslice should increase");

	Push3 = new QPushButton("Over");
	
	m_lineEditcreate.clear();
	m_labelcreate.clear();

	for (int i = 0; i < 3; i++)
	{
		m_lineEditcreate.append(new QLineEdit());//lineedit为单行可输入文本控件
		m_labelcreate.append(new QLabel(QString("No.%1 level queue").arg(i+1)));
	}
	for (int i = 0; i < 3; i++)//只有两列，标签和输入框
	{
		for (int j = 0; j < 2; j++)
		{
			g_level_layer->addWidget(m_labelcreate.at(i), i, j);
			g_level_layer->addWidget(m_lineEditcreate.at(i), i, j + 1);
		}
	}
	mainLayout3->addWidget(Label_2);
	mainLayout3->addLayout(g_level_layer);
	mainLayout3->addWidget(Push3);
	mainLayout3->addStretch(1);
	dialog3->show();

	QObject::connect(Push3, SIGNAL(clicked()), this, SLOT(muti_level_result_get()));
	QObject::connect(Push3, SIGNAL(clicked()), dialog3, SLOT(close()));
}
int* OS_report_design::muti_level_result_get()
{
	int T[3];
	for (int i = 0;i < 3; i++)
	{
		QString text_q;
		QLayoutItem* p = g_level_layer->itemAtPosition(i,1);
		QLineEdit *s = qobject_cast<QLineEdit*>(p->widget());
		T[i] = s->text().toInt();
	}
	Go_on = true;
	return T;
}

void OS_report_design::Cout(vector<process> v,double time_slice,int level[3])
{
	QString extra;
	Browser = ui.textBrowser;
	Browser->clear();

	int NeedNum = ui.comboBox1->currentText().toInt();
	double ATTime = 0, AWTTime = 0;
	for (int i = 0; i < NeedNum; i++)
	{
		ATTime = ATTime + v[i].TTime;
		AWTTime = AWTTime + v[i].WTTime;
	}
	ATTime = static_cast<double>(ATTime) / NeedNum;//计算平均周转时间和带权平均周转时间
	AWTTime = AWTTime / NeedNum;
	
	
	QString qsStr = "Name	ArriveTime	EndTime	TurnaroundTime WeightedTTime\n";
	Browser->append(qsStr);
	for (int i = 0; i < NeedNum; i++)
	{
		QString data;
		data = QString(v[i].Name.c_str());
		data += QString("	%1	%2	%3	 %4\n").arg(v[i].StartTime).arg(v[i].EndTime).arg(v[i].TTime).arg(v[i].WTTime);
		Browser->append(data);
	}

	extra = QString("\nAverage turnaround time is:%1\nThe average weighted turnaround time is:%2").arg(ATTime).arg(AWTTime);
	if (time_slice)
		extra += QString("\nThe size of time slice is %1").arg(time_slice);
	if (level)
		extra += QString("\nThe size of time slice are %1,%2,%3").arg(level[0]).arg(level[1]).arg(level[2]);
	Browser->append(extra);
	
}

void OS_report_design::Data_Generate()
{
	PageNeed = 20,PageNumRange = 10,PageWinodwSize = 4;
	NeedProcess = new int[PageNeed];//delete it after used,not in this function
	srand(time(NULL));
	for (int i = 0; i < PageNeed; i++)
		NeedProcess[i] = (rand() % PageNumRange);
	
	QMessageBox::information(NULL, "SUCESS", "The required data was successfully created.\nThe page size that can be stored is 4.\n We will test 20 pages,please choose algorithm");

}
void OS_report_design::Plot_Inputbox()
{
	//PageInput = ui.gridLayout;

}

int OS_report_design::Page_Cout(int Page, bool status,int i)
{
	Browser = ui.textBrowser;
	QString data;
	data += QString("%1th. page request,Page %2 is accessed	").arg(i+1).arg(Page);
	if(status)
	{
		data += QString("Hitted!\n");
		Browser->append(data);
		return 0;
	}
	data += QString("\n");
	Browser->append(data);
	return 0;
}
void OS_report_design::Page_Cout_ACC(float count )
{
	Browser = ui.textBrowser;
	QString data;
	data += QString("The probability of a page being hit is:%1").arg(count);
	Browser->append(data);
	delete[] NeedProcess;
}