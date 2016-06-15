#ifndef _BINFILE_H_
#define _BINFILE_H_

#include <QFile>
#include <QString>
#include <QByteArray>
#include <QStringList>
#include "CFileStruct.h"

class  CBinFile
{
public:
	CBinFile ();
	~CBinFile();

public:
	bool ReadBinFile(const QString&  strFile,unsigned int nACount,
		unsigned int nDCount,ChanelDataList& dataList);

protected:
	unsigned long	ReadDWORD();
	int	ReadWORD();

private:
	QByteArray m_FileContent;
	int m_nReadPos;

public:
	QStringList m_ErrorList;
};

#endif
