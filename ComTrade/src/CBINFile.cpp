#include "CBINFile.h"
#include <QtDebug>

CBinFile::CBinFile ()
{
	m_nReadPos = 0;
}

CBinFile::~CBinFile()
{

}


bool CBinFile::ReadBinFile(const QString&  strFile,unsigned int nACount,
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
	m_FileContent = file.readAll();
	if (m_FileContent.isEmpty())
	{
		m_ErrorList << QString("文件%1内容为空，读取出错.").arg(strFile);
		file.close();
		return false;
	}

	while (m_nReadPos < m_FileContent.count())
	{
		CChanelSampleData chanelData;
		//采样序号
		chanelData.m_nTick = ReadDWORD();
		//时戳
		chanelData.m_nTimeStamp = ReadDWORD();
		//A通道数据
		unsigned int nIndex=0;
		while(nIndex < nACount) //数据缺失为 0x8000
		{
			int nData = ReadWORD();
			if(nData >= 32768L)	nData = nData - 65536;
			chanelData.m_AChanelDataList << nData;
			if(nData>32767||nData<-32767)
			{
				chanelData.m_AChanelDataValidList <<false;
			}
			else
			{
				chanelData.m_AChanelDataValidList <<true;
			}
			nIndex++;
		}
		nIndex = 0;

		unsigned int nCount = (nDCount%16 == 0)? (nDCount/16):(nDCount/16 + 1);
		//D通道数据
		while(nIndex < nCount)
		{
			short nDValue = ReadWORD(); 
			unsigned int nPos = 0;
			unsigned int nBitCount = (nDCount-16*nIndex > 16)?16:(nDCount-16*nIndex);
			while (nPos < nBitCount)
			{
				short nTmp = (nDValue &(1<<nPos));
				unsigned short nValue = (nTmp != 0);
				chanelData.m_DChanelDataList.append(nValue);
				nPos++;
			}
			nIndex++;
		}
		dataList << chanelData;
	}
	
	file.close();
	return true;
}

unsigned long  CBinFile::ReadDWORD()
{
	if (m_nReadPos < m_FileContent.count() - 4)
	{
		union dwordValue
		{
            qint32 nValue;
			char cValue[4];
		};
        dwordValue dwValue;
		dwValue.cValue[0] = m_FileContent.at(m_nReadPos  );
		dwValue.cValue[1] = m_FileContent.at(m_nReadPos+1);
		dwValue.cValue[2] = m_FileContent.at(m_nReadPos+2);
		dwValue.cValue[3] = m_FileContent.at(m_nReadPos+3);

		m_nReadPos = m_nReadPos + 4;
		return dwValue.nValue;
	}
	m_nReadPos = m_nReadPos + 4;
	return 0xFFFFFFFF;
}

int CBinFile::ReadWORD()
{
	if (m_nReadPos < m_FileContent.count() - 2)
	{
		union wordValue
		{
			short nValue;
			char cValue[2];
		};
        wordValue wValue;
		wValue.cValue[0] = m_FileContent.at(m_nReadPos  );
		wValue.cValue[1] = m_FileContent.at(m_nReadPos+1);
		m_nReadPos = m_nReadPos + 2;
		return wValue.nValue;
	}
	m_nReadPos = m_nReadPos + 2;
	return 0;
}
