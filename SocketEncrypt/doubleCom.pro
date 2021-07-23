QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Affine_Cipher.cpp \
    AutoKey_ciphertext.cpp \
    Autokey_plaintext.cpp \
    CA.cpp \
    DH.cpp \
    MD5.cpp \
    Playfair.cpp \
    RC4.cpp \
    RSA.cpp \
    VigenereCipher.cpp \
    caesar.cpp \
    client.cpp \
    cryptdlg.cpp \
    des.cpp \
    dialog.cpp \
    dialogfile.cpp \
    double_transposition.cpp \
    exchangedh.cpp \
    fileclient.cpp \
    keyword.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    my.cpp \
    stdafx.cpp \
    tool.cpp

HEADERS += \
    Affine_Cipher.h \
    AutoKey_ciphertext.h \
    AutoKey_plaintext.h \
    CA.h \
    DH.h \
    Key.h \
    MD5.h \
    Playfair.h \
    RC4.h \
    RSA.h \
    VigenereCipher.h \
    caesar.h \
    client.h \
    cryptdlg.h \
    des.h \
    dialog.h \
    dialogfile.h \
    double_transprotion.h \
    exchangedh.h \
    fileclient.h \
    keyword.h \
    login.h \
    mainwindow.h \
    my.h \
    stdafx.h \
    tool.h

FORMS += \
    client.ui \
    cryptdlg.ui \
    dialog.ui \
    dialogfile.ui \
    exchangedh.ui \
    fileclient.ui \
    login.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
