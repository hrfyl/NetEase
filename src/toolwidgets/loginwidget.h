#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include "../basedwidget/baseddialog.h"

class TitleWidget;
class QLineEdit;
class QLabel;

class LogInWidget : public BasedDialog
{
	Q_OBJECT
public:
	explicit LogInWidget(QWidget *parent = 0);

signals:
	void loginSuccess();

private slots:
	void logIn();
	void logIned(const bool &);

private:
	void initUi();

private:
	TitleWidget *m_title;
	QLineEdit *m_userNameLineEdit;
	QLineEdit *m_userPasswdLineEdit;
	QLabel *m_loginStatusLabel;
};

#endif // LOGINWIDGET_H
