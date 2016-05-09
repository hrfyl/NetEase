#-------------------------------------------------
#
# Project created by QtCreator 2015-11-12T10:23:39
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NetEase
TEMPLATE = app

INCLUDEPATH += -I /usr/include/python2.7
LIBS = -L/usr/local/lib/ -lpython2.7 -ldl -lutil

SOURCES += \
    src/main.cpp \
    src/mainwindow/mainwindow.cpp \
    src/network/networkbase.cpp \
    src/network/networkmanage.cpp \
    src/network/networklogin.cpp \
    src/network/networkmusiclist.cpp \
    src/configure/configure.cpp \
    src/database/sqlitedata.cpp \
    src/toolwidgets/titlewidget.cpp \
    src/basedwidget/backgroundwidget.cpp \
    src/basedwidget/basedstyleshetwidget.cpp \
    src/basedwidget/basedwidget.cpp \
    src/toolwidgets/imagebutton.cpp \
    src/mediaplayer/mediaplayer.cpp \
    src/mainwindow/maintopwidget.cpp \
    src/mainwindow/tools/musiclistswidget.cpp \
    src/mainwindow/tools/userwidget.cpp \
    src/toolwidgets/buttonlabel.cpp \
    src/toolwidgets/funcbutton.cpp \
    src/toolwidgets/sliderwidget.cpp \
    src/mainwindow/mainbottomwidget.cpp \
    src/toolwidgets/loginwidget.cpp \
    src/network/networklistdetails.cpp \
    src/toolwidgets/lyricwidget.cpp \
    src/lyric/lyric.cpp \
    src/network/networkmusiclyric.cpp \
    src/mainwindow/searchwidget/searchwidget.cpp \
    src/network/networkrecommend.cpp \
    src/network/networkgetsongsbyid.cpp \
    src/network/networksearchmusic.cpp \
    src/basedwidget/baseddialog.cpp \
    src/network/networkaddmusic.cpp \
    src/configure/urlconfigure.cpp \
    src/network/networkgetuserheadimage.cpp \
    src/network/networkradiofm.cpp

HEADERS  += \
    src/mainwindow/mainwindow.h \
    src/network/networkbase.h \
    src/network/networkmanage.h \
    src/network/networklogin.h \
    src/network/networkmusiclist.h \
    src/configure/configure.h \
    src/database/sqlitedata.h \
    src/toolwidgets/titlewidget.h \
    src/basedwidget/backgroundwidget.h \
    src/basedwidget/basedstyleshetwidget.h \
    src/basedwidget/basedwidget.h \
    src/toolwidgets/imagebutton.h \
    src/configure/ButtonConfigure.h \
    src/mediaplayer/mediaplayer.h \
    src/mainwindow/maintopwidget.h \
    src/mainwindow/tools/musiclistswidget.h \
    src/mainwindow/tools/userwidget.h \
    src/toolwidgets/buttonlabel.h \
    src/toolwidgets/funcbutton.h \
    src/toolwidgets/sliderwidget.h \
    src/mainwindow/mainbottomwidget.h \
    src/toolwidgets/loginwidget.h \
    src/configure/urlconfigure.h \
    src/network/networklistdetails.h \
    src/configure/looptype.h \
    src/toolwidgets/lyricwidget.h \
    src/lyric/lyric.h \
    src/network/networkmusiclyric.h \
    src/mainwindow/searchwidget/searchwidget.h \
    src/network/networkrecommend.h \
    src/network/networkgetsongsbyid.h \
    src/network/networksearchmusic.h \
    src/basedwidget/baseddialog.h \
    src/network/networkaddmusic.h \
    src/network/networkgetuserheadimage.h \
    src/network/networkradiofm.h

RESOURCES += \
    resource/NetEase.qrc

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
	resource/py_api/login.py \

!equals(_PRO_FILE_PWD_, OUT_PWD) {
    for(f, FILES_COPY_SRC){
	orig_file = $$_PRO_FILE_PWD_/$$f
	win32{
		dist_file = $$OUT_PWD/$$f
	}else{
		dist_file = $$~/.config/NetEase/$$f
	}
	dist = $$dirname(dist_file)
	win32:dist = $$replace(dist, /, \\)
	win32:orig_file = $$replace(orig_file, /, \\)
	!exists($$dist):system($$MKDIR $$dist)
	!exists($$dist_file):system($$COPY $$orig_file $$dist)
    }
}
