#ifndef _HDRFILE_H_
#define _HDRFILE_H_

#include "LibComTrade.h"
#include "CFileStruct.h"

#include <QFile>
#include <QString>
#include <QDateTime>

#include <QDomElement>
#include <QStringList>

class  CHdrFile
{
public:
	CHdrFile ();
	~CHdrFile();

public:
	bool ReadHdrFile(const QString& strFile);

private:
	bool ReadTripInfo(const QDomElement& tripElement);
	bool ReadFaultInfo(const QDomElement& faultElement,CTripInfo* pTripInfo = NULL);
	bool ReadDigitalStatus(const QDomElement& digitalStatusElement);
	bool ReadDigitalEvent(const QDomElement& digitalEventElement);
	bool ReadSettingValue(const QDomElement& settingValueElement);

	QString ElementValue(const QDomElement& domElement);
public:
	QDateTime		m_dtFaultStartTime;

	unsigned int	m_nDataFileSize;

	unsigned int	m_nFaultKeepTime;

	TripInfoList		m_TripInfoList;
	FaultInfoList		m_FaultInfoList;
	DigitalStatusList	m_DigitalStatusList;
	DigitalEventList	m_DigitalEventList;
	SettingValueList	m_SettingValueList;

	QStringList m_ErrorList;
};

#endif
