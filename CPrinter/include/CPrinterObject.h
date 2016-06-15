#ifndef CPRINTEROBJECT_H
#define CPRINTEROBJECT_H
#include <QObject>
#include "CLibPrinter.h"
class CComTradeFile;
class QPainter;
class QPrinter;
class CPrinterObject:public QObject
{
	Q_OBJECT
public:
	CPrinterObject(QPrinter*printr,CComTradeFile*pObj=NULL);
	virtual void DoPrinter(QPainter*p);
	
	void SetLeftMargin(int n);
	void SetRightMargin(int n);
	void SetTopMargin(int n);
	void SetBottomMargin(int n);	
protected:
	CComTradeFile*m_pComTrade;
	QPrinter     *m_pPrinter;
	int           m_nLeftMargin;
	int           m_nRightMargin;
	int           m_nTopMargin;
	int           m_nBottomMargin;
};

#endif 
