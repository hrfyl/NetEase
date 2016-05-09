#include "networklistdetails.h"
#include "../configure/urlconfigure.h"
#include "../mediaplayer/mediaplayer.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QStringList>
#include <QVector>

NetworkListDetails::NetworkListDetails(QObject *parent) :
	NetworkBase(parent)
{

}

void NetworkListDetails::listDetails(const int &listId)
{
	QUrl url = LISTDETAILS_URL + QString::number(listId, 10);
	sendGet(url);
}

void NetworkListDetails::disposeifno(const QByteArray &data)
{
	QJsonObject obj;
	if (!jsonObject(data, obj))
		return;
	QJsonObject resultObj = obj["result"].toObject();
	QJsonArray listObj = resultObj["tracks"].toArray();
	QVector<int> songIds;

	QList<QStringList> rev_list;
	QStringList urlList;
	for (int i = 0; i < listObj.size(); ++i)
	{
		QJsonObject mp3Obj = listObj[i].toObject();
		QString url = mp3Obj["mp3Url"].toString();
		QString name = mp3Obj["name"].toString();
		QJsonArray artistsList = mp3Obj["artists"].toArray();
		QString artists = "";
		for (int j = 0; j < artistsList.count(); ++j)
		{
			QJsonObject artistsObjName = artistsList[j].toObject();
			artists.append(artistsObjName["name"].toString() + " ");
		}
		QJsonObject albumObj = mp3Obj["album"].toObject();
		QString album = albumObj["name"].toString();

		songIds.append(mp3Obj["id"].toInt());

		urlList.append(url);

		QStringList list;
		list << name << artists << album;
		rev_list.insert(i, list);
	}
	MediaPlayer::sharedMediaPlayer()->setMusicListUrl(urlList);
	MediaPlayer::sharedMediaPlayer()->setSongIds(songIds);
	emit listDetailsSave(rev_list);
}

















