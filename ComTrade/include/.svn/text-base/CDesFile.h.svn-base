#ifndef _DESFILE_H_
#define _DESFILE_H_

#include "LibComTrade.h"
#include "CFileStruct.h"

#include <QFile>
#include <QString>
#include <QStringList>

class  CDesFile
{
public:
	CDesFile ();
	~CDesFile();

public:
	bool ReadDesFile(const QString& strFile);	

	unsigned int GetAChanelCount() const { return m_nAChanelCount; }

	unsigned int GetDChanelCount() const { return m_nDChanelCount; }

private:
	bool ParseAChanelDes(const QString& strLine,int nLine);
	bool ParseDChanelDes(const QString& strLine,int nLine);

	bool ParseFirstLine(const QString& strLine,int nLine);
	bool ParseSecondLine(const QString& strLine,int nLine);

public:
	unsigned int	m_nAChanelCount;	//A通道数量0~999999
	unsigned int	m_nDChanelCount;	//D通道数量0~999999

	AChanelDesList m_AChanelDesList;
	DChanelDesList m_DChanelDesList;

	QString			m_strDevice;	//装置特征 < 64
	QString			m_strVersion;	//标准修改年份 4

	QString			m_strFile;


	QStringList m_ErrorList;
};

#endif
