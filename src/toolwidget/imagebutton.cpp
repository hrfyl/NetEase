#include "imagebutton.h"
#include <QPainter>
#include <QMouseEvent>

ImageButton::ImageButton(QWidget *parent) : QWidget(parent), m_cursoeChangeAble(false)
{
	setAttribute(Qt::WA_TranslucentBackground);
}

ImageButton::ImageButton(const QString &icon, int num, QWidget *parent)
	: QWidget(parent), m_num(num), m_cursoeChangeAble(false)
{
	setAttribute(Qt::WA_TranslucentBackground);

	QPixmap pix(icon);
	for (int i = 0; i != m_num; i++)
		m_pixList.append(pix.copy(i * pix.width() / m_num, 0, pix.width() / m_num, pix.height()));
	m_currentpix = m_pixList.at(0);
	setFixedSize(m_currentpix.size());
}

ImageButton::ImageButton(const QString &icon, bool isDivision, int num, QWidget *parent)
	: QWidget(parent), m_num(num), m_cursoeChangeAble(false)
{
	setAttribute(Qt::WA_TranslucentBackground);

	if (isDivision)
		setDivisionButtonInfo(icon, num);
	else
		setOneButtonInfo(icon, num);
}

ImageButton::~ImageButton()
{

}

void ImageButton::setOneButtonInfo(const QString &icon, int num)
{
	if (!m_pixList.isEmpty())
		m_pixList.clear();

	m_num = num;
	QPixmap pix(icon);
	for (int i = 0; i != m_num; i++)
		m_pixList.append(pix.copy(i * pix.width() / m_num, 0, pix.width() / m_num, pix.height()));
	m_currentpix = m_pixList.at(0);
	setFixedSize(m_currentpix.size());
}

void ImageButton::setDivisionButtonInfo(const QString &icon, int num)
{
	m_num = num;
	m_pixList.append(QPixmap(icon + "_normal"));
	m_pixList.append(QPixmap(icon + "_hover"));
	m_pixList.append(QPixmap(icon + "_press"));
	if (m_num == 4)
		m_pixList.append(QPixmap(icon + "_disable"));
	m_currentpix = m_pixList.at(0);
	setFixedSize(m_currentpix.size());
}

void ImageButton::setButtonSttaus(BUTTONSTATUS status)
{
	if (isEnabled())
	{
		switch (status)
		{
		case BUTTON_PRESSED :
			m_currentpix = m_pixList.at(2);
			break;
		case BUTTON_LEAVE :
			m_currentpix = m_pixList.at(0);
			break;
		case BUTTON_ENTER :
			m_currentpix = m_pixList.at(1);
			break;
		default :
			break;
		}
	}
	else
		if (m_num == 4)
			m_currentpix = m_pixList.at(3);
	update();
}

void ImageButton::setCursorEnable(bool cursorEnable)
{
	m_cursoeChangeAble = cursorEnable;
}

void ImageButton::paintEvent(QPaintEvent * /*e*/)
{
	QPainter painter(this);
	painter.drawPixmap(rect(), m_currentpix);
}

void ImageButton::mousePressEvent(QMouseEvent *e)
{
	if (e->button() != Qt::LeftButton)
		e->ignore();
	else
		setButtonSttaus(BUTTON_PRESSED);
}

void ImageButton::mouseReleaseEvent(QMouseEvent *e)
{
	if (e->button() != Qt::LeftButton)
	{
		e->ignore();
		return;
	}
	if (rect().contains(e->pos()))
	{
		if (isEnabled())
			emit clicked();
		setButtonSttaus(BUTTON_ENTER);
		e->accept();
	}
	else
	{
		setButtonSttaus(BUTTON_LEAVE);
		e->ignore();
	}
}

void ImageButton::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::EnabledChange)
	{
		if (!isEnabled() && (m_num == 4))
			setButtonSttaus(BUTTON_DISABLE);
	}
}
void ImageButton::enterEvent(QEvent * /*e*/)
{
	setButtonSttaus(BUTTON_ENTER);
	if (m_cursoeChangeAble)
	{
		m_currentCursor = cursor();
		setCursor(Qt::PointingHandCursor);
	}
}

void ImageButton::leaveEvent(QEvent * /*e*/)
{
	setButtonSttaus(BUTTON_LEAVE);
	if (m_cursoeChangeAble)
		setCursor(m_currentCursor);
}
