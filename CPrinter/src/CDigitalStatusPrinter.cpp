#include "CDigitalStatusPrinter.h"
#include "CComTradeFile.h"
#include <QPainter>
#include <QRectF>
#include <QPrinter>
#include <stdio.h>
const int minStep = 5;

CDigitalStatusPrinter::CDigitalStatusPrinter(QPrinter*printer,CComTradeFile*pObj):CPrinterObject(printer,pObj)
{
    //m_nTableColum = 45;
	m_font=QFont("Arial",12);
	m_nCurrentY = 0;
    m_nCurrentIndex = 0;

}

void CDigitalStatusPrinter::DoPrinter(QPainter*p)
{
    m_pPrinter->newPage();
	printf("DigitalStatusPrinter start\n");
	PrintTitle(p);

	PrintHeader(p);

	PrintTable(p);
	printf("DigitalStatusPrinter end\n");
}

void CDigitalStatusPrinter::PrintTitle(QPainter*p)
{
	p->setPen(Qt::black);

	p->setFont(m_font);
	QString strDeviceName = m_pComTrade->GetUnitName();
	strDeviceName+=QString("━启动时告警状态");
    int textWidth = p->window().width()-m_nLeftMargin-m_nRightMargin;

	QFontMetrics metics(m_font);

	int nTitleWidth = metics.width(strDeviceName);
	int nTitleHeight = metics.height(); 
	//QRectF rect = p->boundingRect(0,0,textWidth,maxHeight,Qt::AlignCenter,strDeviceName);


    QRectF rect((textWidth-nTitleWidth)/2+m_nLeftMargin,m_nTopMargin,textWidth,nTitleHeight);
	p->drawText(rect,strDeviceName);
	m_nCurrentY = rect.y()+nTitleHeight;
}

void CDigitalStatusPrinter::PrintHeader(QPainter*p)
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

void CDigitalStatusPrinter::PrintSinglTable(QPainter*p)
{
    int textWidth = p->window().width();
    int textHeight = p->window().height();

	QFontMetrics metics(m_font);
	QString strXuhao = QString("序号");
	QString strDesc = QString("描述");//                                     :    实际值");
	QString strMaohao = QString(":");
	QString strActual = QString("实际值");
	int nDescWidth = metics.width(strDesc);
	int nDescHeight = metics.height(); 
    printf("nDescHeight=%d\n",nDescHeight);

	DigitalStatusList SettingLst = m_pComTrade->GetDigitalStatusList();
	/*m_nCurrentY*/
    int nActualWidth = textWidth-m_nLeftMargin-m_nRightMargin;
    int nActualHeight = textHeight-m_nCurrentY-m_nBottomMargin;

	int nTableCoumn = nActualHeight/(nDescHeight+minStep)-1;

	m_nTableColum = nTableCoumn;

	nActualHeight = (nTableCoumn+1)*(nDescHeight+minStep);

    p->drawLine(m_nLeftMargin,m_nCurrentY,textWidth-m_nRightMargin,m_nCurrentY);

    p->drawLine(m_nLeftMargin,m_nCurrentY,m_nLeftMargin,nActualHeight+m_nCurrentY);

    p->drawLine(nActualWidth/2+m_nLeftMargin,m_nCurrentY,nActualWidth/2+m_nLeftMargin,
                nActualHeight+m_nCurrentY);

	//下限不要了
    p->drawLine(m_nLeftMargin,nActualHeight+m_nCurrentY,m_nLeftMargin+nActualWidth,nActualHeight+m_nCurrentY);



	int nTitleWidth = metics.width(strXuhao);
	int nTitleHeight = metics.height(); 

	int nMaohaoWidth = metics.width(strMaohao);

    p->drawLine(nTitleWidth+minStep+m_nLeftMargin,m_nCurrentY,nTitleWidth+minStep+m_nLeftMargin,nActualHeight+m_nCurrentY/*textHeight*/);

    p->drawLine(nActualWidth+m_nLeftMargin,m_nCurrentY,nActualWidth+m_nLeftMargin,nActualHeight+m_nCurrentY/*textHeight*/);

    QRectF rect(m_nLeftMargin+minStep,m_nCurrentY+minStep,minStep+nTitleWidth,minStep+nTitleHeight);
	p->drawText(rect,strXuhao);

    QRectF rect2(minStep+nActualWidth/2+m_nLeftMargin,m_nCurrentY+minStep,minStep+nTitleWidth,
        minStep+nTitleHeight);
	p->drawText(rect2,strXuhao);

    p->drawLine(3*minStep+nActualWidth/2+m_nLeftMargin+nTitleWidth,m_nCurrentY,
                3*minStep+nActualWidth/2+m_nLeftMargin+nTitleWidth,nActualHeight+m_nCurrentY/*textHeight*/);


    QRectF rect3(2*minStep+nTitleWidth+m_nLeftMargin,m_nCurrentY+minStep,nDescWidth,nTitleHeight);
	p->drawText(rect3,strDesc);

    QRectF rect4(4*minStep+nActualWidth/2+nTitleWidth+m_nLeftMargin,m_nCurrentY+minStep,
                 nDescWidth,nTitleHeight);
	p->drawText(rect4,strDesc);

    QRectF rectMao1(3*nActualWidth/8+m_nLeftMargin,m_nCurrentY+minStep,
        nMaohaoWidth+minStep,nTitleHeight);
	p->drawText(rectMao1,strMaohao);

    QRectF rectMao2(7*nActualWidth/8+m_nLeftMargin,m_nCurrentY+minStep,
        nMaohaoWidth+minStep,nTitleHeight);
	p->drawText(rectMao2,strMaohao);

	int nActualWid = metics.width(strActual);
    QRectF rectActual1(m_nLeftMargin+nActualWidth/2-minStep-nActualWid,m_nCurrentY+minStep,
        nActualWid,nTitleHeight);
	p->drawText(rectActual1,strActual);

    QRectF rectActual2(m_nLeftMargin+nActualWidth-minStep-nActualWid,m_nCurrentY+minStep,
        nActualWid,nTitleHeight);
	p->drawText(rectActual2,strActual);

	m_nCurrentY = m_nCurrentY+2*minStep+nTitleHeight;



	for(int nIndex=0;nIndex<m_nTableColum*2;nIndex++)
	{

        //int SettingNum = nIndex+nPageIndex*m_nTableColum*2;
        if(m_nCurrentIndex>=SettingLst.count())
		{
			return;
		}
        CDigitalStatus settingValue = SettingLst.at(m_nCurrentIndex);

        QString strNumber = QString::number(m_nCurrentIndex+1/*nIndex+1*/);

		//int nY =m_nCurrentY+nIndex*(minStep+nDescHeight);

		QString strDesc = settingValue.m_strName;



		QString strValue = QString::number(settingValue.m_nValue)+QString(" ");
		//左边
		if(nIndex<m_nTableColum)
		{
            QRectF rectNum(m_nLeftMargin+minStep,m_nCurrentY+nIndex*(minStep+nDescHeight),
                           minStep+nTitleWidth,nTitleHeight);
            p->drawText(rectNum,strNumber);

            int nDescWid = metics.width(strDesc);
            QRectF rectDesc(2*minStep+nTitleWidth+m_nLeftMargin,m_nCurrentY+nIndex*(minStep+nDescHeight),
                            nDescWid,
                            nDescHeight);
			p->drawText(rectDesc,strDesc);

            QRectF rectMao(3*nActualWidth/8+m_nLeftMargin,m_nCurrentY+nIndex*(minStep+nDescHeight),
                nMaohaoWidth,nDescHeight);
			//p->drawText(rectMao,strMaohao);

			//计算冒号的位置
			int nValueWidth = metics.width(strValue); 
            int nStartPoint = nActualWidth/2-nValueWidth+m_nLeftMargin-minStep;
			QRectF rectValue(nStartPoint,m_nCurrentY+nIndex*(minStep+nDescHeight),
                nValueWidth,nDescHeight);
			p->drawText(rectValue,strValue);

			//如果值太长了，那就不要冒号了
			if(!rectValue.contains(rectMao))
			{
				p->drawText(rectMao,strMaohao);
			}

            p->drawLine(m_nLeftMargin,m_nCurrentY+nIndex*(minStep+nDescHeight),
                        m_nLeftMargin+nActualWidth,m_nCurrentY+nIndex*(minStep+nDescHeight));
		}

		//右边
		else if(nIndex<m_nTableColum*2&&nIndex>=m_nTableColum) 
		{
            QRectF rectNum(minStep*2+nActualWidth/2+m_nLeftMargin,m_nCurrentY+(nIndex-m_nTableColum)*(minStep+nDescHeight),
                           nTitleWidth,nTitleHeight);
            p->drawText(rectNum,strNumber);

            int nDescWid = metics.width(strDesc);
            QRectF rectDesc(4*minStep+nTitleWidth+nActualWidth/2+m_nLeftMargin,
                            m_nCurrentY+(nIndex-m_nTableColum)*(minStep+nDescHeight),
                            nDescWid,
                            nDescHeight);

            //QRectF rectDesc(4*minStep+nTitleWidth+textWidth/2,m_nCurrentY+(nIndex-m_nTableColum)*(minStep+nDescHeight),
            //	2*minStep+nTitleWidth+5*textWidth/8,m_nCurrentY+(nIndex-m_nTableColum)*(minStep+nDescHeight)+nDescHeight);
			p->drawText(rectDesc,strDesc);

            QRectF rectMao(7*nActualWidth/8+m_nLeftMargin,m_nCurrentY+(nIndex-m_nTableColum)*(minStep+nDescHeight),
                nMaohaoWidth,nDescHeight);
			//p->drawText(rectMao,strMaohao);


			int nValueWidth = metics.width(strValue); 
            int nStartPoint = m_nLeftMargin+nActualWidth-nValueWidth-minStep;
			QRectF rectValue(nStartPoint,m_nCurrentY+(nIndex-m_nTableColum)*(minStep+nDescHeight),
                nValueWidth,nDescHeight);
			p->drawText(rectValue,strValue);

			//如果值太长了，那就不要冒号了
			if(!rectValue.contains(rectMao))
			{
				p->drawText(rectMao,strMaohao);
			}
		}
        m_nCurrentIndex++;
	}
    //continue print on new page
    if(m_nCurrentIndex<SettingLst.count())
    {
        m_nCurrentY = m_nTopMargin;
        m_pPrinter->newPage();
        PrintSinglTable(p);
    }

}

void CDigitalStatusPrinter::PrintTable(QPainter*p)
{
	DigitalStatusList settingValueLst = m_pComTrade->GetDigitalStatusList();
    m_nCurrentIndex = 0;
/*	bool IsNeedNewPage = false;
	if(settingValueLst.count()<=m_nTableColum*2)
	{
		//m_nTableColum = devide;

	}
	else
	{
		IsNeedNewPage = true;
    }*/
	//打印第一页
    PrintSinglTable(p);

    /*if(IsNeedNewPage)
	{
		int nPageCount;
		int nSettingCount = settingValueLst.count();
		if(nSettingCount%(m_nTableColum*2))
		{
			nPageCount = nSettingCount/(m_nTableColum*2);
		}
		else
		{
			nPageCount = nSettingCount/(m_nTableColum*2)+1;
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
QRectF CDigitalStatusPrinter::CenterInGrid(int left,int right,int y,QString str)
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

