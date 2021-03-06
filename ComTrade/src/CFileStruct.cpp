﻿#include "CFileStruct.h"

const double MAXINVALIDVALUE = -99999999.0;
const double MININVALIDVALUE = 99999999.0;

CStaticAnalogData::CStaticAnalogData()
{
	m_nIndex = 0;	

	m_fMaxRawValue = MAXINVALIDVALUE;
	m_fMaxRawRealValue = MAXINVALIDVALUE;
	m_fMaxRawImgValue = MAXINVALIDVALUE;
	m_fMaxRawEffectValue = MAXINVALIDVALUE;
	m_fMaxRawAngleValue = MAXINVALIDVALUE;

	m_fMinRawValue = MININVALIDVALUE;
	m_fMinRawRealValue = MININVALIDVALUE;
	m_fMinRawImgValue = MININVALIDVALUE;
	m_fMinRawEffectValue = MININVALIDVALUE;
	m_fMinRawAngleValue = MININVALIDVALUE;

	m_fPMaxSampleValue = MAXINVALIDVALUE;
	m_fPMinSampleValue = MININVALIDVALUE;

	m_fSMaxSampleValue = MAXINVALIDVALUE;
	m_fSMinSampleValue = MININVALIDVALUE;

	m_fPMaxEffectValue = MAXINVALIDVALUE;
	m_fPMinEffectValue = MININVALIDVALUE;

	m_fSMaxEffectValue = MAXINVALIDVALUE;
	m_fSMinEffectValue = MININVALIDVALUE;

	m_fPMaxRealValue = MAXINVALIDVALUE;
	m_fPMinRealValue = MININVALIDVALUE;

	m_fSMaxRealValue = MAXINVALIDVALUE;
	m_fSMinRealValue = MININVALIDVALUE;

	m_fPMaxImgValue = MAXINVALIDVALUE;
	m_fPMinImgValue = MININVALIDVALUE;

	m_fSMaxImgValue = MAXINVALIDVALUE;
	m_fSMinImgValue = MININVALIDVALUE;

	m_fPMaxAmpValue = MAXINVALIDVALUE;
	m_fPMinAmpValue = MININVALIDVALUE;

	m_fSMaxAmpValue = MAXINVALIDVALUE;
	m_fSMinAmpValue = MININVALIDVALUE;

	m_fPMaxAngleValue = MAXINVALIDVALUE;
	m_fPMinAngleValue = MININVALIDVALUE;

	m_fSMaxAngleValue = MAXINVALIDVALUE;
	m_fSMinAngleValue = MININVALIDVALUE;
}

CStaticAnalogData::~CStaticAnalogData()
{

}