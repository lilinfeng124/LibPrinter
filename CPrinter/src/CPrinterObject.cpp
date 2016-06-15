#include "CPrinterObject.h"
#include <QPainter>
#include <QPrinter>
CPrinterObject::CPrinterObject(QPrinter*printer,CComTradeFile*pObj)
:m_pPrinter(printer),
m_pComTrade(pObj)
{
	m_nLeftMargin = 0;
        m_nRightMargin = 0;
	m_nTopMargin = 0;
	m_nBottomMargin = 0;

}

void CPrinterObject::DoPrinter(QPainter*p)
{
	
}

void CPrinterObject::SetLeftMargin(int n)
{
	m_nLeftMargin = n;
}

void CPrinterObject::SetRightMargin(int n)
{
	m_nRightMargin = n;
}

void CPrinterObject::SetTopMargin(int n)
{
	m_nTopMargin = n;
}

void CPrinterObject::SetBottomMargin(int n)
{
	m_nBottomMargin = n;
}

