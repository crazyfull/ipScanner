TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

TARGET = ipScanner.run

LIBS += -llibWrench -pthread -lz

CONFIG(release, debug|release) {
    LIBS += -pthread -static -static-libgcc -static-libstdc++
    LIBS += --static -static
    message( "release static mod" )
} else  {
    message( "debug mod" )
}

DESTDIR=../bin #Target file directory
OBJECTS_DIR=../objs #Intermediate object files directory
MOC_DIR=../moc

SOURCES += \
        Client.cpp \
        main.cpp

HEADERS += \
    Client.h
