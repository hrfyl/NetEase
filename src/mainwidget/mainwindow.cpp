/*#######################################################################
		> File Name: mainwindow.cpp
		# Author: 		WIND
		# mail: 		fengleyl@163.com
		> Created Time: 2015年05月23日 星期六 11时36分08秒
########################################################################*/

#include "mainwindow.h"
#include "tools/titlewidget.h"
#include "mainbottomwidget.h"
#include "maintopwidget.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMediaMetaData>
#include <QIcon>
#include "../toolwidget/addmusiclist.h"
#include "../toolwidget/searchwidget.h"
#include "networkwidget.h"
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QFileInfo>
#include "../Data/sqlitedata.h"
#include <QFileDialog>
#include <QMenu>

bool connectDatabase()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("./resource/sql/music.db");
	//db.setDatabaseName("/home/wind/文档/qt/NetEase/bin/resource/sql/music.db");

	if (!db.open())
	{
		QMessageBox::critical(0, QObject::tr("连接数据出错"), db.lastError().text(), QMessageBox::Ok);
		return false;
	}
	SqliteData sqlite;
	if (!sqlite.initTables())
		return false;
	return true;
}

MainWindow::MainWindow(QWidget *parent)
	: BasedWidget(parent)
{
	setFixedSize(900, 600);
	setWindowIcon(QIcon(":/default/icon"));
	setWindowTitle("网易云音乐");

	m_isDisconnect = 0;

	m_player = new QMediaPlayer(this);
	m_player->setVolume(25);
	m_playList = new QMediaPlaylist(this);
	m_player->setPlaylist(m_playList);

	m_playList->setPlaybackMode(QMediaPlaylist::Loop);

	initUi();
	initMenu();
	initConnect();
	initList();

	getNetUserInfo();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initUi()
{
	m_mainTitle = new TitleWidget(this);
	m_mainTitle->move(0, 0);
	m_mainTitle->netWorkModel(true);

	m_mainTopWidget = new MainTopWidget(this);
	m_mainBottomWidget = new MainBottomWidget(this);
	m_mainTopWidget->move(0, 40);
	m_mainBottomWidget->move(0, 480);

	m_netWorkWidget = new NetWorkWidget(this);
	m_netWorkWidget->move(0, 40);
	//m_netWorkWidget->setVisible(false);

	m_mainTopWidget->setVisible(false);
	m_mainBottomWidget->setVisible(false);

	m_searchWidget = new SearchWidget(this);
	m_searchWidget->setVisible(false);

	if (QSystemTrayIcon::isSystemTrayAvailable())
	{
		initMenu();
		m_systemTray = new QSystemTrayIcon(this);
		m_systemTray->setIcon(QIcon(":/default/icon"));
		m_systemTray->setContextMenu(m_menu);
		m_systemTray->setToolTip("MusicPlayer");
		m_systemTray->show();
		connect(m_systemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
	}
}

void MainWindow::initConnect()
{
	//		连接title的信号
	connect(m_mainTitle ,SIGNAL(closeClicked()), this, SLOT(hide()));
	connect(m_mainTitle, SIGNAL(showMini()), this, SLOT(showMinimized()));
	connect(m_mainTitle, SIGNAL(showSkin()), this, SLOT(showSkin()));
	connect(m_mainTitle, SIGNAL(updateClicked()), this, SLOT(showUpdate()));
	connect(m_mainTitle, SIGNAL(addFile()), this, SLOT(addFifles()));
	connect(m_mainTitle, SIGNAL(addList()), this, SLOT(addLists()));

	connect(m_mainTitle, SIGNAL(exitWidget()), this, SLOT(close()));

	connect(m_mainTitle, SIGNAL(loopOnePlay()), this, SLOT(loopOnePlay()));
	connect(m_mainTitle, SIGNAL(loopPlay()), this, SLOT(loopPlay()));
	connect(m_mainTitle, SIGNAL(listPlay()), this, SLOT(listPlay()));
	connect(m_mainTitle, SIGNAL(randomPlay()), this, SLOT(randomPlay()));
	connect(m_mainTitle, SIGNAL(logOut()), m_netWorkWidget, SLOT(logOut()));
	connect(m_mainTitle, SIGNAL(switchWidget()), this, SLOT(switchNet()));
	connect(m_mainTitle, SIGNAL(search()), this, SLOT(showSearchWidget()));
	connect(m_mainTitle, SIGNAL(rmFiles()), this, SLOT(rmMusicFromData()));
	connect(m_mainTitle, SIGNAL(rmLists()), this, SLOT(rmListFromData()));

	connect(m_searchWidget, SIGNAL(search(QString)), m_netWorkWidget, SLOT(search(QString)));

	connect(m_mainBottomWidget, SIGNAL(setVoice(int)), m_player, SLOT(setVolume(int)));
	connect(m_mainBottomWidget, SIGNAL(setPostion(qint64)), m_player, SLOT(setPosition(qint64)));
	connect(m_player, SIGNAL(positionChanged(qint64)), m_mainBottomWidget, SLOT(postionChangeTo(qint64)));
	connect(m_mainBottomWidget, SIGNAL(play()), m_player, SLOT(play()));
	connect(m_mainBottomWidget, SIGNAL(pasue()), m_player, SLOT(pause()));
	connect(m_mainBottomWidget, SIGNAL(playFront()), m_playList, SLOT(previous()));
	connect(m_mainBottomWidget, SIGNAL(playNext()), m_playList, SLOT(next()));
	connect(m_player, SIGNAL(stateChanged(QMediaPlayer::State)), m_mainBottomWidget, SLOT(playStateChanged(QMediaPlayer::State)));
	connect(m_player, SIGNAL(metaDataChanged()), this, SLOT(mediaMetaChanged()));

	//		topWidget signals

	connect(m_mainTopWidget, SIGNAL(play(int)), this, SLOT(playTo(int)));
	connect(m_playList, SIGNAL(currentIndexChanged(int)), m_mainTopWidget, SLOT(currentMusic(int)));
	connect(m_mainTopWidget, SIGNAL(musicListItemClicked(QString)), this, SLOT(currentListChanged(QString)));

	connect(m_player, SIGNAL(metaDataChanged()), this, SLOT(getMusicInfo()));
}

void MainWindow::closeEvent(QCloseEvent *)
{
	m_netWorkWidget->closeWidget();

	QStringList list = m_netWorkWidget->saveUserInfo();
	if (list[2] != "0")
		m_saveUserInfo = 1;
	else
	{
		m_saveUserInfo = 0;
		return;
	}

	if (m_saveUserInfo == 0)
	{
		int button = QMessageBox::warning(this, "保存用户信息..", "即将退出网易云音乐,你是否要保存用户信息?", QMessageBox::Ok | QMessageBox::Cancel);
		if (button != QMessageBox::Ok)
			return;
	}
	list.append(QString::number(m_saveUserInfo));
	SqliteData sqlite;
	sqlite.updateUserInfo(list);
}


void MainWindow::initMenu()
{
	m_menu = new QMenu(this);

	QAction *quit = new QAction("退出", m_menu);
	QAction *showNor = new QAction("显示主窗口", m_menu);
	connect(showNor, SIGNAL(triggered()), this, SLOT(showNormal()));
	connect(quit, SIGNAL(triggered()), this, SLOT(close()));

	m_menu->addAction(showNor);
	m_menu->addSeparator();
	m_menu->addAction(quit);

}


void MainWindow::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason) {
	case QSystemTrayIcon::Trigger :
		//break;
	case QSystemTrayIcon::DoubleClick :
		showNormal();
		break;
	default:
		break;
	}
}



void MainWindow::switchNet()
{
	if (m_netWorkWidget->isVisible())
	{
		m_netWorkWidget->setVisible(false);
		m_mainTopWidget->setVisible(true);
		m_mainBottomWidget->setVisible(true);
		m_mainTitle->netWorkModel(false);
	}
	else
	{
		m_mainTopWidget->setVisible(false);
		m_mainBottomWidget->setVisible(false);
		m_netWorkWidget->setVisible(true);
		m_mainTitle->netWorkModel(true);
	}
	m_netWorkWidget->quitPlay();
	if (m_player->state() == QMediaPlayer::PlayingState)
		m_player->pause();
}

void MainWindow::showSearchWidget()
{
	m_searchWidget->setVisible(true);
}




void MainWindow::loopOnePlay()
{
	m_playList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
}

void MainWindow::loopPlay()
{
	m_playList->setPlaybackMode(QMediaPlaylist::Loop);
}

void MainWindow::listPlay()
{
	m_playList->setPlaybackMode(QMediaPlaylist::Sequential);
}

void MainWindow::randomPlay()
{
	m_playList->setPlaybackMode(QMediaPlaylist::Random);
}

void MainWindow::showUpdate()
{

}

void MainWindow::showSkin()
{

}

void MainWindow::playTo(int index)
{
	m_playList->setCurrentIndex(index);
	m_player->play();
}

void MainWindow::addFifles()
{
	QString listName = m_mainTopWidget->getCurrentListName();
	if (listName == "")
	{
		QMessageBox::critical(this, "错误", "请先选择歌单再添加歌曲...", QMessageBox::Ok | QMessageBox::Cancel);
		return;
	}
	QStringList files = QFileDialog::getOpenFileNames(this, "打开文件", "./", "MP3 文件 (*.mp3);; 全部文件(*.*)");
	if (files.isEmpty())
		return;
	QStringList::iterator it = files.begin();

	SqliteData m_sqlite;
	QString listId = m_sqlite.getListId(m_currentList);
	while (it != files.end())
	{
		if (m_sqlite.insertIntoList(listId, *it, QFileInfo(*it).baseName(), "", ""))
			m_mainTopWidget->addMusicItem(QFileInfo(*it).baseName(), "", "");
		it++;
	}
}

void MainWindow::addLists()
{
	AddMusicList m_addList;
	connect(&m_addList, SIGNAL(okButtonClicked(QString,QStringList)), this, SLOT(addMusicList(QString,QStringList)));
	m_addList.exec();
}
void MainWindow::addMusicList(QString name, QStringList files)
{
	m_mainTopWidget->addListItem(name);
//	QStringList::iterator it = files.begin();
//	while (it != files.end())
//	{
//		m_playList->addMedia(QUrl::fromLocalFile(*it));
//		m_mainTopWidget->addMusicItem(QFileInfo(*it).baseName(), "", "");
//		it++;
//	}
	SqliteData m_sqlite;

	QString ListId = "list" + QString::number(m_sqlite.musicListCount(), 10);
	if (!m_sqlite.insertIntoMusicList(name, ListId))
		return;
	if (!m_sqlite.createTable(ListId))
		return;
	QStringList::iterator i = files.begin();
	while (i != files.end())
	{
		QString title = QFileInfo(*i).baseName();
		m_sqlite.insertIntoList(ListId, *i, title, "", "");
		i++;
	}
}

void MainWindow::mediaMetaChanged()
{
	m_isDisconnect = 0;
	connect(m_player, SIGNAL(positionChanged(qint64)), this, SLOT(setMaxDruation()));
}

void MainWindow::setMaxDruation()
{
	if (m_isDisconnect >= 10)
		disconnect(m_player, SIGNAL(positionChanged(qint64)), this, SLOT(setMaxDruation()));
	else
	{
		m_isDisconnect++;
		m_mainBottomWidget->setMaxDuration(m_player->duration());
	}
}

void MainWindow::getMusicInfo()
{
	if (m_playList->currentIndex() >= m_playList->mediaCount() - 1)
		if (m_playList->playbackMode() == QMediaPlaylist::Sequential)
			return;
	QString title = m_player->metaData(QMediaMetaData::Title).toString();
	if (title == "")
		return;
	QString artist = m_player->metaData(QMediaMetaData::ContributingArtist).toString();
	QString album = m_player->metaData(QMediaMetaData::AlbumTitle).toString();
	//QMessageBox::information(this, title, artist, QMessageBox::Ok);
	SqliteData m_sqlite;
	QString ListId = m_sqlite.getListId(m_mainTopWidget->getCurrentListName());
	if (ListId == "")
		return;
	QString oldTitle = m_mainTopWidget->currentMusicName();

	if (!m_sqlite.updateTable(ListId, oldTitle, title, artist, album))
		return;
	m_mainTopWidget->setMusicItem(m_playList->currentIndex(), title, artist, album);
}

void MainWindow::initList()
{
	SqliteData m_sqlite;
	if (m_sqlite.musicListCount() == 0)
		return;
	QStringList list = m_sqlite.selectList();
	if (list.isEmpty())
		return;
	QStringList::iterator it = list.begin();
	m_currentList = *it;
	while (it != list.end())
	{
		QString ListId = m_sqlite.getListId(*it);
		m_mainTopWidget->addListItem(*it);
		if (ListId != "" && m_currentList == *it)
		{
			QList<QStringList> files = m_sqlite.selectFiles(ListId);
			for (int i = 0; i < files.count(); i++)
			{
				m_playList->addMedia(QUrl::fromLocalFile(files.at(i).at(0)));
				m_mainTopWidget->addMusicItem(files.at(i).at(1), files.at(i).at(2), files.at(i).at(3));
			}
		}
		it++;
	}
	m_mainTopWidget->setcurrentList(0);
	m_mainTopWidget->setCurrentFile(0);
}

void MainWindow::getNetUserInfo()
{
	SqliteData sqlite;
	QStringList list = sqlite.netUserInfo();
	if ((list[2] == ""))
	{
		m_saveUserInfo = 0;
		return;
	}
	m_saveUserInfo = list.at(4).toInt();
	m_netWorkWidget->setAutoLogInInfo(list);
}

void MainWindow::currentListChanged(const QString &text)
{
	SqliteData m_sqlite;
	QString ListId = m_sqlite.getListId(text);

	if (text == m_currentList)
	{
		m_mainTopWidget->setCurrentFile(m_playList->currentIndex());
		return;
	}

	//	先清空,在添加
	m_playList->clear();
	m_mainTopWidget->rmAll();

	m_currentList = text;
	QList<QStringList> files = m_sqlite.selectFiles(ListId);
	for (int i = 0; i < files.count(); i++)
	{
		m_playList->addMedia(QUrl::fromLocalFile(files.at(i).at(0)));
		m_mainTopWidget->addMusicItem(files.at(i).at(1), files.at(i).at(2), files.at(i).at(3));
	}
	m_mainTopWidget->setCurrentFile(0);
}

void MainWindow::rmListFromData()
{
	SqliteData m_sqlite;
	QString listName = m_mainTopWidget->getCurrentListName();
	QString listId = m_sqlite.getListId(listName);

	if (m_sqlite.deleteData("music_list", listId))
		if (!m_sqlite.dropTable(listId))
			return;
	m_mainTopWidget->rmCurrentListItem();
	if (m_currentList == listName)
		m_player->stop();
	m_playList->clear();
	m_mainTopWidget->rmAll();
}

void MainWindow::rmMusicFromData()
{
	SqliteData m_sqlite;
	QString listName = m_mainTopWidget->getCurrentListName();
	QString musicName = m_mainTopWidget->currentMusicName();
	QString ListId = m_sqlite.getListId(listName);
	m_sqlite.deleteData(ListId, musicName);
	m_mainTopWidget->rmCurrentMusicItem();
}







