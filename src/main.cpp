#include "mainwindow/mainwindow.h"
#include "configure/configure.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
	if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
		qputenv("QT_LOGGING_RULES", "qt.network.ssl.warning=false");

	QApplication a(argc, argv);

	// 初始化配置...
	Configure::initConfig();

	// 设置应用qss样式..
	QFile file(Configure::qssPath);
	file.open(QFile::ReadOnly);
	qApp->setStyleSheet(file.readAll());
	file.close();

	MainWindow w;
	w.show();

	return a.exec();
}
