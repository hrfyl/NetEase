#include "../configure/configure.h"
#include "networkmanage.h"
#include "networkbase.h"
#include "networklogin.h"
#include "networkmusiclist.h"
#include "networklistdetails.h"
#include "networkmusiclyric.h"
#include "networkrecommend.h"
#include "networkgetsongsbyid.h"
#include "networksearchmusic.h"
#include "networkaddmusic.h"
#include "networkgetuserheadimage.h"
#include "networkradiofm.h"

NetworkManage * NetworkManage::m_networkManage = Q_NULLPTR;

NetworkManage *NetworkManage::sharedNetworkManage()
{
	if (m_networkManage == Q_NULLPTR)
	{
		m_networkManage = new NetworkManage;
		m_networkManage->init();
	}
	return m_networkManage;
}


void NetworkManage::init()
{
	m_logIner = new NetworkLogin(this);
	connect(m_logIner, SIGNAL(logined(bool)), this, SIGNAL(logined(bool)));

	m_musicLister = new NetworkMusicList(this);
	connect(m_musicLister, SIGNAL(musicListInfo(QStringList)),
			this, SIGNAL(musicListInfo(QStringList)));

	m_listDetailser = new NetworkListDetails(this);
	connect(m_listDetailser, SIGNAL(listDetailsSave(QList<QStringList>)),
			this, SIGNAL(listDetailsSave(QList<QStringList>)));

	m_musicLyricer =  new NetworkMusicLyric(this);
	connect(m_musicLyricer, SIGNAL(lyric(QString)), this, SIGNAL(lyric(QString)));

	m_getSongUrlsByIder = new NetworkGetSongsById(this, true);

	m_recommender = new NetworkRecommend(this);
	connect(m_recommender, SIGNAL(musicInfo(QList<QStringList>)),
			this, SIGNAL(listDetailsSave(QList<QStringList>)));

	m_searcher = new NetworkSearchMusic(this);
	connect(m_searcher, SIGNAL(msuicInfo(QList<QStringList>)),
			this, SIGNAL(searchDetailsSave(QList<QStringList>)));

	m_addmusicer = new NetworkAddMusic(this);

	m_getUserImager = new NetworkGetUserHeadImage(this);
	connect(m_getUserImager, SIGNAL(userHeadImageGeted(QByteArray)),
			this, SIGNAL(userHeadImageGeted(QByteArray)));

	m_randioFMer = new NetworkRadioFM(this);
	connect(m_randioFMer, SIGNAL(randioFMMusicInfo(QList<QStringList>)),
			this, SIGNAL(listDetailsSave(QList<QStringList>)));
}

void NetworkManage::logIn(const QString &userName, const QString &passwd)
{
	m_logIner->logIn(userName, passwd);
}

void NetworkManage::musicList(const int &userId, int offset, int limit)
{
	m_musicLister->musicList(userId, offset, limit);
}

void NetworkManage::musicListDetails(const int &listId)
{
	m_listDetailser->listDetails(listId);
}

void NetworkManage::lyric(const int &musicId)
{
	m_musicLyricer->musicLyric(musicId);
}

void NetworkManage::recommed()
{
	if (Configure::userCookies != "")
		m_recommender->recommend();
}

void NetworkManage::rmMusicFromrecommed(const int &index)
{
	m_recommender->rmMusicInfo(index);
}

void NetworkManage::refreshRecommed()
{
	m_recommender->refresh();
}

void NetworkManage::likeMusic(const QString &songId, bool like, QString time, QString alg)
{
	QString _like;
	if (like)
		_like = "true";
	else
		_like = "false";
	m_addmusicer->addMusic(songId, _like, time, alg);
}

void NetworkManage::userHeadImage()
{
	m_getUserImager->userHeadImage();
}

QString NetworkManage::getMusicUrlBySongId(const int &id)
{
	return m_getSongUrlsByIder->getMusicUrlBySongId(id);
}

void NetworkManage::randioFM(bool getNew)
{
	if (getNew)
		m_randioFMer->randioFM();
	else
		m_randioFMer->randioMusicInfo();
}

void NetworkManage::trashFM(const int &songId, QString time, QString alg)
{
	m_randioFMer->trashFM(songId, time, alg);
}

void NetworkManage::search(const QString &_search)
{
	m_searcher->search(_search);
}

void NetworkManage::rmMusicFromSearch(const int &index)
{
	m_searcher->rmMusicInfo(index);
}











