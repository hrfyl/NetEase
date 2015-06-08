#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QDialog>

class QLineEdit;

class SearchWidget : public QDialog
{
	Q_OBJECT
public:
	explicit SearchWidget(QWidget *parent = 0);
	~SearchWidget();

signals:
	void search(const QString &search);

private slots:
	void on_ok_clicked();
	void on_cancel_clicked();

private:
	QLineEdit *searchLineEdit;
};

#endif // SEARCHWIDGET_H
