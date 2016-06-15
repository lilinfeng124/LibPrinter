#include "CTmpGUI.h"
#include <QtGui/QApplication>
#include <QTextCodec>
//#include <qDebug>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTextCodec * codec = QTextCodec::codecForName("utf8");
	QTextCodec::setCodecForCStrings(codec);
	TmpGUI w;
	w.show();
	
	return a.exec();
	// 	char * str ="1234567890";
	// 	short Rest = (short)(*(str+2));
	// 	qDebug()<<Rest;
	// 	Rest = *((short *)(str+4));
	// 	qDebug()<<Rest;


}
