#include "maintopwidget.h"
#include "../basedwidget/backgroundwidget.h"
#include "tools/musiclistswidget.h"
#include "tools/userwidget.h"
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QScrollBar>
#include <QAbstractItemDelegate>

MainTopWidget::MainTopWidget(QWidget *parent) : QWidget(parent)
{
	setFixedSize(900, 440);
	m_backGroundWidget = new BackGroundWidget(this);
	m_backGroundWidget->setGeometry(rect());

	initUi();
	initConnect();
}

MainTopWidget::~MainTopWidget()
{

}

void MainTopWidget::initUi()
{
	m_userWidget = new UserWidget(":/default/user_head");
	m_musicListsWidget = new MusicListsWidget;

	QWidget *leftWidget = new QWidget;
	leftWidget->setFixedHeight(440);

	QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);
	leftLayout->addSpacing(10);
	leftLayout->addWidget(m_userWidget, 0, Qt::AlignLeft);
	leftLayout->addSpacing(10);
	leftLayout->addWidget(m_musicListsWidget, 0, Qt::AlignHCenter);
	leftLayout->addStretch();
	leftLayout->setContentsMargins(5, 0, 5, 0);

	//		当前播放列表初始化
	m_currentList = new QTableWidget;

	connect(m_currentList, SIGNAL(cellDoubleClicked(int,int)), this, SIGNAL(play(int)));

	m_currentList->setFixedSize(570, 420);
	m_currentList->setColumnCount(4);
	m_currentList->setSelectionBehavior(QAbstractItemView::SelectRows);		//	设置选中一行
	m_currentList->setEditTriggers(QAbstractItemView::NoEditTriggers);		//	设置不可修改
	m_currentList->setShowGrid(false);		//	格子线不显示
	m_currentList->verticalHeader()->setVisible(false);		//	垂直表头不可见

	m_currentList->setFocusPolicy(Qt::NoFocus);

	m_currentList->horizontalHeader()->setHighlightSections(false);
	m_currentList->horizontalHeader()->resizeSection(0, 50);
	m_currentList->horizontalHeader()->resizeSection(1, 200);
	m_currentList->horizontalHeader()->resizeSection(2, 180);
	m_currentList->horizontalHeader()->resizeSection(3, 150);

	QStringList str;
	str << "序号" << "歌曲名" << "歌手" << "专辑名称";
	m_currentList->setHorizontalHeaderLabels(str);

	//m_currentList->horizontalHeader()->setFixedWidth(590);

	m_currentList->horizontalHeader()->setStretchLastSection(true);	//	占满表头

	//		滚动条隐藏
	//		这种方法右下角出现方块....
	//m_currentList->horizontalScrollBar()->setVisiable(false);
	//m_currentList->verticalScrollBar()->setVisiable(false);

	m_currentList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_currentList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	QHBoxLayout *mainLayout = new QHBoxLayout(this);
	mainLayout->addWidget(leftWidget, 0, Qt::AlignLeft);
	mainLayout->addWidget(m_currentList, 0, Qt::AlignRight);
	mainLayout->setContentsMargins(0, 0, 10, 0);
}

void MainTopWidget::initConnect()
{
	connect(m_musicListsWidget, SIGNAL(itemClicked(QString)), this, SIGNAL(musicListItemClicked(QString)));

	connect(m_userWidget, SIGNAL(logIn()), this, SIGNAL(logIn()));
}

void MainTopWidget::addListItem(const QString &item)
{
	m_musicListsWidget->addItem(item);
}

void MainTopWidget::addMusicItem(const QString &name, const QString &artist, const QString &special)
{
	int row = m_currentList->rowCount();
	m_currentList->insertRow(row);
	QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(row + 1, 10));
	QTableWidgetItem *item2 = new QTableWidgetItem(name);
	QTableWidgetItem *item3 = new QTableWidgetItem(artist);
	QTableWidgetItem *item4 = new QTableWidgetItem(special);

	item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	m_currentList->setItem(row, 0, item1);
	m_currentList->setItem(row, 1, item2);
	m_currentList->setItem(row, 2, item3);
	m_currentList->setItem(row, 3, item4);

	m_currentList->setCurrentCell(row, 0);
}

void MainTopWidget::rmCurrentMusicItem()
{
	int row = m_currentList->currentRow();

	for (int i = 0; i < 4; i++)
	{
		QTableWidgetItem *item = m_currentList->takeItem(row, i);
		delete item;
		item = Q_NULLPTR;
	}
	//		虽然removeRow(int row) 中的Item会被Qt自动删除,
	//		但总觉得自己删除会好一些
	m_currentList->removeRow(row);
	m_currentList->setCurrentCell(m_currentList->rowCount() - 1, 0);
}

void MainTopWidget::rmAll()
{
	int row = m_currentList->rowCount() - 1;
	while (row >= 0)
	{
		for (int i = 0; i < 4; i++)
		{
			QTableWidgetItem *item = m_currentList->takeItem(row, i);
			delete item;
			item = Q_NULLPTR;
		}
		//		虽然removeRow(int row) 中的Item会被Qt自动删除,
		//		但总觉得自己删除会好一些
		m_currentList->removeRow(row);
		m_currentList->setCurrentCell(m_currentList->rowCount() - 1, 0);
		row--;
	}
}

void MainTopWidget::rmAllList()
{
	m_musicListsWidget->rmAllItem();
}

void MainTopWidget::currentMusic(int index)
{
	m_currentList->setCurrentCell(index, 0);
}

void MainTopWidget::setcurrentList(int index)
{
	m_musicListsWidget->setCurrentItem(index);
}

void MainTopWidget::setCurrentFile(int index)
{
	m_currentList->setCurrentCell(index, 0);
}

QString MainTopWidget::getCurrentListName()
{
	return m_musicListsWidget->currentListName();
}

QString MainTopWidget::currentMusicName()
{
	int row = m_currentList->currentRow();
	QString name = m_currentList->item(row, 1)->text();
	return name;
}

void MainTopWidget::setUserInfo(const QString &name, QString head)
{
	m_userWidget->setUserInfo(head, name);
}

void MainTopWidget::setMusicItem(const int &index, const QString &name, const QString &artist, const QString &special)
{
	m_currentList->item(index, 1)->setText(name);
	m_currentList->item(index, 2)->setText(artist);
	m_currentList->item(index, 3)->setText(special);
}

void MainTopWidget::rmCurrentListItem()
{
	m_musicListsWidget->rmCurrentItem();
}
