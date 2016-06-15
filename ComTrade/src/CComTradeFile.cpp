#include "CComTradeFile.h"
#include <QtDebug>
#include "qmath.h"

CComTradeFile::CComTradeFile ()
{
	m_nTrigger_ms = 0;
	m_nFrist_ms = 0;
	m_lFirstMicroSecs = 0;
	m_bDChanelAct = false;
}

CComTradeFile::~CComTradeFile()
{

}

bool CComTradeFile::SampleDataToAnalogData(unsigned long nAIndex, const CChanelSampleData& chanelData,CAnalogData& aData)
{
	if (chanelData.m_AChanelDataList.count()!= chanelData.m_AChanelDataValidList.count()
		|| chanelData.m_AChanelDataList.count() != m_cfgFile.m_nAChanelCount)
	{
		m_ErrorList << QString("模拟采样通道数据与模拟通道配置不一致。");
		return false;
	}
	aData.m_nIndex = nAIndex;
	aData.m_bValid = chanelData.m_AChanelDataValidList.at(nAIndex);
	aData.m_fRawValue = chanelData.m_AChanelDataList.at(nAIndex);
	aData.m_nTimeStamp = chanelData.m_nTimeStamp;

	return true;
}

bool CComTradeFile::SampleDataToDigitalData(unsigned long nDIndex, 
											const CChanelSampleData& chanelData,
											CDigitalData& dData,
											bool& bChanged)
{
	if ( chanelData.m_DChanelDataList.count() != m_cfgFile.m_nDChanelCount)
	{
		m_ErrorList << QString("数字采样通道数据与数字通道配置不一致。");
		return false;
	}

	dData.m_nIndex = nDIndex;
	dData.m_nValue = chanelData.m_DChanelDataList.at(nDIndex);
	dData.m_nTimeStamp = chanelData.m_nTimeStamp;
	if (dData.m_nValue != 0)
	{
		bChanged = true;
	}
	return true;
}

void CComTradeFile::StaticAnalogData(CStaticAnalogData& staticAData,const CAnalogData& aData)
{
	staticAData.m_nIndex = aData.m_nIndex;
	//原始
	staticAData.m_fMaxRawValue = qMax(aData.m_fRawValue,staticAData.m_fMaxRawValue);
	staticAData.m_fMinRawValue = qMin(aData.m_fRawValue,staticAData.m_fMinRawValue);
	
	//原始实部
	staticAData.m_fMaxRawRealValue = qMax(aData.m_fRawRealValue,staticAData.m_fMaxRawRealValue);
	staticAData.m_fMinRawRealValue = qMin(aData.m_fRawRealValue,staticAData.m_fMinRawRealValue);

	//原始虚部
	staticAData.m_fMaxRawImgValue = qMax(aData.m_fRawImgValue,staticAData.m_fMaxRawImgValue);
	staticAData.m_fMinRawImgValue = qMin(aData.m_fRawImgValue,staticAData.m_fMinRawImgValue);

	//原始有效
	staticAData.m_fMaxRawEffectValue = qMax(aData.m_fRawEffectValue,staticAData.m_fMaxRawEffectValue);
	staticAData.m_fMinRawEffectValue = qMin(aData.m_fRawEffectValue,staticAData.m_fMinRawEffectValue);

	//原始相角
	staticAData.m_fMaxRawAngleValue = qMax(aData.m_fRawAngleValue,staticAData.m_fMaxRawAngleValue);
	staticAData.m_fMinRawAngleValue = qMin(aData.m_fRawAngleValue,staticAData.m_fMinRawAngleValue);

	//一次采样
	staticAData.m_fPMaxSampleValue = qMax(aData.m_fPSampleValue,staticAData.m_fPMaxSampleValue);
	staticAData.m_fPMinSampleValue = qMin(aData.m_fPSampleValue,staticAData.m_fPMinSampleValue);

	//二次采样
	staticAData.m_fSMaxSampleValue = qMax(aData.m_fSSampleValue,staticAData.m_fSMaxSampleValue);
	staticAData.m_fSMinSampleValue = qMin(aData.m_fSSampleValue,staticAData.m_fSMinSampleValue);

	//一次有效
	staticAData.m_fPMaxEffectValue = qMax(aData.m_fPEffectValue,staticAData.m_fPMaxEffectValue);
	staticAData.m_fPMinEffectValue = qMin(aData.m_fPEffectValue,staticAData.m_fPMinEffectValue);

	//二次有效
	staticAData.m_fSMaxEffectValue = qMax(aData.m_fSEffectValue,staticAData.m_fSMaxEffectValue);
	staticAData.m_fSMinEffectValue = qMin(aData.m_fSEffectValue,staticAData.m_fSMinEffectValue);
	
	//一次实部
	staticAData.m_fPMaxRealValue = qMax(aData.m_fPRealValue,staticAData.m_fPMaxRealValue);
	staticAData.m_fPMinRealValue = qMin(aData.m_fPRealValue,staticAData.m_fPMinRealValue);

	//二次实部
	staticAData.m_fSMaxRealValue = qMax(aData.m_fSRealValue,staticAData.m_fSMaxRealValue);
	staticAData.m_fSMinRealValue = qMin(aData.m_fSRealValue,staticAData.m_fSMinRealValue);

	//一次虚部
	staticAData.m_fPMaxImgValue = qMax(aData.m_fPImgValue,staticAData.m_fPMaxImgValue);
	staticAData.m_fPMinImgValue = qMin(aData.m_fPImgValue,staticAData.m_fPMinImgValue);
	//二次虚部
	staticAData.m_fSMaxImgValue = qMax(aData.m_fSImgValue,staticAData.m_fSMaxImgValue);
	staticAData.m_fSMinImgValue = qMin(aData.m_fSImgValue,staticAData.m_fSMinImgValue);

	//一次幅值
	staticAData.m_fPMaxAmpValue = qMax(aData.m_fPAmpValue,staticAData.m_fPMaxAmpValue);
	staticAData.m_fPMinAmpValue = qMin(aData.m_fPAmpValue,staticAData.m_fPMinAmpValue);

	//二次幅值
	staticAData.m_fSMaxAmpValue = qMax(aData.m_fSAmpValue,staticAData.m_fSMaxAmpValue);
	staticAData.m_fSMinAmpValue = qMin(aData.m_fSAmpValue,staticAData.m_fSMinAmpValue);

	//一次相角
	staticAData.m_fPMaxAngleValue = qMax(aData.m_fPAngleValue,staticAData.m_fPMaxAngleValue);
	staticAData.m_fPMinAngleValue = qMin(aData.m_fPAngleValue,staticAData.m_fPMinAngleValue);

	//二次相角
	staticAData.m_fSMaxAngleValue = qMax(aData.m_fSAngleValue,staticAData.m_fSMaxAngleValue);
	staticAData.m_fSMinAngleValue = qMin(aData.m_fSAngleValue,staticAData.m_fSMinAngleValue);

}

void CComTradeFile::TransformAnalogData(CStaticAnalogData& staticAData,AnalogDataList& aDataList,
										int nBlock,const CAChanelInfo& aInfo)
{
	Fourier(aDataList,nBlock);
	for (int nIndex = 0; nIndex < aDataList.count(); nIndex++)
	{
		CAnalogData aData = aDataList.at(nIndex);

		//一次二次计算方法如下：fRaw = 采样值
		//如果采样为一次，一次 = fRaw*m_dMultiplier+m_dOffsetAdder
		//二次 = (fRaw*m_dMultiplier+m_dOffsetAdder)*m_fSecondary/m_fPrimary;
        //如果采样为二次，二次 = feRaw*m_dMultiplier+m_dOffsetAdder
		//一次 = (fRaw*m_dMultiplier+m_dOffsetAdder)*m_fPrimary /m_fSecondary;
		if (IsAInfoPrimary(aInfo))
		{
			aData.m_fPSampleValue = aData.m_fRawValue*aInfo.m_fA + aInfo.m_fB;
			aData.m_fSSampleValue = aData.m_fPSampleValue*aInfo.m_fSecondary/aInfo.m_fPrimary;

			aData.m_fPEffectValue = aData.m_fRawEffectValue*aInfo.m_fA + aInfo.m_fB;
			aData.m_fSEffectValue = aData.m_fPEffectValue*aInfo.m_fSecondary/aInfo.m_fPrimary;

			aData.m_fPRealValue = aData.m_fRawRealValue*aInfo.m_fA + aInfo.m_fB;
			aData.m_fSRealValue  = aData.m_fPRealValue*aInfo.m_fSecondary/aInfo.m_fPrimary;

			aData.m_fPImgValue = aData.m_fRawImgValue*aInfo.m_fA + aInfo.m_fB;
			aData.m_fSImgValue = aData.m_fPImgValue*aInfo.m_fSecondary/aInfo.m_fPrimary;

			aData.m_fPAmpValue = aData.m_fRawEffectValue*M_SQRT2*aInfo.m_fA + aInfo.m_fB;
			aData.m_fSAmpValue = aData.m_fPAmpValue*aInfo.m_fSecondary/aInfo.m_fPrimary;	

			aData.m_fPAngleValue = aData.m_fRawAngleValue*aInfo.m_fA + aInfo.m_fB;
			aData.m_fSAngleValue = aData.m_fPAngleValue*aInfo.m_fSecondary/aInfo.m_fPrimary;	
		}
		else
		{
			aData.m_fSSampleValue = aData.m_fRawValue*aInfo.m_fA + aInfo.m_fB;
			aData.m_fPSampleValue = aData.m_fSSampleValue*aInfo.m_fPrimary/aInfo.m_fSecondary;

			aData.m_fSEffectValue = aData.m_fRawEffectValue*aInfo.m_fA + aInfo.m_fB;
			aData.m_fPEffectValue = aData.m_fSEffectValue*aInfo.m_fPrimary/aInfo.m_fSecondary;

			aData.m_fSRealValue = aData.m_fRawRealValue*aInfo.m_fA + aInfo.m_fB;
			aData.m_fPRealValue  = aData.m_fSRealValue*aInfo.m_fPrimary/aInfo.m_fSecondary;

			aData.m_fSImgValue = aData.m_fRawImgValue*aInfo.m_fA + aInfo.m_fB;
			aData.m_fPImgValue = aData.m_fSImgValue*aInfo.m_fPrimary/aInfo.m_fSecondary;

			aData.m_fSAmpValue = aData.m_fRawEffectValue*M_SQRT2*aInfo.m_fA + aInfo.m_fB;
			aData.m_fPAmpValue = aData.m_fSAmpValue*aInfo.m_fPrimary/aInfo.m_fSecondary;

			aData.m_fSAngleValue = aData.m_fRawAngleValue*aInfo.m_fA + aInfo.m_fB;
			aData.m_fPAngleValue = aData.m_fSAngleValue*aInfo.m_fPrimary/aInfo.m_fSecondary;	
		}

		if (m_AnalogDataMap.contains(aData.m_nTimeStamp))
		{
			AnalogDataList& aMapDataList = m_AnalogDataMap[aData.m_nTimeStamp];
			aMapDataList.append(aData);
		}
		else
		{
			AnalogDataList aMapDataList;
			aMapDataList.append(aData);
			m_AnalogDataMap[aData.m_nTimeStamp] = aMapDataList;
		}
		//统计数据
		StaticAnalogData(staticAData,aData);
	}
}

void CComTradeFile::TransformDigitalData(DigitalDataList& dDataList,
										 bool bChanged)
{
	for ( int nIndex=0; nIndex < dDataList.count(); nIndex++)
	{
		CDigitalData dData = dDataList.at(nIndex);
		if (bChanged)
		{
			dData.m_bChanged = true;
			m_bDChanelAct = true;
		}
		if (m_DigitalDataMap.contains(dData.m_nTimeStamp))
		{
			DigitalDataList& dMapDataList = m_DigitalDataMap[dData.m_nTimeStamp];
			dMapDataList.append(dData);
		}
		else
		{
			DigitalDataList dMapDataList;
			dMapDataList.append(dData);
			m_DigitalDataMap[dData.m_nTimeStamp] = dMapDataList;
		}
	}
}

bool CComTradeFile::TransformData()
{
	m_AnalogDataMap.clear();
	m_DigitalDataMap.clear();
	m_StaticValueMap.clear();
	m_MidDigitalChangedMap.clear();
	m_MidSampleDataMap.clear();

	if (IsContinuous())   //连续采样
	{
		return true;
	}

	unsigned int nCount = m_cfgFile.m_nAChanelCount>m_cfgFile.m_nDChanelCount?m_cfgFile.m_nAChanelCount:m_cfgFile.m_nDChanelCount;
	for (unsigned int nIndex = 0; nIndex < nCount;nIndex++)
	{
		unsigned int nStart = 0;
		bool bChanged = false;
		DigitalDataList dDataList;
		CStaticAnalogData staticAnalogData;
		for (unsigned short nBlock=0; nBlock < GetNrates();nBlock++)
		{
			unsigned int nCount = GetSampleCountByBlock(nBlock);
			AnalogDataList aDataList;
			for(unsigned int nSamTick = 0; nSamTick < nCount; nSamTick++)
			{
				CChanelSampleData chanelData = m_datFile.m_ChanelDataList.at(nSamTick+nStart);
				
				if (nIndex < m_cfgFile.m_nAChanelCount)
				{
					CAnalogData aData;
					if (!SampleDataToAnalogData(nIndex,chanelData,aData))
					{
						return false;
					}
					aDataList.append(aData);
				}
				if (nIndex < m_cfgFile.m_nDChanelCount)
				{
					CDigitalData dData;
					if (!SampleDataToDigitalData(nIndex,chanelData,dData,bChanged))
					{
						return false;
					}
					dDataList.append(dData);
				}
			}
			//a通道变换
			if (nIndex < m_cfgFile.m_nAChanelCount)
			{
				CAChanelInfo aInfo  = m_cfgFile.m_AChanelInfoList.at(nIndex);
				TransformAnalogData(staticAnalogData,aDataList,nBlock,aInfo);
			}
			nStart += nCount;
		}
		
		if (nIndex < m_cfgFile.m_nAChanelCount)
		{
			m_StaticValueMap[nIndex] = staticAnalogData;
		}

		//d通道变换
		TransformDigitalData(dDataList,bChanged);
		if (nIndex < m_cfgFile.m_nDChanelCount)
		{
			if (bChanged)
			{
				m_DigitalChangedMap[nIndex] = true;
			}
			else
			{
				m_DigitalChangedMap[nIndex] = false;
			}
		}
	}
	m_TimeArray = m_AnalogDataMap.keys();

	for (unsigned int nIndex = 0; nIndex < m_desFile.m_nDChanelCount;nIndex++)
	{
		for(int nMidIndex=0; nMidIndex < m_midFile.m_ChanelDataList.count();nMidIndex++)
		{
			CChanelSampleData samData = m_midFile.m_ChanelDataList.at(nMidIndex);
			if (samData.m_DChanelDataList.at(nIndex) != 0)
			{
				m_MidDigitalChangedMap[nIndex] = true;
			}
			else
			{
				m_MidDigitalChangedMap[nIndex] = false;
			}
			if (!m_MidSampleDataMap.contains(samData.m_nTimeStamp))
			{
				m_MidSampleDataMap[samData.m_nTimeStamp] = samData;
			}
		}
	}

	return true;
}

QStringList CComTradeFile::GetErrorList()
{
	return m_ErrorList;
}

//1 hdr 2 cfg 3 des 4dat 5 mid
bool CComTradeFile::ReadComTradeFileList(const QStringList& fileList)
{
	//读取hdr文件
	QString strHdrFile = fileList.at(0);
	if (!m_hdrFile.ReadHdrFile(strHdrFile))
	{
		m_ErrorList << m_hdrFile.m_ErrorList;
		return false;
	}

	//读取cfg文件和des文件
	QString strCfgFile = fileList.at(1);
	if (!m_cfgFile.ReadCfgFile(strCfgFile))
	{
		m_ErrorList << m_cfgFile.m_ErrorList;
		return false;
	}

	QString strDesFile = fileList.at(2);
	if (!m_desFile.ReadDesFile(strDesFile))
	{
		m_ErrorList << m_desFile.m_ErrorList;
		return false;
	}

	int nCfgACount = m_cfgFile.m_nAChanelCount;
	int nCfgDCount = m_cfgFile.m_nDChanelCount;
	bool bBin = m_cfgFile.m_bBin;
	int nMidAcount = m_desFile.m_nAChanelCount;
	int nMidDCount = m_desFile.m_nDChanelCount;
	//根据类型读取dat和mid文件
	QString strDatFile = fileList.at(3);
	if (!m_datFile.ReadDatFile(strDatFile,nCfgACount,nCfgDCount,bBin))
	{
		m_ErrorList << m_datFile.m_ErrorList;
		return false;
	}

	QString strMidFile = fileList.at(4);
	if (!m_midFile.ReadDatFile(strMidFile,nMidAcount,nMidDCount,bBin))
	{
		m_ErrorList << m_midFile.m_ErrorList;
		return false;
	}

	//变换数据
	if (!TransformData())
	{
		return false;
	}

	//时间变换为毫秒
	TimeMSecond();

	return true;
}

bool CComTradeFile::ReadComTradeFile(const QString& strFile)	
{
	//读取hdr文件
	QString strHdrFile = FileNameFromType(strFile,Hdr);
	if (!m_hdrFile.ReadHdrFile(strHdrFile))
	{
		m_ErrorList << m_hdrFile.m_ErrorList;
		return false;
	}
	
	//读取cfg文件和des文件
	QString strCfgFile = FileNameFromType(strFile,Cfg);
	if (!m_cfgFile.ReadCfgFile(strCfgFile))
	{
		m_ErrorList << m_cfgFile.m_ErrorList;
		return false;
	}

	QString strDesFile = FileNameFromType(strFile,Des);
	if (!m_desFile.ReadDesFile(strDesFile))
	{
		m_ErrorList << m_desFile.m_ErrorList;
		return false;
	}


	int nCfgACount = m_cfgFile.m_nAChanelCount;
	int nCfgDCount = m_cfgFile.m_nDChanelCount;
	bool bBin = m_cfgFile.m_bBin;
	int nMidAcount = m_desFile.m_nAChanelCount;
	int nMidDCount = m_desFile.m_nDChanelCount;
	//根据类型读取dat和mid文件
	QString strDatFile = FileNameFromType(strFile,Dat);
	if (!m_datFile.ReadDatFile(strDatFile,nCfgACount,nCfgDCount,bBin))
	{
		m_ErrorList << m_datFile.m_ErrorList;
		return false;
	}

	QString strMidFile = FileNameFromType(strFile,Mid);
	if (!m_midFile.ReadDatFile(strMidFile,nMidAcount,nMidDCount,bBin))
	{
		m_ErrorList << m_midFile.m_ErrorList;
		return false;
	}

	//变换数据
	if (!TransformData())
	{
		return false;
	}

	//时间变换为毫秒
	TimeMSecond();

	return true;
}

QList<unsigned long> CComTradeFile::GetTimeArray()
{
	return m_TimeArray;
}

void CComTradeFile::TimeMSecond()
{
	QTime trigerTime = m_cfgFile.m_dtTriggerTime.m_dtTime.time();
	QTime fristTime = m_cfgFile.m_dtFirstDataTime.m_dtTime.time();

	m_lFirstMicroSecs  = m_cfgFile.m_dtFirstDataTime.m_nWs/1000;

	int nTriggerHour = trigerTime.hour();
	int nFirstHour =  fristTime.hour();
	if(nTriggerHour==0) nTriggerHour = 24;
	if(nFirstHour==0) nFirstHour = 24;
	m_nTrigger_ms = ((nTriggerHour*60 + trigerTime.minute())*60 + trigerTime.second())*1000 + m_cfgFile.m_dtTriggerTime.m_nWs/1000;
	m_nFrist_ms = ((nFirstHour*60 + fristTime.minute())*60 + fristTime.second())*1000 + m_cfgFile.m_dtFirstDataTime.m_nWs/1000;

	m_nFrist_ms = 	m_nFrist_ms - m_nTrigger_ms;
	m_nTrigger_ms = 0;
}

int     CComTradeFile::GetFirstDataTimeMs()
{
	return m_nFrist_ms;
}
int     CComTradeFile::GetTriggeredTimeMs()
{
	return m_nTrigger_ms;
}
qint64     CComTradeFile::GetABSFirstDataTimeMs()
{
	return m_lFirstMicroSecs;
}

double CComTradeFile::GetPointTime(unsigned int nPoint)
{
	if (nPoint < (unsigned int)m_TimeArray.count())
	{
		return m_TimeArray.at(nPoint);
	}
	return m_nFrist_ms;
}

int	CComTradeFile::GetTotalPointCount()
{
	QList<unsigned long> timeList =  m_AnalogDataMap.keys();
	return timeList.count();
}

int	CComTradeFile::GetStartPointByBlock(int nBlock)
{
	int nStart = 0;
	for (int nIndex =0; nIndex < GetNrates()&&nIndex<nBlock;nIndex++)
	{
		nStart += m_cfgFile.m_SampleInfoList.at(nIndex).m_nCount;
	}
	return nStart;
}

int	CComTradeFile::GetSampleCountByBlock(int nBlock)
{
	int nStart = 0;
	if (GetNrates() == 0)
	{
		return  m_cfgFile.m_SampleInfoList.at(0).m_nCount;
	}
	if (nBlock < GetNrates())
	{
		nStart = m_cfgFile.m_SampleInfoList.at(nBlock).m_nCount;
	}
	return nStart;
}

double	CComTradeFile::GetSampleRateByBlock(int nBlock)
{
	int nStart = 0;
	if (GetNrates() == 0)
	{
		return  m_cfgFile.m_SampleInfoList.at(0).m_fSamp;
	}
	if (nBlock < GetNrates())
	{
		nStart = m_cfgFile.m_SampleInfoList.at(nBlock).m_fSamp;
	}
	return nStart;
}


double CComTradeFile::GetMaxAbsRawValue(unsigned int nAIndex)
{
	double fMax = GetMaxRawValue(nAIndex);
	double fMin = GetMinRawValue(nAIndex);

	double fRet = qMax(qAbs(fMax),qAbs(fMin));
	
	return fRet;
}

double CComTradeFile::GetMinRawValue(unsigned int nAIndex)
{
	if (m_StaticValueMap.contains(nAIndex))
	{
		return m_StaticValueMap[nAIndex].m_fMinRawValue;
	}
	return 0.0;
}

double CComTradeFile::GetMaxRawValue(unsigned int nAIndex)
{
	if (m_StaticValueMap.contains(nAIndex))
	{
		return m_StaticValueMap[nAIndex].m_fMaxRawValue;
	}
	return 0.0;
}

void CComTradeFile::SetMaxRawValue(unsigned int nAn,double fValue)
{
	m_StaticValueMap[nAn].m_fMaxRawValue = fValue;
}

double CComTradeFile::GetMaxValue(unsigned int nIndex,int nValueType,int nWaveType)
{
	double fValue = 0.0;
	CStaticAnalogData aData = m_StaticValueMap.value(nIndex);
	if (nValueType == PRIMARY)
	{
		if (nWaveType == SAMPLE)
		{
			fValue = aData.m_fPMaxSampleValue;
		}
		if (nWaveType == EFFECT)
		{
			fValue = aData.m_fPMaxEffectValue;
		}
		if (nWaveType == AMP)
		{
			fValue = aData.m_fPMaxAmpValue;
		}
		if (nWaveType == REAL)
		{
			fValue = aData.m_fPMaxRealValue;
		}
		if (nWaveType == ANGLE)
		{
			fValue = aData.m_fPMaxAngleValue;
		}
		if (nWaveType == IMG)
		{
			fValue = aData.m_fPMaxImgValue;
		}
	}
	else
	{
		if (nWaveType == SAMPLE)
		{
			fValue = aData.m_fSMaxSampleValue;
		}
		if (nWaveType == EFFECT)
		{
			fValue = aData.m_fSMaxEffectValue;
		}
		if (nWaveType == AMP)
		{
			fValue = aData.m_fSMaxAmpValue;
		}
		if (nWaveType == REAL)
		{
			fValue = aData.m_fSMaxRealValue;
		}
		if (nWaveType == ANGLE)
		{
			fValue = aData.m_fSMaxAngleValue;
		}
		if (nWaveType == IMG)
		{
			fValue = aData.m_fSMaxImgValue;
		}
	}
	return fValue;
}

double CComTradeFile::GetMinValue(unsigned int nIndex,int nValueType,int nWaveType)
{
	double fValue = 0.0;
	CStaticAnalogData aData = m_StaticValueMap.value(nIndex);
	if (nValueType == PRIMARY)
	{
		if (nWaveType == SAMPLE)
		{
			fValue = aData.m_fPMinSampleValue;
		}
		if (nWaveType == EFFECT)
		{
			fValue = aData.m_fPMinEffectValue;
		}
		if (nWaveType == AMP)
		{
			fValue = aData.m_fPMinAmpValue;
		}
		if (nWaveType == REAL)
		{
			fValue = aData.m_fPMinRealValue;
		}
		if (nWaveType == ANGLE)
		{
			fValue = aData.m_fPMinAngleValue;
		}
		if (nWaveType == IMG)
		{
			fValue = aData.m_fPMinImgValue;
		}
	}
	else
	{
		if (nWaveType == SAMPLE)
		{
			fValue = aData.m_fSMinSampleValue;
		}
		if (nWaveType == EFFECT)
		{
			fValue = aData.m_fSMinEffectValue;
		}
		if (nWaveType == AMP)
		{
			fValue = aData.m_fSMinAmpValue;
		}
		if (nWaveType == REAL)
		{
			fValue = aData.m_fSMinRealValue;
		}
		if (nWaveType == ANGLE)
		{
			fValue = aData.m_fSMinAngleValue;
		}
		if (nWaveType == IMG)
		{
			fValue = aData.m_fSMinImgValue;
		}
	}
	return fValue;
}

bool	CComTradeFile::IsDChanelAct()
{
	return m_bDChanelAct;
}

bool	CComTradeFile::IsDChanelAct(unsigned int nDIndex)
{
	if (m_DigitalChangedMap.contains(nDIndex))
	{
		return m_DigitalChangedMap[nDIndex];
	}
	return false;
}

double CComTradeFile::GetAnalogValueByIndexPoint(unsigned int nIndex,unsigned int nPoint)
{
	if (nPoint >=0 && nPoint < (unsigned int)m_TimeArray.count())
	{
		unsigned int nTime = m_TimeArray.at(nPoint);
		AnalogDataList aDataList = m_AnalogDataMap.value(nTime);
		if (nIndex>=0 && nIndex < (unsigned int)aDataList.count())
		{
			return aDataList.at(nIndex).m_fRawValue;
		}
	}
	return 0;
}

unsigned short CComTradeFile::GetDigitalValueByIndexPoint(unsigned int nIndex,
														  unsigned int nPoint)
{
	if (nPoint >=0 && nPoint < (unsigned int)m_TimeArray.count())
	{
		unsigned int nTime = m_TimeArray.at(nPoint);
		DigitalDataList dDataList = m_DigitalDataMap.value(nTime);
		if (nIndex>=0 && nIndex < (unsigned int)dDataList.count())
		{
			return dDataList.at(nIndex).m_nValue;
		}
	}
	return 0;
}

bool CComTradeFile::GetTypedAnalogValueByIndexPoint(unsigned int nIndex,
									   unsigned int nPoint,int nValueType,
									   int nWaveType,double& fValue)
{
	if (nPoint >=0 && nPoint < (unsigned int)m_TimeArray.count())
	{
		unsigned int nTime = m_TimeArray.at(nPoint);
		AnalogDataList aDataList = m_AnalogDataMap.value(nTime);
		if (nIndex>=0 && nIndex < (unsigned int)aDataList.count())
		{
			CAnalogData aData = aDataList.at(nIndex);
			if (nValueType == PRIMARY)
			{
				if (nWaveType == SAMPLE)
				{
					fValue = aData.m_fPSampleValue;
				}
				if (nWaveType == EFFECT)
				{
					fValue = aData.m_fPEffectValue;
				}
				if (nWaveType == AMP)
				{
					fValue = aData.m_fPAmpValue;
				}
				if (nWaveType == REAL)
				{
					fValue = aData.m_fPRealValue;
				}
				if (nWaveType == ANGLE)
				{
					fValue = aData.m_fPAngleValue;
				}
				if (nWaveType == IMG)
				{
					fValue = aData.m_fPImgValue;
				}
			}
			else
			{
				if (nWaveType == SAMPLE)
				{
					fValue = aData.m_fSSampleValue;
				}
				if (nWaveType == EFFECT)
				{
					fValue = aData.m_fSEffectValue;
				}
				if (nWaveType == AMP)
				{
					fValue = aData.m_fSAmpValue;
				}
				if (nWaveType == REAL)
				{
					fValue = aData.m_fSRealValue;
				}
				if (nWaveType == ANGLE)
				{
					fValue = aData.m_fSAngleValue;
				}
				if (nWaveType == IMG)
				{
					fValue = aData.m_fSImgValue;
				}
			}
			return true;
		}
	}
	return false;
}

PHASE_TYPE CComTradeFile::GetAChanelPhase(unsigned int nIndex)
{
	CAChanelInfo aInfo = GetAInfoByIndex(nIndex);
	if (aInfo.m_strCh_id.contains("A",Qt::CaseInsensitive))
	{
		return PHASE_A;
	}
	else if (aInfo.m_strCh_id.contains("B",Qt::CaseInsensitive))
	{
		return PHASE_B;
	}
	else if (aInfo.m_strCh_id.contains("C",Qt::CaseInsensitive))
	{
		return PHASE_C;
	}
	return PHASE_NULL;
}

bool	CComTradeFile::IsAInfoPrimary(unsigned int nIndex)
{
	CAChanelInfo aInfo = GetAInfoByIndex(nIndex);
	return IsAInfoPrimary(aInfo);
}

QString CComTradeFile::FileNameFromType(const QString& strFile,FileType type)
{
	QString strRet = strFile;
	QString strBefore = strRet.right(4); //.cfg .hdr ...
	switch (type)
	{
	case Hdr:
		{
			strRet.replace(strBefore,".hdr");
			break;
		}
	case Cfg:
		{
			strRet.replace(strBefore,".cfg");
			break;
		}
	case Dat:
		{
			strRet.replace(strBefore,".dat");
			break;
		}
	case Des:
		{
			strRet.replace(strBefore,".des");
			break;
		}
	case Mid:
		{
			strRet.replace(strBefore,".mid");
			break;
		}
	default:
		break;
	}
	return strRet;
}


bool CComTradeFile::IsAInfoPrimary(const CAChanelInfo& aInfo)
{
	if (aInfo.m_cPs.compare("P",Qt::CaseInsensitive)==0)
	{
		return true;
	}
	return false;
}

//对于每个采样块进行变换
bool CComTradeFile::Fourier(AnalogDataList& dataList,int nBlock)
{
	//求取变换系数
	unsigned int i;

	int nRate = int(0.5+GetSampleRateByBlock(nBlock));

	int sampleDot = (nRate + 25) / 50;//基波采样点数,四舍五入处理

	if(sampleDot == 0)
	{
		m_ErrorList << QString("采样点读取错误");
		return false;
	}
	if((sampleDot > 2) && (1 > (sampleDot / 3)))//根据采样定律,至少3个点
	{
		m_ErrorList << QString("根据采样定律,该算法最大只能计算到nn次谐波");
		return false;
	}
	double rad = PI * 2.0 / sampleDot;//基波角速度

	//傅式算法系数计算,固定取基波一周期的采样点,高次谐波系数不一定重复,以减小误差

	double* fourier_coff_cos = new double[sampleDot];
	double* fourier_coff_sin = new double[sampleDot];

	for(i = 0; i < (unsigned int)sampleDot; i++)
	{
		double lv_harmonic_angle = rad * 1 * i;//谐波角度 = 谐波角速度 X i
		fourier_coff_cos[i] = cos(lv_harmonic_angle);
		fourier_coff_sin[i] = sin(lv_harmonic_angle);
	}

	//a通道值进行变换,使用采样值进行变换
	double lv_sum_real; 
	double lv_sum_imag;
	unsigned int  j;

	for(j = (unsigned int)sampleDot; j < (unsigned int)GetSampleCountByBlock(nBlock); j++)
	{
		lv_sum_real = 0;
		lv_sum_imag = 0;
		for(i  =0; i < (unsigned int)sampleDot; i++)
		{
			double lv_val_temp = dataList[j + i - sampleDot].m_fRawValue;
			lv_sum_real += lv_val_temp * fourier_coff_sin[i];
			lv_sum_imag += lv_val_temp * fourier_coff_cos[i];
		}
		dataList[j].m_fRawRealValue = 2 * lv_sum_real / sampleDot / SQRT_OF_2;
		dataList[j].m_fRawImgValue = 2 * lv_sum_imag / sampleDot / SQRT_OF_2;
		dataList[j].m_fRawEffectValue = sqrt(dataList[j].m_fRawRealValue * dataList[j].m_fRawRealValue 
			+ dataList[j].m_fRawImgValue * dataList[j].m_fRawImgValue);
		dataList[j].m_fRawAngleValue = atan2(dataList[j].m_fRawImgValue, dataList[j].m_fRawRealValue) * PI_TO_ANGLE;
	}
	for(j = 0; j < (unsigned int)sampleDot &&j < (unsigned int)dataList.count()&&j <(unsigned int)(dataList.count() - sampleDot); j++)//第一个周期使用第二个周期的重复
	{
		dataList[j].m_fRawRealValue = dataList[j + sampleDot].m_fRawRealValue;
		dataList[j].m_fRawImgValue = dataList[j + sampleDot].m_fRawImgValue;
		dataList[j].m_fRawEffectValue = dataList[j + sampleDot].m_fRawEffectValue;
		dataList[j].m_fRawAngleValue = dataList[j + sampleDot].m_fRawAngleValue;
	}

	delete [] fourier_coff_cos;
	delete [] fourier_coff_sin;

	return true;
}


CComTradeFile & CComTradeFile::operator=( const CComTradeFile & srcFile)
{
	m_cfgFile = srcFile.m_cfgFile;
	m_datFile = srcFile.m_datFile;
	m_hdrFile = srcFile.m_hdrFile;
	m_midFile = srcFile.m_midFile;
	m_desFile = srcFile.m_desFile;


	m_nTrigger_ms = srcFile.m_nTrigger_ms;	
	m_nFrist_ms = srcFile.m_nFrist_ms;

	m_lFirstMicroSecs = srcFile.m_lFirstMicroSecs;

	m_AnalogDataMap = srcFile.m_AnalogDataMap;

	m_DigitalDataMap = srcFile.m_DigitalDataMap;

	m_StaticValueMap = srcFile.m_StaticValueMap;

	return *this;
}

bool CComTradeFile::IsContinuous()
{
	return (m_cfgFile.m_nNrates == 0);
}

//配置
QString			CComTradeFile::GetRevision()  //版本年号
{
	return m_cfgFile.m_strVersion;
}
bool			CComTradeFile::IsDatBin()	//数据文件是否二进制
{
	return m_cfgFile.m_bBin;
}
QString			CComTradeFile::GetSiteName()	//厂站名称
{
	return m_cfgFile.m_strSite;
}
QString			CComTradeFile::GetUnitName()	//装置特征号
{
	return m_cfgFile.m_strDevice;
}
unsigned int	CComTradeFile::GetChanelCount() //通道数量
{
	return m_cfgFile.m_nAChanelCount + m_cfgFile.m_nDChanelCount;
}
unsigned int	CComTradeFile::GetACount()	//A通道数量
{
	return m_cfgFile.m_nAChanelCount;
}
unsigned int	CComTradeFile::GetDCount()	//D通道数量
{
	return m_cfgFile.m_nDChanelCount;
}

CAChanelInfo    CComTradeFile::GetAInfoByIndex(unsigned int nIndex)
{
	if(nIndex>m_cfgFile.m_AChanelInfoList.count()-1)
	{
		return CAChanelInfo();
	}
	return m_cfgFile.m_AChanelInfoList.at(nIndex);
}

CDChanelInfo    CComTradeFile::GetDInfoByIndex(unsigned int nIndex)
{
	if(nIndex>m_cfgFile.m_DChanelInfoList.count()-1)
	{
		return CDChanelInfo();
	}
	return m_cfgFile.m_DChanelInfoList.at(nIndex);
}

CAChanelInfo    CComTradeFile::GetAInfoByName(const QString& strName,bool& bOk)
{
	for (int nIndex=0; nIndex < m_cfgFile.m_AChanelInfoList.count();nIndex++)
	{
		CAChanelInfo aInfo = m_cfgFile.m_AChanelInfoList.at(nIndex);
		if (!aInfo.m_strCh_id.compare(strName,Qt::CaseInsensitive))
		{
			bOk = true;
			return aInfo;
		}
	}
	bOk = false;
	return CAChanelInfo();
}

CDChanelInfo    CComTradeFile::GetDInfoByName(const QString& strName,bool& bOk)
{
	for (int nIndex=0; nIndex < m_cfgFile.m_DChanelInfoList.count();nIndex++)
	{
		CDChanelInfo dInfo = m_cfgFile.m_DChanelInfoList.at(nIndex);
		if (!dInfo.m_strCh_id.compare(strName,Qt::CaseInsensitive))
		{
			bOk = true;
			return dInfo;
		}
	}
	bOk = false;
	return CDChanelInfo();
}

AChanelInfoList CComTradeFile::GetAChanelInfoList()
{
	return m_cfgFile.m_AChanelInfoList;
}

DChanelInfoList CComTradeFile::GetDChanelInfoList()
{
	return m_cfgFile.m_DChanelInfoList;
}

double			CComTradeFile::GetLineFreq()	//线路频率
{
	return m_cfgFile.m_fIF;
}
unsigned short  CComTradeFile::GetNrates()	//采样率块数
{
	return m_cfgFile.m_nNrates;
}
SampleInfoList  CComTradeFile::GetSampleInfos()	//采样信息
{
	return m_cfgFile.m_SampleInfoList;
}
double			CComTradeFile::GetTimeMult()	//时间比例，正常1.0
{
	return m_cfgFile.m_fTimeMult;
}

CExDateTime     CComTradeFile::GetFirstDataTime()	//第一个采样数据时间
{
	return m_cfgFile.m_dtFirstDataTime;
}
CExDateTime     CComTradeFile::GetTriggeredTime()	//触发时间
{
	return m_cfgFile.m_dtTriggerTime;
}

//数据，这一部分数据将保存所有的
//一次，二次，数据采样，有效 ，幅值，实部,虚部，相角等等
//得到某个时间点某个模拟通道的值
double CComTradeFile::GetAnalogValue(unsigned long nTime,unsigned int nAIndex,
									 ValueType valueType,ValueAttr valueAttr)
{
	double fValue = 0.0;
	if (!m_AnalogDataMap.contains(nTime))
	{
		return fValue;
	}
	AnalogDataList aDataList = m_AnalogDataMap.value(nTime);
	if (nAIndex>=0 && nAIndex < (unsigned int)aDataList.count())
	{
		CAnalogData aData = aDataList.at(nAIndex);
		if (valueType == PRIMARY)
		{
			if (valueAttr == SAMPLE)
			{
				fValue = aData.m_fPSampleValue;
			}
			if (valueAttr == EFFECT)
			{
				fValue = aData.m_fPEffectValue;
			}
			if (valueAttr == AMP)
			{
				fValue = aData.m_fPAmpValue;
			}
			if (valueAttr == REAL)
			{
				fValue = aData.m_fPRealValue;
			}
			if (valueAttr == ANGLE)
			{
				fValue = aData.m_fPAngleValue;
			}
			if (valueAttr == IMG)
			{
				fValue = aData.m_fPImgValue;
			}
		}
		else
		{
			if (valueAttr == SAMPLE)
			{
				fValue = aData.m_fSSampleValue;
			}
			if (valueAttr == EFFECT)
			{
				fValue = aData.m_fSEffectValue;
			}
			if (valueAttr == AMP)
			{
				fValue = aData.m_fSAmpValue;
			}
			if (valueAttr == REAL)
			{
				fValue = aData.m_fSRealValue;
			}
			if (valueAttr == ANGLE)
			{
				fValue = aData.m_fSAngleValue;
			}
			if (valueAttr == IMG)
			{
				fValue = aData.m_fSImgValue;
			}
		}
	}
	return fValue;
}

//得到某个时间点某个数字通道的值
unsigned short CComTradeFile::GetDigitalValue(unsigned long nTime,unsigned int nDIndex)
{
	unsigned short nValue = 0;
	if (!m_DigitalDataMap.contains(nTime))
	{
		return nValue;
	}
	DigitalDataList dDataList = m_DigitalDataMap.value(nTime);
	if (nDIndex>=0 && nDIndex < (unsigned int)dDataList.count())
	{
		return dDataList.at(nDIndex).m_nValue;
	}
	return nValue;
}

//得到某个时间点所有模拟通道的值
QVector<double> CComTradeFile::GetAValueList(unsigned long nTime,
										   ValueType valueType,
										   ValueAttr valueAttr)
{
	QVector<double> valueList;
	if (!m_AnalogDataMap.contains(nTime))
	{
		return valueList;
	}
	AnalogDataList aDataList = m_AnalogDataMap.value(nTime);
	for(int nAIndex=0 ; nAIndex < aDataList.count();nAIndex++)
	{
		CAnalogData aData = aDataList.at(nAIndex);
		if (valueType == PRIMARY)
		{
			if (valueAttr == SAMPLE)
			{
				valueList <<  aData.m_fPSampleValue;
			}
			if (valueAttr == EFFECT)
			{
				valueList << aData.m_fPEffectValue;
			}
			if (valueAttr == AMP)
			{
				valueList << aData.m_fPAmpValue;
			}
			if (valueAttr == REAL)
			{
				valueList << aData.m_fPRealValue;
			}
			if (valueAttr == ANGLE)
			{
				valueList << aData.m_fPAngleValue;
			}
			if (valueAttr == IMG)
			{
				valueList << aData.m_fPImgValue;
			}
		}
		else
		{
			if (valueAttr == SAMPLE)
			{
				valueList << aData.m_fSSampleValue;
			}
			if (valueAttr == EFFECT)
			{
				valueList << aData.m_fSEffectValue;
			}
			if (valueAttr == AMP)
			{
				valueList << aData.m_fSAmpValue;
			}
			if (valueAttr == REAL)
			{
				valueList << aData.m_fSRealValue;
			}
			if (valueAttr == ANGLE)
			{
				valueList << aData.m_fSAngleValue;
			}
			if (valueAttr == IMG)
			{
				valueList << aData.m_fSImgValue;
			}
		}
	}
	return valueList ;
}
										 

//得到某个时间点所有数字通道的值
QVector<unsigned short> CComTradeFile::GetDValueList(unsigned long nTime)
{
	QVector<unsigned short> valueList;
	if (!m_AnalogDataMap.contains(nTime))
	{
		return valueList;
	}
	DigitalDataList dDataList = m_DigitalDataMap.value(nTime);
	for (int nDIndex=0; nDIndex < dDataList.count(); nDIndex++)
	{
		valueList << dDataList.at(nDIndex).m_nValue;
	}

	return valueList;
}


//HDR
QDateTime		CComTradeFile::GetFaultStartTime()  //故障时间
{
	return m_hdrFile.m_dtFaultStartTime;
}

unsigned int	CComTradeFile::GetDataFileSize()	  //数据长度
{
	return m_hdrFile.m_nDataFileSize;
}

unsigned int	CComTradeFile::GetFaultKeepTime()   //故障持续时间
{
	return m_hdrFile.m_nFaultKeepTime;
}

TripInfoList		CComTradeFile::GetTripInfoList()
{
	return m_hdrFile.m_TripInfoList;
}

CTripInfo		CComTradeFile::GetTripInfoByTime(unsigned int nTime,bool& bOK)
{
	for (int nIndex=0; nIndex< m_hdrFile.m_TripInfoList.count(); nIndex++)
	{
		CTripInfo tripInfo = m_hdrFile.m_TripInfoList.at(nIndex);
		if (tripInfo.m_nTime == nTime)
		{
			bOK = true;
			return tripInfo;
		}
	}
	bOK = false;
	return CTripInfo();
}

FaultInfoList		CComTradeFile::GetFaultInfoList()
{
	return m_hdrFile.m_FaultInfoList;
}

DigitalStatusList	CComTradeFile::GetDigitalStatusList()
{
	return m_hdrFile.m_DigitalStatusList;
}

DigitalEventList	CComTradeFile::GetDigitalEventList()
{
	return m_hdrFile.m_DigitalEventList;
}

CDigitalEvent  CComTradeFile::GetDigitalEventByTime(unsigned int nTime,bool& bOK)
{
	for (int nIndex=0; nIndex< m_hdrFile.m_DigitalEventList.count(); nIndex++)
	{
		CDigitalEvent digitalEvent  = m_hdrFile.m_DigitalEventList.at(nIndex);
		if (digitalEvent.m_nTime == nTime)
		{
			bOK = true;
			return digitalEvent;
		}
	}
	bOK = false;
	return CDigitalEvent();
}

SettingValueList	CComTradeFile::GetSettingValueList()
{
	return m_hdrFile.m_SettingValueList;
}

//描述des
CAChanelDes    CComTradeFile::GetADesByIndex(unsigned int nIndex)
{
		if(nIndex>m_desFile.m_DChanelDesList.count())
	{
		CAChanelDes des;
/*		des.m_nAn=-1; //通道索引0~999999
		des.m_strName=""; //名称
		des.m_strType="";	//数据类型
		des.m_strUu="";	//单位
		des.m_fScale=0;  //通道比例 
		des.m_nVisible=1;//是否显示“1”表示显示，”0”表示不显示*/
		return des;
	}
	return m_desFile.m_AChanelDesList.at(nIndex);
}

CDChanelDes    CComTradeFile::GetDDesByIndex(unsigned int nIndex)
{
	if(nIndex>m_desFile.m_DChanelDesList.count())
	{
		CDChanelDes des;
/*		des.m_nDn=-1; //通道索引0~999999
		des.m_strName=""; //名称
		des.m_strType="";	//固定为0
		des.m_nVisible=1;//是否显示“1”表示显示，”0”表示不显示*/
		return des;
	}
	return m_desFile.m_DChanelDesList.at(nIndex);
}

CAChanelDes    CComTradeFile::GetADesByName(const QString& strName,bool& bOK)
{
	for (int nIndex=0; nIndex< m_desFile.m_AChanelDesList.count(); nIndex++)
	{
		CAChanelDes aChanelDes = m_desFile.m_AChanelDesList.at(nIndex);
		if (!aChanelDes.m_strName.compare(strName,Qt::CaseInsensitive))
		{
			bOK = true;
			return aChanelDes;
		}
	}
	bOK = false;
	return CAChanelDes();
}

CDChanelDes    CComTradeFile::GetDDesByName(const QString& strName,bool& bOK)
{
	for (int nIndex=0; nIndex< m_desFile.m_DChanelDesList.count(); nIndex++)
	{
		CDChanelDes dChanelDes = m_desFile.m_DChanelDesList.at(nIndex);
		if (!dChanelDes.m_strName.compare(strName,Qt::CaseInsensitive))
		{
			bOK = true;
			return dChanelDes;
		}
	}
	bOK = false;
	return CDChanelDes();
}

AChanelDesList CComTradeFile::GetAChanelDesList()
{
	return m_desFile.m_AChanelDesList;
}

DChanelDesList CComTradeFile::GetDChanelDesList()
{
	return m_desFile.m_DChanelDesList;
}


//中间数据 mid
double CComTradeFile::GetMidAnalogValue(unsigned long nTime,unsigned int nAIndex)
{
	for (int nIndex=0; nIndex < m_midFile.m_ChanelDataList.count(); nIndex++)
	{
		CChanelSampleData samData = 	m_midFile.m_ChanelDataList.at(nIndex);
		if (samData.m_nTimeStamp == nTime && nAIndex < (unsigned int)samData.m_AChanelDataList.count())
		{
			return samData.m_AChanelDataList.at(nAIndex);
		}
	}

	return 0.0;
}

//得到某个时间点某个数字通道的值
unsigned short CComTradeFile::GetMidDigitalValue(unsigned long nTime,
												 unsigned int nDIndex)
{
	if (m_MidSampleDataMap.contains(nTime))
	{
		CChanelSampleData samData = 	m_MidSampleDataMap.value(nTime);
		if (nDIndex < (unsigned int)samData.m_DChanelDataList.count())
		{
			return samData.m_DChanelDataList.at(nDIndex);
		}
	}
	return 0;
}

//得到某个时间点所有模拟通道的值
QVector<double> CComTradeFile::GetAMidValueList(unsigned long nTime)
{
	QVector<double> valueList;
	for (int nIndex=0; nIndex < m_midFile.m_ChanelDataList.count(); nIndex++)
	{
		CChanelSampleData samData = 	m_midFile.m_ChanelDataList.at(nIndex);
		if (samData.m_nTimeStamp == nTime)
		{
			for (int nAIndex=0;nAIndex<samData.m_AChanelDataList.count();nAIndex++)
			{
				valueList << samData.m_AChanelDataList.at(nAIndex);
			}
			break;
		}
	}

	return valueList; 
}

//得到某个时间点所有数字通道的值
QVector<unsigned short> CComTradeFile::GetDMidValueList(unsigned long nTime)
{
	QVector<unsigned short> valueList;
	for (int nIndex=0; nIndex < m_midFile.m_ChanelDataList.count(); nIndex++)
	{
		CChanelSampleData samData = 	m_midFile.m_ChanelDataList.at(nIndex);
		if (samData.m_nTimeStamp == nTime)
		{
			valueList << samData.m_DChanelDataList;
			break;
		}
	}

	return valueList; 
}

bool	CComTradeFile::IsDMidChanelAct(unsigned int nDIndex)
{
	if (m_MidDigitalChangedMap.contains(nDIndex))
	{
		return m_MidDigitalChangedMap[nDIndex];
	}
	return false;
}

unsigned short CComTradeFile::GetMidDigitalValueByIndexPoint(unsigned int nIndex,unsigned int nPoint)
{
	if (nPoint >=0 && nPoint < (unsigned int)m_TimeArray.count())
	{
		unsigned int nTime = m_TimeArray.at(nPoint);
		return GetMidDigitalValue(nTime,nIndex);
	}
	return 0;	
}

//add by llf
int CComTradeFile::GetPathNumByDesc(QString strContent,int src)
{

		bool IsOk=false;


		if(src==0)
		{
			CDChanelInfo CDInfo = GetDInfoByName(strContent,IsOk);

			if(IsOk)
			{

				return CDInfo.m_nDn;
			}
			else
			{
				return -1;
			}
		}

		else
		{
			CDChanelDes CDDes = GetDDesByName(strContent,IsOk);
			if(IsOk)
			{
				return CDDes.m_nDn;
			}
			else
			{
				return -1;
			}
		}


}
