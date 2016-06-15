#ifndef CTMPGUI_H
#define CTMPGUI_H

#include <QtGui/QMainWindow>
#include "ui_TmpGUI.h"
class QPaintEvent;
class CComTradeFile;
class TmpGUI : public QMainWindow
{
	Q_OBJECT

public:
	TmpGUI(QWidget *parent = 0, Qt::WFlags flags = 0);
	~TmpGUI();

    bool OpenComtradeFile(QString strFilePath);

public slots:
	void OpenSlot();
	void PrintViewSlot();
protected:
	//void paintEvent(QPaintEvent*);
private:
	Ui::testClass ui;

	CComTradeFile * m_pComtradeFile;

	bool            m_IsLoadComtrade;

};

#endif // TEST_H
