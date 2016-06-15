#include "CASCIIFile.h"
#include <QtDebug>
#include <QStringList>
#include <QTextCodec>
#include <QTextStream>
CASCIIFile::CASCIIFile ()
{

}

CASCIIFile::~CASCIIFile()
{

}

bool CASCIIFile::ReadAsciiFile(const QString&  strFile,unsigned int nACount,
							   unsigned int nDCount,ChanelDataList& dataList)
{
	QFile file(strFile);
	if (!file.exists())
	{
		m_ErrorList << QString("文件%1不存在，读取出错.").arg(strFile);
		return false;
	}
	if (!file.open(QIODevice::ReadOnly))
	{
		m_ErrorList << QString("文件%1无法打开，读取出错.").arg(strFile);
		return false;
	}
    QTextCodec * tmCode = QTextCodec::codecForName("GBK");
    QTextStream flStream(&file);
    flStream.setCodec(tmCode);
    QString strLine = flStream.readLine().trimmed();
	
    //QByteArray fileLine = file.readLine();

	int nLine = 1;
    while(!strLine.isEmpty())
	{
        QChar cc = strLine.at(0) ;
        if (strLine.count() == 1 && cc == 26)  //dat和mid文件结尾由一个值为26的字符
		{
			break;
		}
        //QString strLine = QString::fromAscii(fileLine.data());//QString(fileLine).trimmed();
	
		//以逗号分隔的数据
		QStringList valueList = strLine.split(",");
		if (valueList.count() != (nACount+nDCount+2))
		{
			valueList = strLine.split("\t",QString::SkipEmptyParts);
			if (valueList.count() != (nACount+nDCount+2))
			{
				valueList = strLine.split(" ",QString::SkipEmptyParts);
				if (valueList.count() != (nACount+nDCount+2))
				{
					m_ErrorList << QString("文件%1行%2内容错误.").arg(strFile).arg(nLine);
					file.close();
					return false;
				}
			}
		}

		
		bool bOk = true;
		CChanelSampleData chanelData;
		chanelData.m_nTick = valueList.at(0).trimmed().toUInt(&bOk);
		if (!bOk)
		{
			m_ErrorList << QString("文件%1行%2采样数值错误.").arg(strFile).arg(nLine);
			file.close();
			return false;
		}
		chanelData.m_nTimeStamp = valueList.at(1).trimmed().toUInt(&bOk);
		if (!bOk)
		{
			m_ErrorList << QString("文件%1行%2采样时戳错误.").arg(strFile).arg(nLine);
			file.close();
			return false;
		}
		unsigned int nIndex=0;
		while(nIndex < nACount)
		{
			int nValue = valueList.at(2+nIndex).trimmed().toInt(&bOk);
			if (!bOk)
			{
				m_ErrorList << QString("文件%1行%2模拟通道数据%3错误.")
					.arg(strFile).arg(nLine).arg(nIndex+1);
				file.close();
				return false;
			}
			chanelData.m_AChanelDataList <<nValue;
			if(nValue>99998||nValue<-99999)
			{
				chanelData.m_AChanelDataValidList <<false;
			}
			else
			{
				chanelData.m_AChanelDataValidList <<true;
			}
			nIndex++;
		}
		nIndex=0;
		while(nIndex < nDCount)
		{
			unsigned short nValue = valueList.at(2 + nACount + nIndex).trimmed().toUShort(&bOk);
			if (!bOk)
			{
				m_ErrorList << QString("文件%1行%2数字通道数据%3错误.")
					.arg(strFile).arg(nLine).arg(nIndex+1);
				return false;
			}
			chanelData.m_DChanelDataList <<nValue;
			nIndex++;
		}
		dataList << chanelData;
		nLine++;
        strLine = file.readLine().trimmed();
	}

	file.close();
	return true;
}
