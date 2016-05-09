#ifndef NETWORKMANAGE_H
#define NETWORKMANAGE_H

#include <QObject>
#include "../configure/configure.h"
#include <QList>
#include <QStringList>

class NetworkBase;
class NetworkLogin;
class NetworkMusicList;
class NetworkListDetails;
class NetworkMusicLyric;
class NetworkRecommend;
class NetworkGetSongsById;
class NetworkSearchMusic;
class NetworkAddMusic;
class NetworkGetUserHeadImage;
class NetworkRadioFM;

class NetworkManage : public QObject
{
	Q_OBJECT
public:
	static NetworkManage *sharedNetworkManage();

	void logIn(const QString &userName, const QString &passwd);			//	登录...

	void musicList(const int &userId, int offset = 0, int limit = 100);	//	获取歌单...

	void musicListDetails(const int &listId);							//	获取歌曲...

	void lyric(const int &);

	void recommed();					//	每日推荐..
	void rmMusicFromrecommed(const int &);
	void refreshRecommed();				//	刷新每日推荐...

	void likeMusic(const QString &, bool like = true,
				   QString time = "25", QString alg = "itembased");	//	将歌曲添加到喜欢列表..

	void userHeadImage();

	QString getMusicUrlBySongId(const int &id);

	void randioFM(bool getNew = false);				//	获取私人FM

	void trashFM(const int &songId, QString time = "25", QString alg = "RT");	//	添加私人FM黑名单..

public slots:
	void search(const QString &);
	void rmMusicFromSearch(const int &);

signals:
	void logined(const bool &);

	void musicListInfo(const QStringList &);			//获取到歌单信息...

	void listDetailsSave(const QList<QStringList> &);	//	歌单中歌曲信息...
	void searchDetailsSave(const QList<QStringList> &);	//	搜索歌单中歌曲信息...

	void lyric(const QString &);						//	获取到当前歌曲的歌词..

	void userHeadImageGeted(const QByteArray &);		//	成功获取用户头像..

private:
	void init();

private:
	static NetworkManage *m_networkManage;

	NetworkLogin *m_logIner;				//	登录...
	NetworkMusicList *m_musicLister;		//	获取歌单..
	NetworkListDetails *m_listDetailser;	//	获取指定歌单的歌曲..
	NetworkMusicLyric *m_musicLyricer;		//	获取指定歌曲的歌词..
	NetworkRecommend *m_recommender;		//	每日推荐歌单...
	NetworkGetSongsById *m_getSongUrlsByIder;	//	通过Songs Id获得Song Url
	NetworkSearchMusic *m_searcher;			//	搜索..
	NetworkAddMusic *m_addmusicer;			//	喜欢music..
	NetworkGetUserHeadImage *m_getUserImager;// 获取用户头像..
	NetworkRadioFM *m_randioFMer;			//	私人FM...
};

#endif // NETWORKMANAGE_H
