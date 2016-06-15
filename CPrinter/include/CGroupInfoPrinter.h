#ifndef CGROUPSTATUSPRINTER_H
#define CGROUPSTATUSPRINTER_H
#include "CLibPrinter.h"
#include "CPrinterObject.h"
#include <QFont>
#include <QRectF>
class QPrinter;

struct MyTripInfo
{
public:
	unsigned int	m_nTime;
	QString			m_strName;
	QString			m_strPhase;
	unsigned short	m_nValue;

	QString m_strFaultName;
	QString m_strFaultValue;
};

class  CGroupInfoPrinter:public CPrinterObject
{
public:
	CGroupInfoPrinter(QPrinter*printer,CComTradeFile*pObj);
	~CGroupInfoPrinter();
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
	QList<MyTripInfo*>m_infoLst;
    int     m_nCurrrentIndex;
};

#endif
