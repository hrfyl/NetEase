#include "basedwidget.h"
#include <QMouseEvent>

BasedWidget::BasedWidget(QWidget *parent) : QWidget(parent)
{
	m_isPressed = false;
	//setAttribute(Qt::WA_TranslucentBackground);	//	背景透明...
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
}

BasedWidget::~BasedWidget()
{

}

void BasedWidget::mousePressEvent(QMouseEvent *e)
{
	if (e->button() != Qt::LeftButton)
		e->ignore();
	else
	{
		m_isPressed = true;
		m_pos = e->globalPos() - pos();
	}
}

void BasedWidget::mouseReleaseEvent(QMouseEvent * /*e*/)
{
	m_isPressed = false;
}

void BasedWidget::mouseMoveEvent(QMouseEvent *e)
{
	if (m_isPressed)
		move(e->globalPos() - m_pos);
	else
		QWidget::mouseMoveEvent(e);
}
