/*#######################################################################
		> File Name: mainwindow.h
		# Author: 		WIND
		# mail: 		fengleyl@163.com
		> Created Time: 2015年05月23日 星期六 11时36分56秒
########################################################################*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../basedwidget/basedwidget.h"
#include <QSystemTrayIcon>

class TitleWidget;
class MainTopWidget;
class MainBottomWidget;
class QMediaPlayer;
class QMediaPlaylist;
class QMenu;
class NetWorkWidget;
class SearchWidget;

class MainWindow : public BasedWidget
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	void closeEvent(QCloseEvent *);

private slots:
	//		title signal-slot
	void showSkin();
	void showUpdate();
	void addFifles();
	void addLists();

	void trayIconClicked(QSystemTrayIcon::ActivationReason);

	void loopOnePlay();
	void loopPlay();
	void listPlay();
	void randomPlay();

	void mediaMetaChanged();
	void setMaxDruation();

	//		双击QTableWidget播放
	void playTo(int index);
	//		添加歌单
	void addMusicList(QString name, QStringList files);
	//		获取当前歌曲的信息
	void getMusicInfo();

	//		当前播放列表改变
	void currentListChanged(const QString &text);

	//		从数据库中移出要删除的数据
	void rmMusicFromData();
	void rmListFromData();

	//		网络资源与本地资源相互却换..
	void switchNet();

	//		显示搜索窗口..
	void showSearchWidget();

private:
	void initUi();
	void initConnect();
	void initMenu();

	//	加载列表
	void initList();

	//		自动登录数据获取..
	void getNetUserInfo();

private:
	TitleWidget *m_mainTitle;

	QMenu *m_menu;
	QSystemTrayIcon *m_systemTray;

	MainTopWidget *m_mainTopWidget;
	MainBottomWidget *m_mainBottomWidget;

	//		网络资源播放...
	NetWorkWidget *m_netWorkWidget;

	//		播放器
	QMediaPlayer *m_player;
	QMediaPlaylist *m_playList;
	int m_isDisconnect;
	QString m_currentList;		//	记录当前播放列表名

	//		搜索...
	SearchWidget *m_searchWidget;
};

#endif // MAINWINDOW_H
