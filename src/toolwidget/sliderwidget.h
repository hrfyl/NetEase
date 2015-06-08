#ifndef SLIDERWIDGET_H
#define SLIDERWIDGET_H

#include "../basedwidget/basedstyleshetwidget.h"

class SliderWidget : public BasedStyleShetWidget
{
	Q_OBJECT
	Q_PROPERTY(QString foreGround READ foreGround WRITE setForeGround)
	Q_PROPERTY(QString backGround READ backGround WRITE setBackGround)
public:
	SliderWidget(QWidget *parent = 0);
	~SliderWidget();

	void setForeGround(const QString &foreColor);
	void setBackGround(const QString &backColor);

	inline QString &foreGround()
	{
		return m_foreGround;
	}

	inline QString &backGround()
	{
		return m_backGround;
	}

public slots:
	void moveTo(qreal pos);

signals:
	void changeTo(qreal pos);

protected:
	void mousePressEvent(QMouseEvent *e);
	void paintEvent(QPaintEvent *e);
	void enterEvent(QEvent *e);
	void leaveEvent(QEvent *e);

private:
	bool m_isPressed;
	qreal m_currentPos;
	QCursor m_preCursor;
	QString m_foreGround;
	QString m_backGround;
};

#endif // SLIDERWIDGET_H
