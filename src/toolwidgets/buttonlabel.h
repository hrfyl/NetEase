#ifndef BUTTONLABEL_H
#define BUTTONLABEL_H

#include <QLabel>

class ButtonLabel : public QLabel
{
	Q_OBJECT
	Q_PROPERTY(QString clickedColor READ clickedColor WRITE setClickedColor)
	Q_PROPERTY(QString clickedSize READ clickedSize WRITE setClickedSize)
public:
	explicit ButtonLabel(QWidget *parent = 0);
	~ButtonLabel();

	void setLabelText(const QString &text);

	QString clickedColor() const;
	void setClickedColor(const QString &clickedColr);

	QString clickedSize() const;
	void setClickedSize(const QString &clickedSize);

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
	QString m_clickedColor;
	QString m_clickedSize;
};

#endif // BUTTONLABEL_H
