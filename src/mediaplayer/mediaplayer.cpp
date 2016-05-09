#include "mediaplayer.h"
#include "../network/networkmanage.h"
#include <QProcess>

#ifdef QT_DEBUG
#include <QDebug>
#endif

MediaPlayer * MediaPlayer::m_player = Q_NULLPTR;
bool MediaPlayer::m_isPlaying = false;
bool MediaPlayer::m_isExitEvent = false;
LOOPTYPE MediaPlayer::m_loopType = ALLLOOP;
int MediaPlayer::m_currentMusicIndex = -1;

MediaPlayer *MediaPlayer::sharedMediaPlayer()
{
	if (m_player == Q_NULLPTR)
	{
		m_player = new MediaPlayer;
		m_player->init();
	}
	return m_player;
}

void MediaPlayer::appExit()
{
	stop();
	killTimer(m_timerId);
}

void MediaPlayer::logOutEvent()
{
	if (m_process->isOpen())
		stop();
	m_currentMusicIndex = -1;
	m_musicListId.clear();
	m_musicListUrl.clear();
	m_songIds.clear();
}

void MediaPlayer::init()
{
	m_process = new QProcess(this);
	connect(m_process, SIGNAL(finished(int)), this, SLOT(finished(int)));
	m_timerId = startTimer(500);
}


QStringList MediaPlayer::musicListUrl() const
{
	return m_musicListUrl;
}

void MediaPlayer::setMusicListUrl(const QStringList &musicListUrl)
{
	m_musicListUrl = musicListUrl;
}


QVector<int> MediaPlayer::musicListId() const
{
	return m_musicListId;
}

void MediaPlayer::setMusicListId(const QVector<int> &musicListId)
{
	m_musicListId = musicListId;
}


void MediaPlayer::stop()
{
	if (!m_process->isOpen())
		return;
	m_process->write("quit\n");
	m_isPlaying = false;
	m_isExitEvent = true;
	m_process->waitForFinished();
}
void MediaPlayer::pause()
{
	if (m_isPlaying)
	{
		m_process->write("pause\n");
		m_isPlaying = false;
	}
	emit buttonStatusChanged();
	emit playStatusChanged(m_currentMusicIndex);
}
void MediaPlayer::play()
{
	if (m_currentMusicIndex < 0)
	{
		if (m_songIds.isEmpty())
		{
#ifdef QT_DEBUG
			qDebug() << "Mediaplayer error: No Media in mediaplay.cpp";
#endif
			return;
		}
		else
			m_currentMusicIndex = 0;
		playMedia(getPlayPath());
		NetworkManage::sharedNetworkManage()->lyric(m_songIds[m_currentMusicIndex]);
		return;
	}

	m_process->write("pause\n");
	if (m_isPlaying)
		m_isPlaying = false;
	else
		m_isPlaying = true;


	emit buttonStatusChanged();
	emit playStatusChanged(m_currentMusicIndex);
}

void MediaPlayer::play(int index)
{
	m_currentMusicIndex = index;
	stop();
	playMedia(getPlayPath());
	NetworkManage::sharedNetworkManage()->lyric(m_songIds[m_currentMusicIndex]);
}
void MediaPlayer::next()
{
	stop();
	playPath();
	playMedia(getPlayPath());
}
void MediaPlayer::front()
{
	stop();
	playPath(false);
	playMedia(getPlayPath());
}

void MediaPlayer::finished(const int &)
{
	if (m_isExitEvent)
	{
		m_isExitEvent = false;
		return;
	}

	playPath();
	playMedia(getPlayPath());
}

void MediaPlayer::playMedia(const QString &path)
{
	if (path == "")
	{
#ifdef QT_DEBUG
		qDebug() << "Mediaplayer error: No Media in playMedia function";
#endif
		m_currentMusicIndex = -1;
		return;
	}
	QStringList arguments;
	arguments << "-slave";			//	slave model..
	arguments << "-quiet";			//	不输出冗余信息...
	arguments << path;

#ifdef Q_OS_UNIX
	m_process->start("mplayer", arguments);
#endif
#ifdef WIN32
	m_process->start("./mplayer/mplayer.exe", arguments);
#endif
	m_isPlaying = true;
	emit playStatusChanged(m_currentMusicIndex);
	emit buttonStatusChanged();
}

void MediaPlayer::playPath(bool order)
{
	int tempIndex = m_currentMusicIndex;
	switch (m_loopType) {
	case ALLLOOP:
	{
		if (m_currentMusicIndex == m_songIds.count() - 1 && order)
			m_currentMusicIndex = 0;
		else if (m_currentMusicIndex != m_songIds.count() - 1 && order)
			++m_currentMusicIndex;
		else if (m_currentMusicIndex == 0 && !order)
			m_currentMusicIndex = m_songIds.count() - 1;
		else if (m_currentMusicIndex != 0 && !order)
			--m_currentMusicIndex;
		break;
	}
	case ONELOOP:
		break;
	case RANDOMLOOP:
		m_currentMusicIndex = RANDOM_NUM(m_songIds.count());
		break;
	default:
		break;
	}
	if (tempIndex != m_currentMusicIndex)
		NetworkManage::sharedNetworkManage()->lyric(m_songIds[m_currentMusicIndex]);
}

QString MediaPlayer::getPlayPath()
{
	QString path = "";
	if (m_musicListUrl.isEmpty() && (!m_songIds.isEmpty()))
		path = NetworkManage::sharedNetworkManage()->getMusicUrlBySongId(m_songIds.at(m_currentMusicIndex));
	else
		path = m_musicListUrl.at(m_currentMusicIndex);

	return path;
}




/*
 * 进度和音量控制...
 *
 */
void MediaPlayer::setVoice(const int &pos)
{
	if (!m_process->isOpen())
		return;
	QString str = QString("volume %1 2\n").arg(pos);
	QByteArray data = "";
	data.append(str);
	m_process->write(data);
}
void MediaPlayer::setPostion(const qreal &pos)
{
	if (!m_process->isOpen())
		return;
	double p = pos * m_duration;
	m_process->write(QString("seek " + QString::number(p) + " 2\n").toUtf8());
}

//		定时器...
void MediaPlayer::timerEvent(QTimerEvent *)
{
	if (m_isPlaying)
	{
		musicTime();
		emit updateLyric(m_currentTime);
	}
}

void MediaPlayer::musicTime()
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
		m_duration = duration;
		m_currentTime = curPos;
		emit postionChanged(current, dur, (qreal)(curPos/duration));
	}
}




int MediaPlayer::currentMusicIndex()
{
	return m_currentMusicIndex;
}

QVector<int> MediaPlayer::songIds() const
{
	return m_songIds;
}

void MediaPlayer::setSongIds(const QVector<int> &songIds)
{
	m_songIds = songIds;
}

LOOPTYPE MediaPlayer::loopType()
{
	return m_loopType;
}

void MediaPlayer::rmMusic(const int &index)
{
	if (m_songIds.isEmpty())
		return;
	m_songIds.remove(index);
	if (!m_musicListUrl.isEmpty())
		m_musicListUrl.removeAt(index);
	play(index);
}

void MediaPlayer::clearUrls()
{
	m_musicListUrl.clear();
}

void MediaPlayer::setLoopType(const LOOPTYPE &loopType)
{
	m_loopType = loopType;
}

double MediaPlayer::duration() const
{
	return m_duration;
}

void MediaPlayer::setDuration(double duration)
{
	m_duration = duration;
}


bool MediaPlayer::isPlaying()
{
	return m_isPlaying;
}

void MediaPlayer::setIsPlaying(bool isPlaying)
{
	m_isPlaying = isPlaying;
}

