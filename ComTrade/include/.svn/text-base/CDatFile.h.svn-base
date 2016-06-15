#ifndef _DATFILE_H_
#define _DATFILE_H_

#include "LibComTrade.h"
#include "CFileStruct.h"

#include <QFile>
#include <QString>
#include <QStringList>

class  CDatFile
{
public:
	CDatFile ();
	~CDatFile();

public:
	bool ReadDatFile(const QString& strFile,unsigned int nACount,
		unsigned int nDCount, bool bBin=true);	

public:
	ChanelDataList	m_ChanelDataList;
	QStringList m_ErrorList;
};

#endif
