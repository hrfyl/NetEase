#ifndef LYRICWIDGET_H
#define LYRICWIDGET_H

#include "../basedwidget/basedstyleshetwidget.h"

class Lyric;
class QLabel;

class LyricWidget : public BasedStyleShetWidget
{
	Q_OBJECT
	Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
	Q_PROPERTY(QString color READ color WRITE setColor)
public:
	explicit LyricWidget(QWidget *parent = 0);
	~LyricWidget();

	qreal opacity() const;
	void setOpacity(const qreal &opacity);

	QString color() const;
	void setColor(const QString &color);

signals:
	void lyricChanged(const QString *lyric);

public slots:
	void lyric(const QString &lyric);				//	获取歌词...
	void setTime(double _time);
	void clearText();

protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void paintEvent(QPaintEvent *);
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);

private:
	QPoint m_pos;
	bool m_isPressed;

	QLabel *m_lyricLabel;
	QLabel *m_nextLyricLabel;

	//QMap<int, QString> m_lyric;
	QHash<int, Lyric> m_lyric;
	int m_count;								//	歌词总行数...
	int m_currentCount;							//	当前行...

	qreal m_opacity;							//	窗口不透明度..
	QString m_color;							//	背景颜色..

	bool m_isCursorEnter;
};

#endif // LYRICWIDGET_H
