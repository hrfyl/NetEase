/*#######################################################################
		> File Name: main.cpp
		# Author: 		WIND
		# mail: 		fengleyl@163.com
		> Created Time: 2015年05月23日 星期六 11时35分51秒
########################################################################*/


#include "mainwidget/mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	if (!connectDatabase())
		return 1;
	
	QFile file(":/qss/default");
	file.open(QFile::ReadOnly);
	qApp->setStyleSheet(file.readAll());
	file.close();

	MainWindow w;
	w.show();

	return a.exec();
}
