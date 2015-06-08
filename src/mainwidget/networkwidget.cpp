#include "networkwidget.h"
#include "maintopwidget.h"
#include "mainbottomwidget.h"
#include "tools/titlewidget.h"
#include "tools/loginwidget.h"
#include "../function/media.h"
#include "../function/network.h"
#include "../toolwidget/lyricwidget.h"
#include <QMessageBox>
#include <QCryptographicHash>
#include <QApplication>
#include <QDesktopWidget>

NetWorkWidget::NetWorkWidget(QWidget *parent)
	: QWidget(parent)
{
	setFixedSize(900, 560);

	m_media = new Media(this);
	m_netWork = new NetWork(this);
	m_currentSongId = -1;

	initUi();
	initConnect();
}

NetWorkWidget::~NetWorkWidget()
{
}

void NetWorkWidget::initUi()
{
	m_topWidget = new MainTopWidget(this);
	m_bottomWIdget = new MainBottomWidget(this);
	m_bottomWIdget->move(0, 440);
	m_bottomWIdget->setNetWorkMode(true);

	m_logInWidget = new LogInWidget(this);
	m_logInWidget->setVisible(false);

	m_lyricWidget = new LyricWidget(this);
	m_lyricWidget->show();
	m_lyricWidget->move( (QApplication::desktop()->width() - width()) / 2, (QApplication::desktop()->height() - 120));
}

void NetWorkWidget::initConnect()
{
	connect(this, SIGNAL(search(QString)), m_netWork, SLOT(search(QString)));

	connect(m_media, SIGNAL(metaDataChanged(int,QString)), this, SLOT(metaDataChanged(int,QString)));
	connect(m_media, SIGNAL(postionChanged(QString,QString,qreal,double)), this, SLOT(setPostion(QString,QString,qreal,double)));
	connect(m_media, SIGNAL(messageBox(QString,QString)), this, SLOT(messageBox(QString,QString)));

	connect(m_netWork, SIGNAL(logInStatus(int, QString)), this, SLOT(logInStatus(int, QString)));
	connect(m_netWork, SIGNAL(musicList(QStringList,QVector<int>&)), this, SLOT(musicListInfo(QStringList,QVector<int>&)));
	connect(m_netWork, SIGNAL(musicInfo(QList<QStringList>)), this, SLOT(musicInfo(QList<QStringList>)));
	connect(m_netWork, SIGNAL(messageBox(QString,QString)), this, SLOT(messageBox(QString,QString)));
	connect(m_netWork, SIGNAL(lyric(QString)), m_lyricWidget, SLOT(lyric(QString)));

	connect(m_topWidget, SIGNAL(logIn()), this, SLOT(logInClicked()));
	connect(m_topWidget, SIGNAL(musicListItemClicked(QString)), this, SLOT(listDoubleClicked(QString)));
	connect(m_topWidget, SIGNAL(play(int)), m_media, SLOT(musicItemClicked(int)));

	connect(m_logInWidget, SIGNAL(logInClicked(QString,QString)), m_netWork, SLOT(logIn(QString,QString)));

	connect(m_bottomWIdget, SIGNAL(play()), m_media, SLOT(play()));
	connect(m_bottomWIdget, SIGNAL(playNext()), m_media, SLOT(playNext()));
	connect(m_bottomWIdget, SIGNAL(playFront()), m_media, SLOT(playFront()));
	connect(m_bottomWIdget, SIGNAL(setVoice(int)), m_media, SLOT(adjustVoice(int)));
	connect(m_bottomWIdget, SIGNAL(setPosTo(qreal)), m_media, SLOT(adjustDuration(qreal)));
}


void NetWorkWidget::logInClicked()
{
	m_logInWidget->setVisible(true);
}

void NetWorkWidget::logInStatus(int code, const QString &msg)
{
	switch (code) {
	case 200:
		m_topWidget->setUserInfo(msg);
		break;
	case 408:
		QMessageBox::critical(this, "登录出错", "网络连接超时", QMessageBox::Ok | QMessageBox::Cancel);
		break;
	case 501:
		QMessageBox::critical(this, "登录出错", "用户名错误", QMessageBox::Ok | QMessageBox::Cancel);
		break;
	case 502:
		QMessageBox::critical(this, "登录出错", "密码错误", QMessageBox::Ok | QMessageBox::Cancel);
		break;
	default:
		QMessageBox::critical(this, "登录出错", "未知错误", QMessageBox::Ok | QMessageBox::Cancel);
		break;
	}
}

void NetWorkWidget::logOut()
{
	m_media->logOut();
	m_netWork->logOut();
	m_topWidget->rmAllList();
	m_topWidget->rmAll();
	m_musicList.clear();
	m_currentListSongId.clear();
	m_currentSongId = -1;
	m_topWidget->setUserInfo("登录");
}

void NetWorkWidget::musicListInfo(const QStringList &list, QVector<int> &vector)
{
	m_topWidget->rmAllList();
	for (int i = 0; i < list.count(); i++)
	{
		m_topWidget->addListItem(list.at(i));
		m_musicList.insert(list.at(i), vector.at(i));
	}
}

void NetWorkWidget::musicInfo(const QList<QStringList> &list)
{
	m_topWidget->rmAll();
	QList<QStringList>::const_iterator it = list.begin();
	QStringList urlList;
	m_currentListSongId.clear();
	while (it != list.end())
	{
		if ((*it)[3] == "")
		{
			it++;
			continue;
		}
		m_topWidget->addMusicItem((*it)[0], (*it)[1], (*it)[2]);
		urlList.append((*it)[3]);
		m_currentListSongId.push_back((*it)[4].toInt());
		it++;
	}
	m_media->setMusicList(urlList);
	m_topWidget->setCurrentFile(0);
}

void NetWorkWidget::listDoubleClicked(const QString &name)
{
	int id = m_musicList.value(name);
	m_netWork->getListInfo(id);
}

void NetWorkWidget::setPostion(const QString &current, const QString &duration, const qreal &pos, const double &curPos)
{
	m_bottomWIdget->setPostion(current, duration);
	m_bottomWIdget->currentPosSlider(pos);
	m_lyricWidget->setTime(curPos);
}

void NetWorkWidget::metaDataChanged(int id, const QString &status)
{
	m_topWidget->setCurrentFile(id);
	if (status == "")
		return;
	m_bottomWIdget->setPlayButtonStatues(status);
	if (id == m_currentSongId)
		return;
	m_currentSongId = id;
	m_lyricWidget->clearText();
	m_netWork->getLyric(m_currentListSongId.at(m_currentSongId));
}

void NetWorkWidget::messageBox(const QString &title, const QString &msg)
{
	QMessageBox::information(this, title, msg, QMessageBox::Ok | QMessageBox::Cancel);
}


void NetWorkWidget::setAutoLogInInfo(QStringList list)
{
	m_netWork->setUserInfo(list);
	m_topWidget->setUserInfo(list[3]);
	m_netWork->getMusicList(0, 200);
}

QStringList NetWorkWidget::saveUserInfo()
{
	return m_netWork->userInfo();
}

void NetWorkWidget::setWidgetVisiable(bool isVisiable)
{
	m_lyricWidget->setVisible(isVisiable);
	this->setVisible(isVisiable);
}

void NetWorkWidget::closeWidget()
{
	m_media->playStop();
}

void NetWorkWidget::pause()
{
	m_media->pause();
}
