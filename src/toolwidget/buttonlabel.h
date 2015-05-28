#ifndef BUTTONLABEL_H
#define BUTTONLABEL_H

#include <QLabel>

class ButtonLabel : public QLabel
{
	Q_OBJECT
public:
	explicit ButtonLabel(QWidget *parent = 0);
	~ButtonLabel();

	void setLabelText(const QString &text);

signals:
	void clicked();

private:
	void isClicked();

protected:
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

private:
	bool m_isPressed;
	bool m_isClicked;
	QCursor m_preCursor;
};

#endif // BUTTONLABEL_H
