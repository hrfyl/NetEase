#ifndef NETWORKLOGIN_H
#define NETWORKLOGIN_H

#include "networkbase.h"
#include <Python.h>

class NetworkLogin : public NetworkBase
{
	Q_OBJECT
public:
	NetworkLogin(QObject *parent = 0);

signals:
	void logined(const bool &success);

public slots:
	void logIn(const QString &userName, const QString &userPasswd);

protected:
	void disposeifno(const QByteArray &);

private:
	bool isOk(PyObject *const, const QString &);
};

#endif // NETWORKLOGIN_H
