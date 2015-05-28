#include "backgroundwidget.h"
#include <QPainter>

BackGroundWidget::BackGroundWidget(QWidget *parent)
	: BasedStyleShetWidget(parent)
{


	//		不使用此方法处理
	//		直接使用样式表方便换肤...
	//m_opacity = 1.0;
	//m_backgrounColor = QColor("#2abf1d");
	//m_wenliPixmap = QPixmap(":/default/wenli");
	//m_guanPixmap = QPixmap(":/default/guangyun_green");

	setObjectName("BackGround");
}

BackGroundWidget::~BackGroundWidget()
{

}

void BackGroundWidget::paintEvent(QPaintEvent * /*e*/)
{
	QPainter painter(this);
	painter.setPen(Qt::NoPen);

	//		使用图片背景时qproperty-backPixmap的属性必须为"default"
	//		否则使用单一颜色作为背景
	if (m_backgroundPixmap == "default")
	{
		painter.setBackground(m_backgrounColor);
		painter.drawRect(rect());
		painter.setOpacity(m_opacity);
		painter.drawPixmap(rect(), m_guanPixmap, rect());
		painter.setOpacity(1.0);
		painter.drawPixmap(rect(), m_wenliPixmap, rect());
	}
	else
		painter.drawPixmap(rect(), QPixmap(m_backgroundPixmap), rect());
}

void BackGroundWidget::setBackPixmap(const QString &pix)
{
	m_backgroundPixmap = pix;
	update();
}

void BackGroundWidget::setColor(const QColor &color)
{
	m_backgrounColor = color;
	update();
}

void BackGroundWidget::setWenliPixmap(const QString &pix)
{
	m_wenliString = pix;
	m_wenliPixmap = QPixmap(pix);
	update();
}

void BackGroundWidget::setGuanPixmap(const QString &pix)
{
	m_guanString = pix;
	m_guanPixmap = QPixmap(pix);
	update();
}

void BackGroundWidget::setOpacity(qreal real)
{
	m_opacity = real;
	update();
}
