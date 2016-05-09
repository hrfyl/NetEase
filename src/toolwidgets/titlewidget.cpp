#include "titlewidget.h"
#include "imagebutton.h"
#include "../configure/configure.h"
#include <QLabel>
#include <QAction>
#include <QMenu>
#include <QHBoxLayout>

TitleWidget::TitleWidget(const bool &isMainTitle, QWidget *parent) :
	BasedStyleShetWidget(parent)
{
	setAttribute(Qt::WA_TranslucentBackground);
	setObjectName("NetNaseTitle");

	initUi(isMainTitle);
	if (isMainTitle)
	{
		setFixedSize(mainTitleSize);
		initAction();
	}
	else
		setFixedSize(logInTitleSize);
}

TitleWidget::~TitleWidget()
{

}


void TitleWidget::initUi(const bool &isMainTitle)
{
	QLabel *logoLabel = new QLabel;
	QPixmap pix(":/title_images/logo");
	QSize s(25, 25);
	//		将图标缩小
	logoLabel->setPixmap(pix.scaled(s, Qt::KeepAspectRatio));

	QLabel *titleLabel = new QLabel(APP_NAME + " " + APP_VERSION);
	titleLabel->setObjectName("TitleLabel");

	ImageButton *upButton = new ImageButton(":/title_images/update_btn");
	upButton->setCursorEnable(true);
	connect(upButton, SIGNAL(clicked()), this, SIGNAL(updateClicked()));

	QWidget *titleWidget = new QWidget(this);
	QHBoxLayout *titleLayout = new QHBoxLayout(titleWidget);
	titleLayout->addWidget(logoLabel, 0, Qt::AlignVCenter);
	titleLayout->addWidget(titleLabel, 0, Qt::AlignVCenter);
	titleLayout->addWidget(upButton, 0, Qt::AlignVCenter);
	titleLayout->setContentsMargins(0, 0, 0, 0);

	ImageButton *closeButton = new ImageButton(":/title_images/close_btn");
	closeButton->setCursorEnable(true);
	connect(closeButton, SIGNAL(clicked()), this, SIGNAL(closeClicked()));

	QWidget *buttonWidget = new QWidget;
	QHBoxLayout *buttnLayout = new QHBoxLayout(buttonWidget);

	if (isMainTitle)
	{
		ImageButton *miniButton = new ImageButton(":/title_images/mini_btn");
		miniButton->setCursorEnable(true);
		connect(miniButton, SIGNAL(clicked()), this, SIGNAL(showMini()));

		ImageButton *menuButton = new ImageButton(":/title_images/menu_btn");
		menuButton->setCursorEnable(true);
		//connect(menuButton, SIGNAL(clicked()), this, SIGNAL(showMenu()));
		connect(menuButton, SIGNAL(clicked()), this, SLOT(menuVisiable()));

		ImageButton *skinButton = new ImageButton(":/title_images/skin_btn");
		skinButton->setCursorEnable(true);
		connect(skinButton, SIGNAL(clicked()), this, SIGNAL(showSkin()));

		buttnLayout->addWidget(skinButton, 0, Qt::AlignVCenter);
		buttnLayout->addWidget(miniButton, 0, Qt::AlignVCenter);
		buttnLayout->addWidget(menuButton, 0, Qt::AlignVCenter);

	}
	buttnLayout->addWidget(closeButton, 0, Qt::AlignVCenter);
	buttnLayout->setContentsMargins(0, 0, 0, 0);

	QHBoxLayout *mainLayout = new QHBoxLayout(this);
	mainLayout->addWidget(titleWidget, 0, Qt::AlignLeft | Qt::AlignTop);
	mainLayout->addStretch();
	mainLayout->addWidget(buttonWidget, 0, Qt::AlignRight | Qt::AlignVCenter);
	mainLayout->setContentsMargins(5, 5, 5, 5);
}

void TitleWidget::initAction()
{
	m_menu = new QMenu(this);
	m_menu->setObjectName("MainMenu");

	QAction *searchAction = new QAction("搜索", m_menu);
	searchAction->setIcon(QIcon(":/menu_icon/search"));
	searchAction->setShortcut(tr("Ctrl+S"));
	searchAction->setStatusTip("搜索单曲,歌单...");
	connect(searchAction, SIGNAL(triggered()), this, SIGNAL(search()));

	QAction *logOutAction = new QAction("注销", m_menu);
	logOutAction->setIcon(QIcon(":/menu_icon/logout"));
	logOutAction->setShortcut(tr("Ctrl+E"));
	logOutAction->setStatusTip("退出登录...");
	connect(logOutAction, SIGNAL(triggered()), this, SIGNAL(logOut()));

	allLoopAction = new QAction("列表循环", m_menu);
	//allLoopAction->setIcon(QIcon(":/menu_icon/list_loop"));
	allLoopAction->setShortcut(tr("Ctrl+A"));
	allLoopAction->setCheckable(true);
	allLoopAction->setChecked(true);
	allLoopAction->setStatusTip("列表循环播放歌曲...");
	connect(allLoopAction, SIGNAL(triggered()), this, SLOT(setAllLoopChecked()));

	oneLoopAction = new QAction("单曲循环", m_menu);
	//oneLoopAction->setIcon(QIcon(":/menu_icon/one_loop"));
	oneLoopAction->setShortcut(tr("Ctrl+O"));
	oneLoopAction->setCheckable(true);
	oneLoopAction->setChecked(false);
	oneLoopAction->setStatusTip("单曲循环播放歌曲...");
	connect(oneLoopAction, SIGNAL(triggered()), this, SLOT(setOneLoopChecked()));

	randomLoopAction = new QAction("随机循环", m_menu);
	//randomLoopAction->setIcon(QIcon(":/menu_icon/random_loop"));
	randomLoopAction->setCheckable(true);
	randomLoopAction->setChecked(false);
	randomLoopAction->setShortcut(tr("Ctrl+R"));
	randomLoopAction->setStatusTip("随机循环播放歌曲...");
	connect(randomLoopAction, SIGNAL(triggered()), this, SLOT(setRandomLoopChecked()));

	QAction *exitAction = new QAction("退出软件", m_menu);
	exitAction->setIcon(QIcon(":/menu_icon/exit"));
	exitAction->setShortcut(tr("Ctrl+Q"));
	connect(exitAction, SIGNAL(triggered()), this, SIGNAL(exitWidget()));


	m_menu->addAction(searchAction);
	m_menu->addSeparator();
	m_menu->addAction(oneLoopAction);
	m_menu->addAction(allLoopAction);
	m_menu->addAction(randomLoopAction);
	m_menu->addSeparator();
	m_menu->addAction(logOutAction);
	m_menu->addSeparator();
	m_menu->addAction(exitAction);
	m_menu->adjustSize();

	m_menu->setVisible(false);
}


void TitleWidget::menuVisiable()
{
	QPoint pos = QCursor::pos();
	m_menu->setVisible(true);
	m_menu->move(pos.x() - 165, pos.y() + 15);
}

void TitleWidget::setAllLoopChecked()
{
	if (allLoopAction->isChecked())
	{
		emit loopTyepChanged(ALLLOOP);
	}
	oneLoopAction->setChecked(false);
	randomLoopAction->setChecked(false);
}

void TitleWidget::setRandomLoopChecked()
{
	if (randomLoopAction->isChecked())
	{
		emit loopTyepChanged(RANDOMLOOP);
	}
	oneLoopAction->setChecked(false);
	allLoopAction->setChecked(false);
}

void TitleWidget::setOneLoopChecked()
{
	if (oneLoopAction->isChecked())
	{
		emit loopTyepChanged(ONELOOP);
	}
	allLoopAction->setChecked(false);
	randomLoopAction->setChecked(false);
}
















