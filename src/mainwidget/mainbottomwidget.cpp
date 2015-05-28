#include "mainbottomwidget.h"
#include "../basedwidget/backgroundwidget.h"
#include "../toolwidget/funcbutton.h"
#include "../toolwidget/sliderwidget.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

MainBottomWidget::MainBottomWidget(QWidget *parent) : QWidget(parent)
{
	setFixedSize(900, 120);
	m_backGroundWidget = new BackGroundWidget(this);
	m_backGroundWidget->setGeometry(rect());
	m_maxDuration = 400000;
	initUi();
	initConnect();
}

MainBottomWidget::~MainBottomWidget()
{

}


void MainBottomWidget::initUi()
{
	m_musicLogo = new QLabel(this);
	QPixmap pix(":/func_btn/music");
	m_musicLogo->setPixmap(pix);
	m_musicLogo->setFixedSize(pix.size());

	m_playButton = new FuncButton(":/func_btn/play_btn", this);
	m_playButton->setObjectName("Play");

	m_playNextButton = new FuncButton(":/func_btn/next_btn", this);
	m_playNextButton->setObjectName("PlayNext");

	m_playFrontButton = new FuncButton(":/func_btn/front_btn", this);
	m_playFrontButton->setObjectName("PlayFront");


	QWidget *buttonWidget = new QWidget;
	QHBoxLayout *button = new QHBoxLayout(buttonWidget);
	button->addWidget(m_playFrontButton, 0, Qt::AlignVCenter);
	button->addSpacing(10);
	button->addWidget(m_playButton, 0, Qt::AlignVCenter);
	button->addSpacing(10);
	button->addWidget(m_playNextButton, 0, Qt::AlignVCenter);
	button->setContentsMargins(10, 10, 5, 5);

	m_voiceWidget = new SliderWidget;
	m_voiceWidget->moveTo(0.25);
	m_voiceWidget->setObjectName("VoiceSlider");
	m_postionWidget = new SliderWidget;
	m_postionWidget->setObjectName("PostionSlider");

	QLabel *maxVoice = new QLabel("100");
	maxVoice->setFixedWidth(40);
	maxVoice->setObjectName("VoiceLabel");
	maxVoice->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

	m_currentVoice = new QLabel("25");
	m_currentVoice->setObjectName("VoiceLabel");
	m_currentVoice->setFixedWidth(40);
	m_currentVoice->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

	m_currentPostion = new QLabel("00:00");
	m_currentPostion->setObjectName("PostionLabel");
	m_currentPostion->setFixedWidth(40);
	m_currentPostion->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

	m_duration = new QLabel("00:00");
	m_duration->setObjectName("PostionLabel");
	m_duration->setFixedWidth(40);
	m_duration->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

	QWidget *voiceWidget = new QWidget;
	QHBoxLayout *voice = new QHBoxLayout(voiceWidget);
	voice->addWidget(m_currentVoice, 0, Qt::AlignLeft | Qt::AlignVCenter);
	voice->addWidget(m_voiceWidget, 0, Qt::AlignVCenter);
	voice->addWidget(maxVoice, 0, Qt::AlignRight | Qt::AlignVCenter);

	QWidget *postionWidget = new QWidget;
	QHBoxLayout *postion = new QHBoxLayout(postionWidget);
	postion->addWidget(m_currentPostion, 0, Qt::AlignLeft | Qt::AlignVCenter);
	postion->addWidget(m_postionWidget, 0, Qt::AlignVCenter);
	postion->addWidget(m_duration, 0, Qt::AlignRight | Qt::AlignVCenter);

	QWidget *slider = new QWidget;
	QVBoxLayout *sliderLayout = new QVBoxLayout(slider);
	sliderLayout->addWidget(voiceWidget, 0, Qt::AlignTop | Qt::AlignHCenter);
	sliderLayout->addSpacing(5);
	sliderLayout->addWidget(postionWidget, 0, Qt::AlignBottom | Qt::AlignHCenter);
	sliderLayout->setContentsMargins(5, 5, 5, 5);

	QHBoxLayout *mainLayout = new QHBoxLayout(this);
	mainLayout->addWidget(m_musicLogo, 0, Qt::AlignLeft | Qt::AlignVCenter);
	mainLayout->addSpacing(15);
	mainLayout->addWidget(buttonWidget, 0, Qt::AlignVCenter);
	mainLayout->addStretch();
	mainLayout->addWidget(slider, 0, Qt::AlignRight | Qt::AlignVCenter);
	mainLayout->setContentsMargins(5, 5, 5, 5);
}

void MainBottomWidget::initConnect()
{
	connect(m_playButton, SIGNAL(clicked()), this, SLOT(on_playButton_clicked()));
	connect(m_playFrontButton, SIGNAL(clicked()), this, SIGNAL(playFront()));
	connect(m_playNextButton, SIGNAL(clicked()), this, SIGNAL(playNext()));

	//		进度条
	connect(m_voiceWidget, SIGNAL(changeTo(qreal)), this, SLOT(voiceChanged(qreal)));
	connect(m_postionWidget, SIGNAL(changeTo(qreal)), this, SLOT(postionChanged(qreal)));
}

void MainBottomWidget::on_playButton_clicked()
{
	if (m_playButton->objectName() == "Play" || m_playButton->objectName() == "Stop")
	{
		emit play();
		m_playButton->setMIcon(":/func_btn/stop_btn");
		m_playButton->setObjectName("Pasue");
		m_playButton->update();
	}
	else
	{
		emit pasue();
		m_playButton->setMIcon(":/func_btn/play_btn");
		m_playButton->setObjectName("Play");
		m_playButton->update();
	}
}

void MainBottomWidget::voiceChanged(qreal pos)
{
	m_currentVoice->setText(QString::number((int)(pos * 100), 10));
	emit setVoice( (int)(pos * 100));
}

void MainBottomWidget::postionChanged(qreal pos)
{
	int tmp = (int)(pos * m_maxDuration / 1000);
	int min = tmp / 60;
	int sec = tmp % 60;
	QString str = QString("%1:%2").arg(QString::number(min, 10)).arg(QString::number(sec, 10));
	m_currentPostion->setText(str);
	emit setPostion( (qint64)(pos * m_maxDuration));
}

void MainBottomWidget::postionChangeTo(qint64 pos)
{
	int min = (int)((pos / 1000) / 60);
	int sec = (int)((pos / 1000) % 60);
	QString str = QString("%1:%2").arg(QString::number(min, 10)).arg(QString::number(sec, 10));
	m_currentPostion->setText(str);
	//		(qreal)pos 是必须的,否则进度条一直为0
	m_postionWidget->moveTo(((qreal)pos / m_maxDuration));
}

void MainBottomWidget::setMaxDuration(qint64 max)
{
	m_maxDuration = max;
	int min = (int)((m_maxDuration /1000) / 60);
	int sec = (int)((m_maxDuration /1000) % 60);
	QString str = QString("%1:%2").arg(QString::number(min, 10)).arg(QString::number(sec, 10));
	m_duration->setText(str);
}

void MainBottomWidget::setPlayButtonStatues(const QString &icon)
{
	m_playButton->setMIcon(icon);
	m_playButton->update();
}

void MainBottomWidget::setNetWorkMode(bool isTrue)
{
	if (isTrue)
	{
		disconnect(m_playButton, SIGNAL(clicked()), this, SLOT(on_playButton_clicked()));
		disconnect(m_postionWidget, SIGNAL(changeTo(qreal)), this, SLOT(postionChanged(qreal)));
		connect(m_playButton, SIGNAL(clicked()), this, SIGNAL(play()));
		connect(m_postionWidget, SIGNAL(changeTo(qreal)), this, SIGNAL(setPosTo(qreal)));
	}
}

void MainBottomWidget::setPostion(const QString &currentPos, const QString &postion)
{
	m_currentPostion->setText(currentPos);
	m_duration->setText(postion);
}

void MainBottomWidget::currentPosSlider(qreal pos)
{
	m_postionWidget->moveTo(pos);
}


void MainBottomWidget::playStateChanged(QMediaPlayer::State state)
{
	if (state == QMediaPlayer::StoppedState)
	{
		m_playButton->setObjectName("Stop");
		m_playButton->setMIcon(":/func_btn/play_btn");
		m_playButton->update();
	}
	else if (state == QMediaPlayer::PausedState)
	{
		m_playButton->setMIcon(":/func_btn/play_btn");
		m_playButton->setObjectName("Pasue");
		m_playButton->update();
	}
	else if (QMediaPlayer::PlayingState)
	{
		m_playButton->setMIcon(":/func_btn/stop_btn");
		m_playButton->setObjectName("Pasue");
		m_playButton->update();
	}
}
