#ifndef CONFIGURE_H
#define CONFIGURE_H

/*
 * Configure 应用的配置...
 * 包括： 应用名，版本号，以及一些常用的常量...
 *
 * */

#include <QString>
class QSize;

extern const QString APP_NAME;
extern const QString APP_VERSION;

extern const QSize mainWindowSize;
extern const QSize mainTitleSize;
extern const QSize mainTopWidgetSize;
extern const QSize mainBootomSize;

extern const QSize currentMusicListWidgetSize;

extern const QSize logInWidgetSize;
extern const QSize logInTitleSize;

extern const QSize lyricWidgetSize;

extern const QSize searchWidgetSize;
extern const QSize searchTitleSize;

extern const QSize musicListWidgetSize;
extern const QSize userWidgetSize;


class Configure
{
private:
	Configure() {}				//	不让该类实例化..

public:
	static QString WHOAMI;

	// NetEase Music Database Path
	static QString databasePath;
	// NetEase Music Login Python Path
	static QString pyLoginPath;
	// NetEase Music Qss Path
	static QString qssPath;

	// NetEase Music User Id
	static int userId;
	// NetEase Music User NickName
	static QString userNickName;
	// NetEase Music Cookies
	static QString userCookies;
	// NetEase Music Auto Login
	static bool userAutoLogin;
	// NetEase Music User Head Image Url
	static QString userHeadImageUrl;

	// function
	static void initConfig();		//	初始化配置...

	// update configure..
	static void updateConfigure();	//	更新配置..
};


#endif // CONFIGURE_H
