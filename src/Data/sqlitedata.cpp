#include "sqlitedata.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QString>
#include <QVariant>
#include <QSqlError>
#include <QStringList>
#include <QProcess>

bool connectDatabase(QString &userName)
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	QStringList list = QProcess::systemEnvironment();
	QStringList::const_iterator it = list.begin();
	QString currentUser = "";
	while (it != list.end())
	{
		if ((*it).left(4) == "USER")
		{
			currentUser = (*it).mid(5);
			break;
		}
		if ((*it).left(8) == "USERNAME")
		{
			currentUser = (*it).mid(9);
			break;
		}
		it++;
	}
	userName = currentUser;

#ifndef WIN32
	db.setDatabaseName("/home/" + currentUser + "/.cache/NetEase/resource/sql/music.db");
#endif
#ifdef WIN32
	db.setDatabaseName("./resource/sql/music.db");
#endif
	if (!db.open())
	{
		QMessageBox::critical(0, QObject::tr("连接数据出错"), db.lastError().text(), QMessageBox::Ok);
		return false;
	}
	SqliteData sqlite;
	if (!sqlite.initTables())
		return false;
	return true;
}




SqliteData::SqliteData()
{

}

SqliteData::~SqliteData()
{

}

bool SqliteData::initTables()
{
	QSqlQuery query;
	if (!query.exec("select count(*) from music_list;"))
	{
		if (!query.exec("CREATE TABLE music_list(Id integer primary key autoincrement, ListName text(50) not null, ListId text(10) not null);"))
		{
			QMessageBox::critical(0, "错误", query.lastError().text(), QMessageBox::Ok);
			return false;
		}
	}
	if (!query.exec("select count(*) from net_info;"))
	{
		if (!query.exec("create table net_info(user_name text(30) default(''), user_passwd text(30) default(''), user_id text(10) default(''), user_nickName text(30), auto_login int default(0));"))
		{
			QMessageBox::critical(0, "错误", query.lastError().text(), QMessageBox::Ok);
			return false;
		}
		if (!query.exec("insert into net_info(user_name, user_passwd, user_id, user_nickName, auto_login) values('test', '', '', '', 0);"))
		{
			query.exec("drop table net_info;");
			QMessageBox::critical(0, "错误", query.lastError().text(), QMessageBox::Ok);
			return false;
		}
	}
	return true;
}

QStringList SqliteData::netUserInfo()
{
	QStringList info;
	QSqlQuery query;
	if (!query.exec("select * from net_info;"))
	{
		QMessageBox::critical(0, "错误", query.lastError().text(), QMessageBox::Ok);
		return info;
	}
	query.next();
	info.append(query.value(0).toString());
	info.append(query.value(1).toString());
	info.append(query.value(2).toString());
	info.append(query.value(3).toString());
	info.append(query.value(4).toString());
	return info;
}

void SqliteData::updateUserInfo(QStringList list)
{
	QSqlQuery query;
	QString sql = QString("update net_info set user_name='%1', user_passwd='%2', user_id='%3', user_nickName='%4', auto_login=%5").arg(list[0]).arg(list[1]).arg(list[2]).arg(list[3]).arg(list[4]);
	query.exec(sql);
}

int SqliteData::musicListCount()
{
	QSqlQuery query;
	query.exec("select count(*) from music_list;");
	query.next();
	int count = query.value(0).toInt();
	return count;
}

bool SqliteData::insertIntoMusicList(const QString &name, const QString &ListId)
{
	QSqlQuery query;
	QString insertSql = QString("insert into music_list(ListName, ListId) values('%1', '%2');").arg(name).arg(ListId);
	if (!query.exec(insertSql))
	{
		QMessageBox::critical(0, "错误", query.lastError().text(), QMessageBox::Ok);
		return false;
	}
	return true;
}

bool SqliteData::createTable(const QString &tableName)
{
	QSqlQuery query;
	QString sql = QString("create table %1(FilePath text(50) primary key, Title text(20), Artist text(20), Album text(20), IsOk int);").arg(tableName);
	if (!query.exec(sql))
	{
		QMessageBox::critical(0, "错误", query.lastError().text(), QMessageBox::Ok);
		return false;
	}
	return true;
}


bool SqliteData::insertIntoList(const QString &tableName, const QString &filePath, const QString &title, const QString &artist, const QString &Album)
{
	QSqlQuery query;
	QString sql;
	if (artist == "" && Album == "")
		sql = QString("insert into %1(FilePath, Title, Artist, Album, IsOk) values('%2', '%3', '%4', '%5', %6);").arg(tableName).arg(filePath).arg(title).arg(artist).arg(Album).arg(0);
	else
		sql = QString("insert into %1(FilePath, Title, Artist, Album, IsOk) values('%2', '%3', '%4', '%5', %6);").arg(tableName).arg(filePath).arg(title).arg(artist).arg(Album).arg(1);
	if (!query.exec(sql))
	{
		QMessageBox::critical(0, "错误", query.lastError().text(), QMessageBox::Ok);
		return false;
	}
	return true;
}

QStringList SqliteData::selectList()
{
	QSqlQuery query;
	QString sql = "select ListName from music_list;";

	QStringList list;

	if (!query.exec(sql))
	{
		QMessageBox::critical(0, "错误", query.lastError().text(), QMessageBox::Ok);
		return list;
	}

	while (query.next())
		list.append(query.value(0).toString());
	return list;
}


QList<QStringList> SqliteData::selectFiles(const QString &tableName)
{
	QSqlQuery query;
	QList<QStringList> files;

	QString sql = QString("select FilePath, Title, Artist, Album from %1").arg(tableName);
	if (!query.exec(sql))
	{
		QMessageBox::critical(0, "错误", query.lastError().text(), QMessageBox::Ok);
		return files;
	}

	while (query.next())
	{
		QStringList list;
		list.append(query.value(0).toString());
		list.append(query.value(1).toString());
		list.append(query.value(2).toString());
		list.append(query.value(3).toString());
		files.append(list);
	}
	return files;
}

QString SqliteData::getListId(const QString &ListName)
{
	QSqlQuery query;
	QString sql = QString("select ListId from music_list where ListName = '%1';").arg(ListName);
	QString recv = "";

	if (!query.exec(sql))
	{
		QMessageBox::critical(0, "错误", query.lastError().text(), QMessageBox::Ok);
		return recv;
	}
	query.next();
	recv = query.value(0).toString();
	return recv;
}

bool SqliteData::updateTable(const QString &tableName, const QString &oldTitle, const QString &title, const QString &artist, const QString &Album)
{
	if (isOk(tableName, oldTitle))
		return false;
	QSqlQuery query;
	QString sql;
	if (artist != "" && Album != "")
		sql = QString("update %1 set Title='%2', Artist='%3', Album='%4', IsOk=%5 where Title='%6';").arg(tableName).arg(title).arg(artist).arg(Album).arg(1).arg(oldTitle);
	else
		sql = QString("update %1 set Title='%2', Artist='%3', Album='%4', IsOk=%5 where Title='%6';").arg(tableName).arg(title).arg(artist).arg(Album).arg(0).arg(oldTitle);
	if (!query.exec(sql))
	{
		QMessageBox::critical(0, "错误", query.lastError().text(), QMessageBox::Ok);
		return false;
	}
	return true;
}


bool SqliteData::isOk(const QString &tableName, const QString &title)
{
	QSqlQuery query;
	QString sql = QString("select IsOk from %1 where Title='%2'").arg(tableName).arg(title);
	if (!query.exec(sql))
		return false;
	query.next();
	int is_ok = query.value(0).toInt();
	if (!is_ok)
		return false;
	return true;
}

bool SqliteData::deleteData(const QString &tableName, const QString &title)
{
	QSqlQuery query;
	QString sql;
	if (tableName == "music_list")
		sql = QString("delete from music_list where ListId='%1';").arg(title);
	else
		sql = QString("delete from %1 where Title='%2';").arg(tableName).arg(title);

	if (!query.exec(sql))
	{
		QMessageBox::critical(0, "错误", query.lastError().text(), QMessageBox::Ok);
		return false;
	}
	return true;
}

bool SqliteData::dropTable(const QString &tableName)
{
	//		防止删除music_list表
	if (tableName == "music_list")
		return false;
	QSqlQuery query;
	QString sql = QString("drop table %1;").arg(tableName);
	if (!query.exec(sql))
	{
		QMessageBox::critical(0, "错误", query.lastError().text(), QMessageBox::Ok);
		return false;
	}
	return true;
}









