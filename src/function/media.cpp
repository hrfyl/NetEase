#include "media.h"
#include <QProcess>

Media::Media(QObject *parent) : QObject(parent)
{
	m_isPlaying = false;
	m_isExitEvent = false;
	m_currentPlayMusic = -1;
	m_timerId = startTimer(200);
	m_process = new QProcess(this);
	connect(m_process, SIGNAL(finished(int)), this, SLOT(playFinished(int)));
}

Media::~Media()
{
	killTimer(m_timerId);
}

void Media::logOut()
{
	if (m_process->isOpen())
	{
		m_isExitEvent = true;
		m_isPlaying = false;
		m_process->write("quit\n");
		m_process->waitForFinished();
	}

	m_currentMusicList.clear();
	m_currentPlayMusic = -1;
}

void Media::setMusicList(const QStringList &list)
{
	if (list.isEmpty())
		return;
	m_currentMusicList.clear();
	m_currentMusicList = list;
}

void Media::musicItemClicked(int index)
{
	if (m_process->isOpen())
	{
		m_process->write("quit\n");
		m_isExitEvent = true;
		m_process->waitForFinished();
	}

	m_currentPlayMusic = index;
	playMedia(m_currentMusicList.at(m_currentPlayMusic));
}

void Media::playMedia(const QString &path)
{
	if (path == "")
	{
		emit messageBox("播放出错", "没有资源可以播放...");
		m_currentPlayMusic = -1;
		return;
	}
	QStringList arguments;
	arguments << "-slave";			//	slave model..
	arguments << "-quiet";			//	不输出冗余信息...
	arguments << path;

#ifdef WIN32
	m_process->start("mplayer/mplayer", arguments);		//	windows...
#endif
#ifndef WIN32
	m_process->start("mplayer", arguments);
#endif
	emit metaDataChanged(m_currentPlayMusic, STOP_BTN);
	m_isPlaying = true;
}

void Media::playFinished(int)
{
	if (m_isExitEvent)
	{
		m_isExitEvent = false;
		return;
	}

	if (m_currentPlayMusic == m_currentMusicList.count() - 1)
		m_currentPlayMusic = 0;
	else
		m_currentPlayMusic++;
	playMedia(m_currentMusicList.at(m_currentPlayMusic));
}

void Media::play()
{
	if (m_currentPlayMusic < 0)
	{
		m_currentPlayMusic = 0;
		QString path = m_currentMusicList.at(m_currentPlayMusic);
		if (path == "")
		{
			emit messageBox("播放出错", "当前播放列表没有资源...");
			m_currentPlayMusic = -1;
			return;
		}
		emit metaDataChanged(m_currentPlayMusic, "");
		playMedia(path);
		return;
	}

	m_process->write("pause\n");
	if (m_isPlaying)
	{
		m_isPlaying = false;
		emit metaDataChanged(m_currentPlayMusic, PLAY_BTN);
	}
	else
	{
		emit metaDataChanged(m_currentPlayMusic, STOP_BTN);
		m_isPlaying = true;
	}
}

void Media::playNext()
{
	if (m_process->isOpen())
	{
		m_process->write("quit\n");
		m_isExitEvent = true;
		m_process->waitForFinished();
	}

	if ((m_currentPlayMusic + 1 >= m_currentMusicList.count()) || (m_currentPlayMusic < 0))
		m_currentPlayMusic = 0;
	else
		m_currentPlayMusic++;
	playMedia(m_currentMusicList.at(m_currentPlayMusic));
}

void Media::playFront()
{
	if (m_process->isOpen())
	{
		m_process->write("quit\n");
		m_isExitEvent = true;
		m_process->waitForFinished();
	}
	if ( (m_currentPlayMusic <= 0) || (m_currentPlayMusic >= m_currentMusicList.count() - 1))
		m_currentPlayMusic = m_currentMusicList.count() - 1;
	else
		m_currentPlayMusic--;
	playMedia(m_currentMusicList.at(m_currentPlayMusic));
}

void Media::playStop()
{
	if (!m_process->isOpen())
		return;

	m_isPlaying = false;
	m_process->write("quit\n");
	m_isExitEvent = true;
	m_process->waitForFinished();
}

void Media::pause()
{
	if (m_isPlaying)
	{
		m_process->write("pause\n");
		m_isPlaying = false;
		emit metaDataChanged(m_currentPlayMusic, PLAY_BTN);
	}
}

void Media::adjustVoice(int voice)
{
	QString str = QString("volume %1 2\n").arg(voice);
	QByteArray data = "";
	data.append(str);
	m_process->write(data);
}
void Media::adjustDuration(qreal pos)
{
	double p = pos * m_duration;
	m_process->write(QString("seek " + QString::number(p) + " 2\n").toUtf8());
}

void Media::getMusicTime()
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
		emit postionChanged(current, dur, (qreal)(curPos/duration), curPos);
	}
}


//		定时器...
void Media::timerEvent(QTimerEvent *)
{
	if (m_isPlaying)
		getMusicTime();
}

