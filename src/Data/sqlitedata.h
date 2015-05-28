#ifndef SQLITEDATA_H
#define SQLITEDATA_H

#include <QList>

class QSqlQuery;
class QString;
class QStringList;

class SqliteData
{
public:
	SqliteData();
	~SqliteData();

	//		是否创建表...没有则创建数据表...
	bool initTables();
	QStringList netUserInfo();
	void updateUserInfo(QStringList list);

	bool isOk(const QString &tableName, const QString &title);

	int musicListCount();
	bool insertIntoMusicList(const QString &name, const QString &ListId);
	bool createTable(const QString &tableName);
	bool insertIntoList(const QString &tableName, const QString &filePath, const QString &title, const QString &artist, const QString &Album);

	QStringList selectList();
	QList<QStringList> selectFiles(const QString &tableName);

	QString getListId(const QString &ListName);

	bool updateTable(const QString &tableName, const QString &oldTitle, const QString &title, const QString &artist, const QString &Album);

	bool deleteData(const QString &tableName, const QString &title);
	bool dropTable(const QString &tableName);
};

#endif // SQLITEDATA_H
