#pragma once
#include "afxtempl.h"
#include <Eigen/Dense>
#include<iomanip>
#include"Vec_nD.h"
#include<vector>
#include<string>
#include<fstream>
#include <sstream>
#include<cstdio>
#include<ctime>
#include<iostream>
using namespace std;
using Eigen::MatrixXd;
#define thea pow(10,-3)
#define random(x) rand()%(x)
const int maxn = 1e4 + 20;
struct point
{
	float x, y;
	point() {}
	point(float xx, float yy)
		:x(xx), y(yy) {}
};

struct HwIsoPt
{
	double x; //������
	double y; //������
	double z; // ����ֵ
};
struct HwIsoLine // ��ֵ���߽ṹ
{
	double zValue; // ��ֵ������ֵ
	HwIsoPt* pts; // ��ֵ��ƽ������
	int ptn; //�������
};
struct TWOVALUE
{
	double X;
	double Y;
};
struct THRVALUE
{
	double X;
	double Y;
	double Z;
};
//��ֵ��������
struct IsoLine
{
	TWOVALUE direction0;	//����λ�ã��Ӽ��𾮵��Ǽ���  20140313
	TWOVALUE direction1;	//�Ǽ��𾮣��Ӽ��𾮵��Ǽ���  20140313
	int  Index;				//��direction0��direction1��Ӧ�ĵ��ڵ�ֵ���е�λ�ã����ڵ�ֵ���������估�������жϣ�20130314
	double Value;			//��ֵ�ߵ�����ֵ
	vector<TWOVALUE> Logic; //��ֵ�ߵ��߼�����
};
struct Dis //�����ݽṹ����ʲô
{
	double dis;//����
	double per;//�ٷֱ�
	double angle;//�Ƕ�
};
struct Fault
{
	int		    m_nfault;					//Fault�ߵ�Ψһ��־	 �ϲ�����
	BOOL		m_bUpOrDown;				//Ϊtrue,�������̣�Ϊfalse��Ϊ�½���
	bool        m_InverseFlag;              //����ϲ��־�����ϲ�:false;��ϲ�ʹ�ֱ�ϲ�:true
	bool        m_Mid;                      //�Ƿ����м���
	int         m_InfaultNum;				//�ϲ������еı�ŵ��߱��

	vector<TWOVALUE> m_pts; // �ϲ�������
};
struct FaultPoint
{
	int sign;					//�ϲ����
	int	mark;					//���ϲ㻹����ϲ�  ���ϲ�Ϊ1 ��ϲ�Ϊ0
	vector<TWOVALUE> UData;		//�����̣�upthrown block��
	vector<TWOVALUE> DData;		//�½��̣�downthrown side��
	vector<THRVALUE> TData;		//�м���
	vector<double>  Gap;        //���
								//��ϲ� ������ ����  + ���½����½� -
								//���ϲ� ������ -  �½���+


								//�������£��˴���������Ӧ�������̡������½���Ӧ��������  ����20130903
								//http://www.doc88.com/p-86415001780.html   ��ʿ��
};
struct CFpoint
{
	double x;
	double y;
};
struct SingleFalut
{
	//vector<TWOVALUE> FLine;	//�ϲ�������
	vector<THRVALUE> FLine;		//�ϲ������� 20140915
};
class Data
{
public:
	Data(double x = 0, double y = 0, double z = 0) { X = x; Y = y; Z = z; mark = 0; };
	double X;
	double Y;
	double Z;
	int mark;//�ж����ϲ㻹����ϲ�  ���ϲ�Ϊ1 ��ϲ�Ϊ0
	void Init(double x = 0, double y = 0, double z = 0) { X = x; Y = y; Z = z; mark = 0; }

};
struct FalutLine
{
	vector<TWOVALUE> FLine;			//�ϲ�������
	bool IsClose;                 	//�Ƿ��Ǳպ϶ϲ㣬TUREΪ�պ�
	bool IsNormal;			  		//�Ƿ�Ϊ���ϲ㣬TUREΪ���ϲ�
	int  IsClock;					//�Ƿ�Ϊ˳ʱ�룬1Ϊ˳ʱ�룬-1Ϊ��ʱ�룬�Խ�Ϻ�Ķϲ㣬ʹ�ú����жϲ��Ǻ���ȷ20140722
};
struct GridFL						//GridFaultLine		
{
	vector<SingleFalut> FLines;	//�ϲ�������
};
struct FLOValue					//�����е����ϲ��ߵ����� 20140724	
{
	double Star;				//�׵������������
	double End;					//ĩ�������������
	SingleFalut FLines;			//�ϲ�������
};
class Jie_Ds
{
public:
	vector<Data> m_oriData; //ԭʼ���ݣ���λ��Ϣ����������
							//vector<FaultPoint> m_FaultData; //�ϲ���Ϣ20130814
	vector<vector<THRVALUE>> m_GridPoint;//���ɵ�����
	int m_XNum; //������������
	int m_YNum; //������������
	int m_DIV; //��ֵ�߼��
	double m_XMin; //x������Сֵ
	double m_XMax;  //X�������ֵ
	double m_YMin; //y������Сֵ
	double m_YMax; //Y�������ֵ
	double m_ZMin; //������Сֵ
	double m_ZMax; //�������ֵ
	double exp;//͹���������
	double m_Show_MinValue;
	double m_Show_MaxValue;
	double m_Show_JianGeValue;
	vector<Data> m_Border;
	vector<Data> m_OriBoder;					//ԭʼ����ʵ�߽磨�����ǰ��ϲ㣩20140801
	bool m_IsConvex;							//ԭʼ�ı߽��Ƿ�Ϊ͹��20140801
	bool m_IsK;								//�Ƿ�ʹ�ÿ�����ֵ 20150323

	bool m_bUseFault;
	double m_valuedis;//��ֵ�߼�� 20140724
	double m_B;		 //ֱ���ͱ��캯��Bֵ20140728
	vector<Data> m_suV;
	vector<double> m_ni;
	vector<IsoLine>m_IsoLine;
	//�洢���еĵ�ֵ��,ÿ����ֵ�ߴ洢ʵ��ֵ����Ҫ�ǵ�ֵ���ڶϲ��ϲ���ʹ������洢
	vector<IsoLine>m_IsoRealLine;
	vector<IsoLine>opt_IsoRealLine;
	vector<double> m_TrackValue;				//ҪѰ�ҵĵ�ֵ��ֵ
	CArray<double,double> m_sameArray;
	vector<HwIsoLine> m_lsoLines;
	vector<IsoLine> Jie_IsoLine;
	vector<TWOVALUE>Jie_OriBoder;
	vector<IsoLine>Jie_IsoRealLine;
	vector<double> Jie_RNum;
	vector<Vector2D> originPoint;
	vector<Vector2D> curvePoint;
	vector<point> input_vertice;
	vector<point> control_point;
	vector<point> b_spline;
	vector<Fault> m_faults;
	CArray<int, int> m_faultNunArray;// ��¼�ϲ����š�
	vector<FaultPoint> m_FaultData;
	CArray<CFpoint, CFpoint> m_TepFPointArray;    //��ʱ���������ɶϲ���ʱ
	vector<FalutLine> m_vecFaultLines;			//����׷�٣��洢���еĶϲ�����Ϣ
	vector<vector<int>> m_NoFault;				//�ж�ÿ���������Ƿ��жϲ㣬����洢�ϲ��ߵ����
	vector<vector<BOOL>> m_XFault;				//�ж�ÿ��X�������Ƿ��жϲ�,���ڳ�ʼ��ֵ��Ĺ��ơ��ϲ����ߵĵ�ֵ����ʱ��������
	vector<vector<BOOL>> m_YFault;				//�ж�ÿ��Y�������Ƿ��жϲ�
	vector<vector<SingleFalut>> m_XFaultPts;	//�洢ÿ��X�������Ƕϲ�����20140916
	vector<vector<SingleFalut>> m_YFaultPts;	//�洢ÿ��Y�������Ƕϲ�����20140916
	vector<vector<BOOL>> m_GridInFault;			//�洢������Ƿ��ڶϲ���   20140916
	vector<GridFL> m_vecGridFL;					//�洢�����ڵĶϲ���Ϣ
	vector<vector<THRVALUE>> m_GridFault;		//����ÿ�������ڸ���ֵ����ɵ�ͼ�ζԲ���
public:
	void ReadFaultStr(CString strFault);
	void SetFault(char* strFaultData);
	void IsFault_ABNull(int group, int &m_flag, bool &InverseFlag);
	void Fault_A(vector<TWOVALUE>& m_FaultA, int m_All_FaultLineNum, int m_CurFaultGroupNum, BOOL m_flag);
	void Fault_B(vector<TWOVALUE>& m_FaultB, int m_All_FaultLineNum, int m_CurFaultGroupNum, BOOL m_flag);
	void Fault_C(vector<TWOVALUE>& m_FaultB, int m_All_FaultLineNum, int m_CurFaultGroupNum, BOOL m_flag);
	void CalcFault();
	bool IsOnePT(TWOVALUE PtA, TWOVALUE PtB);
	int GerEquNum(vector<TWOVALUE> PtA, vector<TWOVALUE>  PtB);
	int  GetCrossNum(FaultPoint LineI, FaultPoint LineJ);
	TWOVALUE GerNearPt(TWOVALUE PtA, TWOVALUE PtB, double Di);
	TWOVALUE AddPt(FaultPoint &Line, int IsU, int IsS);
	void AddPt(FaultPoint &Line, int IsU, int IsS, TWOVALUE AssistPt);
	void AddPt(FaultPoint &LineI, FaultPoint &LineJ, int IU, int IS, int JU, int JS);
	BOOL IsSEinterse(FaultPoint &LineI, FaultPoint &LineJ);
	void Combination(vector<FaultPoint>&Lines);
	void CloseFalut();
	void GetABC(TWOVALUE Star, TWOVALUE End, TWOVALUE P, double &A, double &B, double &C);
	bool L2L_Intersect(THRVALUE p1_s, THRVALUE p1_e, TWOVALUE p2_s, TWOVALUE p2_e);
	bool IsIntersect(TWOVALUE p1_s, TWOVALUE p1_e, vector<THRVALUE> Lines);
	void DealWithSingleFault();
	void GetMiddleData();
	void GetNearByPoint(const TWOVALUE& m_PrePoint, const TWOVALUE& m_NextPoint, const TWOVALUE& point, Data &m_newPoint);
	double GetPointToLineDis(TWOVALUE PrePoint, TWOVALUE NextPoint, TWOVALUE point);
	double GetDistance(vector<TWOVALUE> Pts, TWOVALUE A);
	void SetDIF(double fDis);
	void TidyFaultLine(FalutLine &LineI);
	bool IsClockwise(const TWOVALUE &p0, const TWOVALUE &p1, const TWOVALUE &p2);
	FalutLine ConnectLine(FalutLine &LineI, FalutLine LineJ, int IP[], int JP[]);
	BOOL Isintersect(FalutLine &LineI, FalutLine LineJ);
	void Combination();
	void GetFaultLine(vector<FaultPoint> data);
	void EvaluateFault();
	void NormalVector(vector<THRVALUE>& a, THRVALUE& b, double Position);
	void ReviseWires(int Num, vector<vector<double>>& faultSu, double Position);
	void FaultWires(int Num, vector<Data>& oriData, vector<THRVALUE>& faultSu, double Position);
	void FaultKriking();
	void ReviseWires(int Num, THRVALUE& Point, double Position);
	void EvaluateFault(THRVALUE& Point);
	void FaultInvDis();
	void GetValueByK(THRVALUE &Point);
	BOOL ISInside(TWOVALUE A, TWOVALUE GridIJ[5]);
	void Sort(vector<double>&Mark);
	void GetOneCross(int IndexI, int IndexJ, TWOVALUE GridIJ[5], FalutLine LineOne, GridFL &AllLine);
	void GetFulatInsidePoint(vector<double>Mark, FalutLine LineOne, GridFL &AllLine, TWOVALUE GridIJ[5]);
	void GetAllCross(int IndexI, int IndexJ, TWOVALUE GridIJ[5], GridFL &AllLine);
	void SetOneFault(int IndexI, int IndexJ, GridFL &ALLLine);
	BOOL IsInside(TWOVALUE A, FalutLine Line);
	BOOL IsInside(TWOVALUE RealyPoint);
	void JudgeFaultGridIn();
	void SetNewFault();
	void GetMinD(vector<double> CosS, vector<double> DisT, int &index);
	vector<Data> Withershins(vector<Data> m_point);
	vector<Data> LoadModel(const char* sFileName);
	void Charact();
	void DataOpt();
	void DataRec();
	void CalcBorder();
	void AddData(Data &t);
	double GetDis(double x1, double y1, double x2, double y2);
	void AddPt(vector<Data> &convexBag);
	double Angle(Data &p0, const Data &p1, const Data &p2);
	double bezier3funcX(double uu, Vector2D *controlP);
	double bezier3funcY(double uu, Vector2D *controlP);
	void createCurve();
	void deCasteljau();
	void deBoor();
	void OptimizeBoder(vector<Data> &convexBag,double e);
	void OptimizeBorderBezier(vector<Data> &convexBag,double e);
	void DividedFourParts(int n, vector<int>& FourPort);
	void BordersPointDis(vector<int>& FourPart, vector<Data>& convexBag, vector<Data>& upLine, vector<Data>&downline, vector<Data>&leftLine, vector<Data>& rightLine,
		vector<Dis>& upDis, vector<Dis>&downDis, vector<Dis>&leftDis, vector<Dis>&rightDis);
	void BordersChar(vector<Data>& upLine, vector<Data>&downline, vector<Data>&leftLine, vector<Data>& rightLine,
		vector<Dis>& upDis, vector<Dis>&downdis, vector<Dis>&leftDis, vector<Dis>&rightDis);
	void BordersPoints(vector<int>& FourPart, vector<Data>& convexBag, vector<Data>& upLine, vector<Data>&downline, vector<Data>&leftLine, vector<Data>& rightLine,
		vector<Dis>& upDis, vector<Dis>&downdis, vector<Dis>&leftDis, vector<Dis>&rightDis,
		vector<Data>& DL, vector<Data>& DR, vector<Data>&DD, vector<Data>& DU,
		vector<TWOVALUE>& sim1, vector<TWOVALUE>& sim2, vector<TWOVALUE>& sim3, vector<TWOVALUE>& sim4, int XM, int YM);
	bool Inv(vector<vector<double>>&M);
	double InsertEst(vector<Data>& suV, TWOVALUE& D, vector<double>& ni);
	void PreMatrix(vector<Data>& suV, vector<double>& ni);
	double DisInv(TWOVALUE D);
	void EvaluateNoFault();
	void SetGridXY(double exp);
	double GetMagnitude(double fNumber);
	float FindStep(float StepMin, bool bUporDown);
	void CalcSameArray();
	void SetTrackValue(vector<double> Track);
	double GetIndex(THRVALUE Star, THRVALUE End, THRVALUE Point);
	bool IsinLineK(THRVALUE Star, THRVALUE End, THRVALUE Point);
	bool IsinLineK(THRVALUE Star, THRVALUE End, TWOVALUE Point);
	vector<FLOValue> GetFalutindex(THRVALUE GridIJ[6], GridFL Lines);
	void GetCrossMe(THRVALUE GridIJ[6], vector<FLOValue> &FLIndex, double &Star, vector<THRVALUE> &FlautArae);
	vector<THRVALUE> GetFalutAera(THRVALUE GridIJ[6], GridFL Lines);
	BOOL GetVirtualValue(THRVALUE GridIJ[6], GridFL Lines, TWOVALUE &Point);
	BOOL IsOnGrid(THRVALUE GridIJ[6], TWOVALUE PT);
	void GetT(THRVALUE A, THRVALUE B, THRVALUE C, double &t);
	void GetPtPs(THRVALUE GridIJ[6], THRVALUE Point, int &Index);
	void TrackRightFault(double Value, IsoLine &Line, IsoLine &IsoReal, vector<vector<double>>&X_Used, vector<vector<double>>&Y_Used);
	void TrackLeftFault(double Value, IsoLine &Line, IsoLine &IsoReal, vector<vector<double>>&X_Used, vector<vector<double>>&Y_Used);
	void TrackDownFault(double Value, IsoLine &Line, IsoLine &IsoReal, vector<vector<double>>&X_Used, vector<vector<double>>&Y_Used);
	void TrackUPFault(double Value, IsoLine &Line, IsoLine &IsoReal, vector<vector<double>>&X_Used, vector<vector<double>>&Y_Used);
	void TrackRight(double Value, IsoLine &Line, IsoLine &IsoReal, vector<vector<double>>&X_Used, vector<vector<double>>&Y_Used);
	void TrackLeft(double Value, IsoLine &Line, IsoLine &IsoReal, vector<vector<double>>&X_Used, vector<vector<double>>&Y_Used);
	void TrackDown(double Value, IsoLine &Line, IsoLine &IsoReal, vector<vector<double>>&X_Used, vector<vector<double>>&Y_Used);
	void TrackUp(double Value, IsoLine &Line, IsoLine &IsoReal, vector<vector<double>>&X_Used, vector<vector<double>>&Y_Used);
	void GetReallyPoint(TWOVALUE A, TWOVALUE &B);
	void TrackY(TWOVALUE A, double Value, IsoLine &Line, IsoLine &IsoReal, vector<vector<double>>&X_Used, vector<vector<double>>&Y_Used);
	void TrackX(TWOVALUE A, double Value, IsoLine &Line, IsoLine &IsoReal, vector<vector<double>>&X_Used, vector<vector<double>>&Y_Used);
	//void TrackPoint(double Value, IsoLine &Line, IsoLine &IsoReal, double flag_x[][1000], double flag_y[][1000], TWOVALUE First);
	void TrackPoint(double Value, IsoLine &Line, IsoLine &IsoReal, vector<vector<double>>&X_Used, vector<vector<double>>&Y_Used, TWOVALUE First);
	void GetMinDis(THRVALUE A, vector<THRVALUE>Pts, THRVALUE &Point);
	void SignBorder(vector<vector<double>>& biaoji1, vector<vector<double>>& biaoji2);
	void Y_FalutEquivalent(int X_index, int Y_index, double Value, vector<TWOVALUE>&VirtualIJ);
	void X_FalutEquivalent(int X_index, int Y_index, double Value, vector<TWOVALUE>&VirtualIJ);
	void EquivalentPoints(double Value, vector<TWOVALUE>&VirtualIJ);
	void EquivalentPoints(double Value, vector<THRVALUE>&VirtualIJ);
	void TrackOneValue(double Value);
	void IsolineTracking();
	void IsolineOpt();
	void CreateIsoLine();
	void SetOriBoder(vector<TWOVALUE> OriBoder);
	void SetOriISOLine(vector<IsoLine> IsoRealLine);
	bool L2L_Intersect(TWOVALUE p1_s, TWOVALUE p1_e, TWOVALUE p2_s, TWOVALUE p2_e, TWOVALUE &Point);
	bool GetCrossPt(TWOVALUE Star, TWOVALUE End, vector<TWOVALUE> Line, TWOVALUE &A);
	bool ISIntersect(TWOVALUE p1_s, TWOVALUE p1_e, TWOVALUE p2_s, TWOVALUE p2_e, double &index);
	bool IsinLineK(TWOVALUE Star, TWOVALUE End, TWOVALUE Point);
	bool IsInside(TWOVALUE A, vector<TWOVALUE> Line);
	void DleaIso(IsoLine &OneIso, vector<IsoLine> &NewIso);
	void DleaIso();
	vector<double> Randnum(int k);
};
