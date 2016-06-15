#ifndef _CFGFILE_H_
#define _CFGFILE_H_

#include "LibComTrade.h"
#include "CFileStruct.h"

#include <QFile>
#include <QString>
#include <QDateTime>
#include <QStringList>
#include <QTextStream>
class  CCfgFile
{
public:
	CCfgFile ();
	~CCfgFile();

public:
	bool ReadCfgFile(const QString& strFile);	

    static QString ReadNextLineString( QTextStream& file,unsigned int& nLine);
private:
	bool ParseFirstLine(const QString& strLine,int nLine);
	bool ParseSecondLine(const QString& strLine,int nLine);
	bool ParseAChanelInfo(const QString& strLine,int nLine);
	bool ParseDChanelInfo(const QString& strLine,int nLine);
	bool ParseSampleInfo(const QString& strLine,int nLine);

	bool StringToDateTime(const QString& strLine,CExDateTime& dtTime);
public:
	QString			m_strFile;

	QString			m_strSite;		//站名 < 64
	QString			m_strDevice;	//装置特征 < 64
	QString			m_strVersion;	//标准修改年份 4

	//unsigned int	m_nChanelCount;		//通道数量
	unsigned int	m_nAChanelCount;	//A通道数量0~999999
	unsigned int	m_nDChanelCount;	//D通道数量0~999999


	AChanelInfoList m_AChanelInfoList;
	DChanelInfoList m_DChanelInfoList;

	double			m_fIF;	//线路频率. 可选

	unsigned short	m_nNrates;	// 0~999 

	SampleInfoList  m_SampleInfoList;

	CExDateTime		m_dtFirstDataTime;	//第一个采样数据时间
	CExDateTime		m_dtTriggerTime;	//触发时间

	bool			m_bBin;		//dat文件类型

	double			m_fTimeMult;	

	QStringList m_ErrorList;
};

#endif
