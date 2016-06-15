TEMPLATE = lib
CONFIG += QT 
DEPENDPATH += .

INCLUDEPATH += . \
               ./include   \

DEFINES += LIBCOMTRADE_EXPORTS

MOC_DIR = 		../temp/ComTrade/moc/ 
UI_DIR = 			../temp/ComTrade/uic/ 
OBJECTS_DIR = ../temp/ComTrade/obj/ 

QMAKE_LIBDIR += ../lib 

TARGET = ComTrade
DESTDIR = ../lib
DLLDESTDIR = ../bin

QT += xml

CONFIG += debug \
          warn_on \
          qt \
          x11 \
          exceptions \
          dll \
          stl 

HEADERS	+=  ./include/LibComTrade.h \
						./include/CComTradeFile.h \
						./include/CCfgFile.h \
						./include/CDatFile.h \
						./include/CHdrFile.h \
						./include/CDesFile.h \
						./include/CASCIIFile.h \
						./include/CBINFile.h \
						./include/CFileStruct.h \
						
											
SOURCES	+=  ./src/CComTradeFile.cpp \
						./src/CCfgFile.cpp \
						./src/CDatFile.cpp \
						./src/CHdrFile.cpp \
						./src/CDesFile.cpp \
						./src/CASCIIFile.cpp \
						./src/CBINFile.cpp \
						./src/CFileStruct.cpp \
						
TRANSLATIONS += ../ts/ComTrade_zh_CN.ts	\	
								../ts/ComTrade_en.ts			
                 						
						
#win32-msvc* {
#	QMAKE_CFLAGS -= -Zc:wchar_t-
#}
