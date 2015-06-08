#include "network.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QVector>

NetWork::NetWork(QObject *parent) :
	QObject(parent), m_userId(0), m_userName(""), m_userPasswd("")
{
	m_accessManager = new QNetworkAccessManager(this);
	connect(m_accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
}

NetWork::~NetWork()
{

}

QStringList NetWork::userInfo()
{
	QStringList list;
	list << m_userName << m_userPasswd << QString::number(m_userId) << m_nickName;
	return list;
}

void NetWork::setUserInfo(const QStringList &list)
{
	m_userName = list.at(0);
	m_userPasswd = list.at(1);
	m_userId = list.at(2).toInt();
	m_nickName = list.at(3);
}

void NetWork::logOut()
{
	m_userId = 0;
	m_userName = "";
	m_userPasswd = "";
	m_nickName = "";
}

void NetWork::initHead(QNetworkRequest &request)
{
	request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
	request.setRawHeader("Accept-Language", "zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3");
	request.setRawHeader("Connection", "keep-alive");
	request.setRawHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
	request.setRawHeader("Host", "music.163.com");
	request.setRawHeader("Referer", "http://music.163.com/");
	request.setRawHeader("User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:38.0) Gecko/20100101 Firefox/38.0");
}

void NetWork::sendPost(const QUrl &url, const QByteArray &data)
{
	QNetworkRequest request(url);
	initHead(request);
	m_accessManager->post(request, data);
}

void NetWork::sendGet(const QUrl &url, bool isInitHead)
{
	QNetworkRequest request(url);
	if (isInitHead)
		initHead(request);
	m_accessManager->get(request);
}

QJsonObject NetWork::getObject(QNetworkReply &reply)
{
	QJsonObject rev;
	QByteArray data = reply.readAll();
	QJsonParseError err;
	QJsonDocument json = QJsonDocument::fromJson(data, &err);
	if (err.error != QJsonParseError::NoError)
	{
		emit messageBox("网络错误", "从网络获取的资源好像有点问题...");
		return rev;
	}
	QJsonObject obj = json.object();
	return obj;
}



void NetWork::finished(QNetworkReply *reply)
{
	if (reply->error() != QNetworkReply::NoError)
	{
		if (reply->error() == QNetworkReply::TimeoutError)
			emit messageBox("网络连接错误", "网络连接超时....");
		else if (reply->error() == QNetworkReply::HostNotFoundError)
			emit messageBox("网络错误", "找不到主机....");
		else if (reply->error() == QNetworkReply::RemoteHostClosedError)
			emit messageBox("网络错误", "与主机断开连接....");
		else
			emit messageBox("网络错误", "不明确的网络错误...");
		reply->deleteLater();
		return;
	}

	int statues = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	if (statues == 200)
	{
		QString url = reply->url().toString();
		if (url.left(LOGIN.size()) == LOGIN)
		{
			dealLogIn(reply);
			getMusicList();
		}
		else if (url.left(GETMUSICLIST.size()) == GETMUSICLIST)
			dealGetMusicList(reply);
		else if (url.left(GETLISTINFO.size()) == GETLISTINFO)
			dealGetListInfo(reply);
		else if (url.left(SEARCH.size()) == SEARCH)
		{
			dealSeach(reply);
			getSongsUrl();
		}
		else if (url.left(GETMP3URL.size()) == GETMP3URL)
			dealSongId(reply);
		else if (url.left(LYRIC.size()) == LYRIC)
			dealLyric(reply);
	}
	reply->deleteLater();
}

void NetWork::logIn(const QString &name, const QString &passwd)
{
	m_userName = name;
	m_userPasswd = "";
	QCryptographicHash md(QCryptographicHash::Md5);
	QByteArray ba, bb;
	ba.append(passwd);
	md.addData(ba);
	bb = md.result();
	m_userPasswd.append(bb.toHex());

	QUrl url(LOGIN);
	QByteArray data;
	data.append("username=" + m_userName + "&password=" + m_userPasswd + "&rememberLogin: true");

	sendPost(url, data);
}
void NetWork::dealLogIn(QNetworkReply *reply)
{
	QJsonObject obj = getObject(*reply);
	if (obj.isEmpty())
		return;
	int code = obj.find("code").value().toInt();
	if (code != 200)
	{
		emit logInStatus(code, "");
		return;
	}
	QJsonObject::const_iterator it = obj.find("profile");
	if (it != obj.constEnd())
	{
		QJsonObject profileObj = it.value().toObject();
		m_userId = profileObj.find("userId").value().toInt();
		m_nickName = profileObj.find("nickname").value().toString();
		//	头像....
		logInStatus(code, m_nickName);
	}
}

void NetWork::getMusicList(int offset, int limit)
{
	QString u = GETMUSICLIST + QString("%1&limit=%2&uid=%3").arg(offset).arg(limit).arg(m_userId);
	QUrl url(u);
	sendGet(url, true);
}

void NetWork::dealGetMusicList(QNetworkReply *reply)
{
	QJsonObject obj = getObject(*reply);
	if (obj.isEmpty())
		return;
	QStringList list;
	QVector<int> vector;
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

			list.append(name);
			vector.append(id);
		}
	}
	emit musicList(list, vector);
}

void NetWork::getLyric(int songId)
{
	QUrl url(LYRIC + QString::number(songId) +"&lv=-1&tv=-1");
	sendGet(url, true);
}
void NetWork::dealLyric(QNetworkReply *reply)
{
	QJsonObject obj = getObject(*reply);
	if (obj.isEmpty())
		return;
	QJsonObject lrcObj = obj.find("lrc").value().toObject();
	QString strLyric = lrcObj.find("lyric").value().toString();
	if (strLyric == "")
		strLyric = "当前歌曲没有歌词";
	emit lyric(strLyric);
}

void NetWork::getListInfo(int listId)
{
	QUrl url = GETLISTINFO + QString::number(listId, 10);
	sendGet(url);
}
void NetWork::dealGetListInfo(QNetworkReply *reply)
{
	QJsonObject obj = getObject(*reply);
	if (obj.isEmpty())
		return;
	QJsonObject resultObj = obj.find("result").value().toObject();
	QJsonArray listObj = resultObj.find("tracks").value().toArray();

	QList<QStringList> rev_list;
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
		int songId = mp3Obj.find("id").value().toInt();
		QStringList list;
		list << name << artists << album << url << QString::number(songId);
		rev_list.insert(i, list);
	}
	emit musicInfo(rev_list);
}

void NetWork::search(const QString &search)
{
	QUrl url(SEARCH);
	QByteArray data = "";
	data.append("s=" + search + "&type=1&offset=0&total=true&limit=60");
	sendPost(url, data);
}
void NetWork::dealSeach(QNetworkReply *reply)
{
	QJsonObject obj = getObject(*reply);
	if (obj.isEmpty())
		return;
	QJsonObject resultObj = obj.find("result").value().toObject();
	QJsonArray objList = resultObj.find("songs").value().toArray();

	m_list.clear();
	m_songIds.clear();
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

		m_songIds.insert(songId, i);
		QStringList list;
		list << name << artists << album << "" << QString::number(songId);
		m_list.insert(i, list);
	}
}

void NetWork::getSongsUrl()
{
	m_musicCount = 0;
	QHash<int, int>::const_iterator it = m_songIds.begin();
	while (it != m_songIds.end())
	{
		QUrl url = GETMP3URL + QString("[%1]").arg(it.key());
		sendGet(url);
		it++;
	}
}
void NetWork::dealSongId(QNetworkReply *reply)
{
	QJsonObject obj = getObject(*reply);
	if (obj.isEmpty())
		return;
	QJsonArray songObj = obj.find("songs").value().toArray();
	QString url = songObj.at(0).toObject().find("mp3Url").value().toString();
	int id = songObj.at(0).toObject().find("id").value().toInt();
	m_list[m_songIds.value(id)][3] = url;
	m_musicCount++;
	if (m_musicCount == m_songIds.count())
		emit musicInfo(m_list);
}















