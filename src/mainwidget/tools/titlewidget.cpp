#include "titlewidget.h"
#include "../../toolwidget/imagebutton.h"
#include <QLabel>
#include <QAction>
#include <QMenu>
#include <QHBoxLayout>

TitleWidget::TitleWidget(QWidget *parent) : BasedStyleShetWidget(parent)
{
	setAttribute(Qt::WA_TranslucentBackground);
	setFixedSize(900, 40);
	setObjectName("MyTitle");

	initUi();
	initAction();
}

TitleWidget::~TitleWidget()
{

}


void TitleWidget::initUi()
{
	QLabel *logoLabel = new QLabel;
	QPixmap pix(":/title/logo");
	QSize s(28, 28);
	//		将图标缩小
	logoLabel->setPixmap(pix.scaled(s, Qt::KeepAspectRatio));

	QLabel *titleLabel = new QLabel("网易云音乐 1.0.0");
	titleLabel->setObjectName("TitleLabel");

	ImageButton *upButton = new ImageButton(":/title/update_btn");
	upButton->setCursorEnable(true);
	connect(upButton, SIGNAL(clicked()), this, SIGNAL(updateClicked()));

	QWidget *titleWidget = new QWidget(this);
	QHBoxLayout *titleLayout = new QHBoxLayout(titleWidget);
	titleLayout->addWidget(logoLabel, 0, Qt::AlignVCenter);
	titleLayout->addWidget(titleLabel, 0, Qt::AlignVCenter);
	titleLayout->addWidget(upButton, 0, Qt::AlignVCenter);
	titleLayout->setContentsMargins(0, 0, 0, 0);

	ImageButton *closeButton = new ImageButton(":/title/close_btn");
	closeButton->setCursorEnable(true);
	connect(closeButton, SIGNAL(clicked()), this, SIGNAL(closeClicked()));

	ImageButton *miniButton = new ImageButton(":/title/mini_btn");
	miniButton->setCursorEnable(true);
	connect(miniButton, SIGNAL(clicked()), this, SIGNAL(showMini()));

	ImageButton *menuButton = new ImageButton(":/title/menu_btn");
	menuButton->setCursorEnable(true);
	//connect(menuButton, SIGNAL(clicked()), this, SIGNAL(showMenu()));
	connect(menuButton, SIGNAL(clicked()), this, SLOT(menuVisiable()));

	ImageButton *skinButton = new ImageButton(":/title/skin_btn");
	skinButton->setCursorEnable(true);
	connect(skinButton, SIGNAL(clicked()), this, SIGNAL(showSkin()));

	QWidget *buttonWidget = new QWidget;
	QHBoxLayout *buttnLayout = new QHBoxLayout(buttonWidget);
	buttnLayout->addWidget(skinButton, 0, Qt::AlignVCenter);
	buttnLayout->addWidget(miniButton, 0, Qt::AlignVCenter);
	buttnLayout->addWidget(menuButton, 0, Qt::AlignVCenter);
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

	addFileAction = new QAction("添加歌曲", m_menu);
	addFileAction->setShortcut(tr("Ctrl+A"));
	connect(addFileAction, SIGNAL(triggered()), this, SIGNAL(addFile()));

	addListAction = new QAction("添加歌单", m_menu);
	addListAction->setShortcut(tr("Ctrl+L"));
	connect(addListAction, SIGNAL(triggered()), this, SIGNAL(addList()));

	rmFileAction = new QAction("移出选中歌曲", m_menu);
	rmFileAction->setShortcut(tr("Ctrl+F"));
	connect(rmFileAction, SIGNAL(triggered()), this, SIGNAL(rmFiles()));

	rmListAction = new QAction("移出选中歌单", m_menu);
	rmListAction->setShortcut(tr("Ctrl+L"));
	connect(rmListAction, SIGNAL(triggered()), this, SIGNAL(rmLists()));

	loopOnePlayAction = new QAction("单曲循环", m_menu);
	loopOnePlayAction->setShortcut(tr("Ctrl+D"));
	loopOnePlayAction->setCheckable(true);
	loopOnePlayAction->setChecked(false);
	connect(loopOnePlayAction, SIGNAL(triggered()), this, SLOT(loopOnePlayClicked()));

	loopPlayAction = new QAction("全部循环", m_menu);
	loopPlayAction->setShortcut(tr("Ctrl+H"));
	loopPlayAction->setCheckable(true);
	loopPlayAction->setChecked(true);
	connect(loopPlayAction, SIGNAL(triggered()), this, SLOT(loopPlayClicked()));

	randomPlayAction = new QAction("随机循环", m_menu);
	randomPlayAction->setShortcut(tr("Ctrl+R"));
	randomPlayAction->setCheckable(true);
	randomPlayAction->setChecked(false);
	connect(randomPlayAction, SIGNAL(triggered()), this, SLOT(randomPlayClicked()));

	listPlayAction = new QAction("列表循环", m_menu);
	listPlayAction->setShortcut(tr("Ctrl+B"));
	listPlayAction->setCheckable(true);
	listPlayAction->setChecked(false);
	connect(listPlayAction, SIGNAL(triggered()), this, SLOT(listPlayClicked()));

	searchAction = new QAction("搜索", m_menu);
	searchAction->setShortcut(tr("Ctrl+S"));
	searchAction->setStatusTip("搜索单曲,歌单...");
	connect(searchAction, SIGNAL(triggered()), this, SIGNAL(search()));

	logOutAction = new QAction("注销", m_menu);
	logOutAction->setShortcut(tr("Ctrl+E"));
	logOutAction->setStatusTip("退出登录...");
	connect(logOutAction, SIGNAL(triggered()), this, SIGNAL(logOut()));

	switchWidgetAction = new QAction("切换到云资源", m_menu);
	switchWidgetAction->setShortcut(tr("Ctrl+W"));
	connect(switchWidgetAction, SIGNAL(triggered()), this, SIGNAL(switchWidget()));

	QAction *exitAction = new QAction("退出软件", m_menu);
	exitAction->setShortcut(tr("Ctrl+Q"));
	connect(exitAction, SIGNAL(triggered()), this, SIGNAL(exitWidget()));

	m_menu->addAction(addFileAction);
	m_menu->addAction(addListAction);
	m_menu->addSeparator();
	m_menu->addAction(rmFileAction);
	m_menu->addAction(rmListAction);
	m_menu->addSeparator();
	m_menu->addAction(loopOnePlayAction);
	m_menu->addAction(loopPlayAction);
	m_menu->addAction(randomPlayAction);
	m_menu->addAction(listPlayAction);
	m_menu->addSeparator();
	m_menu->addAction(searchAction);
	m_menu->addAction(logOutAction);
	m_menu->addSeparator();
	m_menu->addAction(switchWidgetAction);
	m_menu->addSeparator();
	m_menu->addAction(exitAction);
	m_menu->adjustSize();

	m_menu->setVisible(false);
}


void TitleWidget::netWorkModel(bool isTrue)
{
	if (isTrue)
	{
		addFileAction->setVisible(false);
		addListAction->setVisible(false);
		rmFileAction->setVisible(false);
		rmListAction->setVisible(false);
		loopPlayAction->setVisible(false);
		loopOnePlayAction->setVisible(false);
		listPlayAction->setVisible(false);
		randomPlayAction->setVisible(false);
		logOutAction->setVisible(true);
		searchAction->setVisible(true);
		switchWidgetAction->setText("切换到本地资源");
	}
	else
	{
		addFileAction->setVisible(true);
		addListAction->setVisible(true);
		rmFileAction->setVisible(true);
		rmListAction->setVisible(true);
		loopPlayAction->setVisible(true);
		loopOnePlayAction->setVisible(true);
		listPlayAction->setVisible(true);
		randomPlayAction->setVisible(true);
		logOutAction->setVisible(false);
		searchAction->setVisible(false);
		switchWidgetAction->setText("切换到云资源");
	}
}

void TitleWidget::menuVisiable()
{
	QPoint pos = QCursor::pos();
	m_menu->setVisible(true);
	m_menu->move(pos.x() - 165, pos.y() + 15);
}

void TitleWidget::loopPlayClicked()
{
	loopOnePlayAction->setChecked(false);
	listPlayAction->setChecked(false);
	randomPlayAction->setChecked(false);
	loopPlayAction->setChecked(true);

	emit loopPlay();
}

void TitleWidget::loopOnePlayClicked()
{
	loopOnePlayAction->setChecked(true);
	listPlayAction->setChecked(false);
	randomPlayAction->setChecked(false);
	loopPlayAction->setChecked(false);

	emit loopOnePlay();
}

void TitleWidget::listPlayClicked()
{
	loopOnePlayAction->setChecked(false);
	listPlayAction->setChecked(true);
	randomPlayAction->setChecked(false);
	loopPlayAction->setChecked(false);

	emit listPlay();
}

void TitleWidget::randomPlayClicked()
{
	loopOnePlayAction->setChecked(false);
	listPlayAction->setChecked(false);
	randomPlayAction->setChecked(true);
	loopPlayAction->setChecked(false);

	emit randomPlay();
}

















