#ifndef NETWORKRADIOFM_H
#define NETWORKRADIOFM_H

#include "networkbase.h"

class QTimer;

class NetworkRadioFM : public NetworkBase
{
	Q_OBJECT
public:
	explicit NetworkRadioFM(QObject *parent = 0);

	void randioFM();
	void trashFM(const int &songId, QString time = "25", QString alg = "RT");
	void randioMusicInfo();

signals:
	void randioFMMusicInfo(const QList<QStringList> &);

protected:
	void disposeifno(const QByteArray &);

private slots:
	void timeOut();

private:
	void rmMusicInfo(const int &songid);

	QTimer *m_timer;
	bool m_isRuning;
	int m_count;
	QList<QStringList> m_randioFmInfo;
	QVector<int> m_songIds;
	QStringList m_mp3Urls;
};

#endif // NETWORKRADIOFM_H
