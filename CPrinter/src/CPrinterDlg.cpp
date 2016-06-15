#include "CPrinterDlg.h"
#include "CPrinterObject.h"
#include "ui_PrinterDlg.h"
#include "CPrinterMng.h"
#include "CWavePrinter.h"
#include <QPrintPreviewDialog>
#include <QPrintDialog>
#include <stdio.h>
CPrinterDlg::CPrinterDlg(QWidget*pWidget):QDialog(pWidget)
{
	m_pUi = new Ui_printDlg;
	m_pUi->setupUi(this);

	m_printer.setPaperSize(QPrinter::A4);
	m_type = SETTINGVALUE;
	m_pPrinterMng = new CPrinterMng;
	m_pPrinterMng->SetPrinter(&m_printer);
	connect(m_pUi->m_btnPreView,SIGNAL(clicked()),this,SLOT(PreViewSlot()));
	connect(m_pUi->m_btnPrint,SIGNAL(clicked()),this,SLOT(PrinterSlot()));

    connect(m_pUi->m_chkSelectAllAnalog,SIGNAL(toggled(bool)),this,SLOT(SetSelectAllAnalogSlot(bool)));
    connect(m_pUi->m_chkUnSelectAllAnalog,SIGNAL(toggled(bool)),this,SLOT(SetUnSelectAllAnalogSlot(bool)));
    connect(m_pUi->m_chkSelectAllDigital,SIGNAL(toggled(bool)),this,SLOT(SetSelectAllDigitalSlot(bool)));
    connect(m_pUi->m_chkUnSelectAllDigital,SIGNAL(toggled(bool)),this,SLOT(SetUnSelectAllDigitalSlot(bool)));


	//connect(m_pUi->pu);
	
}

CPrinterDlg::~CPrinterDlg()
{

}

void CPrinterDlg::PrinterSlot()
{
	QPrintDialog printDialog(&m_printer,this);
	if(printDialog.exec())
	{
		DoPrint(&m_printer);
	}
}

void CPrinterDlg::PreViewSlot()
{
	QPrintPreviewDialog dialog(&m_printer,this);
	connect(&dialog,SIGNAL(paintRequested(QPrinter*)),this,SLOT(DoPrint(QPrinter*)));
	dialog.exec();
}

void CPrinterDlg::CreateObjLst()
{
	//
	int nLeft = m_pUi->m_cmbLeftWidth->currentText().toInt();
	int nRight = m_pUi->m_cmbRightWidth->currentText().toInt();
	int nTop = m_pUi->m_cmbTopWidth->currentText().toInt();
	int nBottom = m_pUi->m_cmbBottomWidth->currentText().toInt();
	printf("nLeft=%d nRight=%d nTop=%d nBottom=%d\n",nLeft,nRight,nTop,nBottom);	
	//先清除了原来的打印要求
	m_pPrinterMng->ClearLst();
	//根据界面选择生成Object的List
	if(m_pUi->m_ckGroupReport->isChecked())
	{
		//整组
		CPrinterObject*pObj = m_pPrinterMng->CreateObject(CPrinterDlg::GROUPREPORT);
		pObj->SetLeftMargin(nLeft);
		pObj->SetRightMargin(nRight);
		pObj->SetTopMargin(nTop);
		pObj->SetBottomMargin(nBottom);
		m_pPrinterMng->AddToLst(pObj);
	}
	//告警
	if(m_pUi->m_ckStatus->isChecked())
	{
		//
		CPrinterObject* pObj = m_pPrinterMng->CreateObject(CPrinterDlg::WARNINGSTATUS);
		pObj->SetLeftMargin(nLeft);
		pObj->SetRightMargin(nRight);
		pObj->SetTopMargin(nTop);
		pObj->SetBottomMargin(nBottom);
		m_pPrinterMng->AddToLst(pObj);
	}
	if(m_pUi->m_ckSettingValue->isChecked())
	{
		CPrinterObject* pObj = m_pPrinterMng->CreateObject(CPrinterDlg::SETTINGVALUE);
		pObj->SetLeftMargin(nLeft);
		pObj->SetRightMargin(nRight);
		pObj->SetTopMargin(nTop);
		pObj->SetBottomMargin(nBottom);
		m_pPrinterMng->AddToLst(pObj);
	}
	//波形
    if(m_pUi->m_ckPrintAnalog->isChecked()||m_pUi->m_ckPrintDigital->isChecked())
    {
        CPrinterObject * pObj = m_pPrinterMng->CreateObject(CPrinterDlg::WAVE);
        QList<int> DigitlLst = GetPrintDigitalChannelLst();
        QList<int> AnalogLst = GetPrintAnalogChannelLst();
        CWavePrinter* pWave = (CWavePrinter*)pObj;
        pWave->SetDigitalChannel(DigitlLst);
        pWave->SetAnalogChannel(AnalogLst);
        pWave->SetLeftMargin(nLeft);
        pWave->SetRightMargin(nRight);
        pWave->SetTopMargin(nTop);
        pWave->SetBottomMargin(nBottom);
        m_pPrinterMng->AddToLst(pObj);
    }
    //test word
    m_pPrinterMng->TestGenericTable();
    printf("set over\n");
}

void CPrinterDlg::DoPrint(QPrinter*p)
{
	Q_UNUSED(p);
 	QPainter painter(&m_printer);
// 	CPrinterObject*pObj = m_pPrinterMng->CreateObject(m_type);
// 	if(pObj)
// 	{
// 		pObj->DoPrinter(&painter);
// 	}
	//产生打印object
	CreateObjLst();
	//开始打印
	m_pPrinterMng->DoPrint(&painter);

    m_pPrinterMng->ClearLst();
}



void CPrinterDlg::SetComTrade(CComTradeFile*pFile)
{
	m_pPrinterMng->SetComTrade(pFile);
    initGui();
}

void CPrinterDlg::initGui()
{
    QStringList AnalogLst =m_pPrinterMng->GetAnalogChannelList();
    foreach(QString strAnalog,AnalogLst)
    {
        QListWidgetItem* pItm = new QListWidgetItem(m_pUi->m_listAnalog,Qt::ItemIsUserCheckable);
        pItm->setCheckState(Qt::Unchecked);
        pItm->setText(strAnalog);
        m_pUi->m_listAnalog->addItem(pItm);
    }
    QStringList DigitalLst = m_pPrinterMng->GetDigitalChannelList();
    foreach(QString strDigital,DigitalLst)
    {
        QListWidgetItem* pItm = new QListWidgetItem(m_pUi->m_listDigital,Qt::ItemIsUserCheckable);
        pItm->setCheckState(Qt::Unchecked);
        pItm->setText(strDigital);
        m_pUi->m_listDigital->addItem(pItm);
    }
}

QList<int>CPrinterDlg::GetPrintDigitalChannelLst()
{
    QList<int> DigtlChlLst;

    //QList<QListWidgetItem*> ItmLst = m_pUi->m_listDigital->Items();
    for(int nIndex=0;nIndex<m_pUi->m_listDigital->count();nIndex++)
    {
        QListWidgetItem* pItm = m_pUi->m_listDigital->item(nIndex);
        if(pItm->checkState()==Qt::Checked)
        {
            QString str = pItm->text();
            QStringList strLst =str.split(":");
            if(strLst.count()>=2)
            {
                QString strNum = strLst.at(0);
                int nNum = strNum.toInt()-1;
                DigtlChlLst.append(nNum);
            }
        }
    }
    return DigtlChlLst;
}

QList<int>CPrinterDlg::GetPrintAnalogChannelLst()
{
    QList<int> AnalgChlLst;

    //QList<QListWidgetItem*> ItmLst = m_pUi->m_listAnalog->Items();
    for(int nIndex=0;nIndex<m_pUi->m_listAnalog->count();nIndex++)
    {
        QListWidgetItem* pItm = m_pUi->m_listAnalog->item(nIndex);
        if(pItm->checkState()==Qt::Checked)
        {
            QString str = pItm->text();
            QStringList strLst =str.split(":");
            if(strLst.count()>=2)
            {
                QString strNum = strLst.at(0);
                int nNum = strNum.toInt()-1;
                AnalgChlLst.append(nNum);
            }
        }
    }
    return AnalgChlLst;
}

void CPrinterDlg::SetSelectAllDigitalSlot(bool b)
{
    if(b)
    {
        for(int nIndex=0;nIndex<m_pUi->m_listDigital->count();nIndex++)
        {
             QListWidgetItem* pItm = m_pUi->m_listDigital->item(nIndex);
             pItm->setCheckState(Qt::Checked);
        }

        m_pUi->m_chkUnSelectAllDigital->setChecked(false);
    }
}

void CPrinterDlg::SetSelectAllAnalogSlot(bool b)
{
    if(b)
    {
        for(int nIndex=0;nIndex<m_pUi->m_listAnalog->count();nIndex++)
        {
             QListWidgetItem* pItm = m_pUi->m_listAnalog->item(nIndex);
             pItm->setCheckState(Qt::Checked);
        }

        m_pUi->m_chkUnSelectAllAnalog->setChecked(false);
    }
}

void CPrinterDlg::SetUnSelectAllDigitalSlot(bool b)
{
    if(b)
    {
        for(int nIndex=0;nIndex<m_pUi->m_listDigital->count();nIndex++)
        {
             QListWidgetItem* pItm = m_pUi->m_listDigital->item(nIndex);
             pItm->setCheckState(Qt::Unchecked);
        }

        m_pUi->m_chkSelectAllDigital->setChecked(false);
    }
}

void CPrinterDlg::SetUnSelectAllAnalogSlot(bool b)
{
    if(b)
    {
        for(int nIndex=0;nIndex<m_pUi->m_listAnalog->count();nIndex++)
        {
             QListWidgetItem* pItm = m_pUi->m_listAnalog->item(nIndex);
             pItm->setCheckState(Qt::Unchecked);
        }

        m_pUi->m_chkSelectAllAnalog->setChecked(false);
    }
}
