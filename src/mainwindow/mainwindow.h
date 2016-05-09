#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../basedwidget/basedwidget.h"
#include <QSystemTrayIcon>
#include "../configure/looptype.h"

class QSystemTrayIcon;
class TitleWidget;
class MainTopWidget;
class MainBottomWidget;
class LyricWidget;
class SearchWidget;

class MainWindow : public BasedWidget
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();


protected:
	// 覆写close事件...
	void closeEvent(QCloseEvent *);

private slots:
	// 系统托盘...
	void trayIconClicked(QSystemTrayIcon::ActivationReason);
	void userHeadImageGeted(const QByteArray &);		//	成功获取用户头像...

	void autoLogIn();				//	开启软件时登录..

	void buttonStatusChanged();

	void ckeckUpdate();				//	检查软件更新..
	void changeSkin();				//	更换皮肤..
	void search();					//	搜索...
	void logOut();					//	注销...
	void logInSuccess();			//	成功登录..

	void exit_app();

private:
	void initUi();
	void initConnect();

	void initSystemTrayIcon();		//	初始化系统托盘..
	void initSystemTrayMenu();		//	初始化系统托盘菜单..

private:
	QSystemTrayIcon *m_systemTray;	//	系统托盘...
	QMenu *m_systemTrayMenu;		//	系统托盘菜单..

	TitleWidget *m_mainTitle;		//	标题栏..

	MainTopWidget *m_maintopwidget;			//	主上界面...
	MainBottomWidget *m_mainbottomWidget;	//	主下界面...
	LyricWidget *m_lyricWidget;				//	桌面歌词..
	QAction *m_desktopLyricAction;

	QAction *m_pauseAction;			//	系统托盘ACtion...

	SearchWidget *m_searchWidget;		//	搜索...
};

#endif // MAINWINDOW_H
