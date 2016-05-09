/*
 * MediaPlayer 使用单例模式，基于mplayer实现音乐播放功能..
 *
 * */

#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <QVector>
#include <QStringList>
#include "../configure/looptype.h"

class QProcess;

class MediaPlayer : public QObject
{
	Q_OBJECT
public:
	static MediaPlayer *sharedMediaPlayer();

	void appExit();				//	程序退出会调用..
	void logOutEvent();

	static int currentMusicIndex();
	static LOOPTYPE loopType();
	void rmMusic(const int &);
	void clearUrls();

	QVector<int> musicListId() const;
	void setMusicListId(const QVector<int> &musicListId);

	QStringList musicListUrl() const;
	void setMusicListUrl(const QStringList &musicListUrl);

	static bool isPlaying();
	static void setIsPlaying(bool isPlaying);

	double duration() const;
	void setDuration(double duration);

	QVector<int> songIds() const;
	void setSongIds(const QVector<int> &songIds);

signals:
	void playStatusChanged(const int &);
	void buttonStatusChanged();
	void postionChanged(const QString &, const QString &, const qreal &);
	void updateLyric(const double &);

public slots:
	void stop();
	void pause();
	void play();
	void play(int);
	void next();
	void front();

	// 进度改变..
	void setPostion(const qreal &);
	// 音量改变..
	void setVoice(const int &);

	static void setLoopType(const LOOPTYPE &loopType);

protected:
	void timerEvent(QTimerEvent *);

private slots:
	void finished(const int &);

private:
	void init();
	void playPath(bool order = true);
	void playMedia(const QString &path);
	void musicTime();						//	定时获取当前歌曲时间..

	QString getPlayPath();

private:
	static MediaPlayer *m_player;

	QVector<int> m_musicListId;				//	歌单列表...
	QStringList m_musicListUrl;				//	当前歌单所有歌曲的url地址..
	QVector<int> m_songIds;					//	当前歌单的歌曲ID...

	// 播放控制...
	static bool m_isPlaying;
	QProcess *m_process;		//		mplayer进程..

	// 循环控制..
	static LOOPTYPE m_loopType;

	static bool m_isExitEvent;
	static int m_currentMusicIndex;

	int m_timerId;				//		定时器...
	double m_duration;			//		当前歌曲总时长..
	double m_currentTime;		//		当前播放进度..
};

#endif // MEDIAPLAYER_H
