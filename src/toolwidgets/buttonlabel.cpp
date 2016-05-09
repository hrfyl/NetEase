#include "buttonlabel.h"
#include <QMouseEvent>

ButtonLabel::ButtonLabel(QWidget *parent) : QLabel(parent)
{
	setAttribute(Qt::WA_TranslucentBackground);
	m_isPressed = false;
	m_isClicked = false;
}

ButtonLabel::~ButtonLabel()
{

}

void ButtonLabel::setLabelText(const QString &text)
{
	setText(text);
}

void ButtonLabel::enterEvent(QEvent *)
{
	m_preCursor = cursor();
	setCursor(Qt::PointingHandCursor);
	QFont font = this->font();
	font.setUnderline(true);
	setFont(font);
}

void ButtonLabel::leaveEvent(QEvent *)
{
	setCursor(m_preCursor);
	QFont font = this->font();
	font.setUnderline(false);
	setFont(font);
}

void ButtonLabel::mousePressEvent(QMouseEvent *e)
{
	if (e->button() != Qt::LeftButton)
		return;
	m_isPressed = true;
}

void ButtonLabel::mouseReleaseEvent(QMouseEvent * /*e*/)
{
	if (m_isPressed)
	{
		emit clicked();
		isClicked();
	}
	m_isPressed = false;
}
QString ButtonLabel::clickedSize() const
{
	return m_clickedSize;
}

void ButtonLabel::setClickedSize(const QString &clickedSize)
{
	m_clickedSize = clickedSize;
}


QString ButtonLabel::clickedColor() const
{
	return m_clickedColor;
}

void ButtonLabel::setClickedColor(const QString &clickedColor)
{
	m_clickedColor = clickedColor;
}


void ButtonLabel::isClicked()
{
	if (m_isClicked)
		return;
	m_isClicked = true;
	if (m_clickedColor == "")
		this->setStyleSheet("color:gray;font-size:20px;");
	else
		this->setStyleSheet(QString("color:%1;font-size:%2;").arg(m_clickedColor).arg(m_clickedSize));
}

