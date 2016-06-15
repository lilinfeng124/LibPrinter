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
	unsigned int	m_nAChanelCount;	//Aͨ������0~999999
	unsigned int	m_nDChanelCount;	//Dͨ������0~999999

	AChanelDesList m_AChanelDesList;
	DChanelDesList m_DChanelDesList;

	QString			m_strDevice;	//װ������ < 64
	QString			m_strVersion;	//��׼�޸���� 4

	QString			m_strFile;


	QStringList m_ErrorList;
};

#endif
