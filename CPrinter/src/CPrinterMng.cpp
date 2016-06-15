#include "CPrinterMng.h"
#include "CPrinterObject.h"
#include "CSettingValuePrinter.h"
#include "CPrinterDlg.h"
#include "CGroupInfoPrinter.h"
#include "CDigitalStatusPrinter.h"
#include "CGenericTable.h"
#include "CWavePrinter.h"
#include "CComTradeFile.h"
CPrinterMng::CPrinterMng(QObject*pParent)
{
	m_pComtradeFile = NULL;
}


void CPrinterMng::SetComTrade(CComTradeFile*pFile)
{
	m_pComtradeFile = pFile;
}

void CPrinterMng::SetPrinter(QPrinter*printer)
{
	m_pPrinter = printer;
}

CPrinterObject* CPrinterMng::CreateObject(int nTp)
{
	CPrinterObject *pObj = NULL;
	CPrinterDlg::PRINTTYPE tp = (CPrinterDlg::PRINTTYPE)nTp;
	switch (tp)
	{
	case CPrinterDlg::SETTINGVALUE:
		{
			pObj = new CSettingValuePrinter(m_pPrinter,m_pComtradeFile);
		}
		break;
	case CPrinterDlg::GROUPREPORT:
		{
			pObj = new CGroupInfoPrinter(m_pPrinter,m_pComtradeFile);
		}
		break;

	case CPrinterDlg::WARNINGSTATUS:
		{
			pObj = new CDigitalStatusPrinter(m_pPrinter,m_pComtradeFile);
		}
		break;
	case  CPrinterDlg::WAVE:
		{
			pObj = new CWavePrinter(m_pPrinter,m_pComtradeFile);
		}
		break;
	default:
		return NULL;
	}
	return pObj;
}

void CPrinterMng::DoPrint(QPainter*painter)
{
	printf("start doPrint\n");
	for(int nIndex=0;nIndex<m_pObjLst.count();nIndex++)
	{
        printf("nIndex=%d\n",nIndex);
		CPrinterObject*pObj = m_pObjLst.at(nIndex);
        /*if(nIndex>0)
		{
			m_pPrinter->newPage();
        }*/
        printf("before doPrinter %d\n",nIndex);
		pObj->DoPrinter(painter);
        printf("after doPrinter %d\n",nIndex);
	}
	
}

void CPrinterMng::AddToLst(CPrinterObject*pObj)
{
	if(pObj&&!m_pObjLst.contains(pObj))
	{
		m_pObjLst.append(pObj);
	}
}

void CPrinterMng::ClearLst()
{
	qDeleteAll(m_pObjLst);
	m_pObjLst.clear();
	//m_pObjLst.clear();
}

QStringList CPrinterMng::GetAnalogChannelList()
{
    QStringList lst;
    if(m_pComtradeFile)
    {
        for(int nIndex=0;nIndex<m_pComtradeFile->GetACount();nIndex++)
        {
            int nCh = nIndex;
            //CAChanelInfo AChnlInfo = m_pComTrade->GetAInfoByIndex(nIndex+pPg->m_nStartChannel);
            QString szName = QString("%1:%2[%3] ").arg(nCh+1)
                .arg(m_pComtradeFile->GetAInfoByIndex(nCh).m_strCh_id )
                .arg(m_pComtradeFile->GetAInfoByIndex(nCh).m_strUu);
            lst.append(szName);
        }

    }
    return lst;
}
QStringList CPrinterMng::GetDigitalChannelList()
{

    QStringList lst;
    if(m_pComtradeFile)
    {
        for(int nIndex=0;nIndex<m_pComtradeFile->GetDCount();nIndex++)
        {
            int nCh = nIndex;
            //CAChanelInfo AChnlInfo = m_pComTrade->GetAInfoByIndex(nIndex+pPg->m_nStartChannel);
            QString szName = QString("%1:%2[%3] ").arg(nCh+1)
                .arg(m_pComtradeFile->GetDInfoByIndex(nCh).m_strCh_id );
            lst.append(szName);
        }

    }
    return lst;
}

void CPrinterMng::TestGenericTable()
{

    CGenericTable * pTbl = new CGenericTable(m_pPrinter);
    QString strTitle = QString("定值打印表");
    pTbl->SetTitle(strTitle);
    QString strDesc = QString("这是定制打印专用的表格");
    pTbl->SetDetailDesc(strDesc);
    QStringList lst;
    lst<<QString("表头kkkkkkkkkk")<<QString("表头wwwwwwwwwww")<<QString("表头3");
    pTbl->SetTableHeader(lst);
    TableNode* p1= new TableNode;

    p1->m_strName = "Root1";
    TableNode * Mid1 = p1->CreateSon(QString("Mid1"));
    pTbl->AddToNodeLst(p1);
    TableNode * Bottom1 = Mid1->CreateSon(QString("Son1"),QString("1110ms"));
    TableNode * Bottom2 = Mid1->CreateSon(QString("Son2"),QString("1111ms"));

    TableNode * Mid2 = p1->CreateSon(QString("Mid2"));
    TableNode * Bottom3 = Mid2->CreateSon(QString("Son3"),QString("1112ms"));
    TableNode * Bottom4 = Mid2->CreateSon(QString("Son4"),QString("1113ms"));

    for(int nCount=0;nCount<100;nCount++)
    {
        TableNode*parnt = new TableNode;
        int nMidNum = qrand()%5;
        QString strparntName = QString("Parent%1").arg(nMidNum);
        parnt->m_strName = strparntName;
        pTbl->AddToNodeLst(parnt);
        for(int n=0;n<nMidNum;n++)
        {
            QString strName =  QString("Mid%1").arg(qrand()%100);
            TableNode * p = parnt->CreateSon(strName);
            int nBottomNum = qrand()%10;
            for(int m=0;m<nBottomNum;m++)
            {
                QString strSon = QString("Son%1").arg(qrand()%100);
                QString strSonVl = strSon+QString("k");
                TableNode * pSon = p->CreateSon(strSon,strSonVl);
            }

        }
    }

    AddToLst(pTbl);

}

