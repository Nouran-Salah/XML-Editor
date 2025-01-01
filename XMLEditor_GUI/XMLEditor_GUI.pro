QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Compress_Decompress_XML.cpp \
    Formatting_XML.cpp \
    Validate_XML.cpp \
    XML_To_JSON.cpp \
    graph.cpp \
    main.cpp \
    mainwindow.cpp \
    xml_tree.cpp

HEADERS += \
    Compress_Decompress_XML.h \
    Formatting_XML.h \
    Validate_XML.h \
    XML_To_JSON.h \
    graph.h \
    mainwindow.h \
    xml_tree.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
