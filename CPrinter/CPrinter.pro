TEMPLATE = lib
CONFIG += QT \
          debug_and_release
DEPENDPATH += .

LIBS += -lComTrade 
				
INCLUDEPATH += . \
               ./include\
               ../ComTrade/include 

TARGET = CPrinter
DESTDIR = ../lib 

DLLDESTDIR = ../bin
BINLIB = ../bin

QMAKE_LIBDIR += ../lib \


HEADERS	=   ./include/CPrinterDlg.h \
                 ./include/CPrinterMng.h \
                ./include/CReportPrinter.h \
                ./include/CPrinterObject.h \
                ./include/CSettingValuePrinter.h \
                ./include/CDigitalStatusPrinter.h \
                ./include/CGroupInfoPrinter.h \
                ./include/CWavePrinter.h \
                ./include/CLibPrinter.h \
                ./include/CGenericTable.h

                                         
SOURCES	=   ./src/CPrinterDlg.cpp \
                 ./src/CPrinterMng.cpp \
                 ./src/CReportPrinter.cpp \
                 ./src/CPrinterObject.cpp \
                 ./src/CSettingValuePrinter.cpp \
                 ./src/CDigitalStatusPrinter.cpp \
                 ./src/CGroupInfoPrinter.cpp \
                 ./src/CWavePrinter.cpp \
		 ./src/CGenericTable.cpp

						
FORMS += 		./ui/PrinterDlg.ui
							
DEFINES += LIBPRINTER_EXPORTS
						

 									

       
  CODECFORTR = gbk 

  QT+=  qt3support xml
                 


