#ifndef CDIGITALSTATUSPRINTER_H
#define CDIGITALSTATUSPRINTER_H
#include "CLibPrinter.h"
#include "CPrinterObject.h"
#include <QFont>
#include <QString>
#include <QRectF>
class QPrinter;



class PRINTER_API CDigitalStatusPrinter:public CPrinterObject
{
public:
	CDigitalStatusPrinter(QPrinter*printer,CComTradeFile*pObj);
	void DoPrinter(QPainter*p);

private:

	void PrintTitle(QPainter*p);

	void PrintHeader(QPainter*p);

	void PrintTable(QPainter*p);

    void PrintSinglTable(QPainter*p);

	QRectF CenterInGrid(int left,int right,int y,QString str);

private:
	//QPixmap m_pixMap; 
	int     m_nCurrentY;
	int     m_nTableColum;	
	QFont   m_font;
    int     m_nCurrentIndex;
};

#endif
