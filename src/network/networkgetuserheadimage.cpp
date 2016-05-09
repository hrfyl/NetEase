#include "networkgetuserheadimage.h"
#include "../configure/urlconfigure.h"
#include "../configure/configure.h"

//#include <QPixmap>
//#include <QFile>

NetworkGetUserHeadImage::NetworkGetUserHeadImage(QObject *parent) :
	NetworkBase(parent)
{
}

void NetworkGetUserHeadImage::userHeadImage()
{
	QUrl url(Configure::userHeadImageUrl + "?param=48y48");
	sendGet(url);
}

void NetworkGetUserHeadImage::disposeifno(const QByteArray &data)
{
	emit userHeadImageGeted(data);

//	QFile file("./test.jpg");
//	if (!file.open(QIODevice::WriteOnly))
//	{
//		qDebug() << "open file error";
//		return;
//	}
//	file.write(data);
//	file.flush();
//	file.close();
}
