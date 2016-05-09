#include "sqlitedata.h"
#include "../configure/configure.h"
#include <QObject>
#include <QVariant>
#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>

#ifdef QT_DEBUG
#include <QDebug>
#endif

SqliteData *SqliteData::m_database = Q_NULLPTR;

SqliteData *SqliteData::sharedDatabase()
{
	if (m_database == Q_NULLPTR)
	{
		m_database = new SqliteData;
	}
	return m_database;
}

bool SqliteData::initDatabase()
{
	m_db = QSqlDatabase::addDatabase("QSQLITE");
	m_db.setDatabaseName(Configure::databasePath);

	if (!m_db.open())
	{
#ifdef QT_DEBUG
		qDebug() << "open database error: " << m_db.lastError().text();
#endif
		return false;
	}
	return true;
}

void SqliteData::closeDatabase()
{
	m_db.close();
}

int SqliteData::userId()
{
	QSqlQuery query;
	QString sql = "select user_id from user_info where id = 1;";
	if (!query.exec(sql))
	{
#ifdef QT_DEBUG
		qDebug() << "select user_id error: " << query.lastError().text();
#endif
		return -1;
	}
	query.next();
	return query.value(0).toInt();
}

QString SqliteData::cookies()
{
	QSqlQuery query;
	QString sql = "select user_cookies from user_info where id = 1;";
	if (!query.exec(sql))
	{
#ifdef QT_DEBUG
		qDebug() << "select user_cookies error: " << query.lastError().text();
#endif
		return "";
	}
	query.next();
	return query.value(0).toString();
}

bool SqliteData::setUserInfo(const QStringList &list)
{
	QSqlQuery query;
	QString sql = QString("update user_info set user_name='%1',\
						  user_id='%2', user_nickName='%3',\
						  user_autologin=%4,\
						  user_cookies='%5';"
						).arg(list[0]).arg(list[1]).arg(list[2]).arg(list[3]).arg(list[4]);

	if (!query.exec(sql))
	{
#ifdef QT_DEBUG
		qDebug() << "update user_info error: " << query.lastError().text();
#endif
		return false;
	}
	return true;
}

bool SqliteData::userInfo(QStringList &list)
{
	bool recv = true;
	QSqlQuery query;
	if (!query.exec("select * from user_info;"))
	{
#ifdef QT_DEBUG
		qDebug() << "select user_info error: " << query.lastError().text();
#endif
		return (recv = false);
	}
	query.next();
	list.append(query.value(1).toString());
	list.append(query.value(2).toString());
	list.append(query.value(3).toString());
	list.append(query.value(4).toString());
	list.append(query.value(5).toString());
	return recv;
}









