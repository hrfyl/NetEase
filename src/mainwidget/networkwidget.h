#ifndef NETWORKWIDGET_H
#define NETWORKWIDGET_H

#include <QWidget>
#include <QHash>

class MainTopWidget;
class MainBottomWidget;
class TitleWidget;
class LogInWidget;
class Media;
class NetWork;
class LyricWidget;

class NetWorkWidget : public QWidget
{
	Q_OBJECT
public:
	explicit NetWorkWidget(QWidget *parent = 0);
	~NetWorkWidget();


	void closeWidget();
	void pause();						//		本地和网络播放却换时设置..
	//		自动登录时设置NetWorkWidget数据..
	void setAutoLogInInfo(QStringList list);
	//		保存用户信息到数据库...
	QStringList saveUserInfo();
	void setWidgetVisiable(bool);

signals:
	void search(const QString &);

private slots:
	void logInClicked();			//	显示登录对话框...
	void logInStatus(int code, const QString &msg);				 //	登录返回信息...
	void logOut();					//	注销...

	void musicListInfo(const QStringList &list, QVector<int> &vector);//	歌单信息..
	void musicInfo(const QList<QStringList> &list);				//	歌单详细信息...

	void listDoubleClicked(const QString &name);

	void setPostion(const QString &,const QString &,const qreal &,const double &);
	void metaDataChanged(int id, const QString &status);		//	播放发生改变..

	void messageBox(const QString &title, const QString &msg);	//	media来的窗口消息...

private:
	void initUi();
	void initConnect();

private:
	//		界面布局...
	LogInWidget *m_logInWidget;
	MainTopWidget *m_topWidget;
	MainBottomWidget *m_bottomWIdget;
	LyricWidget *m_lyricWidget;

	//		数据控制...
	Media *m_media;						//	控制媒体...
	NetWork *m_netWork;					//	控制网络...

	QHash<QString,int> m_musicList;		//	歌单信息..
	QVector<int> m_currentListSongId;	//	当前歌单所有歌曲的songId,用于获取歌词...
	int m_currentSongId;				//	当前歌曲的编号...
};

#endif // NETWORKWIDGET_H
