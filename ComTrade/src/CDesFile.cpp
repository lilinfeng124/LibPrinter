﻿#include "CDesFile.h"
#include "CCfgFile.h"

#include <QtDebug>
#include <QStringList>
#include <QTextCodec>
#include <QTextStream>
CDesFile::CDesFile ()
{
	m_nAChanelCount = 0;
	m_nDChanelCount = 0;
}

CDesFile::~CDesFile()
{

}


bool CDesFile::ReadDesFile(const QString& strFile)
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
    QString strLine =  CCfgFile::ReadNextLineString(flStream,nLine);
	if (!ParseFirstLine(strLine,nLine))
	{
		file.close();
		return false;
	}

    strLine =  CCfgFile::ReadNextLineString(flStream,nLine);
	if (!ParseSecondLine(strLine,nLine))
	{
		file.close();
		return false;
	}	
	//模拟通道描述
	unsigned int nIndex = 0;
	while(nIndex < m_nAChanelCount)
	{
        strLine = CCfgFile::ReadNextLineString(flStream,nLine);
		if (!ParseAChanelDes(strLine,nLine))
		{
			file.close();
			return false;
		}
		nIndex++;
	}

	//数字通道描述
	nIndex = 0;
	while(nIndex < m_nDChanelCount)
	{
        strLine = CCfgFile::ReadNextLineString(flStream,nLine);
		if (!ParseDChanelDes(strLine,nLine))
		{
			file.close();
			return false;
		}
		nIndex++;
	}

	file.close();
	return true;
}

bool CDesFile::ParseAChanelDes(const QString& strLine,int nLine)
{
	QStringList valueList = strLine.split(",");
	if (valueList.count() != 6)
	{
		m_ErrorList << QString("文件%1行%2内容不符合规范.").arg(m_strFile).arg(nLine);
		return false;
	}
	bool bOk = true;
	CAChanelDes aChanelDes;
	aChanelDes.m_nAn = valueList.at(0).trimmed().toUInt(&bOk);
	if (!bOk)
	{
		m_ErrorList << QString("文件%1行%2模拟通道索引解析错误.").arg(m_strFile).arg(nLine);
		return false;
	}
	aChanelDes.m_strName = valueList.at(1).trimmed();
	aChanelDes.m_strType = valueList.at(2).trimmed();
	aChanelDes.m_strUu = valueList.at(3).trimmed();
	aChanelDes.m_fScale = valueList.at(4).trimmed().toDouble(&bOk);
	if (!bOk)
	{
		m_ErrorList << QString("文件%1行%2模拟通道比例解析错误.").arg(m_strFile).arg(nLine);
		return false;
	}
	aChanelDes.m_nVisible = valueList.at(5).trimmed().toShort(&bOk);
	if (!bOk)
	{
		m_ErrorList << QString("文件%1行%2模拟通道是否显示解析错误.").arg(m_strFile).arg(nLine);
		return false;
	}
	m_AChanelDesList.append(aChanelDes);
	return true;
}

bool CDesFile::ParseDChanelDes(const QString& strLine,int nLine)
{
	QStringList valueList = strLine.split(",");
	if (valueList.count() != 4)
	{
		m_ErrorList << QString("文件%1行%2内容不符合规范.").arg(m_strFile).arg(nLine);
		return false;
	}
	bool bOk = true;
	CDChanelDes dChanelDes;
	dChanelDes.m_nDn = valueList.at(0).trimmed().toUInt(&bOk);
	if (!bOk)
	{
		m_ErrorList << QString("文件%1行%2数字通道索引解析错误.").arg(m_strFile).arg(nLine);
		return false;
	}
	dChanelDes.m_strName = valueList.at(1).trimmed();
	dChanelDes.m_strType = valueList.at(2).trimmed();
	dChanelDes.m_nVisible = valueList.at(3).trimmed().toShort(&bOk);
	if (!bOk)
	{
		m_ErrorList << QString("文件%1行%2数字通道是否显示解析错误.").arg(m_strFile).arg(nLine);
		return false;
	}
	m_DChanelDesList.append(dChanelDes);
	return true;
}

bool CDesFile::ParseFirstLine(const QString& strLine,int nLine)
{
	QStringList valueList = strLine.split(",");
	if (valueList.count() != 2)
	{
		m_ErrorList << QString("文件%1行%2内容错误.").arg(m_strFile).arg(nLine);
		return false;
	}

	// 装置名称 0
	m_strDevice = valueList.at(0).trimmed();
	m_strVersion = valueList.at(1).trimmed();	//固定为0

	return true;
}

bool CDesFile::ParseSecondLine(const QString& strLine,int nLine)
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
