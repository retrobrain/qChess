TEMPLATE = app

QT += qml quick


SOURCES += main.cpp \
    gamemanager.cpp \
    chesspiece.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    gamemanager.h \
    chesspiece.h \
    helpers.h

CONFIG += c++11
