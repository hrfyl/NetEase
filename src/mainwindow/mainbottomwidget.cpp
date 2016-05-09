#include "mainbottomwidget.h"
#include "../mediaplayer/mediaplayer.h"
#include "../configure/configure.h"
#include "../basedwidget/backgroundwidget.h"
#include "../toolwidgets/funcbutton.h"
#include "../toolwidgets/sliderwidget.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

MainBottomWidget::MainBottomWidget(QWidget *parent) : QWidget(parent)
{
	setFixedSize(mainBootomSize);
	m_backGroundWidget = new BackGroundWidget(this);
	m_backGroundWidget->setGeometry(rect());
	initUi();
	initConnect();
}

MainBottomWidget::~MainBottomWidget()
{

}


void MainBottomWidget::initUi()
{
	m_musicLogo = new QLabel(this);
	QPixmap pix(":/buttons/music");
	m_musicLogo->setPixmap(pix);
	m_musicLogo->setFixedSize(pix.size());

	m_playButton = new FuncButton(":/buttons/play_btn", this);
	m_playButton->setObjectName("Play");

	m_playNextButton = new FuncButton(":/buttons/next_btn", this);
	m_playNextButton->setObjectName("PlayNext");

	m_playFrontButton = new FuncButton(":/buttons/front_btn", this);
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
	m_voiceWidget->setValue(0.25);
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
	//		进度条
	connect(m_voiceWidget, SIGNAL(value(qreal)), this, SLOT(setVoice(qreal)));
	connect(m_postionWidget, SIGNAL(value(qreal)),
			MediaPlayer::sharedMediaPlayer(), SLOT(setPostion(qreal)));

	connect(m_playButton, SIGNAL(clicked()),
			MediaPlayer::sharedMediaPlayer(), SLOT(play()));
	connect(m_playFrontButton, SIGNAL(clicked()),
			MediaPlayer::sharedMediaPlayer(), SLOT(front()));
	connect(m_playNextButton, SIGNAL(clicked()),
			MediaPlayer::sharedMediaPlayer(), SLOT(next()));
	connect(m_playButton, SIGNAL(clicked()),
			this, SLOT(setPlayButtonStatus()));

	connect(MediaPlayer::sharedMediaPlayer(), SIGNAL(postionChanged(QString,QString,qreal)),
			this, SLOT(postionChanged(QString,QString,qreal)));
}



void MainBottomWidget::setVoice(qreal pos)
{
	m_currentVoice->setText(QString::number((int)(pos * 100), 10));
	MediaPlayer::sharedMediaPlayer()->setVoice((int)(pos * 100));
}


void MainBottomWidget::setPlayButtonStatus()
{
	if (MediaPlayer::sharedMediaPlayer()->isPlaying())
	{
		m_playButton->setMIcon(":/buttons/stop_btn");
		m_playButton->update();
	}
	else
	{
		m_playButton->setMIcon(":/buttons/play_btn");
		m_playButton->update();
	}
}

void MainBottomWidget::postionChanged(const QString &current,
									  const QString &max, const qreal &value)
{
	m_currentPostion->setText(current);
	m_duration->setText(max);
	m_postionWidget->setValue(value);
}

