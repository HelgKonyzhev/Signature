TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
		filereader.cpp \
		filewriter.cpp \
		main.cpp \
		workpool.cpp

HEADERS += \
	commons.h \
	filereader.h \
	filewriter.h \
	workpool.h

LIBS += -pthread #-lcrypto
