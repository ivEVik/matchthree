QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
	src/game/gamestate.cpp \
	src/main.cpp \
	src/ui/mainwindow.cpp

HEADERS += \
	src/game/cell.h \
	src/game/gamestate.h \
	src/ui/mainwindow.h

INCLUDEPATH += src/

RESOURCES += resources/resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


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

# Deployment
QMAKE_POST_LINK = windeployqt --no-patchqt --no-translations --no-system-d3d-compiler --no-virtualkeyboard --no-compiler-runtime --no-webkit2 --no-angle --no-opengl-sw ./${DESTDIR}/

RESOURCES += \
    resources/resources.qrc
