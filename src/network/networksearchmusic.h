#ifndef NETWORKSEARCHMUSIC_H
#define NETWORKSEARCHMUSIC_H

#include "networkbase.h"
#include <QList>

class NetworkSearchMusic : public NetworkBase
{
	Q_OBJECT
public:
	NetworkSearchMusic(QObject *parent = 0);

	void rmMusicInfo(const int &);

signals:
	void msuicInfo(const QList<QStringList> &);

public slots:
	void search(const QString &);

protected:
	void disposeifno(const QByteArray &data);

private:
	QList<QStringList> m_musicInfo;
	QVector<int> m_songIds;
	QString m_search;
};

#endif // NETWORKSEARCHMUSIC_H
