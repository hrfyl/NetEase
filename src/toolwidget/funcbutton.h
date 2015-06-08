#ifndef FUNCBUTTON_H
#define FUNCBUTTON_H

#include "../basedwidget/basedstyleshetwidget.h"

class FuncButton : public BasedStyleShetWidget
{
	Q_OBJECT
public:
	explicit FuncButton(const QString &_icon, QWidget *parent = 0);
	~FuncButton();

	void setMIcon(const QString &_icon);

signals:
	void clicked();

protected:
	void paintEvent(QPaintEvent *e);
	void enterEvent(QEvent *e);
	void leaveEvent(QEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

private:
	bool m_isPressed;
	bool m_isEnter;
	QCursor m_preCursor;
	QPixmap m_pix;
};

#endif // FUNCBUTTON_H
