#ifndef CPRINTERDLG_H
#define CPRINTERDLG_H
#include <QPrinter>
#include <QDialog>
#include <QWidget>
#include <QObject>
#include "CLibPrinter.h"
//#include "ui_PrinterDlg.h"
class Ui_printDlg;
class CComTradeFile;
class CPrinterMng;
class CPrinterDlg:public QDialog
{
	Q_OBJECT
public:
	enum PRINTTYPE
	{
		SETTINGVALUE=0,
		GROUPREPORT=1,
		WARNINGSTATUS=2,
		WAVE=3,
	};
	CPrinterDlg(QWidget*pWidget=NULL);
	~CPrinterDlg();

	void SetComTrade(CComTradeFile*pFile);

    void CreateObjLst();

    void initGui();

  private:

    QList<int>GetPrintDigitalChannelLst();

    QList<int>GetPrintAnalogChannelLst();

public slots:
	void PrinterSlot();
	void DoPrint(QPrinter*p);
	void PreViewSlot();

    void SetSelectAllDigitalSlot(bool);
    void SetUnSelectAllDigitalSlot(bool);
    void SetSelectAllAnalogSlot(bool);
    void SetUnSelectAllAnalogSlot(bool);
private:
	Ui_printDlg * m_pUi;
	QPrinter      m_printer;
	PRINTTYPE     m_type;
	CPrinterMng * m_pPrinterMng;
};

#endif 
