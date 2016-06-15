#ifndef _ASCIIFILE_H_
#define _ASCIIFILE_H_

#include <QFile>
#include <QString>
#include <QStringList>
#include "CFileStruct.h"

class  CASCIIFile
{
public:
	CASCIIFile ();
	~CASCIIFile();

public:
	bool ReadAsciiFile(const QString& strFile,unsigned int nACount,
		unsigned int nDCount,ChanelDataList& dataList);

public:
	QStringList m_ErrorList;
};

#endif
