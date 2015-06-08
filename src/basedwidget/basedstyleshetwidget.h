/*
 *		该类用来解决直接继承QWidget不能使用样式表的问题
 *
 * */

#ifndef BASEDSTYLESHETWIDGET_H
#define BASEDSTYLESHETWIDGET_H

#include <QWidget>

class BasedStyleShetWidget : public QWidget
{
	Q_OBJECT
public:
	explicit BasedStyleShetWidget(QWidget *parent = 0);
	~BasedStyleShetWidget();

protected:
	void paintEvent(QPaintEvent *e);
};

#endif // BASEDSTYLESHETWIDGET_H
