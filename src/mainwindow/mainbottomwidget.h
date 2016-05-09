#ifndef MAINBOTTOMWIDGET_H
#define MAINBOTTOMWIDGET_H

#include <QWidget>

class BackGroundWidget;
class QLabel;
class FuncButton;
class SliderWidget;

class MainBottomWidget : public QWidget
{
	Q_OBJECT
public:
	explicit MainBottomWidget(QWidget *parent = 0);
	~MainBottomWidget();

public slots:
	void setPlayButtonStatus();			//	播放按钮需要改变..

private slots:
	void setVoice(qreal pos);						//	设置音量..
	void postionChanged(const QString &, const QString &, const qreal &);

private:
	void initUi();
	void initConnect();

private:
	BackGroundWidget *m_backGroundWidget;
	QLabel *m_musicLogo;

	//	buttons
	FuncButton *m_playButton;
	FuncButton *m_playNextButton;
	FuncButton *m_playFrontButton;

	//		进度和音量条
	QLabel *m_currentVoice;
	QLabel *m_currentPostion;
	QLabel *m_duration;
	SliderWidget *m_voiceWidget;
	SliderWidget *m_postionWidget;
};

#endif // MAINBOTTOMWIDGET_H
