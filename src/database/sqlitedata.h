/*
 *
 * SqliteData 使用单类模式，实现对sqlite数据库操作..
 *
 * */
#ifndef SQLITEDATA_H
#define SQLITEDATA_H

#include <QSqlDatabase>

class SqliteData
{
public:
	static SqliteData *sharedDatabase();

	bool initDatabase();
	void closeDatabase();

	int userId();
	QString cookies();

	bool setUserInfo(const QStringList &);
	bool userInfo(QStringList &);

private:
	static SqliteData *m_database;
	QSqlDatabase m_db;
};

#endif // SQLITEDATA_H
