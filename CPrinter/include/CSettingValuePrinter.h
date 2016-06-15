#ifndef CSETTINGVALUEPRINTER_H
#define CSETTINGVALUEPRINTER_H
#include "CLibPrinter.h"
#include "CPrinterObject.h"
#include <QFont>
#include <QString>
#include <QRectF>
class QPrinter;
class CSettingValuePrinter:public CPrinterObject
{
public:
	CSettingValuePrinter(QPrinter*printer,CComTradeFile*pObj);
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
    //int     m_nTableColum;
	QFont   m_font;
    int     m_nCurrentIndex;
};

#endif
