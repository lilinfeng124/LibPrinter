#ifndef CWAVEPRINTER_H
#define CWAVEPRINTER_H
#include "CLibPrinter.h"
#include "CPrinterObject.h"
#include <QFont>
#include <QStringList>
#include <QRectF>
class QPrinter;

//打印设置属性
// class PRINTER_API PRINTPARAM{
// public:
// 	double		dLeftMargin;//边距
// 	double		dTopMargin;
// 	double		dRightMargin;
// 	double		dBottomMargin;
// 
// 	bool		bColor;			//彩色或黑白
// 	bool		bZonePrint;		//区域打印或全部打印
// 	bool		bAnaPrint;		//模拟
// 	bool		bDgtPrint;		//状态
// 	//bool		bTimePrint;		//时间
// 	bool		bNamePrint;		//名称
// 	//bool		bTitlePrint;	//页眉与页角
// 	bool		bValuePrint;	//当前时标值
// 	bool		bOrient;		//方向true 横向、false纵向
// 	bool        bCurrentXAxis;  //当前横轴比例
// 	bool        bLabelPrint;    //打印标签
// 	bool        bShowPrintTime;
// 	bool        bShowPrintPage;
// 	bool        bShowWaveLine;
// 
// 	double      dXAxisScale;
// 	double      dYAxisScale;
// 
// 	QString     strTitle;
// 
// 	QStringList m_listAnaPrint; //打印模拟通道
// 	QStringList m_listDgtPrint; //打印状态通道
// };

class PRINTER_API PgContent
{
public:
	int m_nStartChannel;
	int m_nblock;
	int m_nStartTick;
	
};

class PRINTER_API CWavePrinter:public CPrinterObject
{
public:
	CWavePrinter(QPrinter*printer,CComTradeFile*pObj);
	void DoPrinter(QPainter*p);

	//画模拟波形
	void PrintAnalogWave(QPainter*p);
	//画数字波形
	void PrintDigitalWave(QPainter*p);

    void SetDigitalChannel(QList<int>&lst);
    void SetAnalogChannel(QList<int>&lst);
private:

	//画中间虚线
	void drawAnalogChannelLine(QPainter *paint,PgContent*pPg);

	//画一截波形图
	void drawWave(QPainter *paint,PgContent*pPage);

	//画表
	void PrintTable(QPainter*p,PgContent*pPg);

	void PrintHeader(QPainter*p);

    QRectF CenterInTable(QPainter * p,int x,QString str);

	//计算页面等数据
	void CalculateAnalagPageNumber(QPainter*p);

	void PrintAnalogTitle(QPainter*p);

	int CalculateTableHeight(int nChannelNum);

	int SumBlockTick(int block);

	//画刻度
	void drawTimeAxisB( QPainter *paint ,PgContent*pPg);

	void CalculateDigitalPageNumber(QPainter*p);
	
	void PrintDigitalTitle(QPainter*p);

	void PrintDigitalTable(QPainter*p,PgContent*pPg);

	void drawDigitalWave(QPainter *paint,PgContent*pPage);

	void drawDigitalTimeAxisB( QPainter *paint ,PgContent*pPg);

	void drawDigitalChannelLine(QPainter *paint,PgContent*pPg);
private:
	//QPixmap m_pixMap; 
	int     m_nCurrentY;
	int     m_nTableColum;	
	QFont   m_font;
	//刻度尺的高度
	int m_nScaleWidth;
	int m_nScrollTop;
	
	QList<PgContent*> m_pPgContentLst;
	//每一页打多少通道
	int m_nAnalogChannelNum;
	//每一页打多少个点
	int m_nAnalogTickNum;

	//模拟通道宽度
	int m_nAnalogChannelHeight;
	//数字通道宽度
	int m_nDigitalChannelHeight;

	//每一页打多少通道
	int m_nDigitalChannelNum;
	//每一页打多少个点
	int m_nDigitalTickNum;

    //digital Channel need print
    QList<int>m_DigitalChannelLst;

    //analog channel need print
    QList<int>m_AnalogChannelLst;
};

#endif
