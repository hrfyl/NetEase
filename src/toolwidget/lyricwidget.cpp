#include "lyricwidget.h"
#include "../Data/lyric.h"
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QLabel>
#include <qpainter.h>
//#include <QFile>
//#include <QDir>
//#include <QTextStream>

LyricWidget::LyricWidget(QWidget *parent) :
	BasedStyleShetWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
	setAttribute(Qt::WA_TranslucentBackground);
	setFixedSize(900, 80);
	m_isPressed = false;
	setObjectName("LyricWidget");

	m_lyricLabel = new QLabel;
	m_lyricLabel->setObjectName("CurrentLyric");
	m_nextLyricLabel = new QLabel;
	m_nextLyricLabel->setObjectName("NextCurrentLyric");
	QVBoxLayout *lyricLayout = new QVBoxLayout(this);
	lyricLayout->addWidget(m_lyricLabel, 0, Qt::AlignVCenter | Qt::AlignHCenter);
	lyricLayout->addSpacing(0);
	lyricLayout->addWidget(m_nextLyricLabel, 0, Qt::AlignVCenter | Qt::AlignHCenter);
	lyricLayout->setContentsMargins(5, 10, 5, 5);

	m_opacity = 0.5;
	m_color = "gray";
	m_isCursorEnter = false;
	m_lyricLabel->setText("");
	m_nextLyricLabel->setText("");
}

LyricWidget::~LyricWidget()
{

}

void LyricWidget::lyric(const QString &lyric)
{
	//		歌词处理...
//	QString path = QDir::currentPath();
//	path.append("/lyric.lrc");
//	QFile file(path);
//	file.open(QIODevice::WriteOnly | QIODevice::Text);
//	QTextStream stream(&file);
//	stream << lyric;
//	file.close();
	QStringList list = lyric.split("\n");
	QStringList::const_iterator it = list.begin();
	m_count = 0;
	while (it != list.end())
	{
		int index = (*it).indexOf(']');
		if (index <= 0)
		{
			it++;
			continue;
		}
		QString time = (*it).mid(1, index - 1);
		bool isOk = true;
		int min = time.mid(0, 2).toInt(&isOk) * 60000;
		if (!isOk)
		{
			it++;
			continue;
		}
		int sec = (int)(time.mid(3).toDouble(&isOk) * 1000);
		if (!isOk)
		{
			it++;
			continue;
		}
		Lyric lrc(min + sec, (*it).mid(index + 1));
		m_lyric.insert(m_count++, lrc);
		it++;
	}
	if (m_count == 0)
	{
		Lyric lrc(0.0, "当前歌曲没有歌词...");
		m_lyric.insert(m_count++, lrc);
	}
}

void LyricWidget::setTime(double _time)
{
	if (m_lyric.isEmpty())
		return;

	int time = (int)(_time * 1000);
	if ( (time >= m_lyric.value(m_currentCount).getTime()) && (time < m_lyric.value(m_currentCount + 1).getTime()))
		return;
	else
	{
		int i = 0;
		for ( ; i < m_count; i++)
		{
			if (time >= m_lyric.value(i).getTime())
				if (time <= m_lyric.value(i + 1).getTime())
					break;
		}
		m_currentCount = i;
	}

	if (m_currentCount >= m_count)
	{
		m_lyricLabel->setText(m_lyric.value(m_count).getLyrc());
		m_nextLyricLabel->setText("");
	}
	else
	{
		m_lyricLabel->setText(m_lyric.value(m_currentCount).getLyrc());
		m_nextLyricLabel->setText(m_lyric.value(m_currentCount + 1).getLyrc());
	}
}

void LyricWidget::clearText()
{
	m_lyric.clear();
	m_currentCount = 0;
	m_lyricLabel->clear();
	m_nextLyricLabel->clear();
	update();
}

qreal LyricWidget::opacity() const
{
	return m_opacity;
}

void LyricWidget::setOpacity(const qreal &opacity)
{
	m_opacity = opacity;
}







void LyricWidget::mousePressEvent(QMouseEvent *e)
{
	if (e->button() != Qt::LeftButton)
		e->ignore();
	else
	{
		m_isPressed = true;
		m_pos = e->globalPos() - pos();
	}
}

void LyricWidget::mouseReleaseEvent(QMouseEvent * /*e*/)
{
	m_isPressed = false;
}

void LyricWidget::mouseMoveEvent(QMouseEvent *e)
{
	if (m_isPressed)
		move(e->globalPos() - m_pos);
	else
		QWidget::mouseMoveEvent(e);
}

void LyricWidget::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setPen(Qt::NoPen);
	if (m_isCursorEnter)
	{
		painter.setOpacity(m_opacity);
		painter.setBrush(QColor(m_color));
		painter.drawRect(rect());
	}
	else
	{
		painter.setOpacity(0);
		painter.drawRect(rect());
	}
}

void LyricWidget::enterEvent(QEvent *)
{
	m_isCursorEnter = true;
	update();
}

void LyricWidget::leaveEvent(QEvent *)
{
	m_isCursorEnter = false;
	update();
}
QString LyricWidget::color() const
{
	return m_color;
}

void LyricWidget::setColor(const QString &color)
{
	m_color = color;
}

