#include "CHdrFile.h"

#include <QtDebug>
#include <QDomNode>
#include <QDomDocument>

CHdrFile::CHdrFile ()
{

}

CHdrFile::~CHdrFile()
{

}


bool CHdrFile::ReadHdrFile(const QString& strFile)
{
	QDomDocument domDocument;
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
	if (!domDocument.setContent(&file, false)) 
	{	
		m_ErrorList << QString("文件%1不符合XML规范，读取出错.").arg(strFile);
		file.close();
		return false;
	}

	QDomElement root = domDocument.documentElement();
	if (root.tagName() != "FaultReport")
	{
		m_ErrorList << QString("文件%1不是录波信息文件.").arg(strFile);
		file.close();
		return false;
	} 

	QDomElement child = root.firstChildElement();
	while (!child.isNull()) 
	{
		QString strTag = child.tagName();
		if (strTag == "FaultStartTime")
		{
			QString strValue = ElementValue(child);
			m_dtFaultStartTime = QDateTime::fromString(strValue,"yyyy-MM-dd hh:mm:ss:zzz");
		}
		else if (strTag == "TripInfo")
		{
			ReadTripInfo(child);
		}
		else if (strTag == "FaultInfo")
		{
			ReadFaultInfo(child);
		}
		else if (strTag == "DigitalStatus")
		{
			ReadDigitalStatus(child);
		}
		else if (strTag == "DigitalEvent")
		{
			ReadDigitalEvent(child);
		}
		else if (strTag == "SettingValue")
		{
			ReadSettingValue(child);
		}	
		else if (strTag == "DataFileSize")
		{
			bool bOk = true;
			m_nDataFileSize = ElementValue(child).toUInt(&bOk);
			if (!bOk)
			{
				m_ErrorList << QString("文件%1 DataFileSize 无法转换.").arg(strFile);
				file.close();
				return false;
			}
		}
		else if (strTag == "FaultKeepingTime")
		{
			bool bOk = true;
			QString strFaultKeepTime = ElementValue(child);
			if (strFaultKeepTime.endsWith("ms"))
			{
				strFaultKeepTime = strFaultKeepTime.left(strFaultKeepTime.length()-2);
			}
			m_nFaultKeepTime = strFaultKeepTime.toUInt(&bOk);
			if (!bOk)
			{
				m_ErrorList << QString("文件%1 FaultKeepingTime 无法转换.").arg(strFile);
				file.close();
				return false;
			}
		}

		child = child.nextSiblingElement();
	}
	file.close();
	return true;
}


QString CHdrFile::ElementValue(const QDomElement& domElement)
{
	QDomNode valueNode = domElement.firstChild();
	if (!valueNode.isNull())
	{
		return valueNode.nodeValue().trimmed();
	}
	return QString();
}

bool CHdrFile::ReadTripInfo(const QDomElement& tripElement)
{
	CTripInfo tripInfo;
	QDomElement child = tripElement.firstChildElement();
	while (!child.isNull()) 
	{
		QString strTag = child.tagName();
		if (strTag == "time")
		{
			QString strTime = ElementValue(child);
			if (strTime.endsWith("ms"))
			{
				strTime = strTime.left(strTime.length()-2);
			}
			tripInfo.m_nTime = strTime.toUInt();
		}
		else if (strTag == "name")
		{
			tripInfo.m_strName = ElementValue(child);
		}
		else if (strTag == "phase")
		{
			tripInfo.m_strPhase = ElementValue(child);
		}
		else if (strTag == "value")
		{
			tripInfo.m_nValue = ElementValue(child).toUShort();
		}
		else if (strTag == "FaultInfo")
		{
			ReadFaultInfo(child,&tripInfo);
		}

		child = child.nextSiblingElement();
	}
	m_TripInfoList.append(tripInfo);
	return true;
}

bool CHdrFile::ReadFaultInfo(const QDomElement& faultElement,CTripInfo* pTripInfo)
{
	QDomElement child = faultElement.firstChildElement();
	CFaultInfo faultInfo;
	while (!child.isNull()) 
	{
		QString strTag = child.tagName();

		if (strTag == "name")
		{
			faultInfo.m_strName = ElementValue(child);
		}
		else if (strTag == "value")
		{
			faultInfo.m_strValue = ElementValue(child);
		}

		child = child.nextSiblingElement();
	}

	if (NULL == pTripInfo)
	{
		m_FaultInfoList.append(faultInfo);
	}
	else
	{
		pTripInfo->m_FaultInfoList.append(faultInfo);
	}
	return true;
}

bool CHdrFile::ReadDigitalStatus(const QDomElement& digitalStatusElement)
{
	QDomElement child = digitalStatusElement.firstChildElement();
	CDigitalStatus digitalStatus;
	while (!child.isNull()) 
	{
		QString strTag = child.tagName();
		if (strTag == "name")
		{
			digitalStatus.m_strName = ElementValue(child);
		}
		else if (strTag == "value")
		{
			digitalStatus.m_nValue = ElementValue(child).toUShort();
		}

		child = child.nextSiblingElement();
	}
	m_DigitalStatusList.append(digitalStatus);
	return true;
}

bool CHdrFile::ReadDigitalEvent(const QDomElement& digitalEventElement)
{
	CDigitalEvent digitalEvent;
	QDomElement child = digitalEventElement.firstChildElement();
	while (!child.isNull()) 
	{
		QString strTag = child.tagName();
		if (strTag == "time")
		{
			QString strTime = ElementValue(child);
			if (strTime.endsWith("ms"))
			{
				strTime = strTime.left(strTime.length()-2);
			}
			digitalEvent.m_nTime = strTime.toUInt();
		}
		else if (strTag == "name")
		{
			digitalEvent.m_strName = ElementValue(child);
		}
		else if (strTag == "value")
		{
			digitalEvent.m_nValue = ElementValue(child).toUShort();
		}

		child = child.nextSiblingElement();
	}
	m_DigitalEventList.append(digitalEvent);
	return true;
}

bool CHdrFile::ReadSettingValue(const QDomElement& settingValueElement)
{
	QDomElement child = settingValueElement.firstChildElement();
	CSettingValue settingValue;
	while (!child.isNull()) 
	{
		QString strTag = child.tagName();
		if (strTag == "name")
		{
			settingValue.m_strName = ElementValue(child);
		}
		else if (strTag == "value")
		{
			settingValue.m_strValue = ElementValue(child);
		}
		else if (strTag == "unit")
		{
			settingValue.m_strUnit = ElementValue(child);
		}

		child = child.nextSiblingElement();
	}
	m_SettingValueList.append(settingValue);
	return true;
}