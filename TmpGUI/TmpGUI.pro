TEMPLATE = app
CONFIG += QT debug_and_release
DEPENDPATH += .

LIBS += 	-lCPrinter\
                -lComTrade
				
INCLUDEPATH += . \
               ./include\
                ../ComTrade/include \
               ../CPrinter/include

#DEFINES +=  -GX Visual_Program

MOC_DIR = 		../temp/TmpGUI/moc/ 
UI_DIR = 			../temp/TmpGUI/uic/ 
OBJECTS_DIR = ../temp/TmpGUI/obj/ 

QMAKE_LIBDIR += ../lib

TARGET = TmpGUI
DESTDIR = ../lib

CONFIG += debug \
	   warn_on \
          qt \
          x11 \
          exceptions \
          stl 
        

HEADERS	=   ./include/CTmpGUI.h 
                                         
SOURCES	=   ./src/main.cpp \
        	./src/CTmpGUI.cpp
						
FORMS += ./ui/TmpGUI.ui		
							

       
#  CODECFORTR = gbk

  QT+=  qt3support 
QT += xml                  
win32-msvc* {
	QMAKE_CFLAGS -= -Zc:wchar_t-

}


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../lib/release/ -lComTrade
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib/debug/ -lComTrade
else:unix: LIBS += -L$$PWD/../lib/ -lComTrade

INCLUDEPATH += $$PWD/../lib
DEPENDPATH += $$PWD/../lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../lib/release/ -lCPrinter
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib/debug/ -lCPrinter
else:unix: LIBS += -L$$PWD/../lib/ -lCPrinter

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../
