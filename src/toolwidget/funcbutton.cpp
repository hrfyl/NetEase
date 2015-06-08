#include "funcbutton.h"
#include <QPainter>
#include <QMouseEvent>

FuncButton::FuncButton(const QString &_icon, QWidget *parent)
	: BasedStyleShetWidget(parent)
{
	setAttribute(Qt::WA_TranslucentBackground);
	m_pix = QPixmap(_icon);
	setFixedSize(m_pix.size());
	m_isPressed = false;
	m_isEnter = false;
}

FuncButton::~FuncButton()
{

}

void FuncButton::setMIcon(const QString &_icon)
{
	m_pix = QPixmap(_icon);
	update();
}

void FuncButton::paintEvent(QPaintEvent * /*e*/)
{
	QPainter painter(this);
	if (m_isEnter)
	{
		painter.setPen(Qt::NoPen);
		painter.setBrush(QBrush(QColor(Qt::lightGray)));
		painter.drawEllipse(rect());
		painter.drawPixmap(rect(), m_pix);
	}
	else
		painter.drawPixmap(rect(), m_pix);
}

void FuncButton::mousePressEvent(QMouseEvent *e)
{
	if (e->button() != Qt::LeftButton)
		e->ignore();
	else
		m_isPressed = true;
}

void FuncButton::mouseReleaseEvent(QMouseEvent * /*e*/)
{
	if (m_isPressed)
		emit clicked();
	m_isPressed = false;
}

void FuncButton::enterEvent(QEvent * /*e*/)
{
	m_isEnter = true;
	m_preCursor = cursor();
	setCursor(Qt::PointingHandCursor);
	update();
}

void FuncButton::leaveEvent(QEvent * /*e*/)
{
	m_isEnter = false;
	setCursor(m_preCursor);
	update();
}











