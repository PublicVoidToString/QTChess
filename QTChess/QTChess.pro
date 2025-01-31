QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bishop.cpp \
    board.cpp \
    boardgetset.cpp \
    chessboard.cpp \
    engine.cpp \
    evaluation.cpp \
    finalmessage.cpp \
    king.cpp \
    knight.cpp \
    main.cpp \
    mainmenu.cpp \
    pawn.cpp \
    promotionwindow.cpp \
    rook.cpp

HEADERS += \
    bishop.h \
    board.h \
    chessboard.h \
    engine.h \
    evaluation.h \
    finalmessage.h \
    king.h \
    knight.h \
    mainmenu.h \
    pawn.h \
    promotionwindow.h \
    rook.h

FORMS += \
    chessboard.ui \
    finalmessage.ui \
    mainmenu.ui \
    promotionwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Informacje.md

RESOURCES += \
    resources.qrc
