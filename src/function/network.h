#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include "../datatypedef/urlinfo.h"
#include <QHash>
#include <QStringList>

class QNetworkReply;
class QJsonObject;
class QNetworkAccessManager;
class QNetworkRequest;

class NetWork : public QObject
{
	Q_OBJECT
public:
	NetWork(QObject *parent = 0);
	~NetWork();

	QStringList userInfo();
	void setUserInfo(const QStringList &list);

	void logOut();												//	注销...

	void getMusicList(int offset = 0, int limit = 100);			//	请求获取歌单...

	void getLyric(int songId);									//	获取歌词...

signals:
	void logInStatus(int code, const QString &msg);					//	登录失败...
	void musicList(const QStringList &list, QVector<int> &vector);	//	歌单数据,发送到NetWorkWidget..
	void musicInfo(const QList<QStringList> &list);				//	歌单详细信息(url等)...

	void messageBox(const QString &title, const QString &msg);	//	发送窗口消息到NetWoekWidget...

	void lyric(const QString &lyric);							//	发送lyric到NetWorkWidget...

public slots:
	//		登录...
	void logIn(const QString &, const QString &);

	void search(const QString &search);							//	搜索单曲.歌单...
	void getListInfo(int listId);								//	获取歌单信息...

private slots:
	void finished(QNetworkReply *reply);						//	从网络中读取数据..

private:
	//		网络请求...
	void initHead(QNetworkRequest &request);					//	初始化消息头...
	void sendPost(const QUrl &url, const QByteArray &data);		//	发送POST请求..
	void sendGet(const QUrl &url, bool isInitHead=false);			//	发送GET请求..

	QJsonObject getObject(QNetworkReply &reply);				//	返回QJsonObject对象..

	//		处理网络数据..
	void dealLogIn(QNetworkReply *reply);						//	登录处理...
	void dealGetMusicList(QNetworkReply *reply);				//	获取音乐歌单..
	void dealGetListInfo(QNetworkReply *reply);					//	获取歌单详细信息..
	void dealSeach(QNetworkReply *reply);						//	搜索结果处理..
	void getSongsUrl();
	void dealSongId(QNetworkReply *reply);						//	通过songid获取songurl..
	void dealLyric(QNetworkReply *reply);						//	歌词信息处理...

private:
	//		网络相关..
	QNetworkAccessManager *m_accessManager;
	//int m_type;			//		区分不同网络请求...(之前版本使用,现在使用url区分不同请求..)

	//		用户信息...
	int m_userId;
	QString m_nickName;
	QString m_userName;
	QString m_userPasswd;							//		保存md5加密后的数据..

	QHash<QString, int> m_musicList;				//	暂存歌单...
	QList<QStringList> m_list;						//	暂存歌曲信息...
	QHash<int, int> m_songIds;

	int m_musicCount;
};

#endif // NETWORK_H
