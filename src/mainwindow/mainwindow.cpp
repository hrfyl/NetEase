#include "mainwindow.h"
#include "maintopwidget.h"
#include "mainbottomwidget.h"
#include "../configure/configure.h"
#include "../database/sqlitedata.h"
#include "../network/networkmanage.h"
#include "../toolwidgets/titlewidget.h"
#include "../mediaplayer/mediaplayer.h"
#include "../toolwidgets/lyricwidget.h"
#include "searchwidget/searchwidget.h"
#include <QIcon>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
	: BasedWidget(parent)
{
	initUi();
	initConnect();
	autoLogIn();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initUi()
{
	setFixedSize(mainWindowSize);
	setWindowIcon(QIcon(":/main_images/title_format"));
	setWindowTitle(APP_NAME + " " + APP_VERSION);

	m_mainTitle = new TitleWidget(true, this);
	m_maintopwidget = new MainTopWidget(this);
	m_maintopwidget->move(0, mainTitleSize.height());
	m_mainbottomWidget = new MainBottomWidget(this);
	m_mainbottomWidget->move(0, mainTitleSize.height() + mainTopWidgetSize.height());

	m_lyricWidget = new LyricWidget(this);
	m_lyricWidget->show();
	m_lyricWidget->move( (QApplication::desktop()->width() - m_lyricWidget->width()) / 2,
							 (QApplication::desktop()->height() - 130));

	m_searchWidget = new SearchWidget(this);
	m_searchWidget->setVisible(false);

	initSystemTrayIcon();
}

void MainWindow::initConnect()
{
	connect(m_mainTitle, SIGNAL(exitWidget()), this, SLOT(exit_app()));
	connect(m_mainTitle, SIGNAL(closeClicked()), this, SLOT(hide()));
	connect(m_mainTitle, SIGNAL(showMini()), this, SLOT(showMinimized()));
	connect(m_mainTitle, SIGNAL(showSkin()), this, SLOT(changeSkin()));
	connect(m_mainTitle, SIGNAL(updateClicked()), this, SLOT(ckeckUpdate()));
	connect(m_mainTitle, SIGNAL(search()), this, SLOT(search()));
	connect(m_mainTitle, SIGNAL(logOut()), this, SLOT(logOut()));

	connect(m_mainTitle, SIGNAL(loopTyepChanged(LOOPTYPE)),
			MediaPlayer::sharedMediaPlayer(), SLOT(setLoopType(LOOPTYPE)));

	connect(MediaPlayer::sharedMediaPlayer(), SIGNAL(buttonStatusChanged()),
			this, SLOT(buttonStatusChanged()));

	// search...
	connect(m_searchWidget, SIGNAL(search(QString)),
			NetworkManage::sharedNetworkManage(), SLOT(search(QString)));

	connect(m_maintopwidget, SIGNAL(logInSuccess()), this, SLOT(logInSuccess()));

	connect(NetworkManage::sharedNetworkManage(), SIGNAL(userHeadImageGeted(QByteArray)),
			this, SLOT(userHeadImageGeted(QByteArray)));
}


// 系统托盘...
void MainWindow::initSystemTrayIcon()
{
	if (QSystemTrayIcon::isSystemTrayAvailable())
	{
		initSystemTrayMenu();
		m_systemTray = new QSystemTrayIcon(this);
		m_systemTray->setIcon(QIcon(":/main_images/title_format"));
		m_systemTray->setContextMenu(m_systemTrayMenu);
		m_systemTray->setToolTip("NetEase Music");
		m_systemTray->show();
		connect(m_systemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
				this, SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
	}
}
void MainWindow::initSystemTrayMenu()
{
	m_systemTrayMenu = new QMenu(this);
	QAction *quit = new QAction("退出", m_systemTrayMenu);
	quit->setIcon(QIcon(":/menu_icon/exit"));
	connect(quit, SIGNAL(triggered(bool)), this, SLOT(exit_app()));

	QAction *showNor = new QAction("显示主窗口", m_systemTrayMenu);
	showNor->setIcon(QIcon(":/menu_icon/netease"));
	connect(showNor, SIGNAL(triggered(bool)), this, SLOT(showNormal()));

	m_desktopLyricAction = new QAction("显示桌面歌词", m_systemTrayMenu);
	//m_desktopLyricAction->setIcon(QIcon(":/menu_icon/lyric"));
	m_desktopLyricAction->setCheckable(true);
	m_desktopLyricAction->setChecked(true);
	connect(m_desktopLyricAction, SIGNAL(triggered(bool)),
			m_lyricWidget, SLOT(setVisible(bool)));

	QAction *lockDesktopLyricAction = new QAction("锁定桌面歌词", m_systemTrayMenu);
	lockDesktopLyricAction->setCheckable(true);
	lockDesktopLyricAction->setChecked(true);
	connect(lockDesktopLyricAction, SIGNAL(triggered(bool)),
			m_lyricWidget, SLOT(setLockWidget(bool)));

	QAction *frontAction = new QAction("上一首", m_systemTrayMenu);
	frontAction->setIcon(QIcon(":/menu_icon/previous_play"));
	m_pauseAction = new QAction("播放", m_systemTrayMenu);
	m_pauseAction->setIcon(QIcon(":/menu_icon/play"));
	QAction *nextAction = new QAction("下一首", m_systemTrayMenu);
	nextAction->setIcon(QIcon(":/menu_icon/next_play"));
	connect(frontAction, SIGNAL(triggered()), MediaPlayer::sharedMediaPlayer(), SLOT(front()));
	connect(m_pauseAction, SIGNAL(triggered()), MediaPlayer::sharedMediaPlayer(), SLOT(play()));
	connect(nextAction, SIGNAL(triggered()), MediaPlayer::sharedMediaPlayer(), SLOT(next()));

	m_systemTrayMenu->addAction(showNor);
	m_systemTrayMenu->addSeparator();

	m_systemTrayMenu->addAction(frontAction);
	m_systemTrayMenu->addAction(m_pauseAction);
	m_systemTrayMenu->addAction(nextAction);
	m_systemTrayMenu->addSeparator();

	m_systemTrayMenu->addAction(m_desktopLyricAction);
	m_systemTrayMenu->addAction(lockDesktopLyricAction);
	m_systemTrayMenu->addSeparator();

	m_systemTrayMenu->addAction(quit);
}


void MainWindow::userHeadImageGeted(const QByteArray &_data)
{
	m_maintopwidget->setUserHead(_data);
}

void MainWindow::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason) {
	case QSystemTrayIcon::Trigger :
		//break;
	case QSystemTrayIcon::DoubleClick :
		if (isHidden())
			showNormal();
		else
			hide();
		break;
	default:
		break;
	}
}

void MainWindow::buttonStatusChanged()
{
	bool isPlaying = false;
	if ( (isPlaying = MediaPlayer::sharedMediaPlayer()->isPlaying()))
	{
		m_pauseAction->setText("暂停");
		m_pauseAction->setIcon(QIcon(":/menu_icon/pause"));
	}
	else
	{
		m_pauseAction->setText("播放");
		m_pauseAction->setIcon(QIcon(":/menu_icon/play"));
	}
	m_mainbottomWidget->setPlayButtonStatus();
}

void MainWindow::ckeckUpdate()
{
	QDesktopServices::openUrl(QUrl("https://github.com/fengleyl/NetEaseMusic"));
}

void MainWindow::exit_app()
{
	// 释放单例资源...
	if (SqliteData::sharedDatabase())
	{
		SqliteData::sharedDatabase()->closeDatabase();
		delete SqliteData::sharedDatabase();
	}

	if (MediaPlayer::sharedMediaPlayer())
	{
		MediaPlayer::sharedMediaPlayer()->appExit();
		delete MediaPlayer::sharedMediaPlayer();
	}
	if (NetworkManage::sharedNetworkManage())
	{
		delete NetworkManage::sharedNetworkManage();
	}

	qApp->exit(0);
}

void MainWindow::changeSkin()
{

}

void MainWindow::search()
{
	if (m_searchWidget->isVisible())
		m_searchWidget->setVisible(false);
	else
		m_searchWidget->setVisible(true);
}

void MainWindow::logOut()
{
	m_maintopwidget->setUserInfo("LogIn");
	m_maintopwidget->rmAllList();
	m_maintopwidget->rmAllMusic();
	MediaPlayer::sharedMediaPlayer()->logOutEvent();
	m_mainbottomWidget->setPlayButtonStatus();
	QStringList user_info;
	user_info << "test" << "" << "" << "0" << "";
	SqliteData::sharedDatabase()->setUserInfo(user_info);
	Configure::updateConfigure();
}

// 成功登录...
void MainWindow::logInSuccess()
{
	Configure::updateConfigure();
	autoLogIn();
}



// 覆写close事件...
void MainWindow::closeEvent(QCloseEvent *)
{
}


void MainWindow::autoLogIn()
{
	if ( (Configure::userId != -1) && (Configure::userAutoLogin))
	{
		NetworkManage::sharedNetworkManage()->musicList(Configure::userId);
		m_maintopwidget->setUserInfo(Configure::userNickName);
		NetworkManage::sharedNetworkManage()->recommed();
	}
}










