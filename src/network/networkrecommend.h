#ifndef NETWORKRECOMMEND_H
#define NETWORKRECOMMEND_H

#include "networkbase.h"
#include <QStringList>
#include <QList>
#include <QVector>

class NetworkGetSongsById;

class NetworkRecommend : public NetworkBase
{
	Q_OBJECT
public:
	explicit NetworkRecommend(QObject *parent = 0);

	void recommend();
	void refresh();

	void rmMusicInfo(const int &);

signals:
	void musicInfo(const QList<QStringList> &);

protected:
	void disposeifno(const QByteArray &);

private:
	void sendMusicInfo();

private:
	QStringList getAllSongsId(const QString &, const QString &);
	QStringList getAllMatchResults(const QString &, const QString &);
	void setSongsInfo(const QStringList &, const QStringList &, const QStringList &);

	QList<QStringList> m_songsInfo;
	QVector<int> m_songsId;
};

#endif // NETWORKRECOMMEND_H
