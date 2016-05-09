#include "networkbase.h"
#include "../configure/configure.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#ifdef QT_DEBUG
#include <QDebug>
#endif

NetworkBase::NetworkBase(QObject *parent, bool blockade) :
	QObject(parent)
{
	if (blockade)
		return;
	m_accessManage = new QNetworkAccessManager(this);
	connect(m_accessManage, SIGNAL(finished(QNetworkReply*)),
			this, SLOT(finished(QNetworkReply*)));
}

void NetworkBase::initHead(QNetworkRequest &request)
{
	request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
	request.setRawHeader("Accept-Language", "zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3");
	request.setRawHeader("Connection", "keep-alive");
	request.setRawHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
	request.setRawHeader("Host", "music.163.com");
	request.setRawHeader("Referer", "http://music.163.com/");
	request.setRawHeader("Cache-Control", "no-cache");
	request.setRawHeader("Pragma", "no-cache");
	request.setRawHeader("Cookie", Configure::userCookies.toStdString().data());
	request.setRawHeader("User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:38.0) Gecko/20100101 Firefox/38.0");
}

void NetworkBase::sendPost(const QUrl &url, const QByteArray &data)
{
	QNetworkRequest request(url);
	initHead(request);
	m_accessManage->post(request, data);
}

void NetworkBase::sendGet(const QUrl &url, bool isInitHead)
{
	QNetworkRequest request(url);
	if (isInitHead)
		initHead(request);
	m_accessManage->get(request);
}

void NetworkBase::finished(QNetworkReply *reply)
{
	if (reply->error() != QNetworkReply::NoError)
	{
		if (reply->error() == QNetworkReply::TimeoutError)
		{
#ifdef QT_DEBUG
			qDebug() << "网络连接错误: 网络连接超时....";
#endif
		}
		else if (reply->error() == QNetworkReply::HostNotFoundError)
		{
#ifdef QT_DEBUG
			qDebug() << "网络错误: 找不到主机...";
#endif
		}
		else if (reply->error() == QNetworkReply::RemoteHostClosedError)
		{
#ifdef QT_DEBUG
			qDebug() << "网络错误: 与主机断开连接...";
#endif
		}
		else
		{
#ifdef QT_DEBUG
			qDebug() << "网络错误: 不明确的网络错误...";
#endif
		}
		reply->deleteLater();
		return;
	}

	int statCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	if (statCode == 200 || statCode == 302)
	{
		QByteArray data = reply->readAll();
		disposeifno(data);
	}
	reply->deleteLater();
}

bool NetworkBase::jsonObject(const QByteArray &data, QJsonObject &obj)
{
	QJsonParseError err;
	QJsonDocument json = QJsonDocument::fromJson(data, &err);
	if (err.error != QJsonParseError::NoError)
	{
#ifdef QT_DEBUG
		qDebug() << "网络错误: jsonObject 从网络获取的资源好像有点问题...";
#endif
		return false;
	}
	obj = json.object();
	return true;
}



















