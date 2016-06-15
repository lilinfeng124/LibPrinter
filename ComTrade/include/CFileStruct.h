#ifndef _FILESTRUCT_H_
#define _FILESTRUCT_H_

#include <QVector>
#include <QString>
#include <QDateTime>


enum ValueType
{
	PRIMARY,		//һ��
	SECONDARY,		//����
};

enum ValueAttr
{	
	SAMPLE,			//����
	EFFECT,			//��Ч
	AMP,			//��ֵ
	REAL,			//ʵ��
	IMG,			//�鲿
	ANGLE,			//���
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


//Aͨ����Ϣ
class CAChanelInfo
{
public:
	unsigned int m_nAn; //ͨ������0~999999
	QString m_strCh_id; //ͨ��ʶ��� <64 ��ѡ
	QString m_strPh;	//ͨ����λ���� <2 ��ѡ
	QString m_strCcbm;	//�����ӵĻ�·ԭ�� <64 ��ѡ
	QString m_strUu;	//ͨ����λ 
	double  m_fA;		//ͨ������
	double	m_fB;		//ͨ������
	double	m_fSkew;	//�ӳ������ڿ�ʼ���ͨ��ʱ�ͣ�΢�룩��ѡ
	int		m_nMin;		//ͨ��������Сֵ -99999~99999�ڶ�������������-32767~32767
	int		m_nMax;		//ͨ��������Сֵ -99999~99999�ڶ�������������-32767~32767
	double  m_fPrimary;		//ͨ����ѹ������任һ�α�
	double	m_fSecondary;	//ͨ����ѹ������任���α�
	QString	m_cPs;			//һ�λ����
};
typedef QVector<CAChanelInfo> AChanelInfoList;

//Dͨ����Ϣ
class CDChanelInfo
{
public:
	unsigned int m_nDn; //ͨ������0~999999
	QString m_strCh_id; //ͨ��ʶ��� <64 ��ѡ
	QString m_strPh;	//ͨ����λ���� <2 ��ѡ
	QString m_strCcbm;	//�����ӵĻ�·ԭ�� <64 ��ѡ
	short	 m_nY;		//��һ��Ԫ��������̬��������ʱ��Ϊ����״̬��ͬ���̵�״̬
};

typedef QVector<CDChanelInfo> DChanelInfoList;

//��������Ϣ
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
	unsigned int m_nWs;		//΢��
};

typedef QVector<int>			  IntDataList;
typedef QVector<unsigned short> UShortDataList;
typedef QVector<bool>			  BoolDataList;

class CChanelSampleData
{
public:
	unsigned long m_nTick;					//�������
	unsigned long m_nTimeStamp;				//����ʱ��
	IntDataList m_AChanelDataList;
	UShortDataList m_DChanelDataList;
	//�Ƿ���Ч
	BoolDataList m_AChanelDataValidList;
};

typedef QVector<CChanelSampleData> ChanelDataList;

class CAnalogData
{
public:
	//unsigned int	m_nAn;		//ͨ������0~999999
	unsigned long	m_nTimeStamp;	
	unsigned long	m_nIndex;	
	double			m_fRawValue;
	double			m_fRawRealValue;
	double			m_fRawImgValue;
	double			m_fRawEffectValue; //��ֵ=��Ч*����2
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

//Aͨ������
class CAChanelDes
{
public:
	unsigned int m_nAn; //ͨ������0~999999
	QString	 m_strName; //����
	QString  m_strType;	//��������
	QString  m_strUu;	//��λ
	double   m_fScale;  //ͨ������ 
	short	 m_nVisible;//�Ƿ���ʾ��1����ʾ��ʾ����0����ʾ����ʾ
};

typedef QVector<CAChanelDes> AChanelDesList;

//Dͨ������
class CDChanelDes
{
public:
	unsigned int m_nDn; //ͨ������0~999999
	QString	 m_strName; //����
	QString  m_strType;	//�̶�Ϊ0
	short	 m_nVisible;//�Ƿ���ʾ��1����ʾ��ʾ����0����ʾ����ʾ
};

typedef QVector<CDChanelDes> DChanelDesList;


#endif
