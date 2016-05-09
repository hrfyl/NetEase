#ifndef BASEDDIALOG_H
#define BASEDDIALOG_H

#include <QDialog>

class BasedDialog : public QDialog
{
	Q_OBJECT
public:
	explicit BasedDialog(QWidget *parent = 0);
	~BasedDialog();

protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);

private:
	QPoint m_pos;
	bool m_isPressed;
};

#endif // BASEDDIALOG_H
