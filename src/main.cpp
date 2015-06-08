/*#######################################################################
		> File Name: main.cpp
		# Author: 		WIND
		# mail: 		fengleyl@163.com
		> Created Time: 2015年05月23日 星期六 11时35分51秒
########################################################################*/

#include "mainwidget/mainwindow.h"
#include "Data/sqlitedata.h"
#include <QApplication>
#include <QFile>
#include <QDir>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QString userName;
	if (!connectDatabase(userName))
		return 1;
#ifndef WIN32
	QFile file("/home/" + userName + "/.cache/NetEase/resource/qss/default.qss");
#endif
#ifdef WIN32
	QFile file("./resource/qss/default.qss");
#endif
	file.open(QFile::ReadOnly);
	qApp->setStyleSheet(file.readAll());
	file.close();

	MainWindow w;
	w.show();

	return a.exec();
}
