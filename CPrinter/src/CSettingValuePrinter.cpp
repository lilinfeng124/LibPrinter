#include "CSettingValuePrinter.h"
#include "CComTradeFile.h"
#include <QPainter>
#include <QRectF>
#include <QPrinter>
const int minStep = 5;
CSettingValuePrinter::CSettingValuePrinter(QPrinter*printer,CComTradeFile*pObj):CPrinterObject(printer,pObj)
{
    //m_nTableColum = 0;
	m_font=QFont("Arial",12);
	m_nCurrentY = 0;
    m_nCurrentIndex = 0;

}

void CSettingValuePrinter::DoPrinter(QPainter*p)
{
    m_pPrinter->newPage();
	printf("before Printer\n");
	PrintTitle(p);
	printf("Printer Title over\n");
	PrintHeader(p);
	printf("Printer Header over\n");
	PrintTable(p);
	printf("Printer Table over\n");
}

void CSettingValuePrinter::PrintTitle(QPainter*p)
{
	p->setPen(Qt::black);

	p->setFont(m_font);
	QString strDeviceName = m_pComTrade->GetUnitName();
	strDeviceName+=QString("━设备参数定值");
	int textWidth = p->window().width();

	QFontMetrics metics(m_font);
	
	int nTitleWidth = metics.width(strDeviceName);
	int nTitleHeight = metics.height(); 
	//QRectF rect = p->boundingRect(0,0,textWidth,maxHeight,Qt::AlignCenter,strDeviceName);


    QRectF rect((textWidth-nTitleWidth)/2,m_nTopMargin,(textWidth+nTitleWidth)/2,nTitleHeight);
	p->drawText(rect,strDeviceName);
	m_nCurrentY = rect.y()+nTitleHeight;
}

void CSettingValuePrinter::PrintHeader(QPainter*p)
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

void CSettingValuePrinter::PrintSinglTable(QPainter*p)
{
    int textWidth = p->window().width();
    int textHeight = p->window().height();

    QFontMetrics metics(m_font);

	QString strDesc = QString("描述");//                                     :    实际值");
	QString strMaohao = QString(":");
	QString strActual = QString("实际值");
	int nDescWidth = metics.width(strDesc);
	int nDescHeight = metics.height(); 

	SettingValueList SettingLst = m_pComTrade->GetSettingValueList();
	/*m_nCurrentY*/
    int nActualWidth = textWidth-m_nLeftMargin-m_nRightMargin;

    int nActualHeight = textHeight-m_nCurrentY-m_nBottomMargin;
    int nTableRow = nActualHeight/(nDescHeight+minStep)-1;//first row is table header

    int m_nTableRow = nTableRow;

    nActualHeight = (nTableRow+1)*(nDescHeight+minStep);

    p->drawLine(m_nLeftMargin,m_nCurrentY,nActualWidth+m_nLeftMargin,m_nCurrentY);

    p->drawLine(m_nLeftMargin,m_nCurrentY,m_nLeftMargin,nActualHeight+m_nCurrentY);

    p->drawLine(nActualWidth/2+m_nLeftMargin,m_nCurrentY,nActualWidth/2+m_nLeftMargin,nActualHeight+m_nCurrentY);

	//下限不要了
    p->drawLine(m_nLeftMargin,nActualHeight+m_nCurrentY,m_nLeftMargin+nActualWidth,nActualHeight+m_nCurrentY);

	QString strXuhao = QString("序号");



	int nTitleWidth = metics.width(strXuhao);
	int nTitleHeight = metics.height(); 

	int nMaohaoWidth = metics.width(strMaohao);

    p->drawLine(nTitleWidth+minStep+m_nLeftMargin,m_nCurrentY,nTitleWidth+minStep+m_nLeftMargin,nActualHeight+m_nCurrentY/*textHeight*/);

    p->drawLine(m_nLeftMargin+nActualWidth,m_nCurrentY,m_nLeftMargin+nActualWidth,nActualHeight+m_nCurrentY/*textHeight*/);

    QRectF rect(m_nLeftMargin+minStep,m_nCurrentY+minStep,minStep+nTitleWidth,nTitleHeight);
	p->drawText(rect,strXuhao);

    QRectF rect2(minStep+nActualWidth/2+m_nLeftMargin,m_nCurrentY+minStep,nTitleWidth,
        nTitleHeight);
	p->drawText(rect2,strXuhao);

    p->drawLine(minStep+nActualWidth/2+m_nLeftMargin+nTitleWidth,m_nCurrentY+minStep,
                minStep+nActualWidth/2+m_nLeftMargin+nTitleWidth,nActualHeight+m_nCurrentY/*textHeight*/);


    QRectF rect3(2*minStep+nTitleWidth+m_nLeftMargin,m_nCurrentY+minStep,nDescWidth,nTitleHeight);
	p->drawText(rect3,strDesc);

    QRectF rect4(2*minStep+nActualWidth/2+m_nLeftMargin+nTitleWidth,m_nCurrentY+minStep,nDescWidth,
        nTitleHeight);
	p->drawText(rect4,strDesc);

    QRectF rectMao1(3*nActualWidth/8+m_nLeftMargin,m_nCurrentY+minStep,
        nMaohaoWidth,nTitleHeight);
	p->drawText(rectMao1,strMaohao);

    QRectF rectMao2(7*nActualWidth/8+m_nLeftMargin,m_nCurrentY+minStep,
        nMaohaoWidth,nTitleHeight);
	p->drawText(rectMao2,strMaohao);

	int nActualWid = metics.width(strActual);
    QRectF rectActual1(nActualWidth/2+m_nLeftMargin-minStep-nActualWid,m_nCurrentY+minStep,
        nActualWid,nTitleHeight);
	p->drawText(rectActual1,strActual);

    QRectF rectActual2(nActualWidth+m_nLeftMargin-minStep-nActualWid,m_nCurrentY+minStep,
        nActualWid,nTitleHeight);
	p->drawText(rectActual2,strActual);

	m_nCurrentY = m_nCurrentY+2*minStep+nTitleHeight;

    //int m_nTableColum = (textHeight-m_nBottomMargin)/(minStep+nTitleHeight);

    for(int nIndex=0;nIndex<m_nTableRow*2;nIndex++)
	{        
        //int SettingNum = //nIndex+nPageIndex*m_nTableColum*2;
        if(m_nCurrentIndex>=SettingLst.count())
		{
			return;
		}
        CSettingValue settingValue = SettingLst.at(m_nCurrentIndex);

        QString strNumber = QString::number(m_nCurrentIndex+1);

		//int nY =m_nCurrentY+nIndex*(minStep+nDescHeight);

		QString strDesc = settingValue.m_strName;
        // last line
        if(m_nCurrentIndex==SettingLst.count()-1)
        {

            p->drawLine(m_nLeftMargin,m_nCurrentY+(nIndex+1)*(minStep+nDescHeight),
                        m_nLeftMargin+nActualWidth,m_nCurrentY+(nIndex+1)*(minStep+nDescHeight));
        }

		QString strValue = settingValue.m_strValue+settingValue.m_strUnit+QString(" ");
		//左边
        if(nIndex<m_nTableRow)
		{
            QRectF rectNum(minStep+m_nLeftMargin,m_nCurrentY+nIndex*(minStep+nDescHeight),
                           nTitleWidth,nTitleHeight);
			p->drawText(rectNum,strNumber);

            int nDescWidth = metics.width(strDesc);
            QRectF rectDesc(2*minStep+nTitleWidth+m_nLeftMargin,m_nCurrentY+nIndex*(minStep+nDescHeight),
                nDescWidth,nDescHeight);
			p->drawText(rectDesc,strDesc);

            QRectF rectMao(3*nActualWidth/8+m_nLeftMargin,m_nCurrentY+nIndex*(minStep+nDescHeight),
                nMaohaoWidth,nDescHeight);
			//p->drawText(rectMao,strMaohao);

			//计算冒号的位置
			int nValueWidth = metics.width(strValue); 
            int nStartPoint = nActualWidth/2+m_nLeftMargin-nValueWidth-minStep;
			QRectF rectValue(nStartPoint,m_nCurrentY+nIndex*(minStep+nDescHeight),
                nValueWidth,nDescHeight);
			p->drawText(rectValue,strValue);
			
			//如果值太长了，那就不要冒号了
			if(!rectValue.contains(rectMao))
			{
				p->drawText(rectMao,strMaohao);
			}

            p->drawLine(m_nLeftMargin,m_nCurrentY+(nIndex)*(minStep+nDescHeight),
                        m_nLeftMargin+nActualWidth,m_nCurrentY+(nIndex)*(minStep+nDescHeight));

		}

		//右边
        else if(nIndex<m_nTableRow*2&&nIndex>=m_nTableRow)
		{
            QRectF rectNum(minStep+nActualWidth/2+m_nLeftMargin,m_nCurrentY+(nIndex-m_nTableRow)*(minStep+nDescHeight),
                           nTitleWidth,nTitleHeight);
			p->drawText(rectNum,strNumber);

            int nDescWidth = metics.width(strDesc);
            QRectF rectDesc(2*minStep+nTitleWidth+nActualWidth/2+m_nLeftMargin,m_nCurrentY+(nIndex-m_nTableRow)*(minStep+nDescHeight),
                nDescWidth,nDescHeight);
			p->drawText(rectDesc,strDesc);

            QRectF rectMao(7*nActualWidth/8+m_nLeftMargin,m_nCurrentY+(nIndex-m_nTableRow)*(minStep+nDescHeight),
                nMaohaoWidth,nDescHeight);
			//p->drawText(rectMao,strMaohao);


			int nValueWidth = metics.width(strValue); 
            int nStartPoint = nActualWidth+m_nLeftMargin-nValueWidth-minStep;
            QRectF rectValue(nStartPoint,m_nCurrentY+(nIndex-m_nTableRow)*(minStep+nDescHeight),
                textWidth,nDescHeight);
			p->drawText(rectValue,strValue);

			//如果值太长了，那就不要冒号了
			if(!rectValue.contains(rectMao))
			{
				p->drawText(rectMao,strMaohao);
			}
		}
        m_nCurrentIndex++;
	}

    if(m_nCurrentIndex<SettingLst.count())
    {
        m_pPrinter->newPage();
        m_nCurrentY = m_nTopMargin;
        PrintSinglTable(p);
    }

}

void CSettingValuePrinter::PrintTable(QPainter*p)
{
	SettingValueList settingValueLst = m_pComTrade->GetSettingValueList();

    //bool IsNeedNewPage = false;
    /*if(settingValueLst.count()<=m_nTableColum*2)
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
		int nSettingCount = m_pComTrade->GetSettingValueList().count();
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
            m_nCurrentY = m_nTopMargin;
			PrintSinglTable(nPageNum,p);
		}
    }*/

}

//获取grid的str的作画rect
QRectF CSettingValuePrinter::CenterInGrid(int left,int right,int y,QString str)
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
