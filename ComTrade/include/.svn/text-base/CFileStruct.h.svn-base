#ifndef _FILESTRUCT_H_
#define _FILESTRUCT_H_

#include <QVector>
#include <QString>
#include <QDateTime>


enum ValueType
{
	PRIMARY,		//一次
	SECONDARY,		//二次
};

enum ValueAttr
{	
	SAMPLE,			//采样
	EFFECT,			//有效
	AMP,			//幅值
	REAL,			//实部
	IMG,			//虚部
	ANGLE,			//相角
};

class  CFaultInfo
{
public:
	QString m_strName;
	QString m_strValue;
};

typedef QVector<CFaultInfo> FaultInfoList;

class  CTripInfo
{
public:
	unsigned int	m_nTime;
	QString			m_strName;
	QString			m_strPhase;
	unsigned short	m_nValue;
	FaultInfoList	m_FaultInfoList;
};

typedef QVector<CTripInfo> TripInfoList;

class  CDigitalStatus
{
public:
	QString			m_strName;
	unsigned short	m_nValue;
};

typedef QVector<CDigitalStatus> DigitalStatusList;

class  CDigitalEvent
{
public:
	unsigned int	m_nTime;
	QString			m_strName;
	unsigned short	m_nValue;
};

typedef QVector<CDigitalEvent> DigitalEventList;

class  CSettingValue
{

public:
	QString		m_strName;
	QString		m_strValue;
	QString		m_strUnit;

};

typedef QVector<CSettingValue> SettingValueList;


//A通道信息
class CAChanelInfo
{
public:
	unsigned int m_nAn; //通道索引0~999999
	QString m_strCh_id; //通道识别符 <64 可选
	QString m_strPh;	//通道相位特征 <2 可选
	QString m_strCcbm;	//被监视的回路原件 <64 可选
	QString m_strUu;	//通道单位 
	double  m_fA;		//通道乘数
	double	m_fB;		//通道加数
	double	m_fSkew;	//从抽样周期开始后的通道时滞（微秒）可选
	int		m_nMin;		//通道数据最小值 -99999~99999在二进制中限制在-32767~32767
	int		m_nMax;		//通道数据最小值 -99999~99999在二进制中限制在-32767~32767
	double  m_fPrimary;		//通道电压或电流变换一次比
	double	m_fSecondary;	//通道电压或电流变换二次比
	QString	m_cPs;			//一次或二次
};
typedef QVector<CAChanelInfo> AChanelInfoList;

//D通道信息
class CDChanelInfo
{
public:
	unsigned int m_nDn; //通道索引0~999999
	QString m_strCh_id; //通道识别符 <64 可选
	QString m_strPh;	//通道相位特征 <2 可选
	QString m_strCcbm;	//被监视的回路原件 <64 可选
	short	 m_nY;		//当一次元件处于稳态服务条件时作为输入状态的同大盘的状态
};

typedef QVector<CDChanelInfo> DChanelInfoList;

//采样率信息
class CSampleInfo
{
public:
	double		   m_fSamp;		
	unsigned int   m_nCount; //1~9999999999
};

typedef QVector<CSampleInfo> SampleInfoList;

class CExDateTime
{
public :
	QDateTime m_dtTime;
	unsigned int m_nWs;		//微秒
};

typedef QVector<int>			  IntDataList;
typedef QVector<unsigned short> UShortDataList;
typedef QVector<bool>			  BoolDataList;

class CChanelSampleData
{
public:
	unsigned long m_nTick;					//采样序号
	unsigned long m_nTimeStamp;				//采样时间
	IntDataList m_AChanelDataList;
	UShortDataList m_DChanelDataList;
	//是否有效
	BoolDataList m_AChanelDataValidList;
};

typedef QVector<CChanelSampleData> ChanelDataList;

class CAnalogData
{
public:
	//unsigned int	m_nAn;		//通道索引0~999999
	unsigned long	m_nTimeStamp;	
	unsigned long	m_nIndex;	
	double			m_fRawValue;
	double			m_fRawRealValue;
	double			m_fRawImgValue;
	double			m_fRawEffectValue; //幅值=有效*根号2
	double			m_fRawAngleValue;

	double			m_fPSampleValue;
	double			m_fSSampleValue;
	double			m_fPEffectValue;
	double			m_fSEffectValue;
	double			m_fPRealValue;
	double			m_fSRealValue;
	double			m_fPImgValue;
	double			m_fSImgValue;
	double			m_fPAmpValue;
	double			m_fSAmpValue;
	double			m_fPAngleValue;
	double			m_fSAngleValue;
	bool			m_bValid;
};

typedef QVector<CAnalogData> AnalogDataList;

class CStaticAnalogData
{
public:
	CStaticAnalogData();
	~CStaticAnalogData();
public:
	unsigned long	m_nIndex;	

	double			m_fMaxRawValue;
	double			m_fMaxRawRealValue;
	double			m_fMaxRawImgValue;
	double			m_fMaxRawEffectValue;
	double			m_fMaxRawAngleValue;

	double			m_fMinRawValue;
	double			m_fMinRawRealValue;
	double			m_fMinRawImgValue;
	double			m_fMinRawEffectValue;
	double			m_fMinRawAngleValue;

	double			m_fPMaxSampleValue;
	double			m_fPMinSampleValue;

	double			m_fSMaxSampleValue;
	double			m_fSMinSampleValue;

	double			m_fPMaxEffectValue;
	double			m_fPMinEffectValue;

	double			m_fSMaxEffectValue;
	double			m_fSMinEffectValue;

	double			m_fPMaxRealValue;
	double			m_fPMinRealValue;

	double			m_fSMaxRealValue;
	double			m_fSMinRealValue;

	double			m_fPMaxImgValue;
	double			m_fPMinImgValue;

	double			m_fSMaxImgValue;
	double			m_fSMinImgValue;

	double			m_fPMaxAmpValue;
	double			m_fPMinAmpValue;

	double			m_fSMaxAmpValue;
	double			m_fSMinAmpValue;

	double			m_fPMaxAngleValue;
	double			m_fPMinAngleValue;

	double			m_fSMaxAngleValue;
	double			m_fSMinAngleValue;
};

class CDigitalData
{
public:
	unsigned int	m_nIndex; 
	unsigned long	m_nTimeStamp;	
	unsigned short	m_nValue;
	bool			m_bChanged;
};


typedef QVector<CDigitalData> DigitalDataList;

//A通道描述
class CAChanelDes
{
public:
	unsigned int m_nAn; //通道索引0~999999
	QString	 m_strName; //名称
	QString  m_strType;	//数据类型
	QString  m_strUu;	//单位
	double   m_fScale;  //通道比例 
	short	 m_nVisible;//是否显示“1”表示显示，”0”表示不显示
};

typedef QVector<CAChanelDes> AChanelDesList;

//D通道描述
class CDChanelDes
{
public:
	unsigned int m_nDn; //通道索引0~999999
	QString	 m_strName; //名称
	QString  m_strType;	//固定为0
	short	 m_nVisible;//是否显示“1”表示显示，”0”表示不显示
};

typedef QVector<CDChanelDes> DChanelDesList;


#endif
