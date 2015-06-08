#include "basedstyleshetwidget.h"
#include <QPainter>
#include <QStyleOption>

BasedStyleShetWidget::BasedStyleShetWidget(QWidget *parent) : QWidget(parent)
{

}

BasedStyleShetWidget::~BasedStyleShetWidget()
{

}

void BasedStyleShetWidget::paintEvent(QPaintEvent * /*e*/)
{
	QPainter painter(this);
	QStyleOption opt;
	opt.init(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}
