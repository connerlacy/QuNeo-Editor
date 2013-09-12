#-------------------------------------------------
#
# Project created by QtCreator 2012-01-24T15:21:06
#
#-------------------------------------------------

QT       += core gui

TARGET = "QuNeo Editor"
TEMPLATE = app

INCLUDEPATH += ./ui/buttons\
                ./ui/editPanes\
                ./data \
                ./midi \
                ./ui/labels\

SOURCES += main.cpp\
        mainwindow.cpp \
    ui/buttons/padbutton.cpp \
    data/presethandler.cpp \
    ui/buttons/vsliderbutton.cpp \
    ui/buttons/rotarybutton.cpp \
    ui/buttons/hsliderbutton.cpp \
    ui/buttons/lsliderbutton.cpp \
    ui/buttons/buttonbutton.cpp \
    ui/editPanes/padeditpane.cpp \
    ui/editPanes/rotaryeditpane.cpp \
    ui/editPanes/hslidereditpane.cpp \
    ui/editPanes/vslidereditpane.cpp \
    ui/editPanes/lslidereditpane.cpp \
    ui/editPanes/transporteditpane.cpp \
    ui/editPanes/leftrighteditpane.cpp \
    ui/editPanes/rhombuseditpane.cpp \
    ui/editPanes/updowneditpane.cpp \
    ui/editPanes/modeeditpane.cpp \
    ui/labels/padlabels.cpp \
    ui/labels/hsliderlabels.cpp \
    ui/labels/vsliderlabels.cpp \
    ui/labels/lsliderlabels.cpp \
    ui/labels/rotarylabels.cpp \
    ui/editPanes/globalparameters.cpp \
    ui/labels/updateindicator.cpp \
    ui/labels/updownlabels.cpp \
    ui/labels/transportlabels.cpp \
    ui/labels/rhombuslabels.cpp \
    ui/labels/modelabels.cpp \
    ui/labels/leftrightlabels.cpp \
    midi/sysexformat.cpp \
    data/copypastehandler.cpp \
    data/datavalidator.cpp


HEADERS  += mainwindow.h \
    ui/buttons/padbutton.h \
    data/presethandler.h \
    ui/buttons/vsliderbutton.h \
    ui/buttons/rotarybutton.h \
    ui/buttons/hsliderbutton.h \
    ui/buttons/lsliderbutton.h \
    ui/buttons/buttonbutton.h \
    ui/editPanes/padeditpane.h \
    ui/editPanes/rotaryeditpane.h \
    ui/editPanes/hslidereditpane.h \
    ui/editPanes/vslidereditpane.h \
    ui/editPanes/lslidereditpane.h \
    ui/editPanes/transporteditpane.h \
    ui/editPanes/leftrighteditpane.h \
    ui/editPanes/rhombuseditpane.h \
    ui/editPanes/updowneditpane.h \
    ui/editPanes/modeeditpane.h \
    ui/labels/padlabels.h \
    ui/labels/hsliderlabels.h \
    ui/labels/vsliderlabels.h \
    ui/labels/lsliderlabels.h \
    ui/labels/rotarylabels.h \
    ui/editPanes/globalparameters.h \
    ui/labels/updateindicator.h \
    ui/labels/updownlabels.h \
    ui/labels/transportlabels.h \
    ui/labels/rhombuslabels.h \
    ui/labels/modelabels.h \
    ui/labels/leftrightlabels.h \
    midi/sysexformat.h \
    data/copypastehandler.h \
    data/datavalidator.h


FORMS    += mainwindow.ui

RESOURCES += \
    Resources.qrc \

#------QJson-------#
static{
DEFINES += STATIC_BUILD
}

QJSON_SRCBASE = $$PWD/qjson/src

QJSON_CPP = $$QJSON_SRCBASE
INCLUDEPATH += $$QJSON_CPP


SOURCES += \
  $$QJSON_CPP/json_parser.cc \
  $$QJSON_CPP/json_scanner.cpp \
  $$QJSON_CPP/parser.cpp \
   $$QJSON_CPP/qobjecthelper.cpp \
  $$QJSON_CPP/serializer.cpp \

HEADERS += \

#------OS Specific MIDI-------#
win32{
INCLUDEPATH += ./midi/windows
HEADERS += midi/windows/midideviceaccess.h
SOURCES  += midi/windows/midideviceaccess.cpp
LIBS += -lwinmm
}

macx {
INCLUDEPATH += ./midi/mac
HEADERS += midi/mac/midideviceaccess.h
SOURCES  += midi/mac/midideviceaccess.cpp
LIBS += -framework CoreMIDI
LIBS += -framework CoreFoundation
LIBS += -framework Cocoa
LIBS += -framework CoreServices
}

unix:!macx{
INCLUDEPATH += ./midi/alsa
HEADERS += midi/alsa/midideviceaccess.h midi/alsa/midioutworker.h midi/alsa/midiinworker.h
SOURCES  += midi/alsa/midideviceaccess.cpp midi/alsa/midioutworker.cpp midi/alsa/midiintworker.cpp
LIBS += -lasound 
}


#------OS Specific ICONS-------#

win32{
OTHER_FILES += \
    quneoIcon.rc.txt
RC_FILE = quneoIcon.rc.txt
}

macx{
ICON = darth_quneo_icon.icns
}

unix{
ICON = darth_quneo_icon.icns
}

OTHER_FILES += \
    darth_quneo_icon.ico \
    darth_quneo_icon.icns \
    quneoIcon.txt
