#include "maintopwidget.h"
#include "../configure/configure.h"
#include "../mediaplayer/mediaplayer.h"
#include "../toolwidgets/loginwidget.h"
#include "../basedwidget/backgroundwidget.h"
#include "../network/networkmanage.h"
#include "tools/musiclistswidget.h"
#include "tools/userwidget.h"
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QScrollBar>
#include <QAbstractItemDelegate>
#include <QList>
#include <QMenu>
#include <QScrollBar>

MainTopWidget::MainTopWidget(QWidget *parent) : QWidget(parent)
{
	setFixedSize(mainTopWidgetSize);
	m_backGroundWidget = new BackGroundWidget(this);
	m_backGroundWidget->setGeometry(rect());
	setObjectName("MainTopWidget");

	initUi();
	initConnect();
	setUpMouseMenu();		//	初始化右键菜单..
}

MainTopWidget::~MainTopWidget()
{

}

void MainTopWidget::initUi()
{
	m_logWidget = new LogInWidget(this);
	m_logWidget->setVisible(false);

	m_userWidget = new UserWidget(":/main_images/image_login");
	m_musicListsWidget = new MusicListsWidget;

	QWidget *leftWidget = new QWidget;
	leftWidget->setFixedHeight(mainTopWidgetSize.height());

	QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);
	leftLayout->addSpacing(10);
	leftLayout->addWidget(m_userWidget, 0, Qt::AlignLeft);
	leftLayout->addSpacing(10);
	leftLayout->addWidget(m_musicListsWidget, 0, Qt::AlignHCenter);
	leftLayout->addStretch();
	leftLayout->setContentsMargins(5, 0, 5, 0);

	//		当前播放列表初始化
	m_currentList = new QTableWidget;
	m_currentList->setContextMenuPolicy(Qt::CustomContextMenu);				//	右键菜单..

	m_currentList->setFixedSize(currentMusicListWidgetSize);
	m_currentList->setColumnCount(4);
	m_currentList->setSelectionBehavior(QAbstractItemView::SelectRows);		//	设置选中一行
	m_currentList->setEditTriggers(QAbstractItemView::NoEditTriggers);		//	设置不可修改
	m_currentList->setShowGrid(false);						//	格子线不显示
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

	//		滚动条样式...
	m_currentList->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}\
		QScrollBar::handle{background:gray; border:2px solid transparent; border-radius:5px;}\
		QScrollBar::handle:hover{background:lightgray;}\
		QScrollBar::sub-line{background:transparent;}\
		QScrollBar::add-line{background:transparent;}");

	m_currentList->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width:10px;}\
		QScrollBar::handle{background:gray; border:2px solid transparent; border-radius:5px;}\
		QScrollBar::handle:hover{background:lightgray;}\
		QScrollBar::sub-line{background:transparent;}\
		QScrollBar::add-line{background:transparent;}");

	//		滚动条隐藏
	//	在kde5 breeze主题中存在冲突,所以隐藏
	m_currentList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_currentList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//		这种方法右下角出现方块....
	//m_currentList->horizontalScrollBar()->setVisiable(false);
	//m_currentList->verticalScrollBar()->setVisiable(false);

	QHBoxLayout *mainLayout = new QHBoxLayout(this);
	mainLayout->addWidget(leftWidget, 0, Qt::AlignLeft);
	mainLayout->addWidget(m_currentList, 0, Qt::AlignRight);
	mainLayout->setContentsMargins(0, 0, 10, 0);
}

void MainTopWidget::initConnect()
{
	connect(m_logWidget, SIGNAL(loginSuccess()), this, SIGNAL(logInSuccess()));

	connect(m_currentList, SIGNAL(cellDoubleClicked(int,int)),
			MediaPlayer::sharedMediaPlayer(), SLOT(play(int)));
	m_mouseMenu = new QMenu(m_currentList);
	connect(m_currentList, SIGNAL(customContextMenuRequested(QPoint)),
			this, SLOT(customMenuRequested(QPoint)));

	connect(m_userWidget, SIGNAL(logIn()), m_logWidget, SLOT(show()));

	connect(NetworkManage::sharedNetworkManage(), SIGNAL(listDetailsSave(QList<QStringList>)),
			this, SLOT(listDetailsSave(QList<QStringList>)));

	connect(MediaPlayer::sharedMediaPlayer(), SIGNAL(playStatusChanged(int)),
			this, SLOT(setCurrentMusic(int)));

	connect(NetworkManage::sharedNetworkManage(), SIGNAL(searchDetailsSave(QList<QStringList>)),
			this, SLOT(searchDetailsSave(QList<QStringList>)));
}

void MainTopWidget::setTablewidgetOrder()
{
	int row = m_currentList->rowCount() - 1;
	//int current_index = m_currentList->currentRow();
	int current_index = MediaPlayer::sharedMediaPlayer()->currentMusicIndex();
	int i = row;
	while (row >= 0)
	{
		if (i == current_index)
		{
			m_currentList->item(row, 0)->setText(">>>");
		}
		else
		{
			m_currentList->item(row, 0)->setText(QString::number(row + 1, 10));
		}
		row--;
		i--;
	}
}

void MainTopWidget::addMusic(const QStringList &list)
{
	int row = m_currentList->rowCount();
	m_currentList->insertRow(row);
	QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(row + 1, 10));
	QTableWidgetItem *item2 = new QTableWidgetItem(list.at(0));
	QTableWidgetItem *item3 = new QTableWidgetItem(list.at(1));
	QTableWidgetItem *item4 = new QTableWidgetItem(list.at(2));

	item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	m_currentList->setItem(row, 0, item1);
	m_currentList->setItem(row, 1, item2);
	m_currentList->setItem(row, 2, item3);
	m_currentList->setItem(row, 3, item4);

	m_currentList->setCurrentCell(row, 0);
}

void MainTopWidget::rmAllMusic()
{
	int row = m_currentList->rowCount() - 1;
	while (row >= 0)
	{
		for (int i = 0; i < 4; ++i)
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

void MainTopWidget::rmMusicRow(const int &row)
{
	for (int i = 0; i < 4; ++i)
	{
		QTableWidgetItem *item = m_currentList->takeItem(row, i);
		delete item;
		item = Q_NULLPTR;
	}
	m_currentList->removeRow(row);
}

void MainTopWidget::listDetailsSave(const QList<QStringList> &list)
{
	rmAllMusic();
	QList<QStringList>::const_iterator it = list.begin();
	while (it != list.end())
	{
		addMusic(*it);
		++it;
	}
	setCurrentMusic(0);
}

void MainTopWidget::searchDetailsSave(const QList<QStringList> &list)
{
	//m_musicListsWidget->addItem("搜索结果");
	listDetailsSave(list);
	m_musicListsWidget->setCurrentItem(2);
	m_musicListsWidget->adjustItem();
}

void MainTopWidget::setCurrentMusic(int index)
{
	m_currentList->setCurrentCell(index, 0);
	setTablewidgetOrder();

}

void MainTopWidget::customMenuRequested(const QPoint &/*pos*/)
{
	QString currentList = m_musicListsWidget->currentList();
	if (currentList == (">>> " + Configure::userNickName + "喜欢的音乐"))
	{
		m_likeAction->setVisible(false);
		m_unlikeAction->setVisible(true);
	}
	else
	{
		m_likeAction->setVisible(true);
		m_unlikeAction->setVisible(true);
	}
	m_mouseMenu->exec(QCursor::pos());
}


void MainTopWidget::setUserInfo(const QString &name, QString head)
{
	m_userWidget->setUserInfo(head, name);
}

void MainTopWidget::setUserHead(const QByteArray &_data)
{
	m_userWidget->setHeadInfo(_data);
}

void MainTopWidget::setUpMouseMenu()
{
	m_likeAction = new QAction("--收藏这首歌--", this);
	m_likeAction->setIcon(QIcon(":/menu_icon/like"));
	m_mouseMenu->addAction(m_likeAction);
	connect(m_likeAction, SIGNAL(triggered()), this, SLOT(likeMenuClicked()));

	m_unlikeAction = new QAction("--移除这首歌--", this);
	m_unlikeAction->setIcon(QIcon(":/menu_icon/unlike"));
	m_mouseMenu->addAction(m_unlikeAction);
	connect(m_unlikeAction, SIGNAL(triggered()), this, SLOT(unlikeMenuClicked()));

	m_mouseMenu->addSeparator();

	m_refreshAction = new QAction("--刷新此歌单--", this);
	m_refreshAction->setIcon(QIcon(":/menu_icon/refresh"));
	m_mouseMenu->addAction(m_refreshAction);
	connect(m_refreshAction, SIGNAL(triggered()), this, SLOT(refreshMenuClicked()));
}
void MainTopWidget::likeMenuClicked()
{
	QVector<int> songIds = MediaPlayer::sharedMediaPlayer()->songIds();
	int songId = m_currentList->currentRow();
	NetworkManage::sharedNetworkManage()->likeMusic(QString::number(songIds.at(songId)),
													true, "25", "itembased");
}
void MainTopWidget::unlikeMenuClicked()
{
	int  currentList = m_musicListsWidget->currentRow();
	QVector<int> songIds = MediaPlayer::sharedMediaPlayer()->songIds();
	int currRow = m_currentList->currentRow();
	int currentSongId = songIds.at(currRow);

	switch (currentList) {
	case 0:
		NetworkManage::sharedNetworkManage()->rmMusicFromrecommed(currRow);
		break;
	case 1:
		NetworkManage::sharedNetworkManage()->trashFM(currentSongId);	//	添加到私人FM黑名单..
		break;
	case 2:
		NetworkManage::sharedNetworkManage()->rmMusicFromSearch(currRow);
		break;
	default:
		if (m_musicListsWidget->currentList() == ">>> " + Configure::userNickName + "喜欢的音乐")
			NetworkManage::sharedNetworkManage()->likeMusic(QString::number(currentSongId, 10),
															false, "25", "itembased");
		break;
	}
	rmMusicRow(currRow);
	MediaPlayer::sharedMediaPlayer()->rmMusic(currRow);
}

void MainTopWidget::refreshMenuClicked()
{
	int currentList = m_musicListsWidget->currentRow();
	switch (currentList) {
	case 0:
		NetworkManage::sharedNetworkManage()->refreshRecommed();
		break;
	case 1:
		NetworkManage::sharedNetworkManage()->randioFM(true);
		break;
	case 2:
		NetworkManage::sharedNetworkManage()->search("");
		break;
	default:
		NetworkManage::sharedNetworkManage()->musicListDetails(MediaPlayer::sharedMediaPlayer()->musicListId().at(currentList - 3));
		break;
	}
}







