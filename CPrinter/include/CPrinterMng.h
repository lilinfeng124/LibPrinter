#ifndef CPRINTERMNG_H
#define CPRINTERMNG_H
#include <QObject>
#include <QPainter>
#include "CLibPrinter.h"
class CComTradeFile;
class CPrinterObject;
class  CPrinterMng:public QObject
{
	Q_OBJECT
public:
	CPrinterMng(QObject* =NULL);


	CPrinterObject* CreateObject(int tp);

	void DoPrint(QPainter* painter);
	//打印报告
	void PrintReport(QPainter * painter);
	//打印波形
	void PrintWave(QPainter * painter);
	//打印当前状态
	void PrintCurrentStatus(QPainter * painter);
	//打印

	void SetComTrade(CComTradeFile*pFile);
	//
	void SetPrinter(QPrinter*printer);
	//
	void AddToLst(CPrinterObject*pObj);

	void ClearLst();

    QStringList GetAnalogChannelList();

    QStringList GetDigitalChannelList();

    void TestGenericTable();


private:
	CComTradeFile * m_pComtradeFile;
	QPrinter      * m_pPrinter;

	QList<CPrinterObject*>m_pObjLst;
};
#endif
