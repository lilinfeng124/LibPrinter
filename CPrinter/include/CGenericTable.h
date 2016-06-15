#include <QString>
#include <QObject>
#include <QPainter>
#include <QPrinter>
#include "CPrinterObject.h"

class TableNode
{
public:
	TableNode(TableNode*parent=0);
	~TableNode();
	int GetLevel();
	TableNode* CreateSon(QString str,QString strValue="");
	//计算所需行数
	int CalculateLineCount();
	//所占的行数.这个是程序用的，不需要初始化
	int m_lineCount;
	int m_leftCount;
	//子节点
	QList<TableNode*>m_pSonNodeLst;
	//打印内容
	QString    m_strName;
	//最底层的节点打印形式为 m_strName : m_strValue，其他上层node打印形式为 m_strName.
	QString    m_strValue;
	TableNode* m_pParent;
private:

	int        m_nLevel;



};

class CGenericTable:public CPrinterObject
{
public:
	CGenericTable(QPrinter* printer);
	~CGenericTable();
	//打印
	void DoPrinter(QPainter*p);
	//设置打印的title
	void SetTitle(QString& str);
	//设置打印表格描述
	void SetDetailDesc(QString& str);
	//设置表头内容
	void SetTableHeader(const QStringList& lst);

    void AddToNodeLst(TableNode*p);

    void ClearLst();

private:
	//获取指定lvl的node集合	
	void GetNodesByLvl(TableNode*p,int nLvel,QList<TableNode*>&lst);
	//打印标题s	
	void PrintTitle(QPainter*p);
	
	void PrintHeader(QPainter*p);
	//计算数字
	void CalculateLine(QPainter*p);
	//打印表格
	void PrintTable(QPainter*p);
	//翻页
	void NewPage(QPainter*p);
	
	//打印表头
	void DrawTableHeader(QPainter*p);
	//打印表内容
	void DrawTableContent(QPainter*p);


private:
	QList<TableNode*>m_NodeLst;
	QString m_strTitle;
	QString m_strDetail;

	QStringList m_strTableHeaderList;

	//等级最低的node集合
	QList<TableNode*>m_TmpNodeLst;
	int        m_nCurrentNodeIndex;
	//存储列的起点终点的数列
	QList<int>        m_SpanLst;

	
	int     m_nCurrentY;
	int     m_nTableColum;	
	QFont   m_font;

};
