#include "configure.h"
#include "../database/sqlitedata.h"
#include <QString>
#include <QStringList>
#include <QSize>

#ifdef Q_OS_UNIX
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#endif

const QString APP_NAME = "网易云音乐";
const QString APP_VERSION = "1.1.19";

const QSize mainWindowSize(900, 600);
const QSize mainTitleSize(900, 35);
const QSize mainTopWidgetSize(900, 440);
const QSize mainBootomSize(900, 125);

const QSize currentMusicListWidgetSize(570, 420);

const QSize logInWidgetSize(320, 180);
const QSize logInTitleSize(320, 35);

const QSize lyricWidgetSize(900, 80);

const QSize searchWidgetSize(300, 200);
const QSize searchTitleSize(300, 35);

const QSize musicListWidgetSize(300, 350);
const QSize userWidgetSize(300, 50);

QString Configure::WHOAMI = "wind";
QString Configure::databasePath = "./resource/sql/music.db";
QString Configure::pyLoginPath = "./resource/py_api/";
QString Configure::qssPath = "./resource/qss/default.qss";
int Configure::userId = -1;
QString Configure::userHeadImageUrl = "";
QString Configure::userNickName = "wind";
QString Configure::userCookies = "";
bool Configure::userAutoLogin = false;


void Configure::initConfig()
{
#ifdef Q_OS_UNIX
	// 获取当前用户名..
	struct passwd *pwd_st = getpwuid(getuid());
	Configure::WHOAMI = QString(pwd_st->pw_name);

	databasePath =  "/home/" + WHOAMI + "/.config/NetEase/resource/sql/music.db";
	pyLoginPath = "/home/" + WHOAMI +"/.config/NetEase/resource/py_api/";
	qssPath = "/home/" + WHOAMI +"/.config/NetEase/resource/qss/default.qss";
#endif

	// 初始化lqsite数据库...
	if (!SqliteData::sharedDatabase()->initDatabase())
		exit(1);

	updateConfigure();
}

void Configure::updateConfigure()
{
	// 获取NetEase用户信息...
	QStringList userInfo;
	if (SqliteData::sharedDatabase()->userInfo(userInfo))
	{
		bool isOk = false;
		int id = userInfo.at(1).toInt(&isOk, 10);
		if (isOk)
			userId = id;
		userNickName = userInfo.at(2);
		userAutoLogin = userInfo.at(3).toInt();
		userCookies = userInfo.at(4);
	}
}




