#include "userwidget.h"
#include "../../configure/configure.h"
#include "../../toolwidgets/buttonlabel.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QPixmap>

UserWidget::UserWidget(const QString &head, QWidget *parent) : QWidget(parent)
{
	setAttribute(Qt::WA_TranslucentBackground);

	setFixedSize(userWidgetSize);

	m_logInButton = new ButtonLabel;
	m_logInButton->setLabelText("LogIn");
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


void UserWidget::setHeadInfo(const QByteArray &_data)
{
	QPixmap pix;
	if (!pix.loadFromData(_data))
		return;
	m_userHeadLabel->setPixmap(pix.scaled(48, 48, Qt::KeepAspectRatio));
}

void UserWidget::setUserInfo(const QString &head, const QString &name)
{
	if (name == "LogIn")
		connect(m_logInButton, SIGNAL(clicked()), this, SIGNAL(logIn()));
	else
		disconnect(m_logInButton, SIGNAL(clicked()), this, SIGNAL(logIn()));
	m_userHeadLabel->setPixmap(QPixmap(head));
	m_logInButton->setLabelText(name);
}
