/*
 *
 *		标题栏的关闭,最小化等按钮
 *
 * */
#ifndef IMAGEBUTTON_H
#define IMAGEBUTTON_H

#include <QWidget>
#include "../datatypedef/datatypdef.h"

class ImageButton : public QWidget
{
	Q_OBJECT
public:
	explicit ImageButton(QWidget *parent = 0);
	explicit ImageButton(const QString &icon, int num = 4, QWidget *parent = 0);
	explicit ImageButton(const QString &icon, bool isDivision, int num = 4, QWidget *parent = 0);
	~ImageButton();

	void setOneButtonInfo(const QString &icon, int num = 4);
	void setDivisionButtonInfo(const QString &icon, int num = 3);
	void setCursorEnable(bool cursorEnable);
	void setButtonSttaus(BUTTONSTATUS status);

signals:
	void clicked();

protected:
	void enterEvent(QEvent *e);		//	鼠标进入事件
	void leaveEvent(QEvent *e);		//	鼠标移出
	void mousePressEvent(QMouseEvent *e);		//	鼠标按下
	void mouseReleaseEvent(QMouseEvent *e);		//	鼠标释放
	void paintEvent(QPaintEvent *e);			//	窗口重汇
	void changeEvent(QEvent *e);

private:
	QPixmap m_currentpix;
	QList<QPixmap> m_pixList;
	int m_num;
	QCursor m_currentCursor;
	bool m_cursoeChangeAble;		//	控制鼠标进入的形状,默认关闭
};

#endif // IMAGEBUTTON_H
