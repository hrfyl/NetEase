#-------------------------------------------------
#
# Project created by QtCreator 2015-04-24T23:12:41
#
#-------------------------------------------------

QT       += core gui multimedia sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NetEase
TEMPLATE = app


SOURCES += \
    src/mainwidget/mainwindow.cpp \
    src/main.cpp \
    src/basedwidget/basedwidget.cpp \
    src/mainwidget/tools/titlewidget.cpp \
    src/toolwidget/imagebutton.cpp \
    src/basedwidget/basedstyleshetwidget.cpp \
    src/mainwidget/maintopwidget.cpp \
    src/mainwidget/mainbottomwidget.cpp \
    src/basedwidget/backgroundwidget.cpp \
    src/toolwidget/funcbutton.cpp \
    src/toolwidget/sliderwidget.cpp \
    src/mainwidget/tools/musiclistswidget.cpp \
    src/mainwidget/tools/userwidget.cpp \
    src/toolwidget/buttonlabel.cpp \
    src/toolwidget/addmusiclist.cpp \
    src/Data/sqlitedata.cpp \
    src/mainwidget/networkwidget.cpp \
    src/mainwidget/tools/loginwidget.cpp \
    src/toolwidget/searchwidget.cpp \
    src/function/network.cpp \
    src/function/media.cpp \
    src/toolwidget/lyricwidget.cpp \
    src/Data/lyric.cpp

HEADERS  += \
    src/mainwidget/mainwindow.h \
    src/basedwidget/basedwidget.h \
    src/mainwidget/tools/titlewidget.h \
    src/toolwidget/imagebutton.h \
    src/datatypedef/datatypdef.h \
    src/basedwidget/basedstyleshetwidget.h \
    src/mainwidget/maintopwidget.h \
    src/mainwidget/mainbottomwidget.h \
    src/basedwidget/backgroundwidget.h \
    src/toolwidget/funcbutton.h \
    src/toolwidget/sliderwidget.h \
    src/mainwidget/tools/musiclistswidget.h \
    src/mainwidget/tools/userwidget.h \
    src/toolwidget/buttonlabel.h \
    src/toolwidget/addmusiclist.h \
    src/Data/sqlitedata.h \
    src/mainwidget/networkwidget.h \
    src/mainwidget/tools/loginwidget.h \
    src/toolwidget/searchwidget.h \
    src/function/network.h \
    src/function/media.h \
    src/datatypedef/urlinfo.h \
    src/toolwidget/lyricwidget.h \
    src/Data/lyric.h

RESOURCES += \
    resource/netease.qrc

DISTFILES +=

#复制数据库文件
win32{
	RC_FILE = resource/NetEase.rc
	COPY = copy /y
	MKDIR = mkdir
}else{
	COPY = cp
	MKDIR = mkdir -p
}

FILES_COPY_SRC += resource/sql/music.db	\
	resource/qss/default.qss \

!equals(_PRO_FILE_PWD_, OUT_PWD) {
	for(f, FILES_COPY_SRC){
		orig_file = $$_PRO_FILE_PWD_/$$f
		#dist_file = $$OUT_PWD/$$f
		dist_file = $$~/.cache/NetEase/$$f
		dist = $$dirname(dist_file)
		win32:dist = $$replace(dist, /, \\)
		win32:orig_file = $$replace(orig_file, /, \\)
		!exists($$dist):system($$MKDIR $$dist)
		!exists($$dist_file):system($$COPY $$orig_file $$dist)
	}
}

