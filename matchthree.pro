QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
TARGET = "Match Three"

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
	src/game/gamestate.cpp \
	src/game/piece.cpp \
	src/main.cpp \
	src/ui/anchor.cpp \
	src/ui/mainwindow.cpp \
	src/ui/piecewidget.cpp

HEADERS += \
	src/game/gamestate.h \
	src/game/piece.h \
	src/ui/anchor.h \
	src/ui/mainwindow.h \
	src/ui/piecewidget.h

INCLUDEPATH += src/

RESOURCES += resources/resources.qrc

RC_ICONS = resources/art/icon.ico

# USER CONFIG
CONFIG(debug, debug|release) {
	BUILDMODE = debug
} else {
	BUILDMODE = release
}

OBJECTS_DIR = ./$${BUILDMODE}-tmp/obj/
MOC_DIR = ./$${BUILDMODE}-tmp/moc/
RCC_DIR = ./$${BUILDMODE}-tmp/rcc/
UI_DIR = ./$${BUILDMODE}-tmp/ui/

# Windows deployment
CONFIG(nowindeploy) {
} else {
	QMAKE_POST_LINK = windeployqt --no-patchqt --no-translations --no-system-d3d-compiler --no-virtualkeyboard --no-webkit2 --no-angle --no-opengl-sw $${BUILDMODE}
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
