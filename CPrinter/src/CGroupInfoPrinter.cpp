#include "CGroupInfoPrinter.h"
#include "CComTradeFile.h"
#include <QPainter>
#include <QRectF>
#include <QPrinter>
const int minStep = 5;

CGroupInfoPrinter::CGroupInfoPrinter(QPrinter*printer,CComTradeFile*pObj):CPrinterObject(printer,pObj)
{
    //m_nTableColum = 45;
	m_font=QFont("Arial",12);
    m_nCurrentY = 0;
    m_nCurrrentIndex = 0;
	//int nCount = 0;
	TripInfoList TripInfoLst = m_pComTrade->GetTripInfoList();
	foreach(CTripInfo info,TripInfoLst)
	{
		FaultInfoList infoLst =info.m_FaultInfoList;
		foreach(CFaultInfo faultInfo,infoLst)
		{
			MyTripInfo * pInfo = new MyTripInfo;
			pInfo->m_nTime = info.m_nTime;
			pInfo->m_strName = info.m_strName;
			pInfo->m_strPhase = info.m_strPhase;
			pInfo->m_nValue = info.m_nValue;

			pInfo->m_strFaultName = faultInfo.m_strName;
			pInfo->m_strFaultValue = faultInfo.m_strValue;
			m_infoLst.append(pInfo);
		}
	}
}

CGroupInfoPrinter::~CGroupInfoPrinter()
{
	qDeleteAll(m_infoLst);
	m_infoLst.clear();
}

void CGroupInfoPrinter::DoPrinter(QPainter*p)
{
    m_pPrinter->newPage();

	PrintTitle(p);

	PrintHeader(p);

	PrintTable(p);
}

void CGroupInfoPrinter::PrintTitle(QPainter*p)
{
	p->setPen(Qt::black);

	p->setFont(m_font);
	QString strDeviceName = m_pComTrade->GetUnitName();
	strDeviceName+=QString("━━━整组报告");
	int textWidth = p->window().width()-m_nLeftMargin-m_nRightMargin;

	QFontMetrics metics(m_font);

	int nTitleWidth = metics.width(strDeviceName);
	int nTitleHeight = metics.height(); 
	//QRectF rect = p->boundingRect(0,0,textWidth,maxHeight,Qt::AlignCenter,strDeviceName);


	QRectF rect((textWidth-nTitleWidth)/2+m_nLeftMargin,m_nTopMargin,(textWidth+nTitleWidth)/2+m_nLeftMargin,nTitleHeight);
	p->drawText(rect,strDeviceName);
	m_nCurrentY = rect.y()+nTitleHeight;
}

void CGroupInfoPrinter::PrintHeader(QPainter*p)
{
	int textWidth = p->window().width()-m_nLeftMargin-m_nRightMargin;
	m_nCurrentY = m_nCurrentY+minStep;
	p->drawLine(m_nLeftMargin,m_nCurrentY,m_nLeftMargin+textWidth,m_nCurrentY);
	m_nCurrentY = m_nCurrentY+minStep*2;
	int nMaxLength = textWidth/3;
	QString strStation = QString("厂站名:")+m_pComTrade->GetSiteName();

	QFontMetrics metics(m_font);


	int nTitleHeight = metics.height(); 

	//QRectF rect(m_nLeftMargin,m_nCurrentY,nMaxLength,nTitleHeight);
	QRectF rect = CenterInGrid(m_nLeftMargin,m_nLeftMargin+nMaxLength,m_nCurrentY,strStation);
	p->drawText(rect,strStation);

	QString strDevice = QString("特征号:")+m_pComTrade->GetUnitName();
	//QRectF rect2(nMaxLength+m_nLeftMargin,m_nCurrentY,nMaxLength,nTitleHeight);
	QRectF rect2 = CenterInGrid(m_nLeftMargin+nMaxLength,m_nLeftMargin+2*nMaxLength,m_nCurrentY,strDevice);
	p->drawText(rect2,strDevice);

	QString strVersion = QString("版本号:")+m_pComTrade->GetRevision();
	//QRectF rect3(nMaxLength*2+m_nLeftMargin,m_nCurrentY,nMaxLength,nTitleHeight);
	QRectF rect3 = CenterInGrid(m_nLeftMargin+2*nMaxLength,m_nLeftMargin+3*nMaxLength,m_nCurrentY,strVersion);
	p->drawText(rect3,strVersion);

	m_nCurrentY=m_nCurrentY+nTitleHeight+minStep;
}

void CGroupInfoPrinter::PrintSinglTable(QPainter*p)
{
    int textWidth = p->window().width();//-2*minStep-m_nLeftMargin-m_nRightMargin;
    int textHeight = p->window().height();//-2*minStep-m_nTopMargin-m_nBottomMargin;

    int nActualWidth = textWidth-m_nLeftMargin-m_nRightMargin;
    int nActualHeight = textHeight-m_nCurrentY-m_nBottomMargin;

	QFontMetrics metics(m_font);
	QString strXuhao = QString("序号");
	QString strStartTime = QString("启动时间");//                                     :    实际值");
	QString strMaohao = QString(":");
	QString strXiangdui = QString("相对时间");
	QString strPhase = QString("动作相别");
	QString strComponent = QString("动作元件");
	QString strFaultStartTime = m_pComTrade->GetFaultStartTime().toString("yyyy-MM-dd hh:mm:ss");
	//int nDescWidth = metics.width(strDesc);
	int nDescHeight = metics.height(); 


	/*m_nCurrentY*/

    //int nActualHeight = textHeight-m_nCurrentY;
	int nTableCoumn = nActualHeight/(nDescHeight+minStep)-1;

    int m_nTableColum = nTableCoumn;

	nActualHeight = (nTableCoumn+1)*(nDescHeight+minStep);
	//上边线
    p->drawLine(m_nLeftMargin,m_nCurrentY,nActualWidth+m_nLeftMargin,m_nCurrentY);
	//左边线
	p->drawLine(m_nLeftMargin,m_nCurrentY,m_nLeftMargin,nActualHeight+m_nCurrentY);
	//右边线
    p->drawLine(nActualWidth+m_nLeftMargin,m_nCurrentY,nActualWidth+m_nLeftMargin,m_nCurrentY+nActualHeight);
	//p->drawLine(textWidth/2,m_nCurrentY,textWidth/2,nActualHeight+m_nCurrentY);
	//下线
    p->drawLine(m_nLeftMargin,nActualHeight+m_nCurrentY,nActualWidth+m_nLeftMargin,nActualHeight+m_nCurrentY);


	int nTitleWidth = metics.width(strXuhao);
	//int nTitleHeight = metics.height(); 

	//int nMaohaoWidth = metics.width(strMaohao);
	//序号后的那根线
	p->drawLine(nTitleWidth+4*minStep+m_nLeftMargin,m_nCurrentY,nTitleWidth+4*minStep+m_nLeftMargin,nActualHeight+m_nCurrentY/*textHeight*/);
	//
	//	p->drawLine(textWidth,m_nCurrentY,textWidth,nActualHeight+m_nCurrentY/*textHeight*/);

	QRectF rectXuhao = CenterInGrid(m_nLeftMargin,nTitleWidth+4*minStep+m_nLeftMargin,m_nCurrentY+minStep,strXuhao);

	//QRectF rect(2*minStep,m_nCurrentY+minStep,4*minStep+nTitleWidth,m_nCurrentY+minStep+nTitleHeight);
	p->drawText(rectXuhao,strXuhao);

	//int QidongWidth = metics.width(strStartTime);
    int QidongActualWidth = nActualWidth*2/5+m_nLeftMargin;
	//启动时间后面的线
	p->drawLine(QidongActualWidth,m_nCurrentY+minStep,QidongActualWidth,nActualHeight+m_nCurrentY/*textHeight*/);
	//启动时间
	QRectF rectQidong = CenterInGrid(nTitleWidth+4*minStep+m_nLeftMargin,QidongActualWidth,m_nCurrentY+minStep,strStartTime);
	p->drawText(rectQidong,strStartTime);

	//相对时间
    int RelativeActualWidth = nActualWidth/2+m_nLeftMargin;

	//相对时间后面的线
	QRectF rectRelative = CenterInGrid(QidongActualWidth,RelativeActualWidth,m_nCurrentY+minStep,strXiangdui.trimmed());
	p->drawLine(RelativeActualWidth,m_nCurrentY,RelativeActualWidth,m_nCurrentY+nActualHeight);
	p->drawText(rectRelative,strXiangdui.trimmed());

	//相位
    int PhaseActualWidth = nActualWidth*3/5+m_nLeftMargin;
	p->drawLine(PhaseActualWidth,m_nCurrentY,PhaseActualWidth,m_nCurrentY+nActualHeight);
	QRectF rectPhase = CenterInGrid(RelativeActualWidth,PhaseActualWidth,m_nCurrentY+minStep,strPhase);
	p->drawText(rectPhase,strPhase);

	//动作
    QRectF rectAction = CenterInGrid(PhaseActualWidth,nActualWidth+m_nLeftMargin,m_nCurrentY+minStep,strComponent);
	p->drawText(rectAction,strComponent);

    m_nCurrentY += (nDescHeight+minStep);


	for(int nIndex=0;nIndex<nTableCoumn;nIndex++)
	{

        //int SettingNum = nIndex+nPageIndex*m_nTableColum;
        if(m_nCurrrentIndex>=m_infoLst.count())
		{
			return;
		}

		int nY = m_nCurrentY+minStep+nIndex*(minStep+nDescHeight);
        MyTripInfo settingValue = *m_infoLst.at(m_nCurrrentIndex);


        QString strNumber = QString("%1").arg(int(m_nCurrrentIndex+1),(int)4,(int)10,QChar('0'));

		QString strValue = QString::number(settingValue.m_nValue)/*+settingValue.m_strUnit+*/+QString(" ");

		QRectF rectNum = CenterInGrid(m_nLeftMargin,nTitleWidth+4*minStep+m_nLeftMargin,nY,strXuhao);
		p->drawText(rectNum,strNumber);

		//启动时间
		QRectF rectFaultStart = CenterInGrid(nTitleWidth+4*minStep+m_nLeftMargin,QidongActualWidth,nY,strFaultStartTime);
		p->drawText(rectFaultStart,strFaultStartTime);

		//相对时间
		QString strRltime = QString::number(settingValue.m_nTime);
		strRltime+=QString("ms");
        QRectF rectR = CenterInGrid(QidongActualWidth,RelativeActualWidth,nY,strRltime);

		p->drawText(rectR,strRltime);

		//相位
		QString strPh = settingValue.m_strPhase;
        QRectF rectPh = CenterInGrid(RelativeActualWidth,PhaseActualWidth,nY,strPh);
		p->drawText(rectPh,strPh);

		//动作
		QString strAction = settingValue.m_strFaultName;
        QRectF rectA = CenterInGrid(PhaseActualWidth,nActualWidth+m_nLeftMargin,nY,strAction);
		p->drawText(rectA,strAction);

        p->drawLine(m_nLeftMargin,m_nCurrentY+nIndex*(minStep+nDescHeight),nActualWidth+m_nLeftMargin,m_nCurrentY+nIndex*(minStep+nDescHeight));
		
		//没到底，画最后一根线
        if(m_nCurrrentIndex==m_infoLst.count()-1)
		{
            p->drawLine(m_nLeftMargin,m_nCurrentY+(nIndex+1)*(minStep+nDescHeight),m_nLeftMargin+nActualWidth,m_nCurrentY+(nIndex+1)*(minStep+nDescHeight));
		}

        m_nCurrrentIndex++;
	}

    if(m_nCurrrentIndex<m_infoLst.count())
    {
        m_nCurrentY = m_nTopMargin;
        m_pPrinter->newPage();
        PrintSinglTable(p);
    }

}

void CGroupInfoPrinter::PrintTable(QPainter*p)
{

    if(m_infoLst.count()==0)
    {
        return;
    }
    m_nCurrrentIndex = 0;
    PrintSinglTable(p);

    /*bool IsNeedNewPage = false;
	if(m_infoLst.count()<=m_nTableColum)
	{
		//m_nTableColum = devide;

	}
	else
	{
		IsNeedNewPage = true;
	}
	//打印第一页
	PrintSinglTable(0,p);

	if(IsNeedNewPage)
	{
		int nPageCount;
		int nSettingCount = m_infoLst.count();
		if(nSettingCount%m_nTableColum)
		{
			nPageCount = nSettingCount/m_nTableColum;
		}
		else
		{
			nPageCount = nSettingCount/m_nTableColum+1;
		}

		//从第二页开始打了
		for(int nPageNum=1;nPageNum<nPageCount;nPageNum++)
		{
			m_pPrinter->newPage();
			m_nCurrentY = 0;
			PrintSinglTable(nPageNum,p);
		}
    }*/
}

//获取grid的str的作画rect
QRectF CGroupInfoPrinter::CenterInGrid(int left,int right,int y,QString str)
{
	QFontMetrics metics(m_font);

	int nDescWidth = metics.width(str);
	int nHeight = metics.height();
	int nStart = (left+right)/2;
	nStart = nStart - nDescWidth/2;
	int nWidth = right-nStart;
	QRectF rectF(nStart,y,nWidth,nHeight);
	return rectF;
}
