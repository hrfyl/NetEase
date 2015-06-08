#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QDialog>

class QPushButton;
class QLineEdit;

class LogInWidget : public QDialog
{
	Q_OBJECT
public:
	explicit LogInWidget(QWidget *parent = 0);
	~LogInWidget();

signals:
	void logInClicked(const QString &userName, const QString &passwd);

private slots:
	void on_logIn_clicked();

private:
	QPushButton *m_okButton;
	QPushButton *m_cancelButton;
	QLineEdit *m_nameLineEdit;
	QLineEdit *m_passwdLineEdit;
};

#endif // LOGINWIDGET_H
