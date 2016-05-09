#include "sliderwidget.h"
#include <QPainter>
#include <QMouseEvent>

SliderWidget::SliderWidget(QWidget *parent)
	: BasedStyleShetWidget(parent)
{
	setAttribute(Qt::WA_TranslucentBackground);
	m_isPressed = false;
	m_currentPos = 0.0;
	m_backGround = m_foreGround = "";
	setFixedSize(400, 10);
}

SliderWidget::~SliderWidget()
{

}

void SliderWidget::setValue(qreal pos)
{
	m_currentPos = pos;
	update();
}

void SliderWidget::setForeGround(const QString &foreColor)
{
	m_foreGround = foreColor;
	update();
}

void SliderWidget::setBackGround(const QString &backColor)
{
	m_backGround = backColor;
	update();
}

void SliderWidget::paintEvent(QPaintEvent * /*e*/)
{
	QPainter painter(this);
	QPen pen;
	if (m_backGround == "" || m_foreGround == "")
	{
		pen.setColor(Qt::green);
		pen.setWidth(0.5);
		painter.setPen(pen);
		painter.drawRect(0, 0, 399, 9);

		painter.setPen(Qt::NoPen);
		painter.setBrush(Qt::lightGray);
		painter.drawRect(1, 1, 400 * m_currentPos, 8);
		return;
	}
	pen.setColor(QColor(m_backGround));
	pen.setWidth(0.5);
	painter.setPen(pen);
	painter.drawRect(0, 0, 399, 9);

	painter.setPen(Qt::NoPen);
	painter.setBrush(QBrush(QColor(m_foreGround)));
	painter.drawRect(1, 1, 400 * m_currentPos, 8);
}

void SliderWidget::enterEvent(QEvent * /*e*/)
{
	m_preCursor = cursor();
	setCursor(Qt::PointingHandCursor);
}

void SliderWidget::leaveEvent(QEvent * /*e*/)
{
	setCursor(m_preCursor);
}

void SliderWidget::mousePressEvent(QMouseEvent *e)
{
	if (e->button() != Qt::LeftButton)
		return;
	m_currentPos = e->pos().x() / 400.0;
	emit value(m_currentPos);
	update();
}










