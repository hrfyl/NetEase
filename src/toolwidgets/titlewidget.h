/*
 *			自定义窗口标题栏
 *
 * */
#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include "../basedwidget/basedstyleshetwidget.h"
#include "../configure/looptype.h"

class QLabel;
class QMenu;

class TitleWidget : public BasedStyleShetWidget
{
	Q_OBJECT
public:
	explicit TitleWidget(const bool &isMainTitle = false, QWidget *parent = 0);
	~TitleWidget();

signals:
	//void showMenu();
	void closeClicked();		//	最小化到托盘...
	void showMini();
	void showSkin();
	void updateClicked();
	void exitWidget();			//	退出...
	void logOut();
	void search();

	// 循环控制..
	void loopTyepChanged(const LOOPTYPE &);

private slots:
	void menuVisiable();

	void setOneLoopChecked();
	void setAllLoopChecked();
	void setRandomLoopChecked();

private:
	void initUi(const bool &isMainTitle);
	void initAction();

private:
	QMenu *m_menu;

	QAction *allLoopAction;
	QAction *oneLoopAction;
	QAction *randomLoopAction;
};

#endif // TITLEWIDGET_H
