# qmake project generated by QMsDev
#
# General settings

TEMPLATE = app
CONFIG  += qt warn_off
TARGET  += beid-pkcs11-tool

mac: DEFINES += __APPLE__

INCLUDEPATH += ../../ ../include

LIBS += -lcrypto -lssl -L../libopensc -lbeidlibopenscinternal

# Input

SOURCES = \ 
	util.c \
	libpkcs11.c \
	pkcs11-tool.c

HEADERS = \
	pkcs11.h \
	util.h
