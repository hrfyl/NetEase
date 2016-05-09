#ifndef MAINTOPWIDGET_H
#define MAINTOPWIDGET_H

#include <QWidget>

class BackGroundWidget;
class MusicListsWidget;
class UserWidget;
class QTableWidget;
class LogInWidget;
class QMenu;

class MainTopWidget : public QWidget
{
	Q_OBJECT
public:
	explicit MainTopWidget(QWidget *parent = 0);
	~MainTopWidget();

	void addMusic(const QStringList &);

	//		用户信息设置...
	void setUserInfo(const QString &name, QString head=":/main_images/image_login");
	void setUserHead(const QByteArray &);

signals:
	void logInSuccess();

public slots:
	void rmAllMusic();
	void rmAllList();
	void rmMusicRow(const int &);						//	移除一条音乐..

	void setUpMouseMenu();								//	初始化QTableWidget右键菜单..

private slots:
	void listDetailsSave(const QList<QStringList> &);	//	歌单中歌曲信息...
	void searchDetailsSave(const QList<QStringList> &);	//	搜索...

	void setCurrentMusic(int index);					//设置当前歌曲..

	void customMenuRequested(const QPoint &);			//	右键菜单响应..
	void likeMenuClicked();								//	响应右键菜单..
	void unlikeMenuClicked();
	void refreshMenuClicked();							//	刷新

private:
	void initUi();
	void initConnect();

	void setTablewidgetOrder();

private:
	BackGroundWidget *m_backGroundWidget;

	//		歌单界面
	MusicListsWidget *m_musicListsWidget;
	//		用户界面
	UserWidget *m_userWidget;
	//		当前播放列表
	QTableWidget *m_currentList;
	QMenu *m_mouseMenu;				//	给m_currentList添加右键菜单..
	QAction *m_likeAction;
	QAction *m_unlikeAction;
	QAction *m_refreshAction;			//	刷新当前歌单...

	// 登录界面...
	LogInWidget *m_logWidget;
};

#endif // MAINTOPWIDGET_H
