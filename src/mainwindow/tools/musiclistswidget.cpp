#include "musiclistswidget.h"
#include "../../configure/configure.h"
#include "../../network/networkmanage.h"
#include "../../mediaplayer/mediaplayer.h"
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollBar>
#include <QRegExp>

MusicListsWidget::MusicListsWidget(QWidget *parent) : QWidget(parent)
{
	setFixedSize(musicListWidgetSize);
	setAttribute(Qt::WA_TranslucentBackground);
	m_musicList = new QListWidget(this);
	m_musicList->setObjectName("MusicsList");
	m_musicList->horizontalScrollBar()->setVisible(false);
	m_musicList->verticalScrollBar()->setVisible(false);

	connect(m_musicList, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
			this, SLOT(itemDoubleClicked(QListWidgetItem*)));

	QLabel *title = new QLabel("歌单列表");
	title->setObjectName("MusicTitle");

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(title, 0, Qt::AlignLeft | Qt::AlignVCenter);
	mainLayout->addWidget(m_musicList);
	mainLayout->setContentsMargins(5, 0, 5, 5);

	connect(NetworkManage::sharedNetworkManage(), SIGNAL(musicListInfo(QStringList)),
			this, SLOT(musicListInfo(QStringList)));

	m_currentList = 0;
	addItem("每日推荐");
	addItem("私人 FM");
	addItem("搜索结果");
}

MusicListsWidget::~MusicListsWidget()
{

}

QString MusicListsWidget::currentList()
{
	return m_musicList->item(m_currentList)->text();
}

int MusicListsWidget::currentRow()
{
	return m_currentList;
}

void MusicListsWidget::addItem(const QString &item)
{
	m_musicList->addItem(item);
}

void MusicListsWidget::rmAllItem()
{
	for (int i = m_musicList->count() - 1; i > 2; i--)
	{
		QListWidgetItem *item = m_musicList->takeItem(i);
		delete item;
		item = Q_NULLPTR;
	}
}


void MusicListsWidget::setCurrentItem(int index)
{
	m_musicList->setCurrentRow(index);
}




void MusicListsWidget::adjustItem()
{
	QListWidgetItem *currentItem = m_musicList->currentItem();
	for (int i = m_musicList->count() - 1; i >= 0; i--)
	{
		QListWidgetItem *item = m_musicList->item(i);
		QString text = item->text();
		if (regexp(text))
			item->setText(text);
		if (currentItem == item)
		{
			item->setText(">>> " + item->text());
			m_currentList = i;
		}
	}
}

bool MusicListsWidget::regexp(QString &text)
{
	QRegExp rx(">>> (.*)");
	if (rx.indexIn(text, 0) == -1)
		return false;
	text = rx.cap(1);
	return true;
}

void MusicListsWidget::itemDoubleClicked()
{
	itemDoubleClicked(m_musicList->currentItem());
}

void MusicListsWidget::itemDoubleClicked(QListWidgetItem */*item*/)
{
	adjustItem();
	int index = m_musicList->currentRow();
	if (0 == index)
	{
		//	获取每日推荐...
		NetworkManage::sharedNetworkManage()->recommed();
	}
	else if (1 == index)
	{
		// 私人FM..
		NetworkManage::sharedNetworkManage()->randioFM();
	}
	else if (2 == index)
	{
		//	搜索结果...
		NetworkManage::sharedNetworkManage()->search("");
	}
	else
	{
		QVector<int> vector = MediaPlayer::sharedMediaPlayer()->musicListId();
		NetworkManage::sharedNetworkManage()->musicListDetails(vector[index - 3]);
	}
}

void MusicListsWidget::musicListInfo(const QStringList &list)
{
	QStringList::const_iterator it = list.begin();
	while (it != list.end())
	{
		addItem(*it);
		++it;
	}
	m_musicList->setCurrentRow(0);
	NetworkManage::sharedNetworkManage()->userHeadImage();
	adjustItem();
}











