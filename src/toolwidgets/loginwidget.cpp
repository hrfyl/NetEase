#include "loginwidget.h"
#include "../configure/configure.h"
#include "../toolwidgets/titlewidget.h"
#include "../network/networkmanage.h"
#include <QMouseEvent>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>

LogInWidget::LogInWidget(QWidget *parent) :
	BasedDialog(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	m_title = new TitleWidget(false, this);

	setObjectName("LogInWidget");
	setFixedSize(logInWidgetSize);

	connect(m_title, SIGNAL(closeClicked()), this, SLOT(hide()));

	initUi();

	connect(NetworkManage::sharedNetworkManage(), SIGNAL(logined(bool)),
			this, SLOT(logIned(bool)));
}


void LogInWidget::initUi()
{
	QVBoxLayout *mainLayout = new QVBoxLayout(this);

	QLabel *userNameLabel = new QLabel("user name: ", this);
	userNameLabel->setFixedWidth(logInWidgetSize.width() * 0.20);
	m_userNameLineEdit = new QLineEdit(this);
	m_userNameLineEdit->setFixedWidth(logInWidgetSize.width() * 0.70);
	QHBoxLayout *userNameLayout = new QHBoxLayout;
	userNameLayout->addWidget(userNameLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
	userNameLayout->addWidget(m_userNameLineEdit, 0, Qt::AlignLeft | Qt::AlignVCenter);

	QLabel *userPasswdLabel = new QLabel("Password: ", this);
	userPasswdLabel->setFixedWidth(logInWidgetSize.width() * 0.20);
	m_userPasswdLineEdit = new QLineEdit(this);
	m_userPasswdLineEdit->setFixedWidth(logInWidgetSize.width() * 0.70);
	m_userPasswdLineEdit->setEchoMode(QLineEdit::Password);
	QHBoxLayout *passwdLayout = new QHBoxLayout;
	passwdLayout->addWidget(userPasswdLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
	passwdLayout->addWidget(m_userPasswdLineEdit, 0, Qt::AlignLeft | Qt::AlignVCenter);

	QPushButton *logInButton = new QPushButton("LogIn", this);
	logInButton->setFixedWidth(logInWidgetSize.width() * 0.43);
	QPushButton *cancelButton = new QPushButton("Cancel", this);
	cancelButton->setFixedWidth(logInWidgetSize.width() * 0.43);
	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(logInButton, 0, Qt::AlignLeft | Qt::AlignVCenter);
	buttonLayout->addWidget(cancelButton, 0, Qt::AlignRight | Qt::AlignVCenter);

	connect(cancelButton, SIGNAL(clicked()), this, SLOT(hide()));
	connect(logInButton, SIGNAL(clicked()), this, SLOT(logIn()));

	m_loginStatusLabel = new QLabel("", this);
	m_loginStatusLabel->setVisible(false);

	mainLayout->addStretch(10);
	mainLayout->addLayout(userNameLayout, 10);
	mainLayout->addLayout(passwdLayout, 10);
	mainLayout->addLayout(buttonLayout, 10);
	mainLayout->addWidget(m_loginStatusLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
}

void LogInWidget::logIn()
{
	QString userName =m_userNameLineEdit->text();
	QString userPasswd = m_userPasswdLineEdit->text();
	if (userName == "" || userPasswd == "")
	{
		m_loginStatusLabel->setVisible(true);
		m_loginStatusLabel->setText("<font color='red'>用户名和密码不能为空！！！</font>");
	}
	else
	{
		m_loginStatusLabel->setVisible(false);
		NetworkManage::sharedNetworkManage()->logIn(userName, userPasswd);
	}
}

void LogInWidget::logIned(const bool &isLogined)
{
	if (isLogined)
	{
		loginSuccess();
		m_loginStatusLabel->setVisible(false);
		this->hide();
	}
	else
	{
		m_loginStatusLabel->setVisible(true);
		m_loginStatusLabel->setText("<font color='red'>登录失败</font>\
									<font size='18px'>请退出软件重新登录,如一直无法登录清使用Debug调试...</font>");
	}
}

