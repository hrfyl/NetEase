#include "userwidget.h"
#include "../../toolwidget/buttonlabel.h"
#include <QLabel>
#include <QHBoxLayout>

UserWidget::UserWidget(const QString &head, QWidget *parent) : QWidget(parent)
{
	setAttribute(Qt::WA_TranslucentBackground);

	setFixedSize(300, 50);

	m_logInButton = new ButtonLabel;
	m_logInButton->setLabelText("登录");
	m_logInButton->setObjectName("LogInButton");
	connect(m_logInButton, SIGNAL(clicked()), this, SIGNAL(logIn()));

	m_userHeadLabel = new QLabel;
	QPixmap pix(head);
	m_userHeadLabel->setFixedSize(pix.size());
	m_userHeadLabel->setPixmap(pix);

	QHBoxLayout *mainLayout = new QHBoxLayout(this);
	mainLayout->addWidget(m_userHeadLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
	mainLayout->addSpacing(10);
	mainLayout->addWidget(m_logInButton, 0, Qt::AlignVCenter);
	mainLayout->addStretch();
	mainLayout->setContentsMargins(5, 0, 5, 0);
}

UserWidget::~UserWidget()
{

}


void UserWidget::setHeadInfo(const QString &head)
{
	QPixmap pix(head);
	m_userHeadLabel->setPixmap(pix);
}

void UserWidget::setUserInfo(const QString &head, const QString &name)
{
	if (name == "登录")
		connect(m_logInButton, SIGNAL(clicked()), this, SIGNAL(logIn()));
	else
		disconnect(m_logInButton, SIGNAL(clicked()), this, SIGNAL(logIn()));
	m_userHeadLabel->setPixmap(QPixmap(head));
	m_logInButton->setLabelText(name);
}
