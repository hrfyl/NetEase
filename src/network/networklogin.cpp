#include "networklogin.h"
#include "../configure/urlconfigure.h"
#include "../configure/configure.h"
#include <stdlib.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QRegExp>

#ifdef QT_DEBUG
#include <QDebug>
#endif

NetworkLogin::NetworkLogin(QObject *parent) :
	NetworkBase(parent)
{
#ifdef Q_OS_UNIX
	QString pyPath = Configure::pyLoginPath;
	pyPath.append(":.:$PYTHONPATH");
	setenv("PYTHONPATH", pyPath.toStdString().data(), 0);
#endif
#ifndef Q_OS_UNIX
	QString path("PYTHONPATH=" + Configure::pyLoginPath);
	putenv(path.toStdString().data());
#endif
}

void NetworkLogin::logIn(const QString &userName, const QString &userPasswd)
{
	Py_Initialize();		//	初始化Python模块..
	PyObject *pModule = NULL, *pFunc = NULL;
	PyObject *pArgs = NULL, *pValue = NULL;

	pModule = PyImport_ImportModule("login");		//	加载login.py..
	if (!isOk(pModule, "login error: cant't open file: login.py"))
		return;

	pFunc = PyObject_GetAttrString(pModule, "login");
	//pFunc = PyObject_GetAttrString(pModule, "encrypted_login");
	if (!isOk(pFunc, "login error: get python function error"))
		return;

	// 设置参数..
	pArgs = PyTuple_New(3);
	PyTuple_SetItem(pArgs, 0, PyString_FromString(userName.toStdString().data()));
	PyTuple_SetItem(pArgs, 1, PyString_FromString(userPasswd.toStdString().data()));
	PyTuple_SetItem(pArgs, 2, PyString_FromString(Configure::databasePath.toStdString().data()));

	//	call python function
	pValue = PyObject_CallObject(pFunc, pArgs);
	if (!isOk(pValue, "login error: call python function error"))
		return;

	int statCode = PyInt_AsLong(pValue);
	if (statCode != 0)
	{
#ifdef QT_DEBUG
		qDebug() << "login error: check your [username/password] and login.py file";
#endif
		logined(false);
	}
	else
	{
		logined(true);
	}

	Py_Finalize();
}

void NetworkLogin::disposeifno(const QByteArray &/*data*/)
{
}

#ifdef QT_DEBUG
bool NetworkLogin::isOk(PyObject *const obj, const QString &message)
#endif
#ifdef QT_NO_DEBUG
bool NetworkLogin::isOk(PyObject *const obj, const QString &/*message*/)
#endif
{
	if (!obj)
	{
#ifdef QT_DEBUG
		qDebug() << message;
#endif
		Py_Finalize();
		logined(false);
		return false;
	}
	return true;
}








