#ifndef NETWORKBASE_H
#define NETWORKBASE_H

#include <QObject>
#include <QNetworkReply>

class QNetworkAccessManager;
class QNetworkRequest;
class QJsonObject;

typedef enum {
	RECOMMENDCLASS,
	SEARCHCLASS,
	NORMALCLASS
}NetworkType;

class NetworkBase : public QObject
{
	Q_OBJECT
public:
	explicit NetworkBase(QObject *parent = 0, bool blockade = false);

protected:
	void initHead(QNetworkRequest &);
	bool jsonObject(const QByteArray &, QJsonObject &);		//	获取jsonObject...

	void sendPost(const QUrl &, const QByteArray &);
	void sendGet(const QUrl &, bool = false);

protected:
	//virtual void setMusicUrl(const QStringList &) = 0;
	virtual void disposeifno(const QByteArray &) = 0;

private slots:
	void finished(QNetworkReply *);

private:
	QNetworkAccessManager *m_accessManage;
};

#endif // NETWORKBASE_H
