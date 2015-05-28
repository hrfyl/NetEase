#ifndef NETWORKWIDGET_H
#define NETWORKWIDGET_H

#include <QWidget>

#define NULL_REQUEST -1					//		不明操作..
#define LOGIN 0							//		登录操作..
#define GETMUSICLIST 1					//		获取歌单...
#define GETLISTINFO 2					//		获取歌单详细信息...
#define SEARCH 3						//		搜索...
#define GETMP3URL 4						//		通过songs id 获取mp3url..

class MainTopWidget;
class MainBottomWidget;
class TitleWidget;
class LogInWidget;
class QNetworkAccessManager;
class QNetworkReply;
class QProcess;

class NetWorkWidget : public QWidget
{
	Q_OBJECT
public:
	explicit NetWorkWidget(QWidget *parent = 0);
	~NetWorkWidget();


	void closeWidget();
	void quitPlay();			//		本地和网络播放却换时设置..
	//		自动登录时设置NetWorkWidget数据..
	void setAutoLogInInfo(QStringList list);
	//		保存用户信息到数据库...
	QStringList saveUserInfo();

public slots:
	//		注销登录...
	void logOut();
	void search(const QString &search);			//		搜索单曲.歌单...

protected:
	void timerEvent(QTimerEvent *);

private slots:
	//		播放模块...
	void playMedia(const QString &path);	//	播放...
	void playFinished(int);						//	当前歌曲播放完成..
	void musicListItemClicked(const QString &);
	void musicItemClicked(int index);			//		双击播放..
	//		音量调节..
	void adjustVoice(int voice);
	//		进度调节..
	void adjustDuration(qreal pos);
	//		来自MainBottomWidget 的信号处理...
	void play();
	void playNext();
	void playFront();


	//		登录...
	void logInClicked();
	void logIn(const QString &, const QString &);

	void finished(QNetworkReply *reply);			//	从网络中读取数据..

private:
	void initUi();
	void initConnect();

	//		处理网络数据..
	void getMusicTime();							//	获取当前音乐进度及总时间...
	void dealLogIn(QNetworkReply *reply);			//	登录处理...
	void getMusicList(int offset = 0, int limit = 100);
	void dealGetMusicList(QNetworkReply *reply);	//	获取音乐歌单..
	void dealGetListInfo(QNetworkReply *reply);		//	获取歌单详细信息..
	void dealSeach(QNetworkReply *reply);			//	搜索结果处理..
	void getSongsUrl();
	void dealSongId(QNetworkReply *reply);			//	通过songid获取songurl..

private:
	//		界面布局...
	LogInWidget *m_logInWidget;
	MainTopWidget *m_topWidget;
	MainBottomWidget *m_bottomWIdget;

	//		网络相关..
	QNetworkAccessManager *m_accessManager;
	int m_type;			//		区分不同网络请求...

	//		用户信息...
	int m_userId;
	QString m_nickName;
	QString m_userName;
	QString m_userPasswd;		//		保存md5加密后的数据..

	//		歌单信息...
	QHash<int, QString> m_currentMusicList;
	//		保存歌单信息,歌单名的歌单ID...
	QHash<QString, int> m_musicList;
	//		记录搜索获取的songs Id...
	//QVector<int> m_songsId;
	QHash<int, int> m_songsId;


	//		音乐播放模块..
	QProcess *m_process;		//		启动并控制mplayer进程..
	int m_currentPlayMusic;		//		当前播放音乐的索引...
	bool m_isPlaying;			//		是否正在播放..
	bool m_isExitEvent;			//		退出事件..
	int m_timerId;				//		定时器...
	double m_duration;			//		当前歌曲总时长..
};

#endif // NETWORKWIDGET_H
