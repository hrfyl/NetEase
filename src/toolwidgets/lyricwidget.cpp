#include "lyricwidget.h"
#include "../lyric/lyric.h"
#include "../configure/configure.h"
#include "../mediaplayer/mediaplayer.h"
#include "../network/networkmanage.h"
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>

//#include <QFile>
//#include <QDir>
//#include <QTextStream>

LyricWidget::LyricWidget(QWidget *parent) :
	BasedStyleShetWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog | Qt::WindowStaysOnBottomHint |
				   Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);

	setAttribute(Qt::WA_TranslucentBackground);
	setFixedSize(lyricWidgetSize);
	m_isPressed = false;
	setObjectName("LyricWidget");

	m_lyricLabel = new QLabel;
	//m_lyricLabel->setWordWrap(true);
	m_lyricLabel->setObjectName("CurrentLyric");
	m_nextLyricLabel = new QLabel;
	//m_nextLyricLabel->setWordWrap(true);
	m_nextLyricLabel->setObjectName("NextCurrentLyric");
	QVBoxLayout *lyricLayout = new QVBoxLayout(this);
	lyricLayout->addWidget(m_lyricLabel, 0, Qt::AlignVCenter | Qt::AlignHCenter);
	lyricLayout->addSpacing(0);
	lyricLayout->addWidget(m_nextLyricLabel, 0, Qt::AlignVCenter | Qt::AlignHCenter);
	lyricLayout->setContentsMargins(5, 10, 5, 5);

	m_opacity = 0.5;
	m_color = "gray";
	m_lockWidget = true;
	m_isCursorEnter = false;
	m_lyricLabel->setText("");
	m_nextLyricLabel->setText("");

	connect(NetworkManage::sharedNetworkManage(), SIGNAL(lyric(QString)),
			this, SLOT(lyric(QString)));
	connect(MediaPlayer::sharedMediaPlayer(), SIGNAL(updateLyric(double)),
			this, SLOT(setTime(double)));
}

void LyricWidget::lyric(const QString &lyric)
{
	//		歌词处理...
	m_currentMusicLyric.setCurrentLyric(lyric);

	// 写入文件..
//	QString path = QDir::currentPath();
//	path.append("/lyric.lrc");
//	QFile file(path);
//	file.open(QIODevice::WriteOnly | QIODevice::Text);
//	QTextStream stream(&file);
//	stream << lyric;
//	file.close();
}

void LyricWidget::setTime(const double &_time)
{
	if (m_currentMusicLyric.isEmpty())
		return;

	QString currentLyric = "", nextLyric = "";
	m_currentMusicLyric.currentLyric(_time * 1000, currentLyric, nextLyric);

	m_lyricLabel->setText(currentLyric);
	m_nextLyricLabel->setText(nextLyric);
	this->update();
}



// Ui...
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
	if (m_lockWidget)
		return;
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
	if (m_lockWidget)
		return;
	m_isCursorEnter = true;
	update();
}

void LyricWidget::leaveEvent(QEvent *)
{
	m_isCursorEnter = false;
	update();
}

bool LyricWidget::lockWidget() const
{
	return m_lockWidget;
}

void LyricWidget::setLockWidget(bool lockWidget)
{

	m_lockWidget = lockWidget;
}

QString LyricWidget::color() const
{
	return m_color;
}

void LyricWidget::setColor(const QString &color)
{
	m_color = color;
}

