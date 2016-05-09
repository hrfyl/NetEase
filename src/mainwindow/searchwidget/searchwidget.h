#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include "../../basedwidget/basedwidget.h"

class QCheckBox;

class QLineEdit;
class TitleWidget;

class SearchWidget : public BasedWidget
{
	Q_OBJECT
public:
	explicit SearchWidget(QWidget *parent = 0);

signals:
	void search(const QString &);

private slots:
	void searchClicked();

private:
	void initUi();
	void initConnect();

private:
	QLineEdit *m_searLineEdit;
	TitleWidget *m_title;

	QCheckBox *m_musicSearch;
	QCheckBox *m_aritlsSearch;
	QCheckBox *m_albumSearch;
	QCheckBox *m_lyricSearch;
	QCheckBox *m_listsSearch;
	QCheckBox *m_ownerSearch;		//	主播电台...
};

#endif // SEARCHWIDGET_H
