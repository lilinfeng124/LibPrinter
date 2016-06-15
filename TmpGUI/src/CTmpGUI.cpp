#include "CTmpGUI.h"
#include <QPaintEvent>
#include <QPainter>
#include <QFileDialog>
#include "CComTradeFile.h"
#include "CPrinterDlg.h"
TmpGUI::TmpGUI(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	m_pComtradeFile = NULL;
	connect(ui.m_btnOpen,SIGNAL(clicked()),this,SLOT(OpenSlot()));
	connect(ui.m_btnPrint,SIGNAL(clicked()),this,SLOT(PrintViewSlot()));
}

TmpGUI::~TmpGUI()
{

}

// void test::paintEvent(QPaintEvent *)
// {
// 	QPainter painter(this);
// 	QFont font("Arial",10);
// 	QFontMetrics metrics(font); 
// 	QString strText("test");
// 	QRectF rect(200,200,metrics.width(strText),metrics.height());
// 	//painter.drawText(rect.strText);
// 	painter.save(); // 淇濆瓨鍧愭爣绯荤姸鎬?
// 	painter.translate(200+metrics.width(strText)/2,200+metrics.height()/2);
// 	painter.rotate(90);
// 	painter.drawText(0,0,strText);
// 	painter.drawLine(0,0,50,0);
// 	painter.restore(); //鎭㈠浠ュ墠鐨勫潗鏍囩郴鐘舵€?
// 	painter.drawLine(0,0,50,0);
// }
bool TmpGUI::OpenComtradeFile(QString strFilePath)
{
	delete m_pComtradeFile;
	m_pComtradeFile = new CComTradeFile;

	bool  b = m_pComtradeFile->ReadComTradeFile(strFilePath);
	if(b)
	{
		m_IsLoadComtrade = true;

		//emit LoadComtradeFileSignal(0);
	}
	return b;
}

void TmpGUI::OpenSlot()
{
	QString strFile = QFileDialog::getOpenFileName(this, tr("Open File"),
		".",
		tr("Images (*.cfg *.dat *.des *.hdr *.mid *.rpt)"));
	OpenComtradeFile(strFile);
}

void TmpGUI::PrintViewSlot()
{
	CComTradeFile* pFile = m_pComtradeFile;
	if(pFile)
	{
		CPrinterDlg dlg;
		dlg.SetComTrade(pFile);
		dlg.exec();
	}
}