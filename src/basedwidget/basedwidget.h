/*
 *
 *		该类作为MainWindow的父类,实现窗口的移动
 *
 */

#ifndef BASEDWIDGET_H
#define BASEDWIDGET_H

#include <QWidget>

class QPoint;

class BasedWidget : public QWidget
{
	Q_OBJECT
public:
	explicit BasedWidget(QWidget *parent = 0);
	~BasedWidget();

protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);

private:
	QPoint m_pos;
	bool m_isPressed;
};

#endif // BASEDWIDGET_H
