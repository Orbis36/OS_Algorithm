#pragma once

#include <QtWidgets/QWidget>
#include <QDialog.h>
#include <QSpinBox>
#include "ui_OS_report_design.h"
#include <string.h>
#include "Data_structure.h"

class OS_report_design : public QWidget
{
	Q_OBJECT
public:
	OS_report_design(QWidget *parent = Q_NULLPTR);
signals:

public slots:

	void SelectProcessNumber(int index);
	int FCFS();
	int SJF();
	int FPF();
	int RR();
	int MFQ();
	void Time_slice_GUI();
	void Multi_Level_GUI();
	void spinbox_result_get();
	void Cout(vector<process> v,double time_slice, int level[3]);
	int* muti_level_result_get();
	

	void Data_Generate();
	void Plot_Inputbox();
	void Optimal();
	void FIFO();
	void LRU();
	void Clock();
	void Improved_Clock();

	int Page_Cout(int Page,bool status,int i);
	void Page_Cout_ACC(float count);
	vector<process> Get_extra_data(vector<process> v);
	vector<process> Extract(vector<process> v, QGridLayout* m_g, int col, int line, bool main);//default = 3, 3 col. in main window
private:
	double Result_of_SpinBox = 1;
	bool Go_on = false;
	int *NeedProcess;
	int PageNeed = 0, PageNumRange = 0, PageWinodwSize = 0;


	Ui::OS_report_designClass ui;
	QVector<QLineEdit*> m_lineEditcreate;
	QVector<QLabel *> m_labelcreate;
	QGridLayout *m_gLayout,*g_layer,*g_level_layer,*PageInput;
	QDialog *dialog,*dialog2,*dialog3;
	QVBoxLayout* mainLayout1,*mainLayout2,*mainLayout3;
	QLabel* Label_1,*Label_2;
	QPushButton* Push1,*Push2,*Push3;
	QDoubleSpinBox *pSpinBox;
	QTextBrowser* Browser;
};
