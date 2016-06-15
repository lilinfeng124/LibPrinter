#include "CGenericTable.h"
#include <QPainter>
#include <QPrinter>
#include <QFontMetrics>
const int minStep = 5;
TableNode::TableNode(TableNode*parent):m_pParent(parent)
{
	m_lineCount = 0;
	m_leftCount = 0;
	
	m_strName = "";
	m_strValue = "";


	if(m_pParent==NULL)
	{
		m_nLevel = 0;
	}
	else
	{
		m_nLevel = m_pParent->GetLevel()+1;
	}
}

TableNode::~TableNode()
{
	foreach(TableNode*pNode,m_pSonNodeLst)
	{
		delete pNode;
	}
	m_pSonNodeLst.clear();
}

int TableNode::GetLevel()
{
	return m_nLevel;
}

TableNode* TableNode::CreateSon(QString str,QString strValue)
{
	TableNode * pSon = new TableNode(this);
	pSon->m_strName = str;
	pSon->m_strValue = strValue;
	m_pSonNodeLst.append(pSon);
	return pSon;
}

int TableNode::CalculateLineCount()
{
	if(m_pSonNodeLst.count()==0)
	{
		return 1;
	}
	else
	{
		m_lineCount = 0;
		foreach(TableNode*pSon,m_pSonNodeLst)
		{
			m_lineCount+=pSon->CalculateLineCount();
		}
		m_leftCount = m_lineCount;
		return m_lineCount;	
	}
}

CGenericTable::CGenericTable(QPrinter* printer):CPrinterObject(printer,NULL)
{
	m_nTableColum = 45;
	m_font=QFont("Arial",12);
	m_nCurrentY = 0;
}

CGenericTable::~CGenericTable()
{
    foreach(TableNode*p,m_NodeLst)
    {
        delete p;
    }
}

void CGenericTable::DoPrinter(QPainter*p)
{
    m_pPrinter->newPage();

    PrintTitle(p);

	PrintHeader(p);

	PrintTable(p);
}

void CGenericTable::PrintTitle(QPainter*p)
{
    //make the character bigger
    QFont TitleFont("Arial",15);
	p->setPen(Qt::black);

    p->setFont(TitleFont);

	int textWidth = p->window().width();

    QFontMetrics metics(TitleFont);

	int nTitleWidth = metics.width(m_strTitle);
	int nTitleHeight = metics.height(); 
	//QRectF rect = p->boundingRect(0,0,textWidth,maxHeight,Qt::AlignCenter,strDeviceName);


    QRectF rect((textWidth-nTitleWidth)/2,m_nTopMargin,nTitleWidth/*(textWidth+nTitleWidth)/2*/,(nTitleHeight+minStep));
	p->drawText(rect,m_strTitle);
	m_nCurrentY = rect.y()+nTitleHeight;
    //reset
    p->setFont(m_font);
}

void CGenericTable::SetTitle(QString& str)
{
	m_strTitle = str;
}

void CGenericTable::SetDetailDesc(QString& str)
{
	m_strDetail = str;
}

void CGenericTable::PrintHeader(QPainter*p)
{
	int textWidth = p->window().width()-m_nLeftMargin-m_nRightMargin;
	m_nCurrentY = m_nCurrentY+minStep;
	p->drawLine(m_nLeftMargin,m_nCurrentY,m_nLeftMargin+textWidth,m_nCurrentY);
	m_nCurrentY = m_nCurrentY+minStep*2;
	
	QFontMetrics metics(m_font);
	int nActualWidth = metics.width(m_strDetail);
	int nCrHeight = metics.height();
	int nLineCount = nActualWidth/textWidth+1;
    QRectF rect(m_nLeftMargin,m_nCurrentY,textWidth,nLineCount*(nCrHeight+minStep));
	p->drawText(rect,Qt::AlignCenter|Qt::TextWordWrap,m_strDetail);
    m_nCurrentY=m_nCurrentY+nLineCount*(nCrHeight+minStep);
}

void CGenericTable::SetTableHeader(const QStringList& lst)
{
	m_strTableHeaderList = lst;
}



void CGenericTable::CalculateLine(QPainter*p)
{
//计算所有node的行
	foreach(TableNode* pNode,m_NodeLst)
	{
		pNode->CalculateLineCount();
	}
	QFontMetrics metric(m_font);
	//int nWordHeight = metric.height();
	int nTotalWidth = p->window().width();
	//int nTotalHeight = p->window().height();
	//获取列数
/*	if(m_NodeLst.count()==0)
	{
		return;
	}
	TableNode* p = m_NodeLst.at(0);
	int depth = 1;
	while(p->m_NodeLst.count()>0)
	{
		p = p->m_NodeLst.at(0);
		depth++;
	}*/
	int depth = m_strTableHeaderList.count();
	//得到所有的底端节点
	foreach(TableNode*pTblNode,m_NodeLst)
	{
		GetNodesByLvl(pTblNode,depth-1,m_TmpNodeLst);
	}
	//最后一列要占宽度的一半，其他几列均分另外一半
	//左边线
	//p->drawLine(m_nLeftMargin,m_nCurrentY,m_nLeftMargin,nTotalHeight-m_nBottomMargin);
	//右边线
	//p->drawLine(nTotalHeight-m_nRightMargin,m_nCurrentY,nTotalWidth-m_nRightMargin,nTotalHeight-m_nBottomMargin);
	//上边线
	//p->drawLine(m_nLeftMargin,m_nCurrentY,nTotalWidth-m_nRightMargin,m_nCurrentY);
	//下边线
	//p->drawLine(m_nLeftMargin,nTotalHeight-m_nBottomMargin,nTotalWidth-m_nRightMargin,nTotalHeight-m_nBottomMargin);
	//列线
	int halfWidth = (nTotalWidth-m_nLeftMargin-m_nRightMargin)/2+m_nLeftMargin;
	QList<int>spanLst;
    spanLst.prepend(nTotalWidth-m_nRightMargin);
    spanLst.prepend(halfWidth);
	
    for(int i=depth-2;i>0;i--)
	{
        int divideWidth = (nTotalWidth-m_nLeftMargin-m_nRightMargin)/(2*(depth-1));
	
		int divideX = m_nLeftMargin+divideWidth*i;
        spanLst.prepend(divideX);
		//p->drawLine(divideX,m_nCurrentY,divideX,nTotalHeight-m_nBottomMargin);
	}
	m_SpanLst = spanLst;
}

void CGenericTable::PrintTable(QPainter*p)
{
	CalculateLine(p);
	//开始画table	
	m_nCurrentNodeIndex = 0;

	DrawTableHeader(p);
	DrawTableContent(p);

	
}

void CGenericTable::NewPage(QPainter*p)
{
	m_nCurrentY = 0;
	m_pPrinter->newPage();
	
	DrawTableHeader(p);
	DrawTableContent(p);
}

void CGenericTable::GetNodesByLvl(TableNode*p,int nLvel,QList<TableNode*>&lst)
{
	if(p->GetLevel()==nLvel&&!lst.contains(p))
	{
		lst.append(p);	
	}
	else
	{
		foreach(TableNode* pSon,p->m_pSonNodeLst)
		{
			GetNodesByLvl(pSon,nLvel,lst);
		}	
	}
}

void CGenericTable::DrawTableHeader(QPainter*p)
{
	QFontMetrics fontMetrics(m_font);
	//int depth = m_strTableHeaderList.count();
	int nWordHeight = fontMetrics.height();
	int nTotalHeight = p->window().height();
	int nTotalWidth = p->window().width();

    //计算需要多少行
    int nTableNumber = (nTotalHeight-m_nBottomMargin-m_nCurrentY)/(minStep+nWordHeight);
    int nActualHeight = nTableNumber*(minStep+nWordHeight);
	//最后一列要占宽度的一半，其他几列均分另外一半
	//左边线
    p->drawLine(m_nLeftMargin,m_nCurrentY,m_nLeftMargin,m_nCurrentY+nActualHeight/*nTotalHeight-m_nBottomMargin*/);
	//右边线
    p->drawLine(nTotalWidth-m_nRightMargin,m_nCurrentY,nTotalWidth-m_nRightMargin,m_nCurrentY+nActualHeight/*nTotalHeight-m_nBottomMargin*/);
	//上边线
	p->drawLine(m_nLeftMargin,m_nCurrentY,nTotalWidth-m_nRightMargin,m_nCurrentY);
	//下边线
	//p->drawLine(m_nLeftMargin,nTotalHeight-m_nBottomMargin,nTotalWidth-m_nRightMargin,nTotalHeight-m_nBottomMargin);
	//表头
    int i = m_SpanLst.count()-1;
	int lineCount = 1;
    while(i>=0)
	{
		int XRightPos = m_SpanLst.at(i);
        int XLeftPos;
        if(i>0)
        {
            XLeftPos = m_SpanLst.at(i-1);
        }
        else
        {
            XLeftPos = m_nLeftMargin;
        }
        int nWidth = XRightPos-XLeftPos-minStep;
		QString strHeader = m_strTableHeaderList.at(i);
		int nActualWidth = fontMetrics.width(strHeader);
		//有可能会超过宽度，计算行数
        int nTmpCount = 1;
        if(nActualWidth>nWidth)
        {
            nTmpCount = nActualWidth%(nWidth-minStep)==0?nActualWidth/(nWidth-minStep):nActualWidth/(nWidth-minStep)+1;
            if(nTmpCount>lineCount)
            {
                lineCount = nTmpCount;
            }
        }
        //画列线
		//p->drawLine(divideX,m_nCurrentY,divideX,nTotalHeight-m_nBottomMargin);
		//写字
		//QRectF rect(XLeftPos,m_nCurrentY,nWidth,lineCount*nWordHeight);
		//p->drawText(rect,Qt::AlignCenter|TextWordWrap,strHeader);
		i--;	
	}
	//算完后再写字
    i = m_SpanLst.count()-1;
    while(i>=0)
	{
		int XRightPos = m_SpanLst.at(i);
        int XLeftPos;
        if(i>0)
        {
            XLeftPos = m_SpanLst.at(i-1);
        }
        else
        {
            XLeftPos = m_nLeftMargin;
        }
		int nWidth = XRightPos-XLeftPos;
		QString strHeader = m_strTableHeaderList.at(i);
		//画列线
        p->drawLine(XRightPos,m_nCurrentY,XRightPos,m_nCurrentY+nActualHeight/*nTotalHeight-m_nBottomMargin*/);
		//写字
        QRectF rect(XLeftPos+minStep/2,m_nCurrentY+minStep/2,nWidth-minStep,lineCount*(nWordHeight*minStep));
        //cannot use AlignCenter ,it looks like a bug.
        if(lineCount>0)
        {
            p->drawText(rect,Qt::AlignHCenter|Qt::TextWrapAnywhere,strHeader);
        }
        else
        {
            p->drawText(rect,Qt::AlignCenter|Qt::TextWrapAnywhere,strHeader);
        }
		i--;	
	}
	//画下线
    p->drawLine(m_nLeftMargin,m_nCurrentY+lineCount*(nWordHeight+minStep),nTotalWidth-m_nRightMargin,m_nCurrentY+lineCount*(nWordHeight+minStep));
    m_nCurrentY = m_nCurrentY+lineCount*(nWordHeight+minStep);
	
}

void CGenericTable::DrawTableContent(QPainter*p)
{
	QFontMetrics metric(m_font);

	int nWordHeight = metric.height();
	int nTotalWidth = p->window().width();
	int nTotalHeight = p->window().height();
	int nActualWidth = nTotalWidth-m_nLeftMargin-m_nRightMargin;
	
    //表格内容开始点的Y位置
    int nTableStartY = m_nCurrentY;
	//先画最右边的底层Node

	TableNode* pNode = m_TmpNodeLst.at(m_nCurrentNodeIndex);

    while(//m_nCurrentY+nWordHeight+minStep<nTotalHeight&&
	m_nCurrentNodeIndex<m_TmpNodeLst.count())
	{
		int nColumnIndex = m_SpanLst.count()-1;
        pNode = m_TmpNodeLst.at(m_nCurrentNodeIndex);
		TableNode* pCurrentNode = pNode;
        while(nColumnIndex>=0&&pCurrentNode)
		{
			int XRightPos = m_SpanLst.at(nColumnIndex);
            int XLeftPos;//= m_SpanLst.at(nColumnIndex-1);
            if(nColumnIndex!=0)
            {
                XLeftPos = m_SpanLst.at(nColumnIndex-1);
            }
            else
            {
                XLeftPos = m_nLeftMargin;
            }
			if(nColumnIndex==m_SpanLst.count()-1)
			{
                QRectF rect(XLeftPos+minStep/2,m_nCurrentY,XRightPos-XLeftPos-minStep,m_nCurrentY);
				int nMaohaoWidth = metric.width(":");
				QRectF rectMaohao(4*nActualWidth/5,m_nCurrentY,nMaohaoWidth,nWordHeight);

				int nValueWidth = metric.width(pNode->m_strValue); 
                QRectF rectValue(XRightPos-nValueWidth-minStep,m_nCurrentY,nValueWidth,nWordHeight);
				p->drawText(rect,pNode->m_strName);
                //不要画冒号
                if(XRightPos-nValueWidth-minStep>4*nActualWidth/5)
                {
                    p->drawText(rectMaohao,QString(":"));
                }
				p->drawText(rectValue,pNode->m_strValue);
                p->drawLine(XLeftPos,m_nCurrentY+(minStep+nWordHeight),XRightPos,m_nCurrentY+(minStep+nWordHeight));
			
			}
			else
			{
				pCurrentNode->m_leftCount--;
				if(pCurrentNode->m_leftCount==0)
				{
                    int gridHeight = pCurrentNode->m_lineCount*(nWordHeight+minStep);
                    int gridYStart = m_nCurrentY-(pCurrentNode->m_lineCount-1)*(nWordHeight+minStep);

                    if(gridYStart<nTableStartY)
					{
                        gridYStart = nTableStartY;
                        gridHeight = m_nCurrentY+(nWordHeight+minStep)-nTableStartY;
                    }

                        QRectF rectGrid(XLeftPos,gridYStart,XRightPos-XLeftPos,gridHeight);
						p->drawText(rectGrid,Qt::AlignCenter|Qt::TextWordWrap,pCurrentNode->m_strName);

					//画下面的线
                    p->drawLine(XLeftPos,m_nCurrentY+(minStep+nWordHeight),XRightPos,m_nCurrentY+(minStep+nWordHeight));
                }
                //如果没有数完就到最后了，也要打印名称
                else if((m_nCurrentY+2*(nWordHeight+minStep))>(nTotalHeight-m_nBottomMargin))
                {
                    int gridHeight = (pCurrentNode->m_lineCount-pCurrentNode->m_leftCount)*(nWordHeight+minStep);
                    int gridYStart = m_nCurrentY+(nWordHeight+minStep)-gridHeight;

                    if(gridYStart<nTableStartY)
                    {
                        gridYStart = nTableStartY;
                        gridHeight = nTotalHeight-m_nBottomMargin-nTableStartY;//m_nCurrentY+(nWordHeight+minStep);
                    }

                        QRectF rectGrid(XLeftPos,gridYStart,XRightPos-XLeftPos,gridHeight);
                        p->drawText(rectGrid,Qt::AlignCenter|Qt::TextWordWrap,pCurrentNode->m_strName);

                    //画下面的线
                    p->drawLine(XLeftPos,m_nCurrentY+(minStep+nWordHeight),XRightPos,m_nCurrentY+(minStep+nWordHeight));
                }
			}
			nColumnIndex--;
			pCurrentNode = pCurrentNode->m_pParent;
		}
        m_nCurrentNodeIndex++;
        m_nCurrentY+=(nWordHeight+minStep);
        if(m_nCurrentY+(nWordHeight+minStep)>nTotalHeight-m_nBottomMargin)
        {
            break;
        }
	}
	//说明没打印完毕，继续下一页打印
	if(m_nCurrentNodeIndex<m_TmpNodeLst.count())
	{
		NewPage(p);
	}
    else
    {
        m_nCurrentNodeIndex = 0;
        ClearLst();
    }


}

void CGenericTable::AddToNodeLst(TableNode*p)
{
    if(!m_NodeLst.contains(p))
    {
        m_NodeLst.append(p);
    }
}

void CGenericTable::ClearLst()
{
    foreach(TableNode*p,m_NodeLst)
    {
        delete p;
    }
    m_NodeLst.clear();
}
