QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    New_file_Dialog.cpp \
    Python_Editer.cpp \
    Python_SyntaxHighLighter.cpp \
    TabPage.cpp \
    main.cpp \
    DarkCode.cpp

HEADERS += \
    DarkCode.h \
    DarkCode_include.h \
    LineNumberArea.h \
    New_file_Dialog.h \
    Python_Editer.h \
    Python_SyntaxHighLighter.h \
    TabPage.h

FORMS += \
    DarkCode.ui \
    New_file_Dialog.ui \
    TabPage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
