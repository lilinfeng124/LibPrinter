#include "CCfgFile.h"
#include <QtDebug>
#include <QStringList>
#include <QTextStream>
#include <QTextCodec>
CCfgFile::CCfgFile ()
{
	m_nAChanelCount = 0;	//A通道数量0~999999
	m_nDChanelCount = 0;	//D通道数量0~999999
	m_nNrates = 0;	
	m_bBin = true;
	m_fTimeMult = 1.0;
}

CCfgFile::~CCfgFile()
{

}


bool CCfgFile::ReadCfgFile(const QString& strFile)
{
	m_strFile = strFile;
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


	unsigned int nLine = 0;

    QString strLine = ReadNextLineString(flStream,nLine);
	if (!ParseFirstLine(strLine,nLine))
	{
		file.close();
		return false;
	}

    strLine = ReadNextLineString(flStream,nLine);
	if (!ParseSecondLine(strLine,nLine))
	{
		file.close();
		return false;
	}

	//模拟通道信息
	unsigned int nIndex = 0;
	while(nIndex < m_nAChanelCount)
	{
        strLine = ReadNextLineString(flStream,nLine);
		if (!ParseAChanelInfo(strLine,nLine))
		{
			file.close();
			return false;
		}
		nIndex++;
	}
	
	//数字通道信息
	nIndex = 0;
	while(nIndex < m_nDChanelCount)
	{
        strLine = ReadNextLineString(flStream,nLine);
		if (!ParseDChanelInfo(strLine,nLine))
		{
			file.close();
			return false;
		}
		nIndex++;
	}

    strLine = ReadNextLineString(flStream,nLine);
	bool bOk = true;
	m_fIF = strLine.trimmed().toDouble(&bOk);
	if (!bOk)
	{
		m_ErrorList << QString("文件%1行%2线路频率解析错误.").arg(m_strFile).arg(nLine);
		file.close();
		return false;
	}
	//采样信息
    strLine = ReadNextLineString(flStream,nLine);
	m_nNrates = strLine.trimmed().toDouble(&bOk);
	if (!bOk)
	{
		m_ErrorList << QString("文件%1行%2采样次数解析错误.").arg(m_strFile).arg(nLine);
		file.close();
		return false;
	}

    strLine = ReadNextLineString(flStream,nLine);
	if (!ParseSampleInfo(strLine,nLine))
	{
		file.close();
		return false;
	}
	if (m_nNrates == 0)
	{
		CSampleInfo sampleInfo = m_SampleInfoList.at(0);
		if (sampleInfo.m_fSamp != 0.0)
		{
			m_ErrorList << QString("文件%1行%2连续采样时采样率不为零.").arg(m_strFile).arg(nLine);
			file.close();
			return false;
		}
	}
	else
	{
		unsigned short nSam = 1;
		while(nSam < m_nNrates)
		{
            strLine = ReadNextLineString(flStream,nLine);
			if (!ParseSampleInfo(strLine,nLine))
			{
				file.close();
				return false;
			}
			nSam++;
		}
	}

	//数据文件第一个数据时戳
    strLine = ReadNextLineString(flStream,nLine);
	if (!StringToDateTime(strLine,m_dtFirstDataTime))
	{
		m_ErrorList << QString("文件%1行%2解析第一个数据时戳出错.").arg(m_strFile).arg(nLine);
		file.close();
		return false;
	}

	//触发点时间
    strLine = ReadNextLineString(flStream,nLine);
	if (!StringToDateTime(strLine,m_dtTriggerTime))
	{
		m_ErrorList << QString("文件%1行%2解析触发点时间出错.").arg(m_strFile).arg(nLine);
		file.close();
		return false;
	}

    strLine = ReadNextLineString(flStream,nLine);
	if(strLine.trimmed().compare("BINARY",Qt::CaseInsensitive) == 0)
	{
		m_bBin = true;
	}
	else
	{
		m_bBin = false;
	}

	//时间标记乘数系数
    strLine = ReadNextLineString(flStream,nLine);
	bOk = true;
	m_fTimeMult = strLine.trimmed().toDouble(&bOk);
	if (!bOk)
	{
		m_ErrorList << QString("文件%1行%2时间标记乘数系数解析错误.").arg(m_strFile).arg(nLine);
		file.close();
		return false;
	}

	file.close();
	return true;
}

bool CCfgFile::StringToDateTime(const QString& strLine,CExDateTime& dtTime)
{
	QStringList timeList = strLine.split(".");
	if (timeList.count() != 2)
	{
		return false;
	}
	dtTime.m_dtTime = QDateTime::fromString(timeList.at(0).trimmed(),"dd/MM/yyyy,hh:mm:ss");
	bool bOk = true;
	dtTime.m_nWs = timeList.at(1).toUInt(&bOk);
	if (!bOk)
	{
		return false;
	}
	return true;
}

QString CCfgFile::ReadNextLineString( QTextStream& flStream,unsigned int& nLine)
{
	nLine++;
    //QTextCodec * tmCode = QTextCodec::codecForName("GBK");
    //QTextStream flStream(&file);
    //flStream.setCodec(tmCode);
    QString strLine = flStream.readLine();
    //QByteArray fileLine = file.readLine();
    //QString strLine = QString::fromAscii(fileLine.data());//fileLine;
	return strLine;
}

bool CCfgFile::ParseFirstLine(const QString& strLine,int nLine)
{
	//以逗号分隔的数据
	QStringList valueList = strLine.split(",");
	if (valueList.count() != 3)
	{
		m_ErrorList << QString("文件%1行%2内容错误.").arg(m_strFile).arg(nLine);
		return false;
	}

	//第一行 站名 装置特征 Comtrade标准版本
	m_strSite = valueList.at(0).trimmed();
	m_strDevice = valueList.at(1).trimmed();
	m_strVersion = valueList.at(2).trimmed();

	return true;
}

bool CCfgFile::ParseSecondLine(const QString& strLine,int nLine)
{
	//以逗号分隔的数据
	QStringList valueList = strLine.split(",");
	if (valueList.count() != 3)
	{
		m_ErrorList << QString("文件%1行%2内容错误.").arg(m_strFile).arg(nLine);
		return false;
	}
	bool bOk = true;
	unsigned int nTotal = valueList.at(0).trimmed().toUInt(&bOk);
	if (!bOk)
	{
		m_ErrorList << QString("文件%1行%2通道总数解析错误.").arg(m_strFile).arg(nLine);
		return false;
	}
	QString strAChanelCount = valueList.at(1).trimmed();
	if (!strAChanelCount.endsWith("A"))
	{
		m_ErrorList << QString("文件%1行%2模拟通道数目解析错误.").arg(m_strFile).arg(nLine);
		return false;
	}
	m_nAChanelCount = strAChanelCount.left(strAChanelCount.count()-1).toUInt(&bOk);

	if (!bOk)
	{
		m_ErrorList << QString("文件%1行%2模拟通道数目解析错误.").arg(m_strFile).arg(nLine);
		return false;
	}
	QString strDChanelCount = valueList.at(2).trimmed();
	if (!strDChanelCount.endsWith("D"))
	{
		m_ErrorList << QString("文件%1行%2数字通道数目解析错误.").arg(m_strFile).arg(nLine);
		return false;
	}
	m_nDChanelCount = strDChanelCount.left(strDChanelCount.count()-1).toUInt(&bOk);

	if (!bOk)
	{
		m_ErrorList << QString("文件%1行%2数字通道数目解析错误.").arg(m_strFile).arg(nLine);
		return false;
	}
	if(nTotal != m_nAChanelCount + m_nDChanelCount)
	{
		m_ErrorList << QString("文件%1行%2数字通道数目与模拟通道数目之和不等于总通道数目.").arg(m_strFile).arg(nLine);
		return false;
	}
	return true;
}

bool CCfgFile::ParseAChanelInfo(const QString& strLine,int nLine)
{
	QStringList valueList = strLine.split(",");
	if (valueList.count() != 13)
	{
		m_ErrorList << QString("文件%1行%2内容不符合规范.").arg(m_strFile).arg(nLine);
		return false;
	}
	bool bOk = true;
	CAChanelInfo aChanelInfo;
	aChanelInfo.m_nAn = valueList.at(0).trimmed().toUInt(&bOk);
	if (!bOk)
	{
		m_ErrorList << QString("文件%1行%2模拟通道索引解析错误.").arg(m_strFile).arg(nLine);
		return false;
	}
	aChanelInfo.m_strCh_id = valueList.at(1).trimmed();
	aChanelInfo.m_strPh = valueList.at(2).trimmed();
	aChanelInfo.m_strCcbm = valueList.at(3).trimmed();
	aChanelInfo.m_strUu = valueList.at(4).trimmed();
	aChanelInfo.m_fA = valueList.at(5).trimmed().toDouble(&bOk);
	if (!bOk)
	{
		m_ErrorList << QString("文件%1行%2模拟通道乘数解析错误.").arg(m_strFile).arg(nLine);
		return false;
	}
	aChanelInfo.m_fB = valueList.at(6).trimmed().toDouble(&bOk);
	if (!bOk)
	{
		m_ErrorList << QString("文件%1行%2模拟通道加数解析错误.").arg(m_strFile).arg(nLine);
		return false;
	}
	aChanelInfo.m_fSkew = valueList.at(7).trimmed().toDouble(&bOk);
	aChanelInfo.m_nMin = valueList.at(8).trimmed().toInt(&bOk);
	if (!bOk)
	{
		m_ErrorList << QString("文件%1行%2模拟通道数据最小值解析错误.").arg(m_strFile).arg(nLine);
		return false;
	}
	aChanelInfo.m_nMax = valueList.at(9).trimmed().toInt(&bOk);
	if (!bOk)
	{
		m_ErrorList << QString("文件%1行%2模拟通道数据最大值解析错误.").arg(m_strFile).arg(nLine);
		return false;
	}
	aChanelInfo.m_fPrimary = valueList.at(10).trimmed().toDouble(&bOk);
	if (!bOk)
	{
		m_ErrorList << QString("文件%1行%2模拟通道变换一次比解析错误.").arg(m_strFile).arg(nLine);
		return false;
	}
	aChanelInfo.m_fSecondary = valueList.at(11).trimmed().toDouble(&bOk);
	if (!bOk)
	{
		m_ErrorList << QString("文件%1行%2模拟通道变换二次比解析错误.").arg(m_strFile).arg(nLine);
		return false;
	}
	aChanelInfo.m_cPs = valueList.at(12).trimmed();

	m_AChanelInfoList << aChanelInfo;

	return true;
}

bool CCfgFile::ParseDChanelInfo(const QString& strLine,int nLine)
{
	QStringList valueList = strLine.split(",");
	if (valueList.count() != 5)
	{
		m_ErrorList << QString("文件%1行%2内容不符合规范.").arg(m_strFile).arg(nLine);
		return false;
	}
	bool bOk = true;
	CDChanelInfo dChanelInfo;
	dChanelInfo.m_nDn = valueList.at(0).trimmed().toUInt(&bOk);
	if (!bOk)
	{
		m_ErrorList << QString("文件%1行%2数字通道索引解析错误.").arg(m_strFile).arg(nLine);
		return false;
	}
	dChanelInfo.m_strCh_id = valueList.at(1).trimmed();
	dChanelInfo.m_strPh = valueList.at(2).trimmed();
	dChanelInfo.m_strCcbm = valueList.at(3).trimmed();
	dChanelInfo.m_nY = valueList.at(4).trimmed().toShort(&bOk);
	if (!bOk)
	{
		m_ErrorList << QString("文件%1行%2数字通道输入通道状态解析错误.").arg(m_strFile).arg(nLine);
		return false;
	}
	m_DChanelInfoList << dChanelInfo;
	return true;
}

bool CCfgFile::ParseSampleInfo(const QString& strLine,int nLine)
{
	QStringList valueList = strLine.split(",");
	if (valueList.count() != 2)
	{
		m_ErrorList << QString("文件%1行%2内容错误.").arg(m_strFile).arg(nLine);
		return false;
	}

	bool bOk = true;
	CSampleInfo sampleInfo;
	sampleInfo.m_fSamp = valueList.at(0).trimmed().toFloat(&bOk);
	if (!bOk)
	{
		m_ErrorList << QString("文件%1行%2采样率解析错误.").arg(m_strFile).arg(nLine);
		return false;
	}
	sampleInfo.m_nCount = valueList.at(1).trimmed().toUInt(&bOk);
	if (!bOk)
	{
		m_ErrorList << QString("文件%1行%2采样次数解析错误.").arg(m_strFile).arg(nLine);
		return false;
	}
	if (m_SampleInfoList.count()>0)
	{
		for (int nIndex=0;nIndex < m_SampleInfoList.count(); nIndex++)
		{
			sampleInfo.m_nCount = sampleInfo.m_nCount -m_SampleInfoList.at(nIndex).m_nCount;
		}
	}
	m_SampleInfoList << sampleInfo;

	return true;
}
