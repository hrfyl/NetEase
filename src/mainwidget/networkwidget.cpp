#include "networkwidget.h"
#include "maintopwidget.h"
#include "mainbottomwidget.h"
#include "tools/titlewidget.h"
#include "tools/loginwidget.h"
#include <QMessageBox>
#include <QCryptographicHash>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QProcess>

NetWorkWidget::NetWorkWidget(QWidget *parent)
	: QWidget(parent)
{
	setFixedSize(900, 560);

	m_type = NULL_REQUEST;
	m_accessManager = new QNetworkAccessManager(this);

	m_userId = 0;
	m_currentPlayMusic = -1;
	m_isPlaying = false;
	m_isExitEvent = false;
	m_musicList.clear();

	m_process = new QProcess(this);
	m_process->setProcessChannelMode(QProcess::MergedChannels);
	initUi();
	initConnect();

	m_timerId = startTimer(1100);
}

NetWorkWidget::~NetWorkWidget()
{
	killTimer(m_timerId);
}

void NetWorkWidget::initUi()
{
	m_topWidget = new MainTopWidget(this);
	m_bottomWIdget = new MainBottomWidget(this);
	m_bottomWIdget->move(0, 440);
	m_bottomWIdget->setNetWorkMode(true);

	m_logInWidget = new LogInWidget(this);
	m_logInWidget->setVisible(false);
}

void NetWorkWidget::initConnect()
{
	connect(m_topWidget, SIGNAL(logIn()), this, SLOT(logInClicked()));
	//connect(m_topWidget, SIGNAL(logIn()), this, SLOT(getMusicList()));
	connect(m_topWidget, SIGNAL(musicListItemClicked(QString)), this, SLOT(musicListItemClicked(QString)));
	connect(m_topWidget, SIGNAL(play(int)), this, SLOT(musicItemClicked(int)));

	connect(m_logInWidget, SIGNAL(logInClicked(QString,QString)), this, SLOT(logIn(QString,QString)));

	connect(m_accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));

	connect(m_process, SIGNAL(finished(int)), this, SLOT(playFinished(int)));

	connect(m_bottomWIdget, SIGNAL(play()), this, SLOT(play()));
	connect(m_bottomWIdget, SIGNAL(playNext()), this, SLOT(playNext()));
	connect(m_bottomWIdget, SIGNAL(playFront()), this, SLOT(playFront()));
	connect(m_bottomWIdget, SIGNAL(setVoice(int)), this, SLOT(adjustVoice(int)));
	connect(m_bottomWIdget, SIGNAL(setPosTo(qreal)), this, SLOT(adjustDuration(qreal)));
}

void NetWorkWidget::logInClicked()
{
	m_logInWidget->setVisible(true);
}

void NetWorkWidget::setAutoLogInInfo(QStringList list)
{
	m_userName = list.at(0);
	m_userPasswd = list.at(1);
	m_userId = list.at(2).toInt();
	m_nickName = list.at(3);
	m_topWidget->setUserInfo(":/default/user_head", m_nickName);
	getMusicList();
}

QStringList NetWorkWidget::saveUserInfo()
{
	QStringList list;
	list.append(m_userName);
	list.append(m_userPasswd);
	list.append(QString::number(m_userId));
	list.append(m_nickName);
	return list;
}

void NetWorkWidget::logOut()
{
	if (m_userId == 0)
		return;
	m_isPlaying = false;
	if (m_process->isOpen())
	{
		m_process->write("quit\n");
		m_isExitEvent = true;
		m_process->waitForFinished();
	}

	m_topWidget->rmAll();
	m_currentMusicList.clear();
	m_musicList.clear();
	m_currentPlayMusic = -1;
	m_userName = "";
	m_userPasswd = "";
	m_nickName = "";
	m_userId = 0;
	m_topWidget->rmAllList();
	m_topWidget->setUserInfo(":/default/user_head", "登录");
	m_bottomWIdget->setPlayButtonStatues(":/func_btn/play_btn");
}

void NetWorkWidget::closeWidget()
{
	if (!m_process->isOpen())
		return;

	m_isPlaying = false;
	m_process->write("quit\n");
	m_isExitEvent = true;
	m_process->waitForFinished();
}

void NetWorkWidget::quitPlay()
{
	if (m_isPlaying)
	{
		m_isPlaying = false;
		m_process->write("pause\n");
		m_bottomWIdget->setPlayButtonStatues(":/func_btn/play_btn");
	}
}





void NetWorkWidget::finished(QNetworkReply *reply)
{
	int statues = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

	if (reply->error() != QNetworkReply::NoError)
	{
		m_type = NULL_REQUEST;
		reply->deleteLater();
		return;
	}

	if (statues == 200)
	{
		switch (m_type) {
		case LOGIN:
			dealLogIn(reply);
			m_logInWidget->setVisible(false);
			getMusicList();
			break;
		case GETMUSICLIST:
			dealGetMusicList(reply);
			break;
		case GETLISTINFO:
			dealGetListInfo(reply);
			break;
		case SEARCH:
			dealSeach(reply);
			getSongsUrl();
			break;
		case GETMP3URL:
			dealSongId(reply);
			break;
		default:
			break;
		}
	}
	reply->deleteLater();
}


void NetWorkWidget::logIn(const QString &name, const QString &passwd)
{
	m_userName = name;
	m_userPasswd = "";
	QCryptographicHash md(QCryptographicHash::Md5);
	QByteArray ba, bb;
	ba.append(passwd);
	md.addData(ba);
	bb = md.result();
	m_userPasswd.append(bb.toHex());

	QUrl url("http://music.163.com/api/login/");
	QNetworkRequest request(url);
	request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
	//		压缩数据.....
	//request.setRawHeader("Accept-Encoding", "gzip, deflate");
	request.setRawHeader("Accept-Language", "zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3");
	request.setRawHeader("Connection", "keep-alive");
	request.setRawHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
	request.setRawHeader("Host", "music.163.com");
	request.setRawHeader("Referer", "http://music.163.com/");
	request.setRawHeader("User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:38.0) Gecko/20100101 Firefox/38.0");

	QByteArray data;
	data.append("username=" + m_userName + "&password=" + m_userPasswd + "&rememberLogin: true");

	m_type = LOGIN;

	//QNetworkReply *reply = m_accessManager->post(request, data);
	m_accessManager->post(request, data);
}
void NetWorkWidget::dealLogIn(QNetworkReply *reply)
{
	QByteArray data = reply->readAll();
	QJsonParseError err;
	QJsonDocument json = QJsonDocument::fromJson(data, &err);
	if (err.error != QJsonParseError::NoError)
		return;

	QJsonObject obj = json.object();
	int code = obj.find("code").value().toInt();
	switch (code) {
	case 200:
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

	QJsonObject::const_iterator it = obj.find("profile");
	if (it != obj.constEnd())
	{
		QJsonObject profileObj = it.value().toObject();
		m_userId = profileObj.find("userId").value().toInt();
		m_nickName = profileObj.find("nickname").value().toString();
		QString str = ":/default/user_head";
		m_topWidget->setUserInfo(str, m_nickName);
	}
}



void NetWorkWidget::getMusicList(int offset, int limit)
{
	QString u = QString("http://music.163.com/api/user/playlist/?offset=%1&limit=%2&uid=%3").arg(offset).arg(limit).arg(m_userId);
	QUrl url(u);

	QNetworkRequest request(url);

	request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
	//		压缩数据.....
	request.setRawHeader("Accept-Language", "zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3");
	request.setRawHeader("Connection", "keep-alive");
	request.setRawHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
	request.setRawHeader("Host", "music.163.com");
	request.setRawHeader("Pragma", "no-cache");
	request.setRawHeader("Cache-Control", "no-cache");
	request.setRawHeader("Referer", "http://music.163.com/");
	request.setRawHeader("User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:38.0) Gecko/20100101 Firefox/38.0");

	m_type = GETMUSICLIST;
	m_accessManager->get(request);
}
void NetWorkWidget::dealGetMusicList(QNetworkReply *reply)
{
	QByteArray data = reply->readAll();
	QJsonParseError err;
	QJsonDocument json = QJsonDocument::fromJson(data, &err);
	if (err.error != QJsonParseError::NoError)
		return;

	QJsonObject obj = json.object();

	//		获取歌单...
	QJsonObject::const_iterator it = obj.find("playlist");
	if (it != obj.constEnd())
	{
		QJsonArray array = it.value().toArray();
		for (int j = 0; j < array.count(); j++)
		{
			QJsonObject mObj = array.at(j).toObject();
			QString name = mObj.find("name").value().toString();
			int id = mObj.find("id").value().toInt();
			m_topWidget->addListItem(name);
			m_musicList.insert(name, id);
		}
	}
}

void NetWorkWidget::search(const QString &search)
{
	QUrl url("http://music.163.com/api/search/get/web");
	QNetworkRequest request(url);
	request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
	request.setRawHeader("Accept-Language", "zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3");
	request.setRawHeader("Connection", "keep-alive");
	request.setRawHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
	request.setRawHeader("Host", "music.163.com");
	request.setRawHeader("Referer", "http://music.163.com/");
	request.setRawHeader("User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:38.0) Gecko/20100101 Firefox/38.0");

	QByteArray data = "";
	data.append("s=" + search + "&type=1&offset=0&total=true&limit=60");

	m_type = SEARCH;
	m_accessManager->post(request, data);
}




void NetWorkWidget::musicListItemClicked(const QString &name)
{
	int listId = m_musicList.value(name);
	QUrl url = "http://music.163.com/api/playlist/detail?id=" + QString::number(listId, 10);
	QNetworkRequest request(url);

	m_type = GETLISTINFO;
	m_accessManager->get(request);
}
void NetWorkWidget::dealGetListInfo(QNetworkReply *reply)
{
	QByteArray data = reply->readAll();
	QJsonParseError err;
	QJsonDocument json = QJsonDocument::fromJson(data, &err);
	if (err.error != QJsonParseError::NoError)
		return;

	QJsonObject obj = json.object().find("result").value().toObject();
	QJsonArray listObj = obj.find("tracks").value().toArray();
	m_currentMusicList.clear();
	m_topWidget->rmAll();
	for (int i = 0; i < listObj.size(); i++)
	{
		QJsonObject mp3Obj = listObj.at(i).toObject();
		QString url = mp3Obj.find("mp3Url").value().toString();
		QString name = mp3Obj.find("name").value().toString();
		QJsonArray artistsList = mp3Obj.find("artists").value().toArray();
		QString artists = "";
		for (int j = 0; j < artistsList.count(); j++)
		{
			QJsonObject artistsObjName = artistsList.at(j).toObject();
			artists.append(artistsObjName.find("name").value().toString() + " ");
		}
		QJsonObject albumObj = mp3Obj.find("album").value().toObject();
		QString album = albumObj.find("name").value().toString();
		m_currentMusicList.insert(i + 1, url);
		m_topWidget->addMusicItem(name, artists, album);
	}
	m_topWidget->setCurrentFile(0);
}

void NetWorkWidget::dealSeach(QNetworkReply *reply)
{
	QByteArray data = reply->readAll();
	QJsonParseError err;
	QJsonDocument json = QJsonDocument::fromJson(data, &err);
	if (err.error != QJsonParseError::NoError)
		return;
	QJsonObject obj = json.object().find("result").value().toObject();
	QJsonArray objList = obj.find("songs").value().toArray();
	m_topWidget->rmAll();
	m_currentMusicList.clear();
	m_songsId.clear();
	for (int i = 0; i < objList.count(); i++)
	{
		QJsonObject songObj = objList.at(i).toObject();
		int songId = songObj.find("id").value().toInt();
		QString name = songObj.find("name").value().toString();
		QJsonArray artistsList = songObj.find("artists").value().toArray();
		QString artists = "";
		for (int j = 0; j < artistsList.count(); j++)
			artists.append(artistsList.at(j).toObject().find("name").value().toString());
		QString album = songObj.find("album").value().toObject().find("name").value().toString();
		m_songsId.insert(songId, i + 1);
		m_topWidget->addMusicItem(name, artists, album);
	}
	m_topWidget->setCurrentFile(0);
}

void NetWorkWidget::getSongsUrl()
{
	QHash<int, int>::const_iterator it = m_songsId.begin();
	while (it != m_songsId.end())
	{
		QUrl url = QString("http://music.163.com/api/song/detail?ids=[%1]").arg(it.key());
		QNetworkRequest request(url);
		m_type = GETMP3URL;
		m_accessManager->get(request);
		it++;
	}
}

void NetWorkWidget::dealSongId(QNetworkReply *reply)
{
	QByteArray data = reply->readAll();
	QJsonParseError err;
	QJsonDocument json = QJsonDocument::fromJson(data, &err);
	if (err.error != QJsonParseError::NoError)
		return;
	QJsonArray obj = json.object().find("songs").value().toArray();
	QString url = obj.at(0).toObject().find("mp3Url").value().toString();
	int id = obj.at(0).toObject().find("id").value().toInt();
	m_currentMusicList.insert(m_songsId.value(id), url);
}














//			播放模块...

void NetWorkWidget::musicItemClicked(int index)
{
	if (m_process->isOpen())
	{
		m_process->write("quit\n");
		m_isExitEvent = true;
		m_process->waitForFinished();
	}
	if ( (index >=  0) && (index < m_currentMusicList.count()))
		m_currentPlayMusic = index + 1;
	else
		m_currentPlayMusic = 1;
	playMedia(m_currentMusicList.value(m_currentPlayMusic));
}

void NetWorkWidget::playMedia(const QString &path)
{
	if (path == "")
	{
		QMessageBox::critical(this, "播放出错", "当前播放列表没有资源...", QMessageBox::Ok | QMessageBox::Cancel);
		m_currentPlayMusic = -1;
		return;
	}
	QStringList arguments;
	arguments << "-slave";			//	slave model..
	arguments << "-quiet";		//	不输出冗余信息...
	arguments << path;

	//m_process->start("mplayer/mplayer", arguments);
	m_process->start("mplayer", arguments);
	m_topWidget->setCurrentFile(m_currentPlayMusic - 1);
	m_bottomWIdget->setPlayButtonStatues(":/func_btn/stop_btn");
	m_isPlaying = true;
}

void NetWorkWidget::playFinished(int)
{
	if (m_isExitEvent)
	{
		m_isExitEvent = false;
		return;
	}

	if (m_currentPlayMusic == m_currentMusicList.count())
		m_currentPlayMusic = 1;
	else
		m_currentPlayMusic++;
	playMedia(m_currentMusicList.value(m_currentPlayMusic));
}

void NetWorkWidget::play()
{
	if (m_currentPlayMusic <= 0)
	{
		m_currentPlayMusic = 1;
		QString path = m_currentMusicList.value(m_currentPlayMusic);
		if (path == "")
		{
			QMessageBox::critical(this, "播放出错", "当前播放列表没有资源...", QMessageBox::Ok | QMessageBox::Cancel);
			m_currentPlayMusic = -1;
			return;
		}
		m_topWidget->setCurrentFile(m_currentPlayMusic);
		playMedia(path);
		return;
	}

	m_process->write("pause\n");
	if (m_isPlaying)
	{
		m_isPlaying = false;
		m_bottomWIdget->setPlayButtonStatues(":/func_btn/play_btn");
	}
	else
	{
		m_bottomWIdget->setPlayButtonStatues(":/func_btn/stop_btn");
		m_isPlaying = true;
	}
}

void NetWorkWidget::playNext()
{
	if (m_process->isOpen())
	{
		m_process->write("quit\n");
		m_isExitEvent = true;
		m_process->waitForFinished();
	}

	if ((m_currentPlayMusic >= m_currentMusicList.count()) || (m_currentPlayMusic < 1))
		m_currentPlayMusic = 1;
	else
		m_currentPlayMusic++;
	playMedia(m_currentMusicList.value(m_currentPlayMusic));
}

void NetWorkWidget::playFront()
{
	if (m_process->isOpen())
	{
		m_process->write("quit\n");
		m_isExitEvent = true;
		m_process->waitForFinished();
	}
	if (m_currentPlayMusic == 1)
		m_currentPlayMusic = m_currentMusicList.count();
	else
		m_currentPlayMusic--;
	playMedia(m_currentMusicList.value(m_currentPlayMusic));
}


void NetWorkWidget::adjustVoice(int voice)
{
	QString str = QString("volume %1 2\n").arg(voice);
	QByteArray data = "";
	data.append(str);
	m_process->write(data);
}
void NetWorkWidget::adjustDuration(qreal pos)
{
	double p = pos * m_duration;
	m_process->write(QString("seek " + QString::number(p) + " 2\n").toUtf8());
}

void NetWorkWidget::getMusicTime()
{
	m_process->write("get_time_length\n");
	m_process->write("get_time_pos\n");
	QString length = "";
	QString pos = "";

	while (m_process->canReadLine())
	{
		QByteArray data = m_process->readLine();
		data.replace(QByteArray("\n"), QByteArray(""));
		QString s(data);

		if (data.startsWith("ANS_LENGTH"))
		{
			length = s.mid(11);
		}
		else if (data.startsWith("ANS_TIME_POSITION"))
		{
			pos = s.mid(18);
		}
	}

	if ( (length != "") && (pos != ""))
	{
		double curPos = pos.toDouble();
		double duration = length.toDouble();
		int min = curPos / 60;
		int sec = (int)curPos % 60;
		QString current = QString("%1:%2").arg(min).arg(sec);
		min = duration / 60;
		sec = (int)duration % 60;
		QString dur = QString("%1:%2").arg(min).arg(sec);
		m_bottomWIdget->setPostion(current, dur);
		m_bottomWIdget->currentPosSlider((qreal)(curPos/duration));
		m_duration = duration;
	}
}


//		定时器...
void NetWorkWidget::timerEvent(QTimerEvent *)
{
	if (m_isPlaying)
		getMusicTime();
}








