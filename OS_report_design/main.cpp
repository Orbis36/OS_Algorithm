#include "OS_report_design.h"
#include <QtWidgets/QApplication>
#include <QThread>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	OS_report_design w;
	w.show();

	return a.exec();
}
