#include "baseddialog.h"
#include <QMouseEvent>

BasedDialog::BasedDialog(QWidget *parent) :
	QDialog(parent)
{
	m_isPressed = false;
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
}

BasedDialog::~BasedDialog()
{

}

void BasedDialog::mousePressEvent(QMouseEvent *e)
{
	if (e->button() != Qt::LeftButton)
		e->ignore();
	else
	{
		m_isPressed = true;
		m_pos = e->globalPos() - pos();
	}
}

void BasedDialog::mouseReleaseEvent(QMouseEvent * /*e*/)
{
	m_isPressed = false;
}

void BasedDialog::mouseMoveEvent(QMouseEvent *e)
{
	if (m_isPressed)
		move(e->globalPos() - m_pos);
	else
		QWidget::mouseMoveEvent(e);
}
