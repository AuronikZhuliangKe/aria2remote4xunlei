# -------------------------------------------------
# Project created by QtCreator 2010-02-14T09:57:44
# -------------------------------------------------
QT += network \
    xml
# The application version
VERSION = 0.1.1

# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

TARGET = Aria2cRemoteControl
TEMPLATE = app
SOURCES += main.cpp \
    aria2cremote.cpp \
    tabwidget.cpp \
    listview.cpp \
    mainlistview.cpp \
    adduri.cpp \
    add_http_ftp_magnetlink.cpp \
    selecttorrentfiles.cpp \
    bencodeparser.cpp \
    metainfo.cpp \
    util.cpp \
    xmlrpc.cpp \
    xmlrpc/variant.cpp \
    xmlrpc/response.cpp \
    xmlrpc/request.cpp \
    xmlrpc/client.cpp \
    download.cpp \
    globaloptions.cpp \
    localoptions.cpp \
    transfer.cpp \
    addserver.cpp \
    server.cpp \
    aria2options.cpp \
    qtreewidgetitemex.cpp \
    statusbarex.cpp \
    about.cpp \
    gzip/zutil.c \
    gzip/trees.c \
    gzip/deflate.c \
    gzip/compress.c \
    progressbarviewdelegate.cpp \
    peerinfo.cpp \
    geoip.cpp \
    torrentclient.cpp \
    downloadprogressdialog.cpp \
    comboboxitem.cpp \
    templates.cpp \
    checkboxitem.cpp \
    lineedititem.cpp \
    spinboxitem.cpp \
    newtemplate.cpp \
    doublespinboxitem.cpp \
    requestthread.cpp
HEADERS += aria2cremote.h \
    tabwidget.h \
    listview.h \
    mainlistview.h \
    adduri.h \
    add_http_ftp_magnetlink.h \
    selecttorrentfiles.h \
    bencodeparser.h \
    metainfo.h \
    util.h \
    xmlrpc.h \
    xmlrpc/variant.h \
    xmlrpc/response.h \
    xmlrpc/request.h \
    xmlrpc/client.h \
    download.h \
    globaloptions.h \
    localoptions.h \
    transfer.h \
    addserver.h \
    server.h \
    aria2options.h \
    bargraph.h \
    qtreewidgetitemex.h \
    statusbarex.h \
    about.h \
    gzip/zlib.h \
    gzip/zconf.h \
    gzip/trees.h \
    gzip/deflate.h \
    gzip/zutil.h \
    progressbarviewdelegate.h \
    peerinfo.h \
    geoip.h \
    torrentclient.h \
    downloadprogressdialog.h \
    comboboxitem.h \
    templates.h \
    checkboxitem.h \
    lineedititem.h \
    spinboxitem.h \
    newtemplate.h \
    doublespinboxitem.h \
    features.h \
    aria2cparameter.h \
    requestthread.h
FORMS += aria2cremote.ui \
    tabwidget.ui \
    listview.ui \
    mainlistview.ui \
    adduri.ui \
    add_http_ftp_magnetlink.ui \
    selecttorrentfiles.ui \
    globaloptions.ui \
    localoptions.ui \
    transfer.ui \
    addserver.ui \
    aria2options.ui \
    about.ui \
    templates.ui \
    newtemplate.ui
RESOURCES += Aria2cRemote.qrc

TRANSLATIONS = translations/Aria2cRemoteControl_en.ts \
               translations/Aria2cRemoteControl_hu.ts 

win32 {
    LIBS +=  libole32 libcomctl32

    SOURCES += windows/windows7.cpp

    HEADERS += windows/winutils.h \
        windows/windows7.h

    RC_FILE = Aria2cRemoteControl.rc
    #DEFINES += BUILDTIME=\\\"$$system('echo %time%')\\\"
    #DEFINES += BUILDDATE=\\\"$$system('echo %date%')\\\"
} else {
    DEFINES += BUILDTIME=\\\"$$system(date '+%H:%M.%s')\\\"
    DEFINES += BUILDDATE=\\\"$$system(date '+%d/%m/%y')\\\"
}



















































