#include "musiclistswidget.h"
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollBar>

MusicListsWidget::MusicListsWidget(QWidget *parent) : QWidget(parent)
{
	setFixedSize(300, 350);
	setAttribute(Qt::WA_TranslucentBackground);
	m_musicList = new QListWidget(this);
	m_musicList->setObjectName("MusicsList");
	m_musicList->horizontalScrollBar()->setVisible(false);
	m_musicList->verticalScrollBar()->setVisible(false);

	connect(m_musicList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(itemDoubleClicked(QListWidgetItem*)));

	QLabel *title = new QLabel("歌单列表");
	title->setObjectName("MusicTitle");

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(title, 0, Qt::AlignLeft | Qt::AlignVCenter);
	mainLayout->addWidget(m_musicList);
	mainLayout->setContentsMargins(5, 0, 5, 5);
}

MusicListsWidget::~MusicListsWidget()
{

}

void MusicListsWidget::addItem(const QString &item)
{
	m_musicList->addItem(item);
}

void MusicListsWidget::rmCurrentItem()
{
	int curIndex = m_musicList->currentRow();

	m_musicList->takeItem(curIndex);
}

void MusicListsWidget::rmAllItem()
{
	for (int i = m_musicList->count() - 1; i >= 0; i--)
	{
		m_musicList->takeItem(i);
	}
}


void MusicListsWidget::setCurrentItem(int index)
{
	m_musicList->setCurrentRow(index);
}


QString MusicListsWidget::currentListName()
{
	if (m_musicList->count() == 0)
		return "";
	QString name = m_musicList->currentItem()->text();
	return name;
}

void MusicListsWidget::itemDoubleClicked(QListWidgetItem *item)
{
	emit itemClicked(item->text());
}
