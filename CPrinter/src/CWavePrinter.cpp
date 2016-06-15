#include "CWavePrinter.h"
#include "CComTradeFile.h"
#include <QPainter>
#include <QRectF>
#include <QPrinter>
//#include <qDebug>
const int minStep = 5;
CWavePrinter::CWavePrinter(QPrinter*printer,CComTradeFile*pObj):CPrinterObject(printer,pObj)
{
    //m_nTableColum = 45;
	m_font=QFont("Arial",12);
	m_nScrollTop = 0;
	m_nCurrentY = 0;
    m_nScaleWidth = 20;
	m_nAnalogChannelHeight = 60;
	m_nDigitalChannelHeight = 40;
	//每一页打多少通道
	m_nAnalogChannelNum = 0;
	//每一页打多少个点
	m_nAnalogTickNum = 0;
	//每一页打多少通道
	m_nDigitalChannelNum = 0;
	//每一页打多少个点
	m_nDigitalTickNum = 0;
}

void CWavePrinter::DoPrinter(QPainter*p)
{
	PrintAnalogWave(p);

    //m_pPrinter->newPage();

	PrintDigitalWave(p);
}

void CWavePrinter::PrintAnalogWave(QPainter*p)
{
	//计算多少页
    if(m_AnalogChannelLst.count()==0)
    {
        return;
    }
	CalculateAnalagPageNumber(p);

	//
	for(int nIndex=0;nIndex<m_pPgContentLst.count();nIndex++)
	{
		//
        //if(nIndex>0)
		{
			m_pPrinter->newPage();
		}
		PgContent*pPg = m_pPgContentLst.at(nIndex);

		PrintAnalogTitle(p);
		PrintHeader(p);
		PrintTable(p,pPg);
		drawAnalogChannelLine(p,pPg);
		drawWave(p,pPg);
		drawTimeAxisB(p,pPg);
	}

	qDeleteAll(m_pPgContentLst);
	m_pPgContentLst.clear();
}

void CWavePrinter::PrintDigitalWave(QPainter*p)
{
	//计算多少页

    if(m_DigitalChannelLst.count()==0)
    {
        return;
    }
	CalculateDigitalPageNumber(p);

	//
	for(int nIndex=0;nIndex<m_pPgContentLst.count();nIndex++)
	{
		//
        //if(nIndex>0)
		{
			m_pPrinter->newPage();
		}
		PgContent*pPg = m_pPgContentLst.at(nIndex);

		PrintDigitalTitle(p);
		PrintHeader(p);
		PrintDigitalTable(p,pPg);
		drawDigitalChannelLine(p,pPg);
		drawDigitalWave(p,pPg);
        drawDigitalTimeAxisB(p,pPg);
	}
	qDeleteAll(m_pPgContentLst);
	m_pPgContentLst.clear();
}

void CWavePrinter::CalculateAnalagPageNumber(QPainter*p)
{
    int nACount=m_AnalogChannelLst.count();//m_pComTrade->GetACount();
	int nTotalHeight = p->window().height();
	int nTotalWidth = p->window().width();

    int nActualHeight = nTotalHeight-m_nTopMargin-m_nBottomMargin;
    int nActualWidth = nTotalWidth-m_nLeftMargin-m_nRightMargin;

	QFontMetrics metrics(m_font);
	int nWordHeight = metrics.height();

	//除去刻度的高度
	//int nSrollHeight = 20;
	//nTotalWidth-=nSrollHeight;
	//int nChannelHeight = 60;

	double fGridWidth = 2;
	//double dWaveZoomY1  = 1;

	int nOneWidth = 0;
	/*if(fZoomX >= (1.0 - 1E-5))*/	nOneWidth = int(fGridWidth+.5);
/*	else						nOneWidth = int(.5+fGridWidth);*/

	//计算每页可以打印多少通道
    int nChannelNum = (nActualWidth-m_nScaleWidth)/(m_nAnalogChannelHeight+minStep);
// 	if(nTotalWidth%(nChannelHeight+minStep)!=0)
// 	{
// 		nChannelNum+=1;
// 	}

	m_nAnalogChannelNum = nChannelNum;
	int BigPgNum = nACount/nChannelNum;
	if(nACount%nChannelNum!=0)
	{
		BigPgNum+=1;
	}
	//算多少个点
	int nTickNum = 0;
	for(int nBlock=0;nBlock<m_pComTrade->GetNrates();nBlock++)
	{
		nTickNum += m_pComTrade->GetSampleCountByBlock(nBlock);
	}
	//计算每页可以打多少点
	int TableHeight = CalculateTableHeight(nChannelNum);
	int nTmpCurrentY=TableHeight+3*minStep+2*nWordHeight;
    int nTickNumInPage = (nActualHeight-nTmpCurrentY)/nOneWidth;
	
	m_nAnalogTickNum = nTickNumInPage;
	//每个通道的点要几页才能打完
	int SamplePage = nTickNum/nTickNumInPage;
	if(nTickNum%nTickNumInPage!=0)
	{
		SamplePage+=1;
	}
	//int nPageCount = BigPgNum*SamplePage;
	int nPreBlock = 0;
	int nStartTick = 0;
	int bBlock =0;
	for(int m=0;m<BigPgNum;m++)
	{
		//int nAlready = 0;
		nPreBlock = 0;
		nStartTick = 0;
		for(int n=0;n<SamplePage;n++)
		{
			PgContent* pPg = new PgContent;
 			pPg->m_nStartChannel= m*nChannelNum;
			if(n==0)
			{
				pPg->m_nStartTick = 0;
				pPg->m_nblock = 0;
				m_pPgContentLst.append(pPg);
			}
			else
			{
				int tmp = n*nTickNumInPage;
				int nBlock=0;
				while(SumBlockTick(nBlock)<tmp&&nBlock<m_pComTrade->GetNrates())
				{
					nBlock++;
				}
				if(bBlock==m_pComTrade->GetNrates())
				{
					delete pPg;
					return;
				}
				else
				{
					pPg->m_nblock= nBlock;
					pPg->m_nStartTick = tmp-SumBlockTick(nBlock-1);
					m_pPgContentLst.append(pPg);
				}
			}
		}
	}
}


void CWavePrinter::CalculateDigitalPageNumber(QPainter*p)
{
    int nDCount = m_DigitalChannelLst.count();//m_pComTrade->GetDCount();
	int nTotalHeight = p->window().height();
	int nTotalWidth = p->window().width();

    int nActualheight = nTotalHeight-m_nTopMargin-m_nBottomMargin;
    int nActualWidth = nTotalWidth-m_nLeftMargin-m_nRightMargin;
	QFontMetrics metrics(m_font);
	int nWordHeight = metrics.height();

	//除去刻度的高度
	//int nSrollHeight = 20;
	//nTotalWidth-=nSrollHeight;
	//int nChannelHeight = 60;

	double fGridWidth = 2;
	//double dWaveZoomY1  = 1;

	int nOneWidth = 0;
	/*if(fZoomX >= (1.0 - 1E-5))*/	nOneWidth = int(fGridWidth+.5);
	/*	else						nOneWidth = int(.5+fGridWidth);*/

	//计算每页可以打印多少通道
    int nChannelNum = (nActualWidth-m_nScaleWidth)/(m_nDigitalChannelHeight+minStep);
	// 	if(nTotalWidth%(nChannelHeight+minStep)!=0)
	// 	{
	// 		nChannelNum+=1;
	// 	}

	m_nDigitalChannelNum = nChannelNum;
	int BigPgNum = nDCount/nChannelNum;
	if(nDCount%nChannelNum!=0)
	{
		BigPgNum+=1;
	}
	//算多少个点
	int nTickNum = SumBlockTick(m_pComTrade->GetNrates()-1);
// 	for(int nBlock=0;nBlock<m_pComTrade->GetNrates();nBlock++)
// 	{
// 		nTickNum += m_pComTrade->GetSampleCountByBlock(nBlock);
// 	}
	//计算每页可以打多少点
	int TableHeight = CalculateTableHeight(nChannelNum);
    int nTmpCurrentY=TableHeight+3*minStep+2*nWordHeight;
    int nTickNumInPage = (/*nTotalHeight*/nActualheight-nTmpCurrentY)/nOneWidth;

	m_nDigitalTickNum = nTickNumInPage;
	//每个通道的点要几页才能打完
	int SamplePage = nTickNum/nTickNumInPage;
	if(nTickNum%nTickNumInPage!=0)
	{
		SamplePage+=1;
	}
	//int nPageCount = BigPgNum*SamplePage;
	int nPreBlock = 0;
	int nStartTick = 0;
	int bBlock =0;
	for(int m=0;m<BigPgNum;m++)
	{
		//int nAlready = 0;
		nPreBlock = 0;
		nStartTick = 0;
		for(int n=0;n<SamplePage;n++)
		{
			PgContent* pPg = new PgContent;
			pPg->m_nStartChannel= m*nChannelNum;
			if(n==0)
			{
				pPg->m_nStartTick = 0;
				pPg->m_nblock = 0;
				m_pPgContentLst.append(pPg);
			}
			else
			{
				int tmp = n*nTickNumInPage;
				int nBlock=0;
				while(SumBlockTick(nBlock)<tmp&&nBlock<m_pComTrade->GetNrates())
				{
					nBlock++;
				}
				if(bBlock==m_pComTrade->GetNrates())
				{
					delete pPg;
					return;
				}
				else
				{
					pPg->m_nblock= nBlock;
					pPg->m_nStartTick = tmp-SumBlockTick(nBlock-1);
					m_pPgContentLst.append(pPg);
				}
			}
		}
	}
}

int CWavePrinter::SumBlockTick(int block)
{
	if(block<0)
	{
		return 0;
	}
	int nTickCount = 0;
	int n=0;
	while(n<=block)
	{
		nTickCount+=m_pComTrade->GetSampleCountByBlock(n);
		n++;
	}
	return nTickCount;
}
//计算表头高度
int CWavePrinter::CalculateTableHeight(int nChannelNum)
{
	int n = nChannelNum/2;
	QFontMetrics metric(m_font);
	int nHeight = metric.height()+minStep;
	return  (nHeight*n+minStep);
}

void CWavePrinter::PrintAnalogTitle(QPainter*p)
{
	p->setPen(Qt::black);

	p->setFont(m_font);
	QString strDeviceName = m_pComTrade->GetUnitName();
    strDeviceName+=QString("━模拟波形");
	int textWidth = p->window().width();
    int nActualWidth = textWidth-m_nLeftMargin-m_nRightMargin;

	QFontMetrics metics(m_font);
	
	int nTitleWidth = metics.width(strDeviceName);
	int nTitleHeight = metics.height(); 
	//QRectF rect = p->boundingRect(0,0,textWidth,maxHeight,Qt::AlignCenter,strDeviceName);


    QRectF rect((nActualWidth-nTitleWidth)/2+m_nLeftMargin,m_nTopMargin,nTitleWidth,nTitleHeight);
	p->drawText(rect,strDeviceName);
    p->drawLine(m_nLeftMargin,rect.y()+nTitleHeight,m_nLeftMargin+nActualWidth,rect.y()+nTitleHeight);
	m_nCurrentY = rect.y()+nTitleHeight+minStep;
}

void CWavePrinter::PrintDigitalTitle(QPainter*p)
{
	p->setPen(Qt::black);

	p->setFont(m_font);
	QString strDeviceName = m_pComTrade->GetUnitName();
    strDeviceName+=QString("━数字波形");
	int textWidth = p->window().width();
    int nActualWidth = textWidth-m_nLeftMargin-m_nRightMargin;
	QFontMetrics metics(m_font);

	int nTitleWidth = metics.width(strDeviceName);
	int nTitleHeight = metics.height(); 
	//QRectF rect = p->boundingRect(0,0,textWidth,maxHeight,Qt::AlignCenter,strDeviceName);


    QRectF rect((nActualWidth-nTitleWidth)/2+m_nLeftMargin,m_nTopMargin,nTitleWidth,nTitleHeight);
	p->drawText(rect,strDeviceName);
    p->drawLine(m_nLeftMargin,rect.y()+nTitleHeight,m_nLeftMargin+nActualWidth,rect.y()+nTitleHeight);
	m_nCurrentY = rect.y()+nTitleHeight+minStep;
}


void CWavePrinter::PrintHeader(QPainter*p)
{
	int textWidth = p->window().width();
    int nActualWidth = textWidth-m_nLeftMargin-m_nRightMargin;
	m_nCurrentY = m_nCurrentY+minStep;
	//p->drawLine(0,m_nCurrentY,textWidth,m_nCurrentY);
	//m_nCurrentY = m_nCurrentY+minStep*2;
    int nMaxLength = nActualWidth/3;
	QString strStation = QString("厂站名:")+m_pComTrade->GetSiteName();

	QFontMetrics metics(m_font);


	int nTitleHeight = metics.height(); 

    QRectF rect(m_nLeftMargin,m_nCurrentY,nMaxLength,nTitleHeight);
	p->drawText(rect,strStation);

	QString strDevice = QString("特征号:")+m_pComTrade->GetUnitName();
    QRectF rect2(m_nLeftMargin+nMaxLength,m_nCurrentY,nMaxLength,nTitleHeight);
	p->drawText(rect2,strDevice);

	QString strVersion = QString("版本号:")+m_pComTrade->GetRevision();
    QRectF rect3(m_nLeftMargin+nMaxLength*2,m_nCurrentY,nMaxLength,nTitleHeight);
	p->drawText(rect3,strVersion);

	m_nCurrentY=m_nCurrentY+nTitleHeight+minStep;
    p->drawLine(m_nLeftMargin,m_nCurrentY,m_nLeftMargin+nActualWidth,m_nCurrentY);
}


//获取grid的str的作画rect
QRectF CWavePrinter::CenterInTable(QPainter * p,int x,QString str)
{
	QFontMetrics metics(m_font);

	int nDescWidth = metics.width(str);
	int nHeight = metics.height();

	int nStart;
    int nTotalWidth = p->window().width()-m_nLeftMargin-m_nRightMargin;
	if(x%2==0)
	{
        nStart = minStep*2+m_nLeftMargin;
	}
	else if(x%2==1)
	{
        nStart = minStep*2+nTotalWidth/2+m_nLeftMargin;
	}

	//nStart = nStart - nDescWidth/2;
	int nRow = x/2;
// 	if(x%2!=0)
// 	{
// 		nRow+=1;
// 	}
	int y = nRow*(nHeight+minStep)+m_nCurrentY+minStep;
	QRectF rectF(nStart,y,nStart+nDescWidth,y+nHeight);
	return rectF;
}

void CWavePrinter::PrintTable(QPainter*p,PgContent*pPg)
{

	for(int nIndex=0;nIndex<m_nAnalogChannelNum&&
                     nIndex+pPg->m_nStartChannel<m_AnalogChannelLst.count()/*m_pComTrade->GetACount()*/;nIndex++)
	{
        int nCh = m_AnalogChannelLst.at(nIndex+pPg->m_nStartChannel);
		//CAChanelInfo AChnlInfo = m_pComTrade->GetAInfoByIndex(nIndex+pPg->m_nStartChannel);
		QString szName = QString("%1:%2[%3] ").arg(nCh+1)
			.arg(m_pComTrade->GetAInfoByIndex(nCh).m_strCh_id )
			.arg(m_pComTrade->GetAInfoByIndex(nCh).m_strUu);

		double fMax = m_pComTrade->GetMaxValue(nCh,0,0);

		double fMin = m_pComTrade->GetMinValue(nCh,0,0);
		QString str=QString("%1～%2").arg(fMin,0,'f',2).arg(fMax,0,'f',2);
		szName = szName+str;
		QRectF rect = CenterInTable(p,nIndex,szName);
		p->drawText(rect,szName);
	}

	QFontMetrics metics(m_font);


	int nHeight = metics.height();
	int nWidth = p->window().width();
    int nActualWidth = nWidth-m_nLeftMargin-m_nRightMargin;
	int nRow = m_nAnalogChannelNum/2;
	if(m_nAnalogChannelNum%2!=0)
	{
		nRow+=1;
	}
	m_nCurrentY+=nRow*(nHeight+minStep)+minStep;
    p->drawLine(m_nLeftMargin,m_nCurrentY,m_nLeftMargin+nActualWidth,m_nCurrentY);
}

void CWavePrinter::PrintDigitalTable(QPainter*p,PgContent*pPg)
{

	for(int nIndex=0;nIndex<m_nDigitalChannelNum&&
        nIndex+pPg->m_nStartChannel<m_DigitalChannelLst.count()/*m_pComTrade->GetDCount()*/;nIndex++)
	{
        int nCh = m_DigitalChannelLst.at(nIndex+pPg->m_nStartChannel);
		//CAChanelInfo AChnlInfo = m_pComTrade->GetAInfoByIndex(nIndex+pPg->m_nStartChannel);
		QString szName = QString("%1:%2 ").arg(nCh+1)
			.arg(m_pComTrade->GetDInfoByIndex(nCh).m_strCh_id );
			//.arg(m_pComTrade->GetDInfoByIndex(nCh).m_strPh);

		QRectF rect = CenterInTable(p,nIndex,szName);
		p->drawText(rect,szName);
	}

	QFontMetrics metics(m_font);

	int nHeight = metics.height();
	int nWidth = p->window().width();
    int nActualWidth = nWidth-m_nLeftMargin-m_nRightMargin;
	int nRow = m_nDigitalChannelNum/2;
	if(m_nDigitalChannelNum%2!=0)
	{
		nRow+=1;
	}
	m_nCurrentY+=nRow*(nHeight+minStep)+minStep;
    p->drawLine(m_nLeftMargin,m_nCurrentY,m_nLeftMargin+nActualWidth,m_nCurrentY);
}

void CWavePrinter::drawWave(QPainter *paint,PgContent*pPage)
{
	if(!m_pComTrade)	return;

	int nTotalWidth = paint->window().width();;
	int nTotalHeight = paint->window().height();
    int nActualWidth = nTotalWidth-m_nLeftMargin-m_nRightMargin;
    int nActualHeight = nTotalHeight-m_nTopMargin-m_nBottomMargin;
	QFontMetrics Metrics(m_font);
	//int nCHeight=Metrics.height();
	double fZoomX = 1;
	//fZoomX = 25.0/43.0;
	double fGridWidth = 2;
	//double dWaveZoomY1  = 1;

	int nOneWidth = 0;
	if(fZoomX >= (1.0 - 1E-5))	nOneWidth = int(fGridWidth*fZoomX+.5);
	else						nOneWidth = int(.5+fGridWidth);

	//QRect ClientRect = QRect(0,m_nCurrentY,nTotalWidth,nTotalHeight);//paint->viewport();


 	//ClientRect.setRight(qMin(ClientRect.right(), GetParent()->GetMaxWidth() - m_nScrollLeft));
// 	if(ClientRect.right() < ClientRect.left()) return;
    int nLeft = m_nScaleWidth+m_nLeftMargin;

    int nMaxWaveGroupNum = m_AnalogChannelLst.count();//m_pComTrade->GetACount();//GetParent()->GetMaxWaveGroupNum();
    for(int i=0; i<m_nAnalogChannelNum&&i+pPage->m_nStartChannel<nMaxWaveGroupNum && (i+pPage->m_nStartChannel)<MAX_WAVES; i++)
	{
        int ch = m_AnalogChannelLst.at(i+pPage->m_nStartChannel);
		int nWaveHeight = m_nAnalogChannelHeight;//m_pDispAttr->m_dWaveHeight[i]*dWaveZoomY1;
		//if(nWaveHeight<18)	nWaveHeight=18;

		//if(nLeft>ClientRect.right()) break;

		int nMidLineX = nLeft + nWaveHeight/2 + minStep;

        //if(nLeft + nWaveHeight+minStep>nTotalWidth) continue;
		//bool bShow = false;
		nLeft += nWaveHeight+minStep;
		QPoint ptPrev;
		int nTickPos = SumBlockTick(pPage->m_nblock-1)+pPage->m_nStartTick;
		bool bFirst =  FALSE;

		int nBlockLeft = m_nCurrentY;

		int nTickNum = 0;

		for(int nBlk = pPage->m_nblock; nBlk < m_pComTrade->GetNrates(); nBlk++)
		{
			bool bAmplitude = FALSE;
			if(m_pComTrade->GetSampleRateByBlock(nBlk) < 100.1) bAmplitude = TRUE;

			int nTick =0;
			if(nBlk==pPage->m_nblock)
			{
				nTick = pPage->m_nStartTick;
			}
			

			for(; nTick<m_pComTrade->GetSampleCountByBlock(nBlk); nTick++)
			{

				int nYPos;
				if(fZoomX >= (1.0 - 1E-5))	nYPos = nBlockLeft+(nTickNum)*nOneWidth;
				else	nYPos = nBlockLeft + int(double((nTickNum)*nOneWidth)*fZoomX);


 				if((nYPos/*-m_nScrollLeft*/) < 0)
				{
					nTickPos++;continue;
				}
 				if((nYPos/*-m_nScrollLeft*/) >nTotalHeight) break;
                                            
				double Value = m_pComTrade->GetAnalogValueByIndexPoint(ch,nTickPos);

				double fMaxValue = m_pComTrade->GetMaxAbsRawValue(ch);
				if(qAbs(fMaxValue)>1E-5)
				{
					double fRadio = nWaveHeight
						/double(fMaxValue)/2;
					Value= Value*fRadio;
                    //if(ch==0)
                    //{
                    //    printf("Value=%f fMaxValue=%f\n",Value,fMaxValue);
                    //}
				}
				else
				{
					//qDebug()<<"error divide";
				}


				nTickPos++;
				//Value = (Value);
				//Value = Value*m_pDispAttr->m_dWaveZoomY1;
				//qDebug()<<"Value="<<Value;
				int nXValue = nMidLineX+int(Value); //(nMidLineY-int(Value) > nTop)?(nTop-2):(nMidLineY-int(Value));

				QPoint pt(nXValue,nYPos  );
 //               if(ch==0)
 //               {
 //                   printf("nxValue=%d nYPos=%d\n",nXValue,nYPos);
 //               }
				if(!bFirst)
				{
					ptPrev = pt;
					bFirst = TRUE;
				}
				else
				{
					paint->drawLine(ptPrev,pt);
					ptPrev = pt;
				}
				nTickNum++;
				if(nTickNum>=m_nAnalogTickNum)
				{
					break;
				}
			}
			if(nTickNum>=m_nAnalogTickNum)
			{
				break;
			}
		}


	}
}

void CWavePrinter::drawDigitalWave(QPainter *paint,PgContent*pPage)
{
	if(!m_pComTrade)	return;

	int nTotalWidth = paint->window().width();;
	int nTotalHeight = paint->window().height();
    int nActualWidth = nTotalWidth-m_nLeftMargin-m_nRightMargin;
    int nActualHeight = nTotalHeight-m_nTopMargin-m_nBottomMargin;
	QFontMetrics Metrics(m_font);
	//int nCHeight=Metrics.height();
	double fZoomX = 1;
	//fZoomX = 25.0/43.0;
	double fGridWidth = 2;
	//double dWaveZoomY1  = 1;

	int nOneWidth = 0;
	if(fZoomX >= (1.0 - 1E-5))	nOneWidth = int(fGridWidth*fZoomX+.5);
	else						nOneWidth = int(.5+fGridWidth);

	//QRect ClientRect = QRect(0,m_nCurrentY,nTotalWidth,nTotalHeight);//paint->viewport();


	//ClientRect.setRight(qMin(ClientRect.right(), GetParent()->GetMaxWidth() - m_nScrollLeft));
	// 	if(ClientRect.right() < ClientRect.left()) return;
    int nLeft = m_nScaleWidth+m_nLeftMargin+minStep;

    int nMaxWaveGroupNum = m_DigitalChannelLst.count();//m_pComTrade->GetDCount();//GetParent()->GetMaxWaveGroupNum();
    for(int i=0; i<m_nDigitalChannelNum&&(i+pPage->m_nStartChannel)<nMaxWaveGroupNum && (i+pPage->m_nStartChannel)<MAX_WAVES; i++)
	{
        int ch = m_DigitalChannelLst.at(i+pPage->m_nStartChannel);

		int nWaveHeight = m_nDigitalChannelHeight;//m_pDispAttr->m_dWaveHeight[i]*dWaveZoomY1;
		//if(nWaveHeight<18)	nWaveHeight=18;

		//if(nLeft>ClientRect.right()) break;

        int nMidLineX =nLeft+i*(nWaveHeight+minStep);// nLeft /*+ nWaveHeight/2*/ + minStep;

        printf("ch=%d nMidLineX=%d\n",ch,nMidLineX);

        //if(nLeft + nWaveHeight+minStep>nTotalWidth) continue;
		//bool bShow = false;
        //nLeft += nWaveHeight+minStep;
        //printf("nLeft=%d\n",nLeft);
		QPoint ptPrev;
		int nTickPos = SumBlockTick(pPage->m_nblock-1)+pPage->m_nStartTick;
		bool bFirst =  FALSE;

		int nBlockLeft = m_nCurrentY;

		int nTickNum = 0;

		for(int nBlk = pPage->m_nblock; nBlk < m_pComTrade->GetNrates(); nBlk++)
		{
			bool bAmplitude = FALSE;
			if(m_pComTrade->GetSampleRateByBlock(nBlk) < 100.1) bAmplitude = TRUE;

			int nTick =0;
			if(nBlk==pPage->m_nblock)
			{
				nTick = pPage->m_nStartTick;
			}


			for(; nTick<m_pComTrade->GetSampleCountByBlock(nBlk); nTick++)
			{

				int nYPos;
				if(fZoomX >= (1.0 - 1E-5))	nYPos = nBlockLeft+(nTickNum)*nOneWidth;
				else	nYPos = nBlockLeft + int(double((nTickNum)*nOneWidth)*fZoomX);


				if((nYPos/*-m_nScrollLeft*/) < 0)
				{
					nTickPos++;continue;
				}
				if((nYPos/*-m_nScrollLeft*/) >nTotalHeight) break;

				unsigned short Value = m_pComTrade->GetDigitalValueByIndexPoint/*GetMidDigitalValueByIndexPoint*/(ch,nTickPos);
				if(ch==0)
				{
					//qDebug()<<"nTickPos="<<nTickPos<<" value="<<Value;
				}

				if(Value!=0)
				{
					int nValue = m_nDigitalChannelHeight-2*minStep;
					int nXValue = nMidLineX+nValue;
					paint->drawLine(nMidLineX,nYPos,nXValue,nYPos);
				}
				else
				{
					//Value = -m_nDigitalChannelHeight/2+minStep;
				}
				nTickPos++;
				//Value = (Value);
				//Value = Value*m_pDispAttr->m_dWaveZoomY1;
				//qDebug()<<"Value="<<Value;
 //(nMidLineY-int(Value) > nTop)?(nTop-2):(nMidLineY-int(Value));

// 				QPoint pt(nXValue,nYPos  );
// 				if(!bFirst)
// 				{
// 					ptPrev = pt;
// 					bFirst = TRUE;
// 				}
// 				else
// 				{
// 					paint->drawLine(ptPrev,pt);
// 					ptPrev = pt;
// 				}

				nTickNum++;
				if(nTickNum>=m_nDigitalTickNum)
				{
					break;
				}
			}
			if(nTickNum>=m_nDigitalTickNum)
			{
				break;
			}
		}


	}
}

void CWavePrinter::drawAnalogChannelLine(QPainter *paint,PgContent*pPg)
{
	//if(!m_pWaveFile||!m_pDispAttr->m_bShowGrid)	return;
	int nTotalHeight = paint->window().height();
    //int nActualHeight = nTotalHeight-m_nCurrentY-m_nBottomMargin;
	//int nTotalWidth = paint->window().width();
	QFontMetrics metrics(m_font);
	//int nCHeight = metrics.height();
	QPen oldPen = paint->pen();
	QPen newPen = oldPen;
	newPen.setStyle(Qt::DashLine);
	newPen.setWidth(oldPen.width()/2);
	paint->setPen(newPen);
    int nTop = m_nScaleWidth+m_nLeftMargin+minStep;
	int i=0;
	for( i=0; i<m_nAnalogChannelNum; i++)
	{
        if(((pPg->m_nStartChannel+i)>=m_AnalogChannelLst.count())||
                (m_AnalogChannelLst.at(pPg->m_nStartChannel+i))>=m_pComTrade->GetACount())
		{
			break;
		}
		int nWaveHeight = m_nAnalogChannelHeight;

		int nXPos = nTop + nWaveHeight/2 ;
		nTop += (nWaveHeight+minStep);
		//qDebug()<<"nXPos="<<nXPos;		

        QString strText = QString::number(m_AnalogChannelLst.at(i+pPg->m_nStartChannel)+1);
		paint->drawText(nXPos+minStep,m_nCurrentY+metrics.height(),strText);
        paint->drawLine(nXPos, m_nCurrentY, nXPos, nTotalHeight-m_nBottomMargin);
	}



	//paint->setPen(oldPen);
}

void CWavePrinter::drawDigitalChannelLine(QPainter *paint,PgContent*pPg)
{
	//if(!m_pWaveFile||!m_pDispAttr->m_bShowGrid)	return;
	int nTotalHeight = paint->window().height();
    int nActualHeight = nTotalHeight-m_nCurrentY-m_nBottomMargin;
	//int nTotalWidth = paint->window().width();
	QFontMetrics metrics(m_font);
	//int nCHeight = metrics.height();
	QPen oldPen = paint->pen();
	QPen newPen = oldPen;
	newPen.setStyle(Qt::DashLine);
	newPen.setWidth(oldPen.width()/2);
	paint->setPen(newPen);
    int nTop = m_nScaleWidth+m_nLeftMargin+minStep;
	int i=0;
	for( i=0; i<m_nDigitalChannelNum; i++)
	{
        if(pPg->m_nStartChannel+i>=m_DigitalChannelLst.count()/*m_pComTrade->GetDCount()*/)
		{
			break;
		}
		int nWaveHeight = m_nDigitalChannelHeight;

		int nXPos = nTop /*+ nWaveHeight/2*/ ;
		nTop += (nWaveHeight+minStep);
		//qDebug()<<"nXPos="<<nXPos;		

        QString strText = QString::number(m_DigitalChannelLst.at(i+pPg->m_nStartChannel)+1);
		paint->drawText(nXPos+minStep,m_nCurrentY+metrics.height(),strText);
        paint->drawLine(nXPos, m_nCurrentY, nXPos, nActualHeight+m_nCurrentY);
	}
}


void CWavePrinter::drawTimeAxisB( QPainter *paint ,PgContent*pPg)
{
	if(!m_pComTrade) return;

	int nTotalWidth = paint->window().width();
	int nTotalHeight = paint->window().height();
    int nActualWidth = nTotalWidth-m_nLeftMargin-m_nRightMargin;
    int nActualHeight = nTotalHeight-m_nCurrentY-m_nBottomMargin;
	float fZoomX = 1.0;
	int m_nScrollLeft = 0;
	int TEXT_WIDTH = 5;
	QList<unsigned long> timeArray = m_pComTrade->GetTimeArray();
	//长直线的X位置
    int nTopMarin = m_nScaleWidth+m_nLeftMargin;
	int nLeftMargin = m_nCurrentY;//GetParent()->m_nLeftMargin+SPLITTERWIDTH;
	//nLeftMargin = 0;
	//double	fZoomX = m_pDispAttr->m_fZoomX;
	//fZoomX = 25.0/43.0;
	double	dGirdWidth = 2.0;//m_pDispAttr->m_dGridWidth;
	int nY = 0;

	double fOneWidth = int(dGirdWidth+.5);
	if(fOneWidth < dGirdWidth)	
		fOneWidth = dGirdWidth;

    //QPen oldPen = paint->pen();
    //QPen newPen = oldPen;
    //newPen.setWidth(oldPen.width()/2);
// 	paint->setPen(m_pDispAttr->m_TimeForeColor);
// 	QFont OldFont = paint->font();
    //QFont newFont = QFont("Arial",5);
    //paint->setFont(newFont);

    //paint->setPen(newPen);

    paint->drawLine(nTopMarin,nLeftMargin,nTopMarin,nActualHeight+m_nCurrentY);

    QRect ClientRect = QRect(m_nLeftMargin,m_nCurrentY,nActualWidth,nActualHeight);//GetValidRect();

	QRect rcPoint;

// 	int m_nZoomX = int(128.0*fZoomX);
// 	// 画标线
// 	if(m_nZoomX >= 128)
// 	{
		double nOneWidth = 2*fZoomX;
		int nTotalTicks = m_pComTrade->GetTotalPointCount();

		int Tick1 = SumBlockTick(pPg->m_nblock-1)+pPg->m_nStartTick;//(ClientRect.left() + m_nScrollLeft - 51) / nOneWidth - 1;
		int Tick2 = Tick1+m_nAnalogTickNum;//(ClientRect.right() + m_nScrollLeft + 51) / nOneWidth + 1;
		if(Tick1 < 0)						Tick1 = 0;
		if(Tick2 > nTotalTicks)		Tick2 = nTotalTicks;

		// 小格线  每点
		// 中格线, 每100毫秒,  每5ms
		// 大格线, 每500秒, 块的起始点, 每20ms
		// 写文字, 块的起始点, 500ms, 20ms
		for(int TR=pPg->m_nblock; TR<(m_pComTrade->GetNrates()); TR++)
		{
			int nGridAmt = m_pComTrade->GetSampleCountByBlock(TR);
			// 如每格20ms 或 > 20ms
			int TB1 = m_pComTrade->GetStartPointByBlock(TR);
			int TB2 = TB1 + nGridAmt;
			int T1, T2;
			if(TB1>=Tick2) continue;	else	T1 = qMax(TB1, Tick1);
			if(TB2<Tick1) continue;		else	T2 = qMin(TB2, Tick2);

			for(int ii=T1; ii<T2; ii++)
			{
				// 小格线
				int  nHeight = 2/*5*/;
				if( (int)(ii*nOneWidth)%(int)(2) == 0 )
				{
					if(int(ii*nOneWidth) - m_nScrollLeft >= 0)
						//paint->drawLine(nLeftMargin + ii*nOneWidth -m_nScrollLeft, 0,nLeftMargin + ii*nOneWidth -m_nScrollLeft, nHeight);			
                    paint->drawLine(nTopMarin,nLeftMargin+(ii-Tick1)*nOneWidth,nTopMarin-nHeight,
                                    nLeftMargin+(ii-Tick1)*nOneWidth);
				}

				// 起始点,最后一块的中止点
				bool bDrawText = FALSE;
				qlonglong time = timeArray.at(ii);
				int TimeDrawn = 0;
				if( (ii==TB1)||(TR==m_pComTrade->GetNrates()-1 && ii==TB2-1) )
				{
					// 起始点
                    nHeight = 11/*18*/;
					nY = nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft;
					if(int(ii*nOneWidth) - m_nScrollLeft >= 0)
                        paint->drawLine(nTopMarin,nY,nTopMarin-nHeight,nY);//drawLine(nX, 0 ,nX, nHeight );

					bDrawText = true;
					TimeDrawn = (time/1000L);
					//paint->setPen(m_pDispAttr->m_TimeForeColor);
				}
				else 
				{
					if(m_pComTrade->GetSampleRateByBlock(TR) <= 5)		
					{	
						int time1 = timeArray.at(ii-1);
						// 5000ms点
						if((time1 % 5000000L) >= (time % 5000000L))
						{
							nHeight = 4/*8*/;
							if(int(ii*nOneWidth) - m_nScrollLeft >= 0)
                                paint->drawLine(nTopMarin,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft,
                                                nTopMarin-nHeight ,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft);
						}

						// 20000ms点
						if((time1 % 20000000L) >= (time % 20000000L))
						{
                            nHeight = 9/*12*/;
							if(int(ii*nOneWidth) - m_nScrollLeft >= 0)
                                paint->drawLine(nTopMarin,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft,
                                                nTopMarin-nHeight,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft);

							if(abs(ii-TB1)>10 && abs(ii-TB2)>10)
							{
								TimeDrawn = (time/1000L);

								//paint->setPen(m_pDispAttr->m_TimeForeColor);
								bDrawText = TRUE;
							}
						}
					}
					else if(m_pComTrade->GetSampleRateByBlock(TR) <= 20)		
					{	
						int time1 = timeArray.at(ii-1);

						// 200ms点
						if((time1 % 200000L) >= (time % 200000L))
						{
							nHeight = 4/*8*/;
							if(int(ii*nOneWidth) - m_nScrollLeft >= 0)
                                paint->drawLine(nTopMarin,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft,
                                                nTopMarin-nHeight,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft);
						}

						// 10000ms点
						if((time1 % 1000000L) >= (time % 10000000L))
						{
							nHeight = 6/*12*/;
							if(int(ii*nOneWidth) - m_nScrollLeft >= 0)
                                paint->drawLine(nTopMarin,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft,
                                                nTopMarin-nHeight,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft);

							if(abs(ii-TB1)>10 && abs(ii-TB2)>10)
							{
								TimeDrawn = (time/1000L);
								//paint->setPen( QColor( 0,0,64 ) );
								//paint->setPen(m_pDispAttr->m_TimeForeColor);
								bDrawText = TRUE;
							}
						}
					}
					else if(m_pComTrade->GetSampleRateByBlock(TR) <= 50)		
					{	
						int time1 = timeArray.at(ii-1);
						// 100ms点
						if((time1 % 100000L) >= (time % 100000L))
						{
							nHeight = 4/*8*/;
							if(int(ii*nOneWidth) - m_nScrollLeft >= 0)
                                paint->drawLine(nTopMarin,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft,
                                                nTopMarin-nHeight,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft);
						}
						// 500ms点
						if((time1 % 500000L) >= (time % 500000L))
						{
							nHeight = 6/*12*/;
							//if(int((ii-Tick1)*nOneWidth)  >= 0)
                                paint->drawLine(nTopMarin,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft,
                                                nTopMarin-nHeight,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft);

							if(abs(ii-TB1)>10 && abs(ii-TB2)>10)
							{
								TimeDrawn = (time/500000L)*500;
								//paint->setPen( QColor( 0,0,64 ) );
								//paint->setPen(m_pDispAttr->m_TimeForeColor);
								bDrawText = TRUE;
							}
						}
					}
					else 
					{
						int time1 = timeArray.at(ii-1);
						// 5ms点
						nHeight = 4/*8*/;
						if((time1 % 5000L) >= (time % 5000L))
						{
							//if(int(ii*nOneWidth) - m_nScrollLeft >= 0)
                                paint->drawLine(nTopMarin-nHeight,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft,
                                                nTopMarin,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft);
						}
						// 20ms点
						nHeight = 6/*12*/;
						if((time1 % 20000L) >= (time % 20000L))
						{
							//if(int(ii*nOneWidth) - m_nScrollLeft >= 0)
                                paint->drawLine(nTopMarin,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft,
                                                nTopMarin-nHeight, nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft);

							if(abs(ii-TB1)>10 && abs(ii-TB2)>10)
							{
								TimeDrawn = (time/20000L)*20;
								//paint->setPen( QColor( 0,0,64 ) );
								//paint->setPen(m_pDispAttr->m_TimeForeColor);
								bDrawText = TRUE;
							}
						}
					}
				}

				if(bDrawText /*&& (int(ii*nOneWidth) - nLeftMargin >= 0)*/)
				{
					TimeDrawn = time/1000L;
					TimeDrawn += m_pComTrade->GetFirstDataTimeMs();
					nY = nLeftMargin + (ii-Tick1)*nOneWidth ;
					//, /*18*/  TEXT_WIDTH, TEXT_WIDTH);

					QString sComment;					
					if (TimeDrawn < 0)    sComment.sprintf("-%ld", -1*TimeDrawn);
					else if((TimeDrawn < 1000)&&(TimeDrawn >= 0))	sComment.sprintf("%ld", TimeDrawn);
					else				sComment.sprintf("%d\"%03d", TimeDrawn/1000L, TimeDrawn%1000L);
					
					//QFontMetrics metric(newFont);

					//rcPoint = QRect(2,nY - 5,metric.width(sComment),metric.height());
					paint->save();
					paint->translate(2,nY-5);
					paint->rotate(90);
					paint->drawText(0,0,sComment);

					//paint->drawText(rcPoint,Qt::AlignLeft | Qt::TextSingleLine,sComment);
					//paint->rotate(-90);
					paint->restore();
					//paint->setPen(m_pDispAttr->m_TimeForeColor);
				}
			}
		}
	//}



	paint->setFont(m_font);
	//paint->setPen(oldPen);
}

void CWavePrinter::drawDigitalTimeAxisB( QPainter *paint ,PgContent*pPg)
{
	if(!m_pComTrade) return;

	int nTotalWidth = paint->window().width();
	int nTotalHeight = paint->window().height();
    int nActualWidth = nTotalWidth-m_nLeftMargin-m_nRightMargin;
    int nActualHeight = nTotalHeight-m_nCurrentY-m_nBottomMargin;
	float fZoomX = 1.0;
	int m_nScrollLeft = 0;
	//int TEXT_WIDTH = 5;
	QList<unsigned long> timeArray = m_pComTrade->GetTimeArray();
	//长直线的X位置
    int nTopMarin = m_nScaleWidth+m_nLeftMargin;
	int nLeftMargin = m_nCurrentY;//GetParent()->m_nLeftMargin+SPLITTERWIDTH;
	//nLeftMargin = 0;
	//double	fZoomX = m_pDispAttr->m_fZoomX;
	//fZoomX = 25.0/43.0;
	double	dGirdWidth = 2.0;//m_pDispAttr->m_dGridWidth;
	int nY = 0;

	double fOneWidth = int(dGirdWidth+.5);
	if(fOneWidth < dGirdWidth)	
		fOneWidth = dGirdWidth;

	QPen oldPen = paint->pen();
	QPen newPen = oldPen;
	newPen.setWidth(oldPen.width()/2);
	// 	paint->setPen(m_pDispAttr->m_TimeForeColor);
	// 	QFont OldFont = paint->font();
	QFont newFont = QFont("Arial",5);
	paint->setFont(newFont);

	paint->setPen(newPen);

    paint->drawLine(nTopMarin,nLeftMargin,nTopMarin,nActualHeight+m_nCurrentY);

    QRect ClientRect = QRect(m_nLeftMargin,m_nCurrentY,nActualWidth,nActualHeight);//GetValidRect();

	QRect rcPoint;

	// 	int m_nZoomX = int(128.0*fZoomX);
	// 	// 画标线
	// 	if(m_nZoomX >= 128)
	// 	{
	double nOneWidth = 2*fZoomX;
	int nTotalTicks = m_pComTrade->GetTotalPointCount();

	int Tick1 = SumBlockTick(pPg->m_nblock-1)+pPg->m_nStartTick;//(ClientRect.left() + m_nScrollLeft - 51) / nOneWidth - 1;
	int Tick2 = Tick1+m_nDigitalTickNum;//(ClientRect.right() + m_nScrollLeft + 51) / nOneWidth + 1;
	if(Tick1 < 0)						Tick1 = 0;
	if(Tick2 > nTotalTicks)		Tick2 = nTotalTicks;

	// 小格线  每点
	// 中格线, 每100毫秒,  每5ms
	// 大格线, 每500秒, 块的起始点, 每20ms
	// 写文字, 块的起始点, 500ms, 20ms
	for(int TR=pPg->m_nblock; TR<(m_pComTrade->GetNrates()); TR++)
	{
		int nGridAmt = m_pComTrade->GetSampleCountByBlock(TR);
		// 如每格20ms 或 > 20ms
		int TB1 = m_pComTrade->GetStartPointByBlock(TR);
		int TB2 = TB1 + nGridAmt;
		int T1, T2;
		if(TB1>=Tick2) continue;	else	T1 = qMax(TB1, Tick1);
		if(TB2<Tick1) continue;		else	T2 = qMin(TB2, Tick2);

		for(int ii=T1; ii<T2; ii++)
		{
			// 小格线
			int  nHeight = 2/*5*/;
			if( (int)(ii*nOneWidth)%(int)(2) == 0 )
			{
				if(int(ii*nOneWidth) - m_nScrollLeft >= 0)
					//paint->drawLine(nLeftMargin + ii*nOneWidth -m_nScrollLeft, 0,nLeftMargin + ii*nOneWidth -m_nScrollLeft, nHeight);			
					paint->drawLine(nTopMarin,nLeftMargin+(ii-Tick1)*nOneWidth,nTopMarin-nHeight,nLeftMargin+(ii-Tick1)*nOneWidth);
			}

			// 起始点,最后一块的中止点
			bool bDrawText = FALSE;
			qlonglong time = timeArray.at(ii);
			int TimeDrawn = 0;
			if( (ii==TB1)||(TR==m_pComTrade->GetNrates()-1 && ii==TB2-1) )
			{
				// 起始点
				nHeight = 8/*18*/;
				nY = nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft;
				if(int(ii*nOneWidth) - m_nScrollLeft >= 0)
					paint->drawLine(nTopMarin,nY,nTopMarin-nHeight,nY);//drawLine(nX, 0 ,nX, nHeight );

				bDrawText = true;
				TimeDrawn = (time/1000L);
				//paint->setPen(m_pDispAttr->m_TimeForeColor);
			}
			else 
			{
				if(m_pComTrade->GetSampleRateByBlock(TR) <= 5)		
				{	
					int time1 = timeArray.at(ii-1);
					// 5000ms点
					if((time1 % 5000000L) >= (time % 5000000L))
					{
						nHeight = 4/*8*/;
						if(int(ii*nOneWidth) - m_nScrollLeft >= 0)
							paint->drawLine(nTopMarin,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft,nTopMarin-nHeight ,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft);
					}

					// 20000ms点
					if((time1 % 20000000L) >= (time % 20000000L))
					{
						nHeight = 6/*12*/;
						if(int(ii*nOneWidth) - m_nScrollLeft >= 0)
							paint->drawLine(nTopMarin,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft, nTopMarin-nHeight,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft);

						if(abs(ii-TB1)>10 && abs(ii-TB2)>10)
						{
							TimeDrawn = (time/1000L);

							//paint->setPen(m_pDispAttr->m_TimeForeColor);
							bDrawText = TRUE;
						}
					}
				}
				else if(m_pComTrade->GetSampleRateByBlock(TR) <= 20)		
				{	
					int time1 = timeArray.at(ii-1);

					// 200ms点
					if((time1 % 200000L) >= (time % 200000L))
					{
						nHeight = 4/*8*/;
						if(int(ii*nOneWidth) - m_nScrollLeft >= 0)
							paint->drawLine(nTopMarin,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft, nTopMarin-nHeight,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft);
					}

					// 10000ms点
					if((time1 % 1000000L) >= (time % 10000000L))
					{
						nHeight = 6/*12*/;
						if(int(ii*nOneWidth) - m_nScrollLeft >= 0)
							paint->drawLine(nTopMarin,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft, nTopMarin-nHeight,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft);

						if(abs(ii-TB1)>10 && abs(ii-TB2)>10)
						{
							TimeDrawn = (time/1000L);
							//paint->setPen( QColor( 0,0,64 ) );
							//paint->setPen(m_pDispAttr->m_TimeForeColor);
							bDrawText = TRUE;
						}
					}
				}
				else if(m_pComTrade->GetSampleRateByBlock(TR) <= 50)		
				{	
					int time1 = timeArray.at(ii-1);
					// 100ms点
					if((time1 % 100000L) >= (time % 100000L))
					{
						nHeight = 4/*8*/;
						if(int(ii*nOneWidth) - m_nScrollLeft >= 0)
							paint->drawLine(nTopMarin,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft, nTopMarin-nHeight,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft);
					}
					// 500ms点
					if((time1 % 500000L) >= (time % 500000L))
					{
						nHeight = 6/*12*/;
						//if(int((ii-Tick1)*nOneWidth)  >= 0)
						paint->drawLine(nTopMarin,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft, nTopMarin-nHeight,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft);

						if(abs(ii-TB1)>10 && abs(ii-TB2)>10)
						{
							TimeDrawn = (time/500000L)*500;
							//paint->setPen( QColor( 0,0,64 ) );
							//paint->setPen(m_pDispAttr->m_TimeForeColor);
							bDrawText = TRUE;
						}
					}
				}
				else 
				{
					int time1 = timeArray.at(ii-1);
					// 5ms点
					nHeight = 4/*8*/;
					if((time1 % 5000L) >= (time % 5000L))
					{
						//if(int(ii*nOneWidth) - m_nScrollLeft >= 0)
						paint->drawLine(nTopMarin-nHeight,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft,nTopMarin,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft);
					}
					// 20ms点
					nHeight = 6/*12*/;
					if((time1 % 20000L) >= (time % 20000L))
					{
						//if(int(ii*nOneWidth) - m_nScrollLeft >= 0)
						paint->drawLine(nTopMarin,nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft,nTopMarin-nHeight, nLeftMargin + (ii-Tick1)*nOneWidth - m_nScrollLeft);

						if(abs(ii-TB1)>10 && abs(ii-TB2)>10)
						{
							TimeDrawn = (time/20000L)*20;
							//paint->setPen( QColor( 0,0,64 ) );
							//paint->setPen(m_pDispAttr->m_TimeForeColor);
							bDrawText = TRUE;
						}
					}
				}
			}

			if(bDrawText /*&& (int(ii*nOneWidth) - nLeftMargin >= 0)*/)
			{
				TimeDrawn = time/1000L;
				TimeDrawn += m_pComTrade->GetFirstDataTimeMs();
				nY = nLeftMargin + (ii-Tick1)*nOneWidth ;
				//, /*18*/  TEXT_WIDTH, TEXT_WIDTH);

				QString sComment;					
				if (TimeDrawn < 0)    sComment.sprintf("-%ld", -1*TimeDrawn);
				else if((TimeDrawn < 1000)&&(TimeDrawn >= 0))	sComment.sprintf("%ld", TimeDrawn);
				else				sComment.sprintf("%d\"%03d", TimeDrawn/1000L, TimeDrawn%1000L);

				QFontMetrics metric(newFont);

                rcPoint = QRect(2+m_nLeftMargin,nY - 5,metric.width(sComment),metric.height());
				paint->save();
                paint->translate(2+m_nLeftMargin,nY-5);
				paint->rotate(90);
				paint->drawText(0,0,sComment);

				//paint->drawText(rcPoint,Qt::AlignLeft | Qt::TextSingleLine,sComment);
				//paint->rotate(-90);
				paint->restore();
			}
		}
	}
	//}



	paint->setFont(m_font);
	paint->setPen(oldPen);
}

void CWavePrinter::SetDigitalChannel(QList<int>&lst)
{
    m_DigitalChannelLst = lst;
    foreach(int i,m_DigitalChannelLst)
    {
        printf("Insert %d to DigitalChannelLst\n",i);
    }
}

void CWavePrinter::SetAnalogChannel(QList<int>&lst)
{
    m_AnalogChannelLst = lst;
    foreach(int i,m_AnalogChannelLst)
    {
        printf("Insert %d to DigitalChannelLst\n",i);
    }
}
