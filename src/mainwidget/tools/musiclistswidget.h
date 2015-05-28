#ifndef MUSICLISTSWIDGET_H
#define MUSICLISTSWIDGET_H

#include <QWidget>

class QListWidget;
class QListWidgetItem;

class MusicListsWidget : public QWidget
{
	Q_OBJECT
public:
	explicit MusicListsWidget(QWidget *parent = 0);
	~MusicListsWidget();

	//		返回当前列表名
	QString currentListName();

signals:
	void itemClicked(const QString &text);

private slots:
	void itemDoubleClicked(QListWidgetItem *item);

public slots:
	void addItem(const QString &item);
	void rmCurrentItem();
	void rmAllItem();

	void setCurrentItem(int index);

private:
	QListWidget *m_musicList;
};

#endif // MUSICLISTSWIDGET_H
