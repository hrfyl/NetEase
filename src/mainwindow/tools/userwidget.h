#ifndef USERWIDGET_H
#define USERWIDGET_H

#include <QWidget>

class ButtonLabel;
class QLabel;

class UserWidget : public QWidget
{
	Q_OBJECT
public:
	explicit UserWidget(const QString &head, QWidget *parent = 0);
	~UserWidget();

	void setHeadInfo(const QByteArray &);
	void setUserInfo(const QString &, const QString &);

signals:
	void logIn();

public slots:

private:
	ButtonLabel *m_logInButton;
	QLabel *m_userHeadLabel;
};

#endif // USERWIDGET_H
