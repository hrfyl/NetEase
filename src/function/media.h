#ifndef MEDIA_H
#define MEDIA_H

#include <QObject>
#include <QStringList>

#define STOP_BTN QString(":/func_btn/stop_btn")					//	停止播放按钮..
#define PLAY_BTN QString(":/func_btn/play_btn")					//	停止播放按钮..

class QProcess;

class Media : public QObject
{
	Q_OBJECT
public:
	Media(QObject *parent = 0);
	~Media();

	void setMusicList(const QStringList &list);
	void pause();
	void logOut();

signals:
	void metaDataChanged(int id, const QString &status);		//	播放发生改变..
	void postionChanged(const QString &currentPos, const QString &duration, qreal current, const double &curPos);		//	进度条改变..
	void messageBox(const QString &title, const QString &msg);	//	窗口消息...

public slots:
	//		播放模块...
	void playMedia(const QString &path);		//	播放...
	void playFinished(int);						//	当前歌曲播放完成..


	void musicItemClicked(int index);			//		双击播放..
	//		音量调节..
	void adjustVoice(int voice);
	//		进度调节..
	void adjustDuration(qreal pos);
	//		来自MainBottomWidget 的信号处理...
	void play();
	void playNext();
	void playFront();
	void playStop();


protected:
	void timerEvent(QTimerEvent *);				//	定时器,刷新进度条的....

private:
	void getMusicTime();

private:
	//		歌单详细信息...
	QStringList m_currentMusicList;

	//		音乐播放模块..
	QProcess *m_process;		//		启动并控制mplayer进程..
	int m_currentPlayMusic;		//		当前播放音乐的索引...
	bool m_isPlaying;			//		是否正在播放..
	bool m_isExitEvent;			//		退出事件..
	int m_timerId;				//		定时器...
	double m_duration;			//		当前歌曲总时长..
};

#endif // MEDIA_H
