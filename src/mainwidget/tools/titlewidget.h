/*
 *			自定义窗口标题栏
 *
 * */
#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include "../../basedwidget/basedstyleshetwidget.h"

class QLabel;
class QMenu;

class TitleWidget : public BasedStyleShetWidget
{
	Q_OBJECT
public:
	explicit TitleWidget(QWidget *parent = 0);
	~TitleWidget();
	void netWorkModel(bool isTrue);

signals:
	//void showMenu();
	void closeClicked();
	void showMini();
	void showSkin();
	void updateClicked();

	void loopPlay();
	void loopOnePlay();
	void listPlay();
	void randomPlay();

	//		QAction Signals
	void addFile();
	void addList();
	void rmFiles();
	void rmLists();
	void exitWidget();
	void switchWidget();
	void logOut();
	void search();

public slots:
	void menuVisiable();
	void loopPlayClicked();
	void loopOnePlayClicked();
	void listPlayClicked();
	void randomPlayClicked();

private:
	void initUi();
	void initAction();

private:
	QMenu *m_menu;

	QAction *switchWidgetAction;

	QAction *addFileAction;
	QAction *addListAction;
	QAction *rmFileAction;
	QAction *rmListAction;
	QAction *loopPlayAction;
	QAction *loopOnePlayAction;
	QAction *listPlayAction;
	QAction *randomPlayAction;
	QAction *logOutAction;
	QAction *searchAction;
};

#endif // TITLEWIDGET_H
