#ifndef LYRICWIDGET_H
#define LYRICWIDGET_H

#include "../basedwidget/basedstyleshetwidget.h"
#include "../lyric/lyric.h"

class QLabel;

class LyricWidget : public BasedStyleShetWidget
{
	Q_OBJECT
	Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
	Q_PROPERTY(QString color READ color WRITE setColor)
public:
	explicit LyricWidget(QWidget *parent = 0);

	qreal opacity() const;
	void setOpacity(const qreal &opacity);

	QString color() const;
	void setColor(const QString &color);

	bool lockWidget() const;						//	锁定歌词界面..

signals:

public slots:
	void lyric(const QString &lyric);				//	获取歌词...
	void setTime(const double &);

	void setLockWidget(bool lockWidget);

protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void paintEvent(QPaintEvent *);
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);
	//void focusInEvent(QFocusEvent *);

private:
	QPoint m_pos;
	bool m_isPressed;

	QLabel *m_lyricLabel;
	QLabel *m_nextLyricLabel;

	Lyric m_currentMusicLyric;	//	当前歌曲歌词...

	qreal m_opacity;			//	窗口不透明度..
	QString m_color;			//	背景颜色..
	bool m_isCursorEnter;
	bool m_lockWidget;			//	是否锁定歌词界面..
};

#endif // LYRICWIDGET_H
