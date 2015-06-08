#ifndef BACKGROUNDWIDGET_H
#define BACKGROUNDWIDGET_H

#include "basedstyleshetwidget.h"

class BackGroundWidget : public BasedStyleShetWidget
{
	Q_OBJECT
	//		属性定义,为了使用样式表换肤
	Q_PROPERTY(QColor color READ color WRITE setColor)
	Q_PROPERTY(QString backPixmap READ backPixmap WRITE setBackPixmap DESIGNABLE true SCRIPTABLE true)
	Q_PROPERTY(QString wenliPixmap READ wenliPixmap WRITE setWenliPixmap DESIGNABLE true SCRIPTABLE true)
	Q_PROPERTY(QString guanPixmap READ guanPixmap WRITE setGuanPixmap DESIGNABLE true SCRIPTABLE true)
	Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity DESIGNABLE true SCRIPTABLE true)
public:
	explicit BackGroundWidget(QWidget *parent = 0);
	~BackGroundWidget();

	void setBackPixmap(const QString &pix);
	void setColor(const QColor &color);
	void setWenliPixmap(const QString &pix);
	void setGuanPixmap(const QString &pix);
	void setOpacity(qreal real);

	inline QColor &color()
	{
		return m_backgrounColor;
	}

	inline QString &backPixmap()
	{
		return m_backgroundPixmap;
	}

	inline QString &wenliPixmap()
	{
		return m_wenliString;
	}

	inline QString &guanPixmap()
	{
		return m_guanString;
	}

	inline qreal opacity()
	{
		return m_opacity;
	}

protected:
	void paintEvent(QPaintEvent *e);

private:
	qreal m_opacity;

	QString m_guanString;
	QString m_wenliString;
	QPixmap m_guanPixmap;
	QPixmap m_wenliPixmap;

	QColor m_backgrounColor;
	QString m_backgroundPixmap;
};

#endif // BACKGROUNDWIDGET_H
