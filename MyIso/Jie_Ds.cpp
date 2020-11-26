#include "Jie_Ds.h"


void Jie_Ds::ReadFaultStr(CString strFault)
{
	Fault faultObj;
	BOOL bHave = FALSE;
	int pos = 0;
	CString str = _T("");
	str = strFault.Tokenize(",", pos);
	if (str != "")
	{//�ϲ���ࣨ���ϲ�/��ϲ㣩
	//	faultObj.m_InverseFlag = (atoi(str) != 0);
		int flag = atoi(str);
		if (flag == 1)
			faultObj.m_InverseFlag = false;
		else if (flag == 0)
			faultObj.m_InverseFlag = true;
		else
			faultObj.m_Mid = true;
	}
	else
		return;
	str = strFault.Tokenize(",", pos);
	if (str != "")
	{//�ϲ����ţ�0��1��2...��
		faultObj.m_nfault = atol(str);

		for (int i = 0; i < m_faultNunArray.GetCount(); i++) // �����鿴�Ƿ��е�ǰ��ţ�û�оͼ�¼�¡�
		{
			if (m_faultNunArray[i] == faultObj.m_nfault)
			{
				bHave = TRUE;
				break;
			}
		}
	}
	else
		return;
	str = strFault.Tokenize(",", pos);
	if (str != "")
	{//��λ(�����̡��½���)
		faultObj.m_bUpOrDown = atol(str);
	}
	else
		return;
	str = strFault.Tokenize(",", pos);
	if (str != "")
	{//�߱��
		faultObj.m_InfaultNum = atol(str);
	}
	else
		return;

	str = strFault.Tokenize(",", pos);
	if (str != "")
	{//x&y;x&y;....
		CStringArray xStrArray;
		CStringArray yStrArray;
		xStrArray.RemoveAll();
		yStrArray.RemoveAll();
		CString xStr, yStr;
		int pos1 = 0;
		xStr = str.Tokenize("&;", pos1);
		if (xStr == "")
		{
			return;
		}
		yStr = str.Tokenize("&;", pos1);
		if (yStr == "")
		{
			return;
		}
		while (xStr != "")
		{
			xStrArray.Add(xStr);
			yStrArray.Add(yStr);
			xStr = str.Tokenize("&;", pos1);
			if (xStr == "")
			{
				break;
			}
			yStr = str.Tokenize("&;", pos1);
			if (yStr == "")
			{
				break;
			}
		}
		if (xStrArray.GetCount()>0)
		{
			int nSize = (int)xStrArray.GetCount();
			for (int i = 0; i<nSize; i++)
			{
				TWOVALUE pt;
				pt.X = (float)atof(xStrArray.GetAt(i));
				pt.Y = (float)atof(yStrArray.GetAt(i));
				faultObj.m_pts.push_back(pt);
				//m_txLine.m_Pts[i].x = fx - Offset_X;
				//m_txLine.m_Pts[i].y = fy - Offset_Y;
			}
		}
		else
			return;
	}
	else
		return;

	if (!bHave)
		m_faultNunArray.Add(faultObj.m_nfault);

	m_faults.push_back(faultObj);
}
void Jie_Ds::SetFault(char* strFaultData)
{
	//strFaultData��ʽ������ϲ㣬�ϲ���ţ��������½��̣��ϲ��ߺţ�x&y;x&y;....x&y;#.....�ظ�
	CString strData = strFaultData;
	CString strOne = "", strTmp = "";
	int n = 0, pos = 0;

	m_faultNunArray.RemoveAll();

	strOne = strData.Tokenize("#", n);
	strOne.Trim();
	while (!strOne.IsEmpty())
	{
		ReadFaultStr(strOne);

		strOne = strData.Tokenize("#", n);
		strOne.Trim();
	}
}
void Jie_Ds::IsFault_ABNull(int group, int &m_flag, bool &InverseFlag)
{
	//����ֵΪ1��������Ϊ�� 2���½���Ϊ�� 3������Ϊ��
	InverseFlag = false;
	BOOL m_bUp = FALSE;
	BOOL m_bDown = FALSE;
	BOOL m_bMid = FALSE;
	//�����еĶϲ��߽��б���
	int faultline_count = m_faults.size();
	for (int i = 0; i<faultline_count; i++)
	{
		Fault m_TmpFault = m_faults[i];
		if (m_TmpFault.m_nfault == group && m_TmpFault.m_Mid!=TRUE && m_TmpFault.m_bUpOrDown == TRUE)
		{
			m_bUp = TRUE;//�����̲�Ϊ��
			InverseFlag = m_TmpFault.m_InverseFlag;//����ϲ��־�����ϲ�:false;��ϲ�ʹ�ֱ�ϲ�:true
		}
		else if (m_TmpFault.m_nfault == group && m_TmpFault.m_Mid != TRUE && m_TmpFault.m_bUpOrDown == FALSE)
		{
			m_bDown = TRUE;//�½��̲�Ϊ��
			InverseFlag = m_TmpFault.m_InverseFlag;//����ϲ��־�����ϲ�:false;��ϲ�ʹ�ֱ�ϲ�:true
		}
		else if (m_TmpFault.m_nfault == group && m_TmpFault.m_Mid == TRUE)
		{
			m_bMid = TRUE;//�м��߲�Ϊ��
			InverseFlag = true;
			//InverseFlag = m_TmpFault.m_InverseFlag;//����ϲ��־�����ϲ�:false;��ϲ�ʹ�ֱ�ϲ�:true
		}
		
	}
	//ֻ���м�ϲ���
	if (m_bMid == true)
	{
		m_flag = 4;
	}
	else
	{
		if (!m_bUp)
		{
			m_flag = 1;
		}
		else if (!m_bDown)
		{
			m_flag = 2;
		}
		else
			m_flag = 3;
	}
}
//�õ��϶ϲ���
void Jie_Ds::Fault_A(vector<TWOVALUE>& m_FaultA, int m_All_FaultLineNum, int m_CurFaultGroupNum, BOOL m_flag)
{
	CUIntArray m_templineNO;
	m_templineNO.RemoveAll();

	//���ڵ���ͳ��
	for (int i = 0; i<m_All_FaultLineNum; i++)
	{
		Fault m_TmpFault = m_faults[i];
		//������
		if (m_TmpFault.m_nfault == m_CurFaultGroupNum && m_TmpFault.m_bUpOrDown == TRUE)
		{
			int m_temNO = m_TmpFault.m_InfaultNum;//�ϲ������еı�ŵ��߱��
												  //ͳ���ߺ� 
			int num = 0;
			for (; num<(int)m_templineNO.GetSize(); num++)
			{
				if (m_templineNO.GetAt(num) == m_temNO)
					break;
			}
			if (num == (int)m_templineNO.GetSize())
				m_templineNO.Add(m_temNO);
		}
	}

	//�ߺ�����
	//ð������
	for (int i = 0; i<(int)m_templineNO.GetSize(); i++)
	{
		for (int k = (int)m_templineNO.GetSize() - 1; k>i; k--)
		{
			if (m_templineNO[k] < m_templineNO[k - 1])
			{
				int n = m_templineNO[k];
				//ע����������������������ʹ�ã�������С���Ӳ���
				m_templineNO.RemoveAt(k);
				m_templineNO.InsertAt(k - 1, n);
			}
		}
	}

	//ȡ��
	m_TepFPointArray.RemoveAll();
	for (int i = 0; i<m_All_FaultLineNum; i++)
	{
		Fault m_TmpFault = m_faults[i];
		//������
		if (m_TmpFault.m_nfault == m_CurFaultGroupNum &&m_TmpFault.m_Mid != TRUE && m_TmpFault.m_bUpOrDown == TRUE)
		{
			for (int k = 0; k != (int)m_templineNO.GetSize(); k++)
			{
				if (m_TmpFault.m_InfaultNum == (int)m_templineNO.GetAt(k))
				{
					//if (!m_flag) //���½���ʱ
					{
						//��ϡ�ϲ��						
						//m_TepFPointArray.RemoveAll();
						CFpoint m_T;
						for (int j = 0; j<m_TmpFault.m_pts.size(); j++)
						{
							//��ϡ��
							if (m_TepFPointArray.GetSize() == 0)//��һ���㲻���жϣ�ֱ�Ӽ���
							{
								CFpoint m_tFP;
								m_tFP.x = m_TmpFault.m_pts[j].X;
								m_tFP.y = m_TmpFault.m_pts[j].Y;
								m_TepFPointArray.Add(m_tFP);
								m_T = m_tFP;
							}
							else
							{
								float m_dis = (float)_hypot(m_TmpFault.m_pts[j].X - m_TepFPointArray[m_TepFPointArray.GetSize() - 1].x, m_TmpFault.m_pts[j].Y - m_TepFPointArray[m_TepFPointArray.GetSize() - 1].y);
								if (m_dis>0.00f) // ԭ�ж�ֵ��20.00f����ɸѡ����ĳЩ���ݣ����º���С��ƽ��ͼ�ϲ�Ч������ʱ����������Ϊ�μӴ��жϡ�
								{
									CFpoint m_tFP;
									m_tFP.x = m_TmpFault.m_pts[j].X;
									m_tFP.y = m_TmpFault.m_pts[j].Y;
									m_TepFPointArray.Add(m_tFP);
								}
							}

						}

					}
				}
			}
		}
	}


	//m_FaultA.Resize((int)m_TepFPointArray.GetSize());
	for (int j = 0; j<(int)(m_TepFPointArray.GetSize()); j++)
	{
		TWOVALUE pt;

		pt.X = m_TepFPointArray[j].x;
		pt.Y = m_TepFPointArray[j].y;
		m_FaultA.push_back(pt);
	}
}
//�õ��¶ϲ���
void Jie_Ds::Fault_B(vector<TWOVALUE>& m_FaultB, int m_All_FaultLineNum, int m_CurFaultGroupNum, BOOL m_flag)
{
	CUIntArray m_templineNO;
	m_templineNO.RemoveAll();

	//int m_ptnGroup = 0;//��¼�����������½��̶ϲ��ĺ�
	BOOL m_b = FALSE;	//����½����Ƿ�Ϊ��
						//���ڵ���ͳ��
	for (int i = 0; i<m_All_FaultLineNum; i++)
	{
		Fault m_TmpFault = m_faults[i];
		if (m_TmpFault.m_nfault == m_CurFaultGroupNum &&m_TmpFault.m_Mid!=TRUE && m_TmpFault.m_bUpOrDown == FALSE)
		{
			int m_temNO = m_TmpFault.m_InfaultNum;
			//ͳ���ߺ� 
			int num = 0;
			for (; num<(int)m_templineNO.GetSize(); num++)
			{
				if (m_templineNO.GetAt(num) == m_temNO)
					break;
			}
			if (num == (int)m_templineNO.GetSize())
			{
				m_templineNO.Add(m_temNO);//�����߱��
										  //m_ptnGroup += m_TmpFault;
			}
		}
	}

	//�ߺ�����
	//ð������
	for (int i = 0; i<(int)m_templineNO.GetSize(); i++)
	{
		for (int k = (int)m_templineNO.GetSize() - 1; k>i; k--)
		{
			if (m_templineNO[k] < m_templineNO[k - 1])
			{
				int n = m_templineNO[k];
				m_templineNO.RemoveAt(k);
				m_templineNO.InsertAt(k - 1, n);
			}
		}
	}

	//ȡ��
	m_TepFPointArray.RemoveAll();
	for (int i = 0; i<m_All_FaultLineNum; i++)
	{
		Fault m_TmpFault = m_faults[i];
		if (m_TmpFault.m_nfault == m_CurFaultGroupNum && m_TmpFault.m_bUpOrDown == FALSE)
		{
			for (int k = 0; k != (int)m_templineNO.GetSize(); k++)
			{
				if (m_TmpFault.m_InfaultNum == (int)m_templineNO.GetAt(k))
				{

					for (int j = 0; j<m_TmpFault.m_pts.size(); j++)
					{
						//��ϡ��
						if (m_TepFPointArray.GetSize() == 0)
						{
							CFpoint m_tFP;
							m_tFP.x = m_TmpFault.m_pts[j].X;
							m_tFP.y = m_TmpFault.m_pts[j].Y;
							m_TepFPointArray.Add(m_tFP);
						}
						else
						{
							float m_dis = (m_TmpFault.m_pts[j].X - m_TepFPointArray[m_TepFPointArray.GetSize() - 1].x)*(m_TmpFault.m_pts[j].X - m_TepFPointArray[m_TepFPointArray.GetSize() - 1].x) +
								(m_TmpFault.m_pts[j].Y - m_TepFPointArray[m_TepFPointArray.GetSize() - 1].y)*(m_TmpFault.m_pts[j].Y - m_TepFPointArray[m_TepFPointArray.GetSize() - 1].y);
							if (m_dis>0.00f) // ԭ�ж�ֵ��100.00f����ɸѡ����ĳЩ���ݣ����º���С��ƽ��ͼ�ϲ�Ч������ʱ����������Ϊ�μӴ��жϡ�
							{
								CFpoint m_tFP;
								m_tFP.x = m_TmpFault.m_pts[j].X;
								m_tFP.y = m_TmpFault.m_pts[j].Y;
								m_TepFPointArray.Add(m_tFP);
							}
						}

					}

				}//if
			}//for
		}//if	
	}//for

	 //m_FaultB.Resize((int)m_TepFPointArray.GetSize());
	for (int j = 0; j<(int)(m_TepFPointArray.GetSize()); j++)
	{
		TWOVALUE pt;
		pt.X = m_TepFPointArray[j].x;
		pt.Y = m_TepFPointArray[j].y;
		m_FaultB.push_back(pt);
	}
}

//�õ��м���
void Jie_Ds::Fault_C(vector<TWOVALUE>& m_FaultC, int m_All_FaultLineNum, int m_CurFaultGroupNum, BOOL m_flag)
{
	CUIntArray m_templineNO;
	m_templineNO.RemoveAll();

	//int m_ptnGroup = 0;//��¼�����������½��̶ϲ��ĺ�
	BOOL m_b = FALSE;	//����½����Ƿ�Ϊ��
						//���ڵ���ͳ��
	for (int i = 0; i<m_All_FaultLineNum; i++)
	{
		Fault m_TmpFault = m_faults[i];
		if (m_TmpFault.m_nfault == m_CurFaultGroupNum && m_TmpFault.m_Mid == TRUE)
		{
			int m_temNO = m_TmpFault.m_InfaultNum;
			//ͳ���ߺ� 
			int num = 0;
			for (; num<(int)m_templineNO.GetSize(); num++)
			{
				if (m_templineNO.GetAt(num) == m_temNO)
					break;
			}
			if (num == (int)m_templineNO.GetSize())
			{
				m_templineNO.Add(m_temNO);//�����߱��
										  //m_ptnGroup += m_TmpFault;
			}
		}
	}

	//�ߺ�����
	//ð������
	for (int i = 0; i<(int)m_templineNO.GetSize(); i++)
	{
		for (int k = (int)m_templineNO.GetSize() - 1; k>i; k--)
		{
			if (m_templineNO[k] < m_templineNO[k - 1])
			{
				int n = m_templineNO[k];
				m_templineNO.RemoveAt(k);
				m_templineNO.InsertAt(k - 1, n);
			}
		}
	}

	//ȡ��
	m_TepFPointArray.RemoveAll();
	for (int i = 0; i<m_All_FaultLineNum; i++)
	{
		Fault m_TmpFault = m_faults[i];
		if (m_TmpFault.m_nfault == m_CurFaultGroupNum && m_TmpFault.m_bUpOrDown == FALSE)
		{
			for (int k = 0; k != (int)m_templineNO.GetSize(); k++)
			{
				if (m_TmpFault.m_InfaultNum == (int)m_templineNO.GetAt(k))
				{

					for (int j = 0; j<m_TmpFault.m_pts.size(); j++)
					{
						//��ϡ��
						if (m_TepFPointArray.GetSize() == 0)
						{
							CFpoint m_tFP;
							m_tFP.x = m_TmpFault.m_pts[j].X;
							m_tFP.y = m_TmpFault.m_pts[j].Y;
							m_TepFPointArray.Add(m_tFP);
						}
						else
						{
							float m_dis = (m_TmpFault.m_pts[j].X - m_TepFPointArray[m_TepFPointArray.GetSize() - 1].x)*(m_TmpFault.m_pts[j].X - m_TepFPointArray[m_TepFPointArray.GetSize() - 1].x) +
								(m_TmpFault.m_pts[j].Y - m_TepFPointArray[m_TepFPointArray.GetSize() - 1].y)*(m_TmpFault.m_pts[j].Y - m_TepFPointArray[m_TepFPointArray.GetSize() - 1].y);
							if (m_dis>0.00f) // ԭ�ж�ֵ��100.00f����ɸѡ����ĳЩ���ݣ����º���С��ƽ��ͼ�ϲ�Ч������ʱ����������Ϊ�μӴ��жϡ�
							{
								CFpoint m_tFP;
								m_tFP.x = m_TmpFault.m_pts[j].X;
								m_tFP.y = m_TmpFault.m_pts[j].Y;
								m_TepFPointArray.Add(m_tFP);
							}
						}

					}

				}//if
			}//for
		}//if	
	}//for

	 //m_FaultB.Resize((int)m_TepFPointArray.GetSize());
	for (int j = 0; j<(int)(m_TepFPointArray.GetSize()); j++)
	{
		TWOVALUE pt;
		pt.X = m_TepFPointArray[j].x;
		pt.Y = m_TepFPointArray[j].y;
		m_FaultC.push_back(pt);
	}
}


void Jie_Ds::CalcFault()
{
	int faultline_count = m_faults.size();//�ϲ��ߵ�����
	int m_FaultGroupNum = m_faultNunArray.GetCount();//�ϲ��������

	m_FaultData.resize(m_faultNunArray.GetCount());//�ǵ�����ϲ��������

															 //ͳ������������
	for (int j = 0; j<(int)m_faultNunArray.GetSize(); j++)
	{//ͳ��ÿһ��Ķϲ�
		vector<TWOVALUE> m_FaultA, m_FaultB, m_FaultC;
		int m_flag = 3;
		bool m_InverseFlag = false;    //����ϲ��־�����ϲ�:false;��ϲ�ʹ�ֱ�ϲ�:true
		int m_num = m_faultNunArray.GetAt(j);//�ϲ����
		IsFault_ABNull(m_num, m_flag, m_InverseFlag); //�жϸ��������̡��½����Ƿ�Ϊ��




		//ͳ��һ���е������̡��½��̶�Ӧ�Ķϲ��ߵ�����
		Fault_A(m_FaultA, faultline_count, m_faultNunArray.GetAt(j), FALSE);
		Fault_B(m_FaultB, faultline_count, m_faultNunArray.GetAt(j), FALSE);
		Fault_C(m_FaultC, faultline_count, m_faultNunArray.GetAt(j), FALSE);

		//cout << "flag = " << m_flag << endl;
		//��ʼ���ؼ��ϲ�����
		m_FaultData[j].mark = m_InverseFlag;

		if (m_flag == 2)//ֻ��������
		{
			m_FaultData[j].UData.resize((int)m_FaultA.size());
			//m_IsoGrid.m_FaultData[j].TData.resize((int)m_FaultA.size());
			//����Ķϲ��м��ߺ������̶�Ӧ�Ķϲ����غ�
			for (int i = 0; i<(int)(m_FaultA.size()); i++)
			{
				m_FaultData[j].UData[i].X = m_FaultA[i].X;
				m_FaultData[j].UData[i].Y = m_FaultA[i].Y;
				//m_IsoGrid.m_FaultData[j].TData[i].X = m_FaultA[i].X;
				//m_IsoGrid.m_FaultData[j].TData[i].Y = m_FaultA[i].Y;
			}
			for (int index = 0; index < (int)m_FaultA.size(); index++)  // minZZBX֮�����Щ�������õ�ȷ��border�����С�
			{
				//if (minZZBX > m_IsoGrid.m_FaultData[j].TData[index].X)
				//	minZZBX = m_IsoGrid.m_FaultData[j].TData[index].X;
				//if (maxZZBX < m_IsoGrid.m_FaultData[j].TData[index].X)
				//	maxZZBX = m_IsoGrid.m_FaultData[j].TData[index].X;
				//if (minHZBY > m_IsoGrid.m_FaultData[j].TData[index].Y)
				//	minHZBY = m_IsoGrid.m_FaultData[j].TData[index].Y;
				//if (maxHZBY < m_IsoGrid.m_FaultData[j].TData[index].Y)
				//	maxHZBY = m_IsoGrid.m_FaultData[j].TData[index].Y;
			}
			vector<TWOVALUE> va;
			m_FaultA.swap(va);
			m_FaultA.clear();
		}
		else if (m_flag == 1) //ֻ���½���
		{
			m_FaultData[j].DData.resize((int)m_FaultB.size());
			//m_IsoGrid.m_FaultData[j].TData.resize((int)m_FaultB.size());
			for (int i = 0; i<(int)(m_FaultB.size()); i++)
			{
				m_FaultData[j].DData[i].X = m_FaultB[i].X;
				m_FaultData[j].DData[i].Y = m_FaultB[i].Y;
				//m_IsoGrid.m_FaultData[j].TData[i].X = m_FaultB[i].X;
				//m_IsoGrid.m_FaultData[j].TData[i].Y = m_FaultB[i].Y;
			}
			for (int index = 0; index < (int)m_FaultB.size(); index++)
			{
				//if (minZZBX > m_IsoGrid.m_FaultData[j].TData[index].X)
				//	minZZBX = m_IsoGrid.m_FaultData[j].TData[index].X;
				//if (maxZZBX < m_IsoGrid.m_FaultData[j].TData[index].X)
				//	maxZZBX = m_IsoGrid.m_FaultData[j].TData[index].X;
				//if (minHZBY > m_IsoGrid.m_FaultData[j].TData[index].Y)
				//	minHZBY = m_IsoGrid.m_FaultData[j].TData[index].Y;
				//if (maxHZBY < m_IsoGrid.m_FaultData[j].TData[index].Y)
				//	maxHZBY = m_IsoGrid.m_FaultData[j].TData[index].Y;
			}
			vector<TWOVALUE> vb;
			m_FaultB.swap(vb);
			m_FaultB.clear();
		}
		else if (m_flag == 4)//ֻ���м���
		{
			m_FaultData[j].TData.resize((int)m_FaultC.size());
			//m_IsoGrid.m_FaultData[j].TData.resize((int)m_FaultB.size());
			for (int i = 0; i<(int)(m_FaultC.size()); i++)
			{
				m_FaultData[j].TData[i].X = m_FaultC[i].X;
				m_FaultData[j].TData[i].Y = m_FaultC[i].Y;
				//m_IsoGrid.m_FaultData[j].TData[i].X = m_FaultB[i].X;
				//m_IsoGrid.m_FaultData[j].TData[i].Y = m_FaultB[i].Y;
			}
		}
		else  //����������Ҳ���½���
		{
			m_FaultData[j].UData.resize((int)m_FaultA.size());
			for (int i = 0; i<(int)(m_FaultA.size()); i++)
			{
				m_FaultData[j].UData[i].X = m_FaultA[i].X;
				m_FaultData[j].UData[i].Y = m_FaultA[i].Y;
			}
			for (int index = 0; index < (int)m_FaultA.size(); index++)
			{
				//if (minZZBX > m_IsoGrid.m_FaultData[j].UData[index].X)
				//	minZZBX = m_IsoGrid.m_FaultData[j].UData[index].X;
				//if (maxZZBX < m_IsoGrid.m_FaultData[j].UData[index].X)
				//	maxZZBX = m_IsoGrid.m_FaultData[j].UData[index].X;
				//if (minHZBY > m_IsoGrid.m_FaultData[j].UData[index].Y)
				//	minHZBY = m_IsoGrid.m_FaultData[j].UData[index].Y;
				//if (maxHZBY < m_IsoGrid.m_FaultData[j].UData[index].Y)
				//	maxHZBY = m_IsoGrid.m_FaultData[j].UData[index].Y;
			}
			m_FaultData[j].DData.resize((int)m_FaultB.size());
			for (int i = 0; i<(int)(m_FaultB.size()); i++)
			{
				m_FaultData[j].DData[i].X = m_FaultB[i].X;
				m_FaultData[j].DData[i].Y = m_FaultB[i].Y;
			}
			for (int index = 0; index < (int)m_FaultB.size(); index++)
			{
				//if (minZZBX > m_IsoGrid.m_FaultData[j].DData[index].X)
				//	minZZBX = m_IsoGrid.m_FaultData[j].DData[index].X;
				//if (maxZZBX < m_IsoGrid.m_FaultData[j].DData[index].X)
				//	maxZZBX = m_IsoGrid.m_FaultData[j].DData[index].X;
				//if (minHZBY > m_IsoGrid.m_FaultData[j].DData[index].Y)
				//	minHZBY = m_IsoGrid.m_FaultData[j].DData[index].Y;
				//if (maxHZBY < m_IsoGrid.m_FaultData[j].DData[index].Y)
				//	maxHZBY = m_IsoGrid.m_FaultData[j].DData[index].Y;
			}

			vector<TWOVALUE> v1, v2;
			m_FaultA.swap(v1);
			m_FaultB.swap(v2);
			m_FaultA.clear();
			m_FaultB.clear();
		}
	}
}
//�����Ƿ���ͬ 20140728
bool Jie_Ds::IsOnePT(TWOVALUE PtA, TWOVALUE PtB)
{
	if ((PtA.X == PtB.X) && (PtA.Y == PtB.Y))
	{
		return true;
	}
	//if (abs(PtA.X -PtB.X) < 0.00001 && abs(PtA.Y - PtB.Y) < 0.00001)
	//{
	//	AfxMessageBox("IsOnePt");
	//	return true;
	//}
	else
	{
		return false;
	}
}

//��ͬ����� 20140728
int Jie_Ds::GerEquNum(vector<TWOVALUE> PtA, vector<TWOVALUE>  PtB)
{
	int Num = 0;
	for (int i = 0; i < (int)PtA.size(); i++)
	{
		TWOVALUE A = PtA[i];
		for (int j = 0; j < (int)PtB.size(); j++)
		{
			TWOVALUE B = PtB[j];
			if (IsOnePT(A, B))
			{
				Num++;
			}
		}
	}
	return Num;
}

//�õ��ཻ�ĵ����20140728
int Jie_Ds::GetCrossNum(FaultPoint LineI, FaultPoint LineJ)
{
	vector<TWOVALUE> IUP = LineI.UData;
	vector<TWOVALUE> IDP = LineI.DData;

	vector<TWOVALUE> JUP = LineJ.UData;
	vector<TWOVALUE> JDP = LineJ.DData;

	int Num = 0;
	Num += GerEquNum(IUP, JUP);
	Num += GerEquNum(IUP, JDP);
	Num += GerEquNum(IDP, JUP);
	Num += GerEquNum(IDP, JDP);

	return Num;
}
//�õ�PTA,PTB֮�����PTA����Di�ĵ�  20140729
TWOVALUE Jie_Ds::GerNearPt(TWOVALUE PtA, TWOVALUE PtB, double Di)
{
	TWOVALUE Pt;
	double dx = PtB.X - PtA.X;
	double dy = PtB.X - PtA.X;
	double d = sqrt(pow(dx, 2) + pow(dy, 2));
	if (abs(dx) + abs(dy) < 0.001)
	{
		Pt.X = (PtB.X + PtA.X) / 2;
		Pt.Y = (PtB.Y + PtA.Y) / 2;
	}
	else
	{
		dx = dx / d * Di;
		dy = dy / d * Di;
		Pt.X = PtA.X + dx;
		Pt.Y = PtA.Y + dy;
	}
	return  Pt;
}

//���ݶ˵��ཻ�����Ӹ�����,�����ظ�����20140729
TWOVALUE Jie_Ds::AddPt(FaultPoint &Line, int IsU, int IsS)
{
	vector<TWOVALUE> UP = Line.UData;
	vector<TWOVALUE> DP = Line.DData;

	double GridDis = (m_XMax - m_XMin) / m_XNum + (m_YMax - m_YMin) / m_YNum;
	GridDis = GridDis / 100;

	TWOVALUE BeginPt, NextPt, AssistPt;		//���㣨�׵����β�㣩����������ڵ㣬Ҫ��ӵĸ�����
	double   PtDis;

	if (IsU == 1)		//�������϶ϲ�
	{
		if (IsS == 1)	//�������׵�
		{
			BeginPt = UP[0];
			NextPt = UP[1];
			PtDis = (sqrt(pow(BeginPt.X - NextPt.X, 2) + pow(BeginPt.Y - NextPt.Y, 2))) / 50;
			if (PtDis < GridDis)
			{
				AssistPt = GerNearPt(BeginPt, NextPt, GridDis);
			}
			else
			{
				AssistPt = GerNearPt(BeginPt, NextPt, PtDis);
			}
			UP.insert(UP.begin() + 1, AssistPt);			//���׵����븨����
		}
		else			//������β��
		{
			BeginPt = UP[(int)UP.size() - 1];
			NextPt = UP[(int)UP.size() - 2];
			PtDis = (sqrt(pow(BeginPt.X - NextPt.X, 2) + pow(BeginPt.Y - NextPt.Y, 2))) / 50;
			if (PtDis < GridDis)
			{
				AssistPt = GerNearPt(BeginPt, NextPt, GridDis);
			}
			else
			{
				AssistPt = GerNearPt(BeginPt, NextPt, PtDis);
			}
			UP.insert(UP.begin() + (int)UP.size() - 1, AssistPt);			//�����һ��֮ǰ����븨����
		}
	}
	else				//�������¶ϲ�
	{
		if (IsS == 1)	//�������׵�
		{
			BeginPt = DP[0];
			NextPt = DP[1];
			PtDis = (sqrt(pow(BeginPt.X - NextPt.X, 2) + pow(BeginPt.Y - NextPt.Y, 2))) / 50;
			if (PtDis < GridDis)
			{
				AssistPt = GerNearPt(BeginPt, NextPt, GridDis);
			}
			else
			{
				AssistPt = GerNearPt(BeginPt, NextPt, PtDis);
			}
			DP.insert(DP.begin() + 1, AssistPt);			//���׵����븨����
		}
		else			//������β��
		{
			BeginPt = DP[(int)DP.size() - 1];
			NextPt = DP[(int)DP.size() - 2];
			PtDis = (sqrt(pow(BeginPt.X - NextPt.X, 2) + pow(BeginPt.Y - NextPt.Y, 2))) / 50;
			if (PtDis < GridDis)
			{
				AssistPt = GerNearPt(BeginPt, NextPt, GridDis);
			}
			else
			{
				AssistPt = GerNearPt(BeginPt, NextPt, PtDis);
			}
			DP.insert(DP.begin() + (int)DP.size() - 1, AssistPt);			//�����һ��֮ǰ����븨����
		}
	}
	Line.UData = UP;
	Line.DData = DP;
	return AssistPt;
}

//���ݶ˵��ཻ�����Ӹ�����Pt  20140729
void Jie_Ds::AddPt(FaultPoint &Line, int IsU, int IsS, TWOVALUE AssistPt)
{
	vector<TWOVALUE> UP = Line.UData;
	vector<TWOVALUE> DP = Line.DData;

	if (IsU == 1)		//�������϶ϲ�
	{
		if (IsS == 1)	//�������׵�
		{
			UP.insert(UP.begin() + 1, AssistPt);			//���׵����븨����
		}
		else			//������β��
		{
			UP.insert(UP.begin() + (int)UP.size() - 1, AssistPt);			//�����һ��֮ǰ����븨����
		}
	}
	else				//�������¶ϲ�
	{
		if (IsS == 1)	//�������׵�
		{
			DP.insert(DP.begin() + 1, AssistPt);			//���׵����븨����
		}
		else			//������β��
		{
			DP.insert(DP.begin() + (int)DP.size() - 1, AssistPt);			//�����һ��֮ǰ����븨����
		}
	}
	Line.UData = UP;
	Line.DData = DP;
}

//���ݶ˵��ཻ�����Ӹ����㣬ʹһ�������Ϊ��������20140728
void Jie_Ds::AddPt(FaultPoint &LineI, FaultPoint &LineJ, int IU, int IS, int JU, int JS)
{

	TWOVALUE AssistPt = AddPt(LineI, IU, IS);		//��Զϲ�i��Ӹ����㣬��������ӵ�
	AddPt(LineJ, JU, JS, AssistPt);				//��Զϲ�j��Ӹ�����
}
//�Ƿ��ڶ˵��ཻ����ֻ��һ������20140728
BOOL Jie_Ds::IsSEinterse(FaultPoint &LineI, FaultPoint &LineJ)
{
	vector<TWOVALUE> IUP = LineI.UData;
	vector<TWOVALUE> IDP = LineI.DData;

	vector<TWOVALUE> JUP = LineJ.UData;
	vector<TWOVALUE> JDP = LineJ.DData;

	int IU = 1; int IS = 1; int JU = 1; int JS = 1;	//IU = 1��ʾ������I�ϲ���϶ϲ�
	int Num = 0;		//�������
	TWOVALUE IUS, IUE, IDS, IDE, JUS, JUE, JDS, JDE;//���¶ϲ����β��
	int iuNum, idNum, juNum, jdNum;
	//�����ϲ����¶ϲ����ϵ�ĸ���
	iuNum = (int)IUP.size();
	idNum = (int)IDP.size();
	juNum = (int)JUP.size();
	jdNum = (int)JDP.size();
	//�ϲ�㲻��̫��
	if (iuNum < 2 || idNum < 2 || juNum < 2 || jdNum < 2)
	{
		return FALSE;
	}
	//�÷�����������һ�����㲢���ڶ˵㴦
	if (GetCrossNum(LineI, LineJ) != 1)			//�����������ϵĽ�����޽���
	{
		return FALSE;
	}
	IUS = IUP[0];								//�ϲ�I���϶ϲ��׵�
	IUE = IUP[iuNum - 1];

	IDS = IDP[0];								//�ϲ�I���¶ϲ��׵�
	IDE = IDP[idNum - 1];

	JUS = JUP[0];
	JUE = JUP[juNum - 1];

	JDS = JDP[0];
	JDE = JDP[jdNum - 1];
	//���� +~~~��һ���ϲ���϶ϲ��׵���ڶ����ϲ�Ƚ�
	if (IsOnePT(IUS, JUS))						//�ϲ�I���϶ϲ��׵���ϲ�J���϶ϲ��׵���ͬ
	{
		IU = 1;		IS = 1;		JU = 1;		JS = 1;
		Num++;
	}
	if (IsOnePT(IUS, JUE))						//�ϲ�I���϶ϲ��׵���ϲ�J���϶ϲ�β����ͬ
	{
		IU = 1;		IS = 1;		JU = 1;		JS = 0;
		Num++;
	}
	if (IsOnePT(IUS, JDS))						//�ϲ�I���϶ϲ��׵���ϲ�J���¶ϲ��׵���ͬ
	{
		IU = 1;		IS = 1;		JU = 0;		JS = 1;
		Num++;
	}
	if (IsOnePT(IUS, JDE))						//�ϲ�I���϶ϲ��׵���ϲ�J���¶ϲ�β����ͬ
	{
		IU = 1;		IS = 1;		JU = 0;		JS = 0;
		Num++;
	}

	/************************************************/
	//��β +~~~��һ���ϲ���϶ϲ�δ����ڶ����ϲ�Ƚ�
	if (IsOnePT(IUE, JUS))						//�ϲ�I���϶ϲ��׵���ϲ�J���϶ϲ��׵���ͬ
	{
		IU = 1;		IS = 0;		JU = 1;		JS = 1;
		Num++;
	}
	if (IsOnePT(IUE, JUE))						//�ϲ�I���϶ϲ��׵���ϲ�J���϶ϲ�β����ͬ
	{
		IU = 1;		IS = 0;		JU = 1;		JS = 0;
		Num++;
	}
	if (IsOnePT(IUE, JDS))						//�ϲ�I���϶ϲ��׵���ϲ�J���¶ϲ��׵���ͬ
	{
		IU = 1;		IS = 0;		JU = 0;		JS = 1;
		Num++;
	}
	if (IsOnePT(IUE, JDE))						//�ϲ�I���϶ϲ��׵���ϲ�J���¶ϲ�β����ͬ
	{
		IU = 1;		IS = 0;		JU = 0;		JS = 0;
		Num++;
	}

	/***********************************************************/
	//���� +~~~��һ���ϲ���¶ϲ��׵���ڶ����ϲ�Ƚ�
	if (IsOnePT(IDS, JUS))						//�ϲ�I���¶ϲ��׵���ϲ�J���϶ϲ��׵���ͬ
	{
		IU = 0;		IS = 1;		JU = 1;		JS = 1;
		Num++;
	}
	if (IsOnePT(IDS, JUE))						//�ϲ�I���¶ϲ��׵���ϲ�J���϶ϲ�β����ͬ
	{
		IU = 0;		IS = 1;		JU = 1;		JS = 0;
		Num++;
	}
	if (IsOnePT(IDS, JDS))						//�ϲ�I���¶ϲ��׵���ϲ�J���¶ϲ��׵���ͬ
	{
		IU = 0;		IS = 1;		JU = 0;		JS = 1;
		Num++;
	}
	if (IsOnePT(IDS, JDE))						//�ϲ�I���¶ϲ��׵���ϲ�J���¶ϲ�β����ͬ
	{
		IU = 0;		IS = 1;		JU = 0;		JS = 0;
		Num++;
	}

	/************************************************/
	//��β +~~~��һ���ϲ���¶ϲ�δ����ڶ����ϲ�Ƚ�
	if (IsOnePT(IDE, JUS))						//�ϲ�I���¶ϲ�β����ϲ�J���϶ϲ��׵���ͬ
	{
		IU = 0;		IS = 0;		JU = 1;		JS = 1;
		Num++;
	}
	if (IsOnePT(IDE, JUE))						//�ϲ�I���¶ϲ�β����ϲ�J���϶ϲ�β����ͬ
	{
		IU = 0;		IS = 0;		JU = 1;		JS = 0;
		Num++;
	}
	if (IsOnePT(IDE, JDS))						//�ϲ�I���¶ϲ�β����ϲ�J���¶ϲ��׵���ͬ
	{
		IU = 0;		IS = 0;		JU = 0;		JS = 1;
		Num++;
	}
	if (IsOnePT(IDE, JDE))						//�ϲ�I���¶ϲ�β����ϲ�J���¶ϲ�β����ͬ
	{
		IU = 0;		IS = 0;		JU = 0;		JS = 0;
		Num++;
	}
	if (Num == 1)
	{
		//cout << "hello==================================" << endl;
		//ֻ��һ���˵�λ�õĽ��㣬��Ӹ�����
		AddPt(LineI, LineJ, IU, IS, JU, JS);
		return TRUE;
	}
	return FALSE;
}

void Jie_Ds::Combination(vector<FaultPoint>&Lines)
{
	for (int i = 0; i < (int)Lines.size(); i++)
	{
		FaultPoint LineI = Lines[i];
		for (int j = 0; j < (int)Lines.size(); j++)
		{
			FaultPoint LineJ = Lines[j];
			if (i == j)//ͬһ���ϲ�
			{
				continue;
			}
			//TODO���ж��Ƿ�����β���ཻ ����ཻ����ͨ����Ӹ����㽫һ��������չ����������
			if (IsSEinterse(LineI, LineJ))
			{
				//��ʱ�Ѿ������ӵ�Ĳ���������Lines����
				Lines[i] = LineI;
				Lines[j] = LineJ;
			}
		}
	}
}
//�����պϵĵ������¶ϲ�Ķϲ�պ�20131206  20131209
//����ıպ��벻�պ�ָ��Ӧ���Ƕϲ���û���ڵ����������Լ�����
void Jie_Ds::CloseFalut()
{
	for (int i = 0; i < (int)m_FaultData.size(); i++)
	{
		int UDateSize = (int)m_FaultData[i].UData.size() - 1;
		int DDateSize = (int)m_FaultData[i].DData.size() - 1;
		//Ӧ��Ϊ�պ϶ϲ�
		if (UDateSize > 0 && DDateSize > 0)
		{
			TWOVALUE UpSTar, UpEnd, DownStar, DownEnd;
			UpSTar = m_FaultData[i].UData[0];//�϶ϲ��һ�����ݵ���Ϣ
			DownStar = m_FaultData[i].DData[0];//�¶ϲ��һ�����ݵ���Ϣ
			UpEnd = m_FaultData[i].UData[UDateSize];//�϶ϲ����һ�����ݵ���Ϣ
			DownEnd = m_FaultData[i].DData[DDateSize];//�¶ϲ����һ�����ݵ���Ϣ
			if (UpEnd.X != DownEnd.X || UpEnd.Y != DownEnd.Y)
			{
				//double dx = (UpEnd.X - m_FaultData[i].UData[UDateSize - 1].X) * 0.0001;
				//double dy = (UpEnd.Y - m_FaultData[i].UData[UDateSize - 1].Y) * 0.0001;
				//TWOVALUE TempEnd;
				////TempEnd.X = (UpEnd.X + DownEnd.X) / 2.0 ;
				////TempEnd.Y = (UpEnd.Y + DownEnd.Y) / 2.0 ;
				//TempEnd.X = UpEnd.X + dx ;
				//TempEnd.Y = UpEnd.Y + dy ;  //��ֹ����׷��ʱ���ж϶ϲ�˳ʱ��ʱ���������һ�����20131209
				////m_FaultData[i].UData.push_back(TempEnd);
				////m_FaultData[i].DData.push_back(TempEnd);
				m_FaultData[i].DData.push_back(UpEnd);//�����̵����һ������뵽������
			}
			if (UpSTar.X != DownStar.X || UpSTar.Y != DownStar.Y)
			{
				//double dx = (UpSTar.X - m_FaultData[i].UData[1].X) * 0.0001;
				//double dy = (UpSTar.Y - m_FaultData[i].UData[1].Y) * 0.0001;
				//TWOVALUE TempSTar;
				////TempSTar.X = (UpSTar.X + DownStar.X) / 2.0 ;
				////TempSTar.Y = (UpSTar.Y + DownStar.Y) / 2.0 ;
				//TempSTar.X = UpSTar.X + dx ;
				//TempSTar.Y = UpSTar.Y + dy ;//��ֹ����׷��ʱ���ж϶ϲ�˳ʱ��ʱ���������һ�����20131209

				vector<TWOVALUE> Up, Down;
				//Up = m_FaultData[i].UData;
				Down = m_FaultData[i].DData;
				//Up.insert(Up.begin(),TempSTar);
				//Down.insert(Down.begin(),TempSTar);
				Down.insert(Down.begin(), UpSTar);
				m_FaultData[i].DData = Down;//�⼸�д�������̵�һ������뵽������
											//m_FaultData[i].UData = Up;
			}
		}
	}
}

//�õ���P��Ĵ��߷���
void Jie_Ds::GetABC(TWOVALUE Star, TWOVALUE End, TWOVALUE P, double &A, double &B, double &C)
{
	double A1, B1;
	A1 = End.Y - Star.Y;
	B1 = Star.X - End.X;
	//A1X+B1Y+C1 = 0;ԭʼ������ɵ�ֱ�߷���
	if (A1 == 0)
	{
		A = 0;
		B = 1;
	}
	else if (B1 == 0)
	{
		A = 1;
		B = 0;
	}
	else
	{
		A = B1;
		B = -A1;
		C = -A * P.X - B * P.Y;
	}

}
//�Ƿ��н���
bool Jie_Ds::L2L_Intersect(THRVALUE p1_s, THRVALUE p1_e, TWOVALUE p2_s, TWOVALUE p2_e)
{
	double Xmax_1, Xmax_2, Xmin_1, Xmin_2, Ymax_1, Ymax_2, Ymin_1, Ymin_2;
	double V1, V2, V3, V4;

	if (p1_s.X > p1_e.X)
	{
		Xmax_1 = p1_s.X;
		Xmin_1 = p1_e.X;
	}
	else
	{
		Xmax_1 = p1_e.X;
		Xmin_1 = p1_s.X;
	}


	if (p1_s.Y > p1_e.Y)
	{
		Ymax_1 = p1_s.Y;
		Ymin_1 = p1_e.Y;
	}
	else
	{
		Ymax_1 = p1_e.Y;
		Ymin_1 = p1_s.Y;
	}


	if (p2_s.X > p2_e.X)
	{
		Xmax_2 = p2_s.X;
		Xmin_2 = p2_e.X;
	}
	else
	{
		Xmax_2 = p2_e.X;
		Xmin_2 = p2_s.X;
	}

	if (p2_s.Y > p2_e.Y)
	{
		Ymax_2 = p2_s.Y;
		Ymin_2 = p2_e.Y;
	}
	else
	{
		Ymax_2 = p2_e.Y;
		Ymin_2 = p2_s.Y;
	}


	if (Xmax_1 < Xmin_2 || Xmin_1 > Xmax_2 || Ymin_1 > Ymax_2 || Ymax_1 < Ymin_2)   //���߶���С���β��ཻ���ó����߶β��ཻ
		return false;
	else
	{
		V1 = (p1_e.X - p1_s.X) * (p2_s.Y - p1_s.Y) - (p2_s.X - p1_s.X) * (p1_e.Y - p1_s.Y);
		V2 = (p1_e.X - p1_s.X) * (p2_e.Y - p1_s.Y) - (p2_e.X - p1_s.X) * (p1_e.Y - p1_s.Y);
		V3 = (p2_e.X - p2_s.X) * (p1_s.Y - p2_s.Y) - (p1_s.X - p2_s.X) * (p2_e.Y - p2_s.Y);
		V4 = (p2_e.X - p2_s.X) * (p1_e.Y - p2_s.Y) - (p1_e.X - p2_s.X) * (p2_e.Y - p2_s.Y);
		//�����������ֱ����ʱ��   20131015
		bool Mark = false;
		TWOVALUE TempPoint;
		if (abs(V1)< 0.00000001)
		{
			Mark = true;
		}
		else if (abs(V2)< 0.00000001)
		{
			Mark = true;
		}
		else if (abs(V3)< 0.00000001)
		{
			Mark = true;
		}
		else if (abs(V4)< 0.00000001)
		{
			Mark = true;
		}
		if (Mark)
		{
			double t = 0;
			if (abs(TempPoint.X - p1_s.X)<abs(TempPoint.Y - p1_s.Y))
			{
				t = (TempPoint.Y - p1_s.Y) / (p1_e.Y - p1_s.Y);/////////////////////
			}
			else
			{
				t = (TempPoint.X - p1_s.X) / (p1_e.X - p1_s.X);
			}
			return true;
		}
		if (V3 * V4 <= 0 && V1 * V2 <= 0)
		{
			double dy = p1_e.Y - p1_s.Y;
			double dx = p1_e.X - p1_s.X;
			double t = 0;
			if (abs(p2_e.X - p2_s.X)<0.00000000001)
			{
				t = (p2_e.X - p1_s.X) / dx;/////////////////////
			}
			else
			{
				double k = (p2_s.Y - p2_e.Y) / (p2_s.X - p2_e.X);
				t = ((p2_s.Y - p1_s.Y) - k*p2_s.X + k*p1_s.X) / (dy - k*dx);
			}
			return true;
		}
		else
			return false;
	}
}

bool Jie_Ds::IsIntersect(TWOVALUE p1_s, TWOVALUE p1_e, vector<THRVALUE> Lines)
{
	for (int i = 0; i < (int)Lines.size() - 1; i++)
	{
		THRVALUE Star = Lines[i];
		THRVALUE End = Lines[i + 1];
		if (L2L_Intersect(Star, End, p1_s, p1_e))
		{
			return true;
		}
	}
	return false;
}

//�����߶ϲ�Ϊ�ϲ����Σ��õ����ڹ�ֵ�Ķϲ�����
void Jie_Ds::DealWithSingleFault()
{
	double k1 = 0;//б��
	double distance = 0.05*(m_XMax - m_XMin) / m_XNum; //��Ĳ�־���
	//cout << distance << endl;
	for (int i = 0; i < (int)m_FaultData.size(); i++)
	{
		int UDateSize = (int)m_FaultData[i].UData.size();//�϶ϲ���
		int DDateSize = (int)m_FaultData[i].DData.size();//�¶ϲ���
		int TDateSize = (int)m_FaultData[i].TData.size();//�м�ϲ���
														 //ֻ�������̻��½��̣������ݵ����м�������

														 //ֻ���м������ݣ���û�����¶ϲ�����
		if (TDateSize>0 && DDateSize == 0 && UDateSize == 0)
		{
			if (TDateSize == 2)		//ֻ������
			{
				//TODO:�������⴦������м��һ����TDateSize������
			}
			//Ϊ�����̺��½����������ռ�
			m_FaultData[i].DData.resize(TDateSize);
			m_FaultData[i].UData.resize(TDateSize);
			//���¶ϲ�ĵ�һ��������һ����Ĭ���Ǻ��м��ߵ�λ����һ����
			//��ʼ�����ǵĵ�һ����
			m_FaultData[i].DData[0].X = m_FaultData[i].TData[0].X + 0;
			m_FaultData[i].DData[0].Y = m_FaultData[i].TData[0].Y + 0;
			m_FaultData[i].UData[0].X = m_FaultData[i].TData[0].X - 0;
			m_FaultData[i].UData[0].Y = m_FaultData[i].TData[0].Y - 0;
			//��ʼ�����һ����
			m_FaultData[i].DData[TDateSize - 1].X = m_FaultData[i].TData[TDateSize - 1].X + 0;
			m_FaultData[i].DData[TDateSize - 1].Y = m_FaultData[i].TData[TDateSize - 1].Y + 0;
			m_FaultData[i].UData[TDateSize - 1].X = m_FaultData[i].TData[TDateSize - 1].X - 0;
			m_FaultData[i].UData[TDateSize - 1].Y = m_FaultData[i].TData[TDateSize - 1].Y - 0;
			//Ϊ�����������㸳ֵ
			for (int j = 1; j < (int)m_FaultData[i].TData.size() - 1; j++)
			{
				THRVALUE A = m_FaultData[i].TData[j - 1];
				THRVALUE O = m_FaultData[i].TData[j];
				THRVALUE B = m_FaultData[i].TData[j + 1];
				double dx1 = A.X - O.X;
				double dx2 = B.X - O.X;
				double dy1 = A.Y - O.Y;
				//double dy2 = B.X - O.Y;��һ��Ӧ���е����
				double dy2 = B.Y - O.Y;
				if (abs(dx1*dy2 - dx2*dy1)<0.000001)	//����һ��
				{
					//TODO:���⴦��Ӧ�����м���λ����
					//����ֱƽ����
					double A1, B1, C1;
					TWOVALUE Star, End, P;
					Star.X = A.X;
					Star.Y = A.Y;
					End.X = B.X;
					End.Y = B.Y;
					P.X = O.X;
					P.Y = O.Y;
					GetABC(Star, End, P, A1, B1, C1);
					TWOVALUE D;
					//���ߵĵ�λ��������
					double dx = A1 / sqrt(pow(A1, 2) + pow(B1, 2));
					double dy = B1 / sqrt(pow(A1, 2) + pow(B1, 2));
					//cout << A1 << " " << B1 << " " << C1 << endl;
					//cout << dx << " " << dy << endl;
					//cout << dx*distance << " " << dy*distance << endl;
					D.X = O.X + dx*distance;
					D.Y = O.Y - dy*distance;

					TWOVALUE E;
					E.X = 2 * O.X - D.X;
					E.Y = 2 * O.Y - D.Y;
					if (D.X > O.X || (D.X == O.X && D.Y > O.Y))	//������ϲ�������
					{
						m_FaultData[i].DData[j] = E;
						m_FaultData[i].UData[j] = D;
					}
					else
					{
						m_FaultData[i].DData[j] = D;
						m_FaultData[i].UData[j] = E;
					}
				}
				else				//ʹ�����������������ı��εķ���
				{
					//cout << "hello" << endl;
					//dx1 = dx1/sqrt(pow(dx1,2)+pow(dy1,2));
					//dy1 = dy1/sqrt(pow(dx1,2)+pow(dy1,2));
					//dx2 = dx2/sqrt(pow(dx2,2)+pow(dy2,2));
					//dy2 = dy2/sqrt(pow(dx2,2)+pow(dy2,2));
					//A.X = O.X + dx1 * distance;
					//A.Y = O.Y + dy1 * distance;

					//B.X = O.X + dx2 * distance;
					//B.Y = O.Y + dy2 * distance;

					//TWOVALUE D;
					//D.X = 0.5*(A.X + B.X);
					//D.Y = 0.5*(A.Y + B.Y);
					//TWOVALUE E;
					//E.X = 2*O.X - D.X;
					//E.Y = 2*O.Y - D.Y;
					//if (D.X >O.X || (D.X == O.X&&D.Y > O.Y))				//������ϲ�������
					//{
					//	m_FaultData[i].DData[j] = E;
					//	m_FaultData[i].UData[j] = D;
					//}
					//else
					//{
					//	m_FaultData[i].DData[j] = D;
					//	m_FaultData[i].UData[j] = E;
					//}
					//20141122�����޶���ƽ�����㷨
					double x1, y1, x2, y2;
					//�����Ӧ�ķ���λ����
					x1 = dx1 / sqrt(pow(dx1, 2) + pow(dy1, 2));
					y1 = dy1 / sqrt(pow(dx1, 2) + pow(dy1, 2));
					x2 = dx2 / sqrt(pow(dx2, 2) + pow(dy2, 2));
					y2 = dy2 / sqrt(pow(dx2, 2) + pow(dy2, 2));

					A.X = x1 * distance;//���Ե�Ĳ�־���
					A.Y = y1 * distance;

					B.X = x2 * distance;
					B.Y = y2 * distance;

					TWOVALUE D;
					D.X = O.X + (A.X + B.X);
					D.Y = O.Y + (A.Y + B.Y);

					TWOVALUE E;
					E.X = 2 * O.X - D.X;
					E.Y = 2 * O.Y - D.Y;
					if (j == 1)
					{
						if (D.X > O.X || (D.X == O.X && D.Y > O.Y))	//������ϲ�������
						{
							m_FaultData[i].DData[j] = E;
							m_FaultData[i].UData[j] = D;
						}
						else
						{
							m_FaultData[i].DData[j] = D;
							m_FaultData[i].UData[j] = E;
						}
					}
					else
					{
						//��������Ҫ�жϵ�D��E��һ�����м�ϲ��ߵ��·�����һ�����м�ϲ��ߵ��Ϸ�
						if (!IsIntersect(m_FaultData[i].DData[j - 1], D, m_FaultData[i].TData))		//���м�ϲ����޽��㣬��D���·�
						{
							m_FaultData[i].DData[j] = D;
							m_FaultData[i].UData[j] = E;
						}
						else   //���м�ϲ����н��㣬��E���·�
						{
							m_FaultData[i].DData[j] = E;
							m_FaultData[i].UData[j] = D;
						}
					}
				}
			}
		}
	}
}

//���ֻ�����¶ϲ㣬û�жϲ����ߵ�ʱ�򣬹�������20131119
void Jie_Ds::GetMiddleData()
{
	for (int i = 0; i < (int)m_FaultData.size(); i++)
	{
		FaultPoint OneLine = m_FaultData[i];

		/***��ֹ���¶ϲ����Ŀ��һ��20131209***/
		int MinNum = (int)OneLine.UData.size();
		if (MinNum >(int)OneLine.DData.size())
		{
			MinNum = (int)OneLine.DData.size();
		}
		/**************************************/
		if (OneLine.TData.size() <= 1)
		{
			for (int j = 0; j < MinNum; j++)
			{
				THRVALUE Middle;
				Middle.X = (OneLine.DData[j].X + OneLine.UData[j].X) / 2.0;
				Middle.Y = (OneLine.DData[j].Y + OneLine.UData[j].Y) / 2.0;
				Middle.Z = 0;
				m_FaultData[i].TData.push_back(Middle);//TData�б����˶ϲ����ߵ���Ϣ
			}
		}
	}
}

void Jie_Ds::GetNearByPoint(const TWOVALUE& m_PrePoint, const TWOVALUE& m_NextPoint, const TWOVALUE& point, Data &m_newPoint)
{
	double X0 = point.X;      //�߶���һ��                              
	double Y0 = point.Y;
	double X1 = m_PrePoint.X; //�߶������˵�                              
	double Y1 = m_PrePoint.Y;
	double X2 = m_NextPoint.X;
	double Y2 = m_NextPoint.Y;

	double X, Y;//����

				//б��Ϊ����������
	if (X1 == X2)
	{
		X = X1;
		Y = Y0;
	}
	//б��Ϊ0�����
	else if (Y1 == Y2)
	{
		X = X0;
		Y = Y1;
	}
	else
	{
		//б��
		double K = (Y2 - Y1) / (X2 - X1);
		X = (K*Y0 + X0 + K*(K*X1 - Y1)) / (K*K + 1.0f);
		Y = (-1 * K*X1 + Y1 + K*(K*Y0 + X0)) / (K*K + 1.0f);
	}
	m_newPoint.X = X;
	m_newPoint.Y = Y;

}

double Jie_Ds::GetPointToLineDis(TWOVALUE PrePoint, TWOVALUE NextPoint, TWOVALUE point)
{
	Data ND;
	//�ҵ�����
	GetNearByPoint(PrePoint, NextPoint, point, ND);
	//�������߶���
	//if ((((ND.X - PrePoint.X) * (ND.X - NextPoint.X) < 0)||(ND.X==PrePoint.X)||(ND.X==NextPoint.X))||
	//	(((ND.Y - PrePoint.Y) * (ND.Y - NextPoint.Y) < 0) || (ND.Y == PrePoint.Y) || (ND.Y == NextPoint.Y)))
	if (((ND.X - PrePoint.X) * (ND.X - NextPoint.X) <= 0)||((ND.Y - PrePoint.Y) * (ND.Y - NextPoint.Y)<=0))
	//if ((ND.X - PrePoint.X) * (ND.X - NextPoint.X) <= 0)
		return GetDis(point.X, point.Y, ND.X, ND.Y);
	else
		return 999999;
}

//A�㵽���߶εľ���		20140731
double Jie_Ds::GetDistance(vector<TWOVALUE> Pts, TWOVALUE A)
{
	double Mind = 1000000000;
	for (int i = 0; i < (int)Pts.size() - 1; i++)
	{
		TWOVALUE Star = Pts[i];
		TWOVALUE End = Pts[i + 1];
		double ds = sqrt(pow(Star.X - A.X, 2) + pow(Star.Y - A.Y, 2));
		double de = sqrt(pow(End.X - A.X, 2) + pow(End.Y - A.Y, 2));
		double d = GetPointToLineDis(Star, End, A);
		if (ds < Mind)
		{
			Mind = ds;
		}
		if (de < Mind)
		{
			Mind = de;
		}
		if (d < Mind)
		{
			Mind = d;
		}
	}
	return Mind;
}

//���ϲ���û�������Լ�����ϲ����20131115
void Jie_Ds::SetDIF(double fDis)
{
	BOOL Mark = FALSE;
	double MarkD = 0;
	for (int i = 0; i < (int)m_FaultData.size(); i++)
	{
		FaultPoint OneLine = m_FaultData[i];
		for (int j = 0; j < (int)OneLine.TData.size(); j++)
		{
			//�ϲ�����洢�����ߵ�Z����֮��
			if (abs(OneLine.TData[j].Z) > 0.001)//������������
			{
				Mark = TRUE;
				break;
			}
		}
		if (Mark)
		{
			break;
		}
	}
	//if (Mark)
	{
		//��ƶϲ����������ƽ�����ԽС�����ԽС
		double dx = (m_XMax - m_XMin) / m_XNum;
		double dy = (m_YMax - m_YMin) / m_YNum;
		MarkD = dx;
		if (MarkD < dy)
		{
			MarkD = dy;		//d����һ��fdis
		}
	}
	for (int i = 0; i < (int)m_FaultData.size(); i++)
	{
		FaultPoint OneLine = m_FaultData[i];
		//for (int j = 0 ; j < (int)OneLine.UData.size() ; j ++)
		for (int j = 0; j < (int)OneLine.TData.size(); j++)		//������Ϊ��׼20131209
		{
			TWOVALUE UpPt = OneLine.UData[j];
			TWOVALUE DownPt = OneLine.DData[j];
			//TODO:��OneLine.TData[j]���϶ϲ�����¶ϲ�ľ���20140731
			double d = GetDistance(OneLine.DData, UpPt);	//20140731	
															//double d = sqrt(pow(UpPt.X - DownPt.X , 2) + pow(UpPt.Y - DownPt.Y , 2));
			if (d > 2 * MarkD)
			{
				d = 2 * MarkD;		//��ֹ���̫�� 20131209
			}
			//else if (d < 0.02 * fDis )
			//{
			//d = 0.02 * fDis ;		//����̫С20140722
			//}
			double DiF = d / MarkD * fDis / 6;	//���С��
			m_FaultData[i].TData[j].Z = DiF;  //TData�е�Z�������������Ϣ
		}
	}
}

//����ϲ��ߣ�ʹ��û���غϵ㣬������β��20140721
void Jie_Ds::TidyFaultLine(FalutLine &LineI)
{
	vector<TWOVALUE> Fline = LineI.FLine;
	//�������������ʵû��Ҫ����Ϊ��һ���������Ѿ��жϹ���
	if ((int)Fline.size() < 2)
	{
		return;
	}
	for (int i = 0; i < (int)Fline.size() - 1; i++)
	{
		TWOVALUE P1, P2;
		P1 = Fline[i];
		P2 = Fline[i + 1];
		//��������غ�����
		if (abs(P1.X - P2.X) < 0.001 && abs(P1.Y - P2.Y) < 0.001)
		{
			Fline.erase(Fline.begin() + i + 1);
			i = i - 1;
		}
	}
	LineI.FLine = Fline;
}
bool Jie_Ds::IsClockwise(const TWOVALUE &p0, const TWOVALUE &p1, const TWOVALUE &p2)
{
	double TempVal = (p1.X - p0.X) * (p2.Y - p1.Y) - (p2.X - p1.X) * (p1.Y - p0.Y);
	//���:��ʱ��Ϊ��  ˳ʱ��Ϊ��

	if (TempVal > 0) //��ʱ��
		return false;
	else            //˳ʱ��
		return true;

}

//���Ӷϲ���20140721
//LineI,LineJΪҪ���ӵĶϲ���
//IP,JPΪ�߶��ཻ�Ľ�����ţ�����P1,P2;  P1��LineI�ϵ����ΪIP[0],��LineJ�ϵ����ΪJP[0]
FalutLine Jie_Ds::ConnectLine(FalutLine &LineI, FalutLine LineJ, int IP[], int JP[])
{
	FalutLine NewLine;
	NewLine.IsClose = LineI.IsClose;

	//�޸Ĳ���
	//NewLine.IsNormal = NewLine.IsNormal;
	NewLine.IsNormal = LineI.IsNormal;

	NewLine.IsClock = LineI.IsClock;
	//������ͬ�������
	int IS = 0, JSd = 0, IE = 0, JEd = 0;
	IS = IP[0];		IE = IP[1];
	JSd = JP[0];		JEd = JP[1];
	cout << "=====================" << endl;
	cout << "IS;" << IS << " " << "IE;" << IE << endl;
	cout << "JSd;" << JSd << " " << "JEd;" << JEd<< endl;
	cout << "=====================" << endl;
	//һ����˵��ȫ���ĵ㶼���¶ϲ��ϣ��Լ�����ϲ���������β����ܲ���
	//�����һ����
	if (IE - IS < (int)LineI.FLine.size() / 2)
	{
		//IS ---0,End-1 ~~~IE(������IE)
		NewLine.IsClock = LineI.IsClock * (-1);		//�޸���˳��20140722
		for (int i = IS; i >= 0; i--)
		{
			TWOVALUE Pt = LineI.FLine[i];
			NewLine.FLine.push_back(Pt);
		}
		//�����FLine[(int)LineI.FLine.size() - 1]��FLine[0]Ӧ����ͬһ���㣬�ǲ����ظ������
		for (int i = (int)LineI.FLine.size() - 2; i > IE; i--)
			//for (int i = (int)LineI.FLine.size() - 1 ; i > IE ; i --)
		{
			TWOVALUE Pt = LineI.FLine[i];
			NewLine.FLine.push_back(Pt);
		}
	}
	else
	{
		//IS~~~IE(������IE)
		for (int i = IS; i < IE; i++)
		{
			TWOVALUE Pt = LineI.FLine[i];
			NewLine.FLine.push_back(Pt);
		}
	}
	//���������һ����
	if (JEd > JSd)
	{
		if (JEd - JSd < (int)LineJ.FLine.size() / 2)
		{
			//JEd ---End,`1~~~JSd
			for (int j = JEd; j < (int)LineJ.FLine.size(); j++)
			{
				TWOVALUE Pt = LineJ.FLine[j];
				NewLine.FLine.push_back(Pt);
			}
			//����֮����Ҫ�ӵȺţ���Ϊ�˱պ�
			//for (int j = 1; j < JSd; j++)
			for (int j = 1; j <= JSd; j++)		//�������һ���㣬����һ�����Ӧ
			{
				TWOVALUE Pt = LineJ.FLine[j];
				NewLine.FLine.push_back(Pt);
			}
		}
		else	//JEd---JSd````�ݼ�
		{
			for (int j = JEd; j >= JSd; j--)
				//for (int j = JEd ; j >= JSd ; j --)		//�������һ���㣬����һ�����Ӧ
			{
				TWOVALUE Pt = LineJ.FLine[j];
				NewLine.FLine.push_back(Pt);
			}
		}
	}
	else
	{
		//cout << "hello" << endl;
		if (JSd - JEd  < (int)LineJ.FLine.size() / 2)
		{
			//JEd ---0,`End-1~~~JSd
			for (int j = JEd; j >= 0; j--)
			{
				TWOVALUE Pt = LineJ.FLine[j];
				NewLine.FLine.push_back(Pt);
			}
			for (int j = (int)LineJ.FLine.size() - 2; j >= JSd; j--)
				//for (int j = (int)LineJ.FLine.size() - 2 ; j >= JSd ; j --)		//�������һ���㣬����һ�����Ӧ
			{
				TWOVALUE Pt = LineJ.FLine[j];
				NewLine.FLine.push_back(Pt);
			}
		}
		else	//JEd---JSd````����
		{
			//cout << "hello1" << endl;
			for (int j = JEd; j <= JSd; j++)
				//for (int j = JEd ; j >= JSd ; j --)		
			{
				TWOVALUE Pt = LineJ.FLine[j];
				NewLine.FLine.push_back(Pt);
			}
		}
	}
	return NewLine;
}

//�ϲ����Ƿ��ཻ���ཻ�Ķϲ����ڶϲ�����غ�20140719
BOOL Jie_Ds::Isintersect(FalutLine &LineI, FalutLine LineJ)
{
	//TODO:�ж϶ϲ����Ƿ��ཻ
	BOOL IsCross = FALSE;
	int IP[20], JP[20];
	int Num = 0;
	for (int i = 0; i < (int)LineI.FLine.size() - 1; i++)//���һ����׵���ͬ����ֹ����������Ϊ��ʹ�ϲ��߱պϣ��ϲ����ϵ����һ����͵�һ������ͬһ����
	{
		TWOVALUE PLineI = LineI.FLine[i];
		for (int j = 0; j < (int)LineJ.FLine.size() - 1; j++)
		{
			TWOVALUE PLineJ = LineJ.FLine[j];
			//�ж��������غϵ�����
			if (abs(PLineI.X - PLineJ.X) < 0.001 && abs(PLineI.Y - PLineJ.Y) < 0.001)
			{
				IP[Num] = i;
				JP[Num] = j;
				Num++;
			}
		}
	}
	if (Num == 2)			//��������˵ֻ��0����2������
	{
		IsCross = TRUE;
		FalutLine NewLine = ConnectLine(LineI, LineJ, IP, JP);
		LineI = NewLine;
	}
	return IsCross;
}

//���ཻ�Ķϲ��߽������ 20140717
void Jie_Ds::Combination()
{
	for (int i = 0; i < (int)m_vecFaultLines.size(); i++)
	{
		FalutLine LineI = m_vecFaultLines[i];
		for (int j = 0; j < (int)m_vecFaultLines.size(); j++)
		{
			if (i == j)
			{
				continue;
			}
			FalutLine LineJ = m_vecFaultLines[j];

			//TODO:�ж϶ϲ����Ƿ��ཻ���ཻ�������ϲ���Ϊһ��,�����¿�ʼ׷��
			if (Isintersect(LineI, LineJ))
			{
				TidyFaultLine(LineI);

				m_vecFaultLines[i] = LineI;
				m_vecFaultLines.erase(m_vecFaultLines.begin() + j);//��Ϊ�ϲ���LineJ�Ѿ��ϲ����ϲ���LineI�У����Կ��Խ��ϲ���LineJɾ������
				if (j < i)
				{
					i--;
				}
				j = 0;		//���¿�ʼ�жϡ�������ֹ���֣�01���뽻��02�ཻ��21�ཻ�����
			}
		}
	}
}

//�õ�����׷�ٵĶϲ�����
void Jie_Ds::GetFaultLine(vector<FaultPoint> data)
{
	m_vecFaultLines.clear();
	//��������Ƿ����
	for (int i = 0; i < (int)data.size(); i++)
	{
		FalutLine Line;
		FaultPoint LineOne = data[i];
		vector<TWOVALUE> LineU = LineOne.UData;//�϶ϲ���
		vector<TWOVALUE> LineD = LineOne.DData;//�¶ϲ���

		int Num = LineD.size();
		//����֮���Բ�����¶ϲ����β�㣬����Ϊ֮ǰ�����պ϶ϲ���бպϲ�����ʱ���϶ϲ����β�ֱ�������¶ϲ����β�㣬��������û��Ҫ�ظ����
		//for (int j = Num - 1; j > -1; j --)
		for (int j = Num - 2; j > 0; j--)
		{
			LineU.push_back(LineD[j]);
		}
		LineU.push_back(LineU[0]);//��β����
		Line.FLine = LineU;//�õ�һ���ϲ���
		Line.IsClose = TRUE;//�պ϶ϲ�
		m_vecFaultLines.push_back(Line);//�ϲ����ϵĵ�ǰһ����Ϊ�϶ϲ����ϵĵ㣬��һ����Ϊ�¶ϲ��ϵĵ�
	}
	int NumLines = (int)m_vecFaultLines.size();
	//�ϲ��ߵ������������1
	if (NumLines<1)
	{
		return;
	}
	//ɾ������������
	for (int i = 0; i < (int)m_vecFaultLines.size(); i++)
	{
		//�ϲ����ϵ����Ŀ����̫��
		if (m_vecFaultLines[i].FLine.size()<2)
		{
			m_vecFaultLines.erase(m_vecFaultLines.begin() + i);
			i--;
		}
		else
		{
			FalutLine LineI = m_vecFaultLines[i];
			TidyFaultLine(LineI);					//����ϲ��ߣ�ʹ��û���غϵ㣬������β��20140721

													//���ϲ�˳��ֵ20140722
			vector<TWOVALUE> Fline = LineI.FLine;
			int Num = (int)Fline.size();
			//���ﻹ��ͬ���ĵ���Ϊ��ʹ�ϲ��߱պϣ�Fline[Num-1]��ʵ�Ǻ�Fline[0]�����ͬһ����
			bool Clock = IsClockwise(Fline[Num - 2], Fline[0], Fline[1]);
			if (Clock)
			{
				LineI.IsClock = 1;		//˳ʱ��
			}
			else
			{
				LineI.IsClock = -1;		//��ʱ��
			}

			m_vecFaultLines[i] = LineI;
		}
	}
	//���ཻ�Ķϲ㻮��Ϊһ���ϲ�20140717
	Combination();
}

void Jie_Ds::NormalVector(vector<THRVALUE>& a, THRVALUE& b, double Position)
{
	//�������
	double Dis = sqrt((a[1].X - a[0].X) * (a[1].X - a[0].X) + (a[1].Y - a[0].Y) * (a[1].Y - a[0].Y));
	//X������ͬ ������Ϊx�᷽��
	if (a[0].X == a[1].X)
	{
		b.X = 1;
		b.Y = 0;
	}
	//y������ͬ  y�᷽��
	else if (a[0].Y == a[1].Y)
	{
		b.X = 0;
		b.Y = 1;
	}
	//����ͬ ��Ϊ���� k1*k2=-1�� ��λ������ʼ��Ϊ
	else
	{
		b.X = -(a[1].Y - a[0].Y) / Dis;
		b.Y = (a[1].X - a[0].X) / Dis;
	}

	//����һ�����߶�ͬ����ĵ�λ����
	vector<THRVALUE> lineP(2);
	lineP[0].X = 0;
	lineP[0].Y = 0;
	lineP[1].X = (a[1].X - a[0].X) / Dis;
	lineP[1].Y = (a[1].Y - a[0].Y) / Dis;
	int judgePoint;
	//��b()�ڵ�λ����linp����໹���Ҳ�
	judgePoint = (int)(lineP[0].X * (b.Y - lineP[1].Y) + lineP[1].X* (lineP[0].Y - b.Y) + b.X * (lineP[1].Y - lineP[0].Y));

	//���϶ϲ���ͬһ�� �������ķ�����Ҫָ���϶ϲ�ķ���
	if (judgePoint * Position > 0)
	{
		//�����κδ���
	}
	else
	{
		//���� �����������ķ���
		b.X = -b.X;
		b.Y = -b.Y;
	}
}

void Jie_Ds::ReviseWires(int Num, vector<vector<double>>& faultSu, double Position)
{
	vector<THRVALUE> overA(2);  //�洢�ϲ�����������
	THRVALUE overB; //�洢���ڶϲ����� �Ĵ�ֱ��λ��������ָ������
	int insertCout = 8;
	vector<THRVALUE> newFault(insertCout * (m_FaultData[Num].TData.size() - 1) + 1);
	vector<TWOVALUE> newFaultVec(newFault.size());
	//�����ϲ����ߵ�ÿ���߶�
	for (int i = 0; i < (int)m_FaultData[Num].TData.size() - 1; i++)
	{
		overA[0].X = m_FaultData[Num].TData[i].X;
		overA[0].Y = m_FaultData[Num].TData[i].Y;
		overA[1].X = m_FaultData[Num].TData[i + 1].X;
		overA[1].Y = m_FaultData[Num].TData[i + 1].Y;

		//����һ�����߶εĵķ���������ָ�����ϲ�ĵ�λ����
		NormalVector(overA, overB, Position);
		//������Ķϲ�㸳����ֵ������ֵ �����Բ�ֵ
		for (int j = 1; j <= insertCout; j++)
		{
			newFault[i * insertCout + j].X = m_FaultData[Num].TData[i].X + (m_FaultData[Num].TData[i + 1].X - m_FaultData[Num].TData[i].X) * j / insertCout;
			newFault[i* insertCout + j].Y = m_FaultData[Num].TData[i].Y + (m_FaultData[Num].TData[i + 1].Y - m_FaultData[Num].TData[i].Y) * j / insertCout;
			newFault[i * insertCout + j].Z = m_FaultData[Num].TData[i].Z + (m_FaultData[Num].TData[i + 1].Z - m_FaultData[Num].TData[i].Z) * j / insertCout;
			//�Ѵ˵�λ������ֵ��newFaultVec �� 0,1
			newFaultVec[i * insertCout + j].X = overB.X;
			newFaultVec[i * insertCout + j].Y = overB.Y;
		}
	}

	double distance;
	double cosFault;
	double avilityVal;
	double disSum;
	//�������нǵ���������ֵ ��������ֵ
	for (int i = 1; i < (int)faultSu.size(); i++)
	{
		faultSu[i].resize(m_GridPoint.size());
		for (int k = 1; k < (int)faultSu.size(); k++)
		{
			avilityVal = 0;
			disSum = 0;
			for (int j = 1; j < (int)newFault.size(); j++)
			{
				// ����ֵ�㵽���Ƶ�ľ���
				distance = sqrt((newFault[j].X - m_GridPoint[i][k].X) * (newFault[j].X - m_GridPoint[i][k].X)
					+ (newFault[j].Y - m_GridPoint[i][k].Y) * (newFault[j].Y - m_GridPoint[i][k].Y));

				cosFault = (m_GridPoint[i][k].X - newFault[j].X) / distance * newFaultVec[j].X
					+ (m_GridPoint[i][k].Y - newFault[j].Y) / distance * newFaultVec[j].Y;

				avilityVal = (newFault[j].Z / 2 * cosFault) / (distance * distance) + avilityVal;

				disSum = disSum + 1 / (distance * distance);
			}
			faultSu[i][k] = avilityVal / disSum * pow((-1.0), (m_FaultData[Num].mark + 2));
		}
	}
}

void Jie_Ds::FaultWires(int Num, vector<Data>& oriData, vector<THRVALUE>& faultSu, double Position)
{
	vector<THRVALUE> overA(2);  //�洢�ϲ�����������
	THRVALUE overB; //�洢���ڶϲ����� �Ĵ�ֱ��λ��������ָ������
	int insertCout = 8;
	vector<THRVALUE> newFault(insertCout * (m_FaultData[Num].TData.size() - 1) + 1);
	vector<TWOVALUE> newFaultVec(newFault.size());
	//�����ϲ����ߵ�ÿ���߶�
	for (int i = 0; i < (int)m_FaultData[Num].TData.size() - 1; i++)
	{
		//ȡ��ǰ�ϲ���м�ϲ��ߵ�������
		overA[0].X = m_FaultData[Num].TData[i].X;
		overA[0].Y = m_FaultData[Num].TData[i].Y;
		overA[1].X = m_FaultData[Num].TData[i + 1].X;
		overA[1].Y = m_FaultData[Num].TData[i + 1].Y;

		//����һ�����߶εĵķ���������ָ�����ϲ�ĵ�λ�������洢��overB��
		NormalVector(overA, overB, Position);
		//������Ķϲ�㸳����ֵ������ֵ �����Բ�ֵ
		for (int j = 1; j <= insertCout; j++)
		{
			newFault[i * insertCout + j].X = m_FaultData[Num].TData[i].X + (m_FaultData[Num].TData[i + 1].X - m_FaultData[Num].TData[i].X) * j / insertCout;
			newFault[i * insertCout + j].Y = m_FaultData[Num].TData[i].Y + (m_FaultData[Num].TData[i + 1].Y - m_FaultData[Num].TData[i].Y) * j / insertCout;
			newFault[i * insertCout + j].Z = m_FaultData[Num].TData[i].Z + (m_FaultData[Num].TData[i + 1].Z - m_FaultData[Num].TData[i].Z) * j / insertCout;
			//�Ѵ˵�λ������ֵ��newFaultVec �� 0,1
			//�ڴ˶���ÿһ���㴦��ָ�����̵ĵ�λ����������һ����
			newFaultVec[i * insertCout + j].X = overB.X;
			newFaultVec[i * insertCout + j].Y = overB.Y;
		}
	}

	//�Ѿ�λ�����긴�Ƹ� faultsu	
	faultSu.resize(oriData.size());
	for (int i = 0; i < (int)oriData.size(); i++)
	{
		faultSu[i].X = oriData[i].X;
		faultSu[i].Y = oriData[i].Y;
		faultSu[i].Z = oriData[i].Z;
	}

	//����ϲ�
	//�ü�Ȩ�������ֵ
	double distance;
	double cosFault;
	double avilityVal;
	double disSum;
	//����������ɢ������ֵ
	for (int i = 0; i < (int)faultSu.size(); i++)
	{
		avilityVal = 0;
		disSum = 0;
		//���²���ĵ����ιɵ�
		for (int j = 1; j < (int)newFault.size(); j++)
		{
			//�����¹���Ķϲ����ϵ�ÿ���㵽�˾�λ��ľ���
			distance = sqrt((newFault[j].X - faultSu[i].X) * (newFault[j].X - faultSu[i].X)
				+ (newFault[j].Y - faultSu[i].Y) * (newFault[j].Y - faultSu[i].Y));
			//������ܽ�����ֱ�ӽ���Z�����ĸ�ֵ�������Z����ָ��Ӧ�������ֵ
			if (abs(distance) < 0.0001)
			{
				faultSu[i].Z = newFault[j].Z;
				break;
			}
			//��λ��Ͳ�ֵ��Ķϲ�㹹�ɵĵ�λ�����ͷ������ĵ��
			//https://wenku.baidu.com/view/dd5572e183d049649b6658ad.html
			cosFault = (faultSu[i].X - newFault[j].X) / distance * newFaultVec[j].X +
				(faultSu[i].Y - newFault[j].Y) / distance * newFaultVec[j].Y;

			avilityVal = (newFault[j].Z / 2 * cosFault) / (distance * distance) + avilityVal;

			//����ֻ�Ƕ��ھ���ƽ���ĵ���ֵ���е���
			disSum = disSum + 1 / (distance * distance);
		}

		//����ɢ�������ֵ
		//mark��ʾ�����ϲ㻹����ϲ�  ���ϲ�Ϊ1 ��ϲ�Ϊ0
		vector<TWOVALUE> UData;
		faultSu[i].Z = avilityVal / disSum * pow((-1.0), m_FaultData[Num].mark + 2);

	}
}


//�����ϲ��ֵ
void Jie_Ds::FaultKriking()
{
	//���û�жϲ����ݣ������޶ϲ�Ŀ�����ֵ
	if ((int)m_FaultData.size() <= 0)	
	{
		EvaluateNoFault();
		return;
	}
	//�����жϲ�Ŀ�����ֵ
	double Position;
	vector<Data> suV;
	vector<THRVALUE> faultTemp;
	suV.assign(m_oriData.begin(), m_oriData.end());//��ֵ����   ԭʼ����

	int XM = m_XNum;
	int	YM = m_YNum;

	vector<THRVALUE> faultSu(suV.size());
	for (int i = 0; i < (int)m_FaultData.size(); i++)
	{
		//��ȡ�ϲ������еĵ����ϲ�����
		//�ж� ���������ߵ���һ��
		//������ʵ���������е�һ�����������ж��������������㹹�ɵ�ֱ�ߵ���һ�࣬�����ʽ�����Ϊ���ϵĹ�ʽ
		//https://blog.csdn.net/qq_27606639/article/details/80905311
		Position = m_FaultData[i].TData[0].X * (m_FaultData[i].UData[1].Y - m_FaultData[i].TData[1].Y)
			+ m_FaultData[i].TData[1].X * (m_FaultData[i].TData[0].Y - m_FaultData[i].UData[1].Y)
			+ m_FaultData[i].UData[1].X * (m_FaultData[i].TData[1].Y - m_FaultData[i].TData[0].Y);
		//�ϲ�������������,ԭʼ����,�洢�������
		//�������ϲ��������������ֵ����ֵ
		FaultWires(i, suV, faultTemp, Position);
		//��ÿһ���ϲ�Ӱ�����ɢ������ֵ���ӣ�Ϊ�ϲ�Ӱ�������ֵ
		for (int j = 0; j < (int)suV.size(); j++)
			faultSu[j].Z = faultSu[j].Z + faultTemp[j].Z;

		faultTemp.clear();
	}

	//��������ֵ ������ɢ������ֵΪû�жϲ�ǰ������ֵ
	for (int i = 0; i < (int)suV.size(); i++)
		suV[i].Z = suV[i].Z - faultSu[i].Z;
	std::ofstream out("data//fault_test//cor_afterP.obj");
	for (int i = 0; i < suV.size(); i++)
		out << "v " << suV[i].X << " " << suV[i].Y << " " << "0" << endl;
	out.close();
	vector<double> ni;
	//�����ֵʱ��ÿһ��ǰ���ϵ��
	PreMatrix(suV, ni);

	faultSu.clear();
	faultSu.resize(XM);


	//������ϲ�Ӱ��� ���нǵ���������������ֵ  ����ͬ��
	vector<vector<double>> faultT(m_GridPoint.size());
	vector<vector<double>> faultS(XM + 1);

	for (int i = 0; i < (int)m_FaultData.size(); i++)
	{
		faultT.resize(m_GridPoint.size());
		//��ȡ�ϲ������еĵ����ϲ�����
		Position = m_FaultData[i].TData[0].X * (m_FaultData[i].UData[1].Y - m_FaultData[i].TData[1].Y)
			+ m_FaultData[i].TData[1].X * (m_FaultData[i].TData[0].Y - m_FaultData[i].UData[1].Y)
			+ m_FaultData[i].UData[1].X * (m_FaultData[i].TData[1].Y - m_FaultData[i].TData[0].Y);
		//����˶ϲ�Ӱ��Ľǵ���������ֵ������ֵ
		ReviseWires(i, faultT, (int)Position);
		//�Ѷϲ�Ӱ��ǵ�����������ֵ����
		for (int k = 1; k <= XM; k++)
		{
			faultS[k].resize(YM + 1);
			for (int j = 1; j <= YM; j++)
			{
				faultS[k][j] = faultS[k][j] + faultT[k][j];
			}
		}
		faultT.clear();
	}
	for (int i = 1; i <= XM; i++)
	{
		for (int j = 1; j <= YM; j++)
		{
			TWOVALUE D;
			D.X = m_GridPoint[i][j].X;
			D.Y = m_GridPoint[i][j].Y;
			//if (!IsInside(D))		//���Ƕϲ��ڲ���
			{
				//���ƴ˵������ֵ    û�жϲ�Ĺ���ֵ -/+ ����ֵ			
				m_GridPoint[i][j].Z = InsertEst(suV, D, ni) - faultS[i][j];
			}
			//else
			{
				//������ֵ������ֵ
				//m_GridPoint[i][j].Z = InsertEst(suV, D, ni) ;	
			}
			if (m_ZMin > m_GridPoint[i][j].Z)
			{
				m_ZMin = m_GridPoint[i][j].Z;
			}

			if (m_ZMax < m_GridPoint[i][j].Z)
			{
				m_ZMax = m_GridPoint[i][j].Z;
			}

		}
	}
	m_suV = suV;
	m_ni = ni;				//20130913 Ӧ�������Ʒ�
}

//�õ�һ����Ĺ���ֵ
void Jie_Ds::ReviseWires(int Num, THRVALUE& Point, double Position)
{
	vector<THRVALUE> overA(2);  //�洢�ϲ�����������
	THRVALUE overB; //�洢���ڶϲ�����Ĵ�ֱ��λ��������ָ������
	int insertCout = 8;
	vector<THRVALUE> newFault(insertCout * (m_FaultData[Num].TData.size() - 1) + 1);
	vector<TWOVALUE> newFaultVec(newFault.size());
	//�����ϲ����ߵ�ÿ���߶�
	for (int i = 0; i < (int)m_FaultData[Num].TData.size() - 1; i++)
	{
		overA[0].X = m_FaultData[Num].TData[i].X;
		overA[0].Y = m_FaultData[Num].TData[i].Y;
		overA[1].X = m_FaultData[Num].TData[i + 1].X;
		overA[1].Y = m_FaultData[Num].TData[i + 1].Y;

		//����һ�����߶εĵķ���������ָ�����ϲ�ĵ�λ����
		NormalVector(overA, overB, Position);
		//������Ķϲ�㸳����ֵ������ֵ �����Բ�ֵ
		for (int j = 1; j <= insertCout; j++)
		{
			newFault[i * insertCout + j].X = m_FaultData[Num].TData[i].X + (m_FaultData[Num].TData[i + 1].X - m_FaultData[Num].TData[i].X) * j / insertCout;
			newFault[i* insertCout + j].Y = m_FaultData[Num].TData[i].Y + (m_FaultData[Num].TData[i + 1].Y - m_FaultData[Num].TData[i].Y) * j / insertCout;
			newFault[i * insertCout + j].Z = m_FaultData[Num].TData[i].Z + (m_FaultData[Num].TData[i + 1].Z - m_FaultData[Num].TData[i].Z) * j / insertCout;
			//�Ѵ˵�λ������ֵ��newFaultVec �� 0,1
			newFaultVec[i * insertCout + j].X = overB.X;
			newFaultVec[i * insertCout + j].Y = overB.Y;
		}
	}

	double distance;
	double cosFault;
	double avilityVal;
	double disSum;
	//�������нǵ���������ֵ ��������ֵ
	avilityVal = 0;
	disSum = 0;
	for (int j = 1; j < (int)newFault.size(); j++)
	{
		// ����ֵ�㵽���Ƶ�ľ���
		distance = sqrt((newFault[j].X - Point.X) * (newFault[j].X - Point.X)
			+ (newFault[j].Y - Point.Y) * (newFault[j].Y - Point.Y));
		if (distance == 0.0)
		{
			continue;
		}

		cosFault = (Point.X - newFault[j].X) / distance * newFaultVec[j].X
			+ (Point.Y - newFault[j].Y) / distance * newFaultVec[j].Y;

		avilityVal = (newFault[j].Z / 2 * cosFault) / (distance * distance) + avilityVal;
		disSum = disSum + 1 / (distance * distance);
	}
	Point.Z = Point.Z + avilityVal / disSum * pow((-1.0), (m_FaultData[Num].mark + 1));
}

//�õ�һ����Ĺ�ֵ
void Jie_Ds::EvaluateFault(THRVALUE& Point)
{
	double Position;
	for (int i = 0; i < (int)m_FaultData.size(); i++)
	{
		//��ȡ�ϲ����� �еĵ����ϲ�����
		//�ж� ���������ߵ��Ĳ� ���Ϊ 1 �Ҳ�Ϊ-1
		Position = m_FaultData[i].TData[0].X * (m_FaultData[i].UData[1].Y - m_FaultData[i].TData[1].Y)
			+ m_FaultData[i].TData[1].X * (m_FaultData[i].TData[0].Y - m_FaultData[i].UData[1].Y)
			+ m_FaultData[i].UData[1].X * (m_FaultData[i].TData[1].Y - m_FaultData[i].TData[0].Y);
		//�ϲ�������������,ԭʼ����,�洢�������
		//���� ���ϲ��������������ֵ����ֵ
		ReviseWires(i, Point, Position);
	}
}

//���뷴�ȶϲ��ֵ
void Jie_Ds::FaultInvDis()
{
	if ((int)m_FaultData.size() <= 0)
	{
		EvaluateNoFault();
		return;
	}
	double Position;
	vector<Data> suV;
	vector<THRVALUE> faultTemp;
	suV.assign(m_oriData.begin(), m_oriData.end());

	int XM = m_XNum;
	int	YM = m_YNum;

	vector<THRVALUE> faultSu(suV.size());
	for (int i = 0; i < (int)m_FaultData.size(); i++)
	{

		//��ȡ�ϲ����� �еĵ����ϲ�����
		//�ж� ���������ߵ��Ĳ� ���Ϊ 1 �Ҳ�Ϊ-1
		Position = m_FaultData[i].TData[0].X * (m_FaultData[i].UData[1].Y - m_FaultData[i].TData[1].Y)
			+ m_FaultData[i].TData[1].X * (m_FaultData[i].TData[0].Y - m_FaultData[i].UData[1].Y)
			+ m_FaultData[i].UData[1].X * (m_FaultData[i].TData[1].Y - m_FaultData[i].TData[0].Y);
		//�ϲ�������������,ԭʼ����,�洢�������
		//���� ���ϲ��������������ֵ����ֵ
		FaultWires(i, suV, faultTemp, Position);
		//�������ϲ�Ӱ�����ɢ������ֵ���� Ϊ�ϲ�Ӱ�������ֵ
		for (int j = 0; j < (int)suV.size(); j++)
			faultSu[j].Z = faultSu[j].Z + faultTemp[j].Z;

		faultTemp.clear();
	}
	//��������ֵ ������ɢ������ֵΪû�жϲ�ǰ������ֵ
	for (int i = 0; i < (int)suV.size(); i++)
		suV[i].Z = suV[i].Z - faultSu[i].Z;

	faultSu.clear();
	faultSu.resize(XM);

	//������ϲ�Ӱ��� ���нǵ���������������ֵ  ����ͬ��
	vector<vector<double>> faultT(m_GridPoint.size());
	vector<vector<double>> faultS(XM + 1);

	for (int i = 0; i < (int)m_FaultData.size(); i++)
	{
		faultT.resize(m_GridPoint.size());
		//��ȡ�ϲ����� �еĵ����ϲ�����
		Position = m_FaultData[i].TData[0].X * (m_FaultData[i].UData[1].Y - m_FaultData[i].TData[1].Y)
			+ m_FaultData[i].TData[1].X * (m_FaultData[i].TData[0].Y - m_FaultData[i].UData[1].Y)
			+ m_FaultData[i].UData[1].X * (m_FaultData[i].TData[1].Y - m_FaultData[i].TData[0].Y);
		//����˶ϲ�Ӱ��Ľǵ���������ֵ ������ֵ
		ReviseWires(i, faultT, (int)Position);
		//�� �ϲ�Ӱ��ǵ�����������ֵ ����
		for (int k = 1; k <= XM; k++)
		{
			faultS[k].resize(YM + 1);
			for (int j = 1; j <= YM; j++)
			{
				faultS[k][j] = faultS[k][j] + faultT[k][j];
			}
		}
		faultT.clear();
	}
	TWOVALUE D;
	m_suV = suV;
	for (int i = 1; i <= m_XNum; i++)
	{
		for (int j = 1; j <= m_YNum; j++)
		{
			D.X = m_GridPoint[i][j].X;
			D.Y = m_GridPoint[i][j].Y;
			//���ƴ˵������ֵ    û�жϲ�Ĺ���ֵ + ����ֵ
			//m_GridPoint[i][j].Z = InsertEst(Suv, D, ni);//ȫ����λ��Ŀ�����ֵ
			//m_GridPoint[i][j].Z = Well_Near_K(D);		//�ھ���Χ�ڵĿ�����ֵ
			//m_GridPoint[i][j].Z = DisInv(D);
			m_GridPoint[i][j].Z = DisInv(D)+ faultS[i][j];			//�ھ���Χ�ڵľ��뷴�ȹ�ֵ
			//EvaluateFault(m_GridPoint[i][j]);			//��Ӷϲ��ֵ
			if (m_ZMin > m_GridPoint[i][j].Z)
			{
				//cout << "���ֵ�����˸���" << endl;
				m_ZMin = m_GridPoint[i][j].Z;
			}

			if (m_ZMax < m_GridPoint[i][j].Z)
			{
				//cout << "��Сֵ�����˸���" << endl;
				m_ZMax = m_GridPoint[i][j].Z;
			}
		}
	}
}

//���ϲ��Ĺ�ֵ
void Jie_Ds::EvaluateFault()
{
	if ((int)m_oriData.size() < 800)//800�ھ�����ʹ�ÿ���𷽷����ٶ�4s���ҿ��Խ���
	{
		m_IsK = true;				//��������ʹ�ÿ�����ֵ
	}
	else
	{
		m_IsK = false;				//����̫��ʹ���ݾ��뷴�ȹ�ֵ
	}
	if (m_IsK)
	{
		FaultKriking();		//�����ϲ��ֵ
	}
	else
	{
		FaultInvDis();		//���뷴�ȹ�ֵ
	}
}

//���Ƿ��������ڲ�
BOOL Jie_Ds::ISInside(TWOVALUE A, TWOVALUE GridIJ[5])
{
	double x = A.X;
	double y = A.Y;
	//ͨ������жϵ��Ƿ�������i,j����
	//double xmax = A.X;
	//double xmin = A.X;
	//double ymax = A.Y;
	//double ymin = A.Y;
	//for (int i = 0 ; i < 4 ; i ++)
	//{
	//	double tx = GridIJ[i].X;
	//	double ty = GridIJ[i].Y;
	//	if (tx > xmax)
	//	{
	//		xmax = tx;
	//	}
	//	else if (tx < xmin)
	//	{
	//		xmin = tx
	//	}
	//}
	double x1 = GridIJ[0].X - A.X;
	double x2 = GridIJ[1].X - A.X;
	double x3 = GridIJ[2].X - A.X;
	double x4 = GridIJ[3].X - A.X;

	double y1 = GridIJ[0].Y - A.Y;
	double y2 = GridIJ[1].Y - A.Y;
	double y3 = GridIJ[2].Y - A.Y;
	double y4 = GridIJ[3].Y - A.Y;


	double s1 = abs((y4 - y1)*x2 - (x4 - x1)*y2 + y1*x4 - y4*x1);
	double s2 = abs((y4 - y3)*x2 - (x4 - x3)*y2 + y3*x4 - y4*x3);
	double s = s1 + s2;

	x = 0;
	y = 0;

	double ss1 = abs((y2 - y1)*x - (x2 - x1)*y + y1*x2 - y2*x1);
	double ss2 = abs((y3 - y2)*x - (x3 - x2)*y + y2*x3 - y3*x2);
	double ss3 = abs((y3 - y4)*x - (x3 - x4)*y + y4*x3 - y3*x4);
	double ss4 = abs((y1 - y4)*x - (x1 - x4)*y + y4*x1 - y1*x4);
	double ss = ss1 + ss2 + ss3 + ss4;

	double d = abs(s - ss);
	if (d<0.00000000001)
	{
		return TRUE;
	}
	return FALSE;
}

//�Խ����������
void Jie_Ds::Sort(vector<double>&Mark)
{
	vector<double>NewMark;
	for (int i = 0; i < (int)Mark.size() - 1; i++)
	{
		for (int j = i + 1; j < (int)Mark.size(); j++)
		{
			if (Mark[i]>Mark[j])
			{
				double midd = Mark[j];
				Mark[j] = Mark[i];
				Mark[i] = midd;
			}
		}
	}
}


//ͨ�������õ���Point���ڵ�����ֵ 20150323
void Jie_Ds::GetValueByK(THRVALUE &Point)
{
	if (m_IsK)
	{
		TWOVALUE D;
		D.X = Point.X;
		D.Y = Point.Y;
		Point.Z = InsertEst(m_suV, D, m_ni) /*+ Point.Z*/;//���ÿ�����ֵ
		EvaluateFault(Point);							//��Ӷϲ��ֵ
	}
	else
	{
		TWOVALUE D;
		D.X = Point.X;
		D.Y = Point.Y;
		//Point.Z = InsertEst(m_suV, D, m_ni) + Point.Z;//20140813,
		Point.Z = DisInv(D);                             //ʹ������ľ��뷴�ȷ�������ֵ
		EvaluateFault(Point);							//��Ӷϲ��ֵ
	}
}

//�õ���������ʵ������
void Jie_Ds::GetFulatInsidePoint(vector<double>Mark, FalutLine LineOne, GridFL &AllLine, TWOVALUE GridIJ[5])
{
	for (int i = 0; i < (int)Mark.size() - 1; i = i + 2)
	{
		SingleFalut Line;
		if ((int)Mark[i] == (int)Mark[i + 1])//������֮��û�жϲ�㣬������������ڶϲ����ϵ�������ֱ�Ӵ������������
		{
			int Star = (int)Mark[i];
			int End = (int)Mark[i] + 1;
			TWOVALUE SP = LineOne.FLine[Star];
			TWOVALUE EP = LineOne.FLine[End];
			double dx = EP.X - SP.X;
			double dy = EP.Y - SP.Y;
			double t2 = Mark[i + 1] - Star;
			double t1 = Mark[i] - Star;
			/*--------------------------------------------*/
			//20140915
			//TWOVALUE t1p;
			//t1p.X = SP.X + t1*dx;
			//t1p.Y = SP.Y + t1*dy;

			//TWOVALUE t2p;
			//t2p.X = SP.X + t2*dx;
			//t2p.Y = SP.Y + t2*dy;
			THRVALUE t1p;
			t1p.X = SP.X + t1*dx;
			t1p.Y = SP.Y + t1*dy;
			t1p.Z = 0.0;
			GetValueByK(t1p);
			THRVALUE t2p;
			t2p.X = SP.X + t2*dx;
			t2p.Y = SP.Y + t2*dy;
			t2p.Z = 0.0;
			GetValueByK(t2p);
			/*--------------------------------------------*/
			Line.FLine.push_back(t1p);
			Line.FLine.push_back(t2p);
		}
		else							//������֮����ڶϲ��
		{
			int Star = (int)Mark[i];
			int End = (int)Mark[i] + 1;
			TWOVALUE SP = LineOne.FLine[Star];
			TWOVALUE EP = LineOne.FLine[End];
			double dx = EP.X - SP.X;
			double dy = EP.Y - SP.Y;
			double t1 = Mark[i] - Star;
			//double t2 = Mark[i+1]-Star;
			/*--------------------------------------------*/
			//20140915
			//TWOVALUE t1p;
			//t1p.X = SP.X + t1*dx;
			//t1p.Y = SP.Y + t1*dy;
			THRVALUE t1p;
			t1p.X = SP.X + t1*dx;
			t1p.Y = SP.Y + t1*dy;
			t1p.Z = 0.0;
			GetValueByK(t1p);
			/*--------------------------------------------*/
			Line.FLine.push_back(t1p);
			for (int h = End; h <= (int)Mark[i + 1]; h++)
			{
				TWOVALUE A = LineOne.FLine[h];
				if (!ISInside(A, GridIJ))
				{
					AllLine.FLines.push_back(Line);//Ԥ����ĩ���ڶϲ��ϵ����20130816,
					break;
				}
				/*--------------------------------------------*/
				//20140915
				THRVALUE B;
				B.X = A.X;
				B.Y = A.Y;
				B.Z = 0.0;
				GetValueByK(B);
				//Line.FLine.push_back(A);
				Line.FLine.push_back(B);
				/*--------------------------------------------*/
			}
			if ((int)Mark[i + 1] == (int)LineOne.FLine.size() - 1)//��ֹĩ��
			{
				AllLine.FLines.push_back(Line);
				return;
			}
			Star = (int)Mark[i + 1];
			End = (int)Mark[i + 1] + 1;
			SP = LineOne.FLine[Star];
			EP = LineOne.FLine[End];
			dx = EP.X - SP.X;
			dy = EP.Y - SP.Y;
			t1 = Mark[i + 1] - Star;
			t1p.X = SP.X + t1*dx;
			t1p.Y = SP.Y + t1*dy;

			/*--------------------------------------------*/
			//20140915
			t1p.Z = 0.0;
			GetValueByK(t1p);
			/*--------------------------------------------*/
			Line.FLine.push_back(t1p);
		}
		AllLine.FLines.push_back(Line);//���öϲ��߶μ���
	}
}


//�õ������ڲ�һ���ϲ��ߵ���Ϣ
void Jie_Ds::GetOneCross(int IndexI, int IndexJ, TWOVALUE GridIJ[5], FalutLine LineOne, GridFL &AllLine)
{
	vector<double>mark;
	int NumLineOne = LineOne.FLine.size();
	int Star = 0;
	if (ISInside(LineOne.FLine[0], GridIJ))	//����׵��������ڣ�����룬��������������Ҳ�����ڲ�
	{
		//cout << "hello" << endl;
		//cout << "v " << LineOne.FLine[1].X << " " << LineOne.FLine[1].Y << " " << "0" << endl;
		//mark.push_back(0.0);             //20130917
		Star = 1;
		//��Ϊ�׵��������ڣ���ôһ������ȷ������Ϊ0-1,0(size()-1)��size()-2���ɵ��߶ο��ܶ���������н��㣬���Լ���ڶ�����
		LineOne.FLine.push_back(LineOne.FLine[1]);//���ڶ�������Ϊ���һ���㣬�غ�0-1���֡�~ʹ����β�����ڵ��߶�Ϊһ������������߶�
	}
	for (int i = Star/*0*/; i < (int)LineOne.FLine.size() - 1; i++)  //20130918
	{
		TWOVALUE StarA = LineOne.FLine[i];
		TWOVALUE EndA = LineOne.FLine[i + 1];
		double Index = i;
		for (int j = 0; j < 4; j++)
		{
			TWOVALUE StarB = GridIJ[j];
			TWOVALUE EndB = GridIJ[j + 1];
			if (ISIntersect(StarA, EndA, StarB, EndB, Index))
			{
				/*--------------------------------------*/
				//20140916,�ڴ˺���������������ϵĶϲ��
				THRVALUE MidPt;
				double t = Index - i;
				MidPt.X = StarA.X + (EndA.X - StarA.X) * t;
				MidPt.Y = StarA.Y + (EndA.Y - StarA.Y) * t;
				MidPt.Z = 0.0;
				GetValueByK(MidPt);
				/*--------------------------------------*/
				mark.push_back(Index);
				Index = i;
				//�洢�ϲ��Ƿ��з�����ߡ��������ڳ�ʼ��ֵ��ѡ�񡤡�
				//��ʼ��ֵ��ѡ����ѡ�񱻶ϲ�ͨ���ıߡ����ϲ��������ݲ������Ե�
				if (j == 0)
				{
					m_XFault[IndexI][IndexJ] = TRUE;
					//20140916  �������ϵĶϲ��
					m_XFaultPts[IndexI][IndexJ].FLine.push_back(MidPt);
				}
				else if (j == 1)
				{
					m_YFault[IndexI + 1][IndexJ] = TRUE;
					//20140916
					m_YFaultPts[IndexI + 1][IndexJ].FLine.push_back(MidPt);
				}
				else if (j == 2)
				{
					m_XFault[IndexI][IndexJ + 1] = TRUE;
					//20140916
					m_XFaultPts[IndexI][IndexJ + 1].FLine.push_back(MidPt);
				}
				else
				{
					m_YFault[IndexI][IndexJ] = TRUE;
					//20140916
					m_YFaultPts[IndexI][IndexJ].FLine.push_back(MidPt);
				}
			}
		}
		/////*if (ISInside(StarA,GridIJ))
		////{
		////	mark.push_back(i);
		////}*/
	}
	if (ISInside(LineOne.FLine[NumLineOne - 1], GridIJ))	//��ĩ���������ڣ������
	{
		//mark.push_back(NumLineOne-1);					//20130918
	}
	if (mark.size() <1)//��ֹ�ϲ��߸�������ཻ����Ϊ�ϲ��ߺ������ཻ�Ļ���Ӧ����������������
	{
		return;
	}
	Sort(mark);//mark֮�ڴ洢���Ƕϲ����ϵ��������ֵ
	GetFulatInsidePoint(mark, LineOne, AllLine, GridIJ);//�õ������ڵĶϲ���Լ��ϲ������Ľ�����Ϣ
														//if (mark.size()>1)
														//{
														//	m_NoFault[IndexI][IndexJ]
														//}
}

//�õ������ڲ��Ľ�����Ϣ
void Jie_Ds::GetAllCross(int IndexI, int IndexJ, TWOVALUE GridIJ[5], GridFL &AllLine)
{
	for (int i = 0; i < (int)m_vecFaultLines.size(); i++)
	{
		FalutLine LineOne = m_vecFaultLines[i];
		GetOneCross(IndexI, IndexJ, GridIJ, LineOne, AllLine);	//�ж�����IndexI,IndexJ�ڲ��Ƿ����i���ϲ����ཻ
	}
}

//����һ���ϲ��������
void Jie_Ds::SetOneFault(int IndexI, int IndexJ, GridFL &ALLLine)
{
	TWOVALUE GridIJ[5];//˳ʱ��洢����β����,���������������ֵ
	GridIJ[0].X = m_GridPoint[IndexI][IndexJ].X;
	GridIJ[1].X = m_GridPoint[IndexI + 1][IndexJ].X;
	GridIJ[2].X = m_GridPoint[IndexI + 1][IndexJ + 1].X;
	GridIJ[3].X = m_GridPoint[IndexI][IndexJ + 1].X;
	GridIJ[4].X = m_GridPoint[IndexI][IndexJ].X;

	GridIJ[0].Y = m_GridPoint[IndexI][IndexJ].Y;
	GridIJ[1].Y = m_GridPoint[IndexI + 1][IndexJ].Y;
	GridIJ[2].Y = m_GridPoint[IndexI + 1][IndexJ + 1].Y;
	GridIJ[3].Y = m_GridPoint[IndexI][IndexJ + 1].Y;
	GridIJ[4].Y = m_GridPoint[IndexI][IndexJ].Y;
	GetAllCross(IndexI, IndexJ, GridIJ, ALLLine);		//�õ����жϲ���
}

//��A�Ƿ��ڶϲ�Line�ڲ�
BOOL Jie_Ds::IsInside(TWOVALUE A, FalutLine Line)
{
	double Xmax = Line.FLine[0].X;
	double Xmin = Line.FLine[0].X;
	double Ymax = Line.FLine[0].Y;
	double Ymin = Line.FLine[0].Y;
	for (int i = 1; i < (int)Line.FLine.size(); i++)
	{
		if (Line.FLine[i].X > Xmax)
		{
			Xmax = Line.FLine[i].X;
		}
		else if (Line.FLine[i].X < Xmin)
		{
			Xmin = Line.FLine[i].X;
		}

		if (Line.FLine[i].Y > Ymax)
		{
			Ymax = Line.FLine[i].Y;
		}
		else if (Line.FLine[i].Y < Ymin)
		{
			Ymin = Line.FLine[i].Y;
		}
	}
	if (A.X > Xmax || A.X <Xmin || A.Y>Ymax || A.Y < Ymin)
	{
		return FALSE;
	}
	//�����ж��Ƿ����ڲ�
	TWOVALUE B = A;
	//B.X = m_XMin - 200.0;     //AB����
	B.X = Xmin - 200.0;     //AB���� 20140805,
	B.Y = Ymin - 200.0;     //AB���� 20140805,
	int Sum = 0;
	for (int i = 0; i < (int)Line.FLine.size() - 1; i++)
	{
		double t = 0;
		TWOVALUE LineA = Line.FLine[i];
		TWOVALUE LineB = Line.FLine[i + 1];
		if (ISIntersect(LineA, LineB, A, B, t))
		{
			//if (t<0.00000001)//��ֹ���׵��ཻ�������ڶ˵��ϣ�ֻ����һ����β���ϵ�
			//{
			//	continue;
			//}
			Sum = Sum + 1;
		}
	}
	if (Sum % 2 == 1)//����Ϊ�����������ڲ�
	{
		return TRUE;
	}
	return FALSE;

}
//��A�Ƿ��ڶϲ��ڲ�
BOOL Jie_Ds::IsInside(TWOVALUE RealyPoint)
{
	for (int i = 0; i < (int)m_vecFaultLines.size(); i++)
	{
		FalutLine Line = m_vecFaultLines[i];
		if (Line.IsClose)
		{
			if (IsInside(RealyPoint, Line))
			{
				return TRUE;
			}
		}
	}
	return FALSE;

}
//�ж϶ϲ������ߵ�������Ƿ��ڶϲ��ڲ�20140916
void Jie_Ds::JudgeFaultGridIn()
{
	TWOVALUE A, B;
	for (int j = 1; j <= m_YNum; j++)			//����ϵ������
	{
		for (int i = 1; i < m_XNum; i++)
		{
			if (m_XFault[i][j])//�˺���������жϲ���ͨ��
			{
				//����Ҫ���һ�㣬m_GridPoint��һ�������±꣬�ڶ��������±�
				A.X = m_GridPoint[i][j].X;
				A.Y = m_GridPoint[i][j].Y;

				B.X = m_GridPoint[i + 1][j].X;
				B.Y = m_GridPoint[i + 1][j].Y;

				if (IsInside(A))
				{
					m_GridInFault[i][j] = TRUE;		//i,j������ڶϲ���
				}
				else if (IsInside(B))
				{
					m_GridInFault[i + 1][j] = TRUE;	//i + 1,j������ڶϲ���
				}
			}
		}
	}

	for (int i = 1; i <= m_XNum; i++)			//
	{
		for (int j = 1; j < m_YNum; j++)
		{
			if (m_YFault[i][j])
			{
				A.X = m_GridPoint[i][j].X;
				A.Y = m_GridPoint[i][j].Y;

				B.X = m_GridPoint[i][j + 1].X;
				B.Y = m_GridPoint[i][j + 1].Y;
				if (IsInside(A))
				{
					m_GridInFault[i][j] = TRUE;		//i,j������ڶϲ���
				}
				else if (IsInside(B))
				{
					m_GridInFault[i][j + 1] = TRUE;	//i,j + 1������ڶϲ���
				}
			}
		}
	}
	
	for (int i = 1; i <= m_XNum; i++)			
	{
		for (int j = 1; j <= m_YNum; j++)
		{
				A.X = m_GridPoint[i][j].X;
				A.Y = m_GridPoint[i][j].Y;
				if (IsInside(A))
				{
					m_GridInFault[i][j] = TRUE;		//i,j������ڶϲ���
					if (m_XFault[i][j] != true)
						m_XFault[i][j] = true;
					if (m_YFault[i][j] != true)
						m_YFault[i][j] = true;
				}
		}
	}
	std::ofstream out("data//fault_test//pinfault.obj");
	for (int i = 0; i < m_XNum; i++)
	{
		for (int j = 0; j < m_YNum; j++)
		{
			if (m_GridInFault[i][j] == TRUE)
				out << "v " << fixed << setprecision(5)<< m_GridPoint[i][j].X << " " << fixed << setprecision(5)<<m_GridPoint[i][j].Y << " " << "0" << endl;
		}
	}

}
//���ϲ�������Ľ����������ϲ���������
void Jie_Ds::SetNewFault()
{
	m_NoFault.resize(m_XNum + 3);				//�������Ƿ��жϲ�ı�� //�ж�ÿ���������Ƿ��жϲ㣬����洢�ϲ��ߵ����
	m_XFault.resize(m_XNum + 3);				//X������Ƿ��жϲ�ı��20130913
	m_YFault.resize(m_XNum + 3);				//Y������Ƿ��жϲ�ı��
	m_XFaultPts.resize(m_XNum + 3);			//�洢ÿ��X�������Ƕϲ�����20140916
	m_YFaultPts.resize(m_XNum + 3);			//�洢ÿ��Y�������Ƕϲ�����20140916
	m_GridInFault.resize(m_XNum + 3);			//�洢ÿ��������Ƿ��ڶϲ���20140916
	for (int i = 0; i < m_XNum + 3; i++)
	{
		m_NoFault[i].resize(m_YNum + 3);

		m_XFault[i].resize(m_YNum + 3);		//X������Ƿ��жϲ�ı��
		m_YFault[i].resize(m_YNum + 3);		//Y������Ƿ��жϲ�ı��
		m_XFaultPts[i].resize(m_XNum + 3);	//�洢ÿ��X�������Ƕϲ�����20140916
		m_YFaultPts[i].resize(m_XNum + 3);	//�洢ÿ��Y�������Ƕϲ�����20140916
		m_GridInFault[i].resize(m_XNum + 3);	//�洢ÿ��������Ƿ��ڶϲ���20140916
		for (int j = 0; j < m_YNum + 3; j++)
		{
			m_NoFault[i][j] = -1;		//��ʾ�����ϲ�
			m_XFault[i][j] = FALSE;	//��ʾ�����ϲ�
			m_YFault[i][j] = FALSE;	//��ʾ�����ϲ�			20130913
			m_GridInFault[i][j] = FALSE;	//��ʾ����㲻�ڶϲ���	20140916
		}
	}

	for (int i = 1; i < m_XNum; i++)
	{
		for (int j = 1; j < m_YNum; j++)
		{
			GridFL AllLine;
			SetOneFault(i, j, AllLine);		//�ж�����i,j���Ƿ��жϲ���
			if ((int)AllLine.FLines.size()>0)
			{
				m_vecGridFL.push_back(AllLine);//����Ӧע�⣬ÿһ�������ڿ��ܴ洢�����ϲ���
				m_NoFault[i][j] = m_vecGridFL.size() - 1;
			}
		}
	}
	//cout << m_vecGridFL.size() << endl;
	/*std::ofstream out("data//fault//test.obj");
	int k = 2;
	cout << m_vecGridFL[k].FLines.size() << endl;
	for (int i = 0; i < m_vecGridFL[k].FLines.size(); i++)
		for (int j = 0; j < m_vecGridFL[k].FLines[i].FLine.size(); j++)
			out << "v " << m_vecGridFL[k].FLines[i].FLine[j].X << " " << m_vecGridFL[k].FLines[i].FLine[j].Y << " " << "0" << endl;
	out.close();*/
	//20140916  �ж���ϲ��ཻ���������ϵ�������Ƿ��ڶϲ���
	JudgeFaultGridIn();
}

void Jie_Ds::GetMinD(vector<double> CosS, vector<double> DisT, int &index)
{
	double MinCos = CosS[0];
	index = 0;
	for (int i = 0; i < (int)CosS.size(); i++)
	{
		if (CosS[i] < MinCos)
		{
			MinCos = CosS[i];
			index = i;
		}
		else if (CosS[i] == MinCos && DisT[i] >= DisT[index])
		{
			index = i;
		}
	}
}
vector<Data> Jie_Ds::Withershins(vector<Data> m_point)
{

	Data Temppoint;
	int Numindex = (int)m_point.size();
	int Num = (int)m_point.size();
	Temppoint = m_point[0];
	vector<Data> Point;
	int StarIndex = 0;
	//ѡ����С��x,����y��Ϊ��ʼ��,�洢��m_point[0]��
	for (int i = 1; i< (int)m_point.size(); i++)
	{
		if (Temppoint.X == m_point[i].X && Temppoint.Y < m_point[i].Y)
		{
			Temppoint = m_point[i];
			StarIndex = i;
		}
		else if (Temppoint.X > m_point[i].X)
		{
			Temppoint = m_point[i];
			StarIndex = i;
		}
	}
	//�������е�͸õ���ɵ������ͳ�ʼ������pt = ptStartA-ptEndA����ʹ�ø�������xֵΪ��ֵ��֮��ĵļнǣ�ȡһ�����ļн�(��С��cos x = (a*b)/(|a|*|b|))
	//����ĳ�ʼ����ָ��Ӧ����ȡһ��y����������������ɣ�����˵��0,1��
	Point.push_back(Temppoint);
	m_point[StarIndex].mark = 1;	//��ʹ�ù�
	if ((int)m_point.size() == 1)
	{
		return Point;		//��������λ��ֵ 20131108
	}
	vector<double> CosS, NiCosS, Dis, NiDis;
	vector<int> IndexS, NiIndexS;
	//�ҵ��ڶ����㣬����ŵ�m_point[1];
	for (int i = 0; i< Num; i++)
	{
		if (m_point[i].mark == 1)
		{
			continue;	//��ʹ�ù�
		}
		double x1 = 0.0;
		double y1 = 1.0;
		double x2 = m_point[i].X - Point[0].X;
		double y2 = m_point[i].Y - Point[0].Y;
		double d = -x2;		//��˽����������Ҫ���ݷ������ж�(x2,y2)�����ɵ�������(x1,y1)������������˳ʱ�뻹����ʱ�뷽��
							//��С��0������˳ʱ�뷽��������0��������ʱ�뷽��������0������

		double d1 = sqrt(pow(x1, 2) + pow(y1, 2));
		double d2 = sqrt(pow(x2, 2) + pow(y2, 2));

		if (d1 == 0 || d2 == 0)
		{
			continue;
		}

		double cosa = (x1*x2 + y1*y2) / (d1*d2);//����Ӧ���Ǵ���������ֱ���ϵ������нǵ�����ֵ
		if (d >= 0)
		{
			NiCosS.push_back(cosa);
			NiIndexS.push_back(i);
			NiDis.push_back(d2);
		}
		else
		{
			CosS.push_back(cosa);
			IndexS.push_back(i);
			Dis.push_back(d2);	//�洢����
		}
	}
	if ((int)NiCosS.size() >= 1)
	{
		int Good;
		GetMinD(NiCosS, NiDis, Good);
		Good = NiIndexS[Good];
		Temppoint = m_point[Good];
		m_point[Good].mark = 1;
	}
	else
	{
		int Good;
		GetMinD(CosS, Dis, Good);
		Good = IndexS[Good];
		Temppoint = m_point[Good];
		m_point[Good].mark = 1;
	}
	Point.push_back(Temppoint);
	m_point[StarIndex].mark = 0;	//ɾ���׵㱻ʹ�úۼ�
	for (int j = 2; j < Num + 1; j++)
	{
		NiIndexS.clear(); IndexS.clear();
		NiDis.clear();	  Dis.clear();
		NiCosS.clear();	  CosS.clear();
		for (int i = 0; i < Num; i++)
		{
			if (m_point[i].mark == 1)
			{
				continue;
			}
			double x2 = m_point[i].X - Point[j - 1].X;
			double y2 = m_point[i].Y - Point[j - 1].Y;
			double x1 = Point[j - 1].X - Point[j - 2].X;
			double y1 = Point[j - 1].Y - Point[j - 2].Y;
			double d = x1 * y2 - y1 * x2;		//��˽��
			double d1 = sqrt(pow(x1, 2) + pow(y1, 2));
			double d2 = sqrt(pow(x2, 2) + pow(y2, 2));
			if (d1 == 0 || d2 == 0)
			{
				continue;
			}
			double cosa = (-x1*x2 - y1*y2) / (d1*d2);
			if (d > 0)
			{
				NiCosS.push_back(cosa);
				NiIndexS.push_back(i);
				NiDis.push_back(d2);
			}
		}
		if ((int)NiCosS.size() >= 1)
		{
			int Good;
			GetMinD(NiCosS, NiDis, Good);
			Good = NiIndexS[Good];
			Temppoint = m_point[Good];
			m_point[Good].mark = 1;
		}
		else
		{
			return Point;
		}
		Point.push_back(Temppoint);
		if (Point[j].X == Point[0].X && Point[j].Y == Point[0].Y)
		{
			return Point;
		}
	}
	return Point;
}
vector<Data> Jie_Ds::LoadModel(const char* sFileName)
{
	vector<Data> Convexhull;
	std::ifstream in(sFileName);
	std::ifstream in2(sFileName);
	if (!in)
	{
		std::cout << "error of load mesh" << std::endl;
	}
	string str;
	Vector3D point2;

	in >> str;
	if (str == "v")
	{
		Data Points;
		in >> point2.x >> point2.y>>point2.z;
		//cout << point2.x << " " << point2.y << " "<<point2.z<<endl;
		Points.X = point2.x;
		Points.Y = point2.y;
		Points.Z = point2.z;
		Convexhull.push_back(Points);
	}
	while (getline(in, str))
		//while (!in.eof())
	{
		in >> str;
		if (str == "v")
		{
			Data Points;
			in >> point2.x >> point2.y>>point2.z;
			//cout << point2.x << " " << point2.y << " "<<point2.z<<endl;
			Points.X = point2.x;
			Points.Y = point2.y;
			Points.Z = point2.z;
			Convexhull.push_back(Points);
		}

	}
	return Convexhull;
}
void Jie_Ds::Charact()	
{
	double XMin = m_oriData[0].X;//m_oriData�д洢ԭʼ���ݣ���λ��Ϣ
	double XMax = m_oriData[0].X;
	double YMin = m_oriData[0].Y;
	double YMax = m_oriData[0].Y;
	double ZMin = m_oriData[0].Z;
	double ZMax = m_oriData[0].Z;
	for (size_t i = 1; i<m_oriData.size(); i++)
	{
		if (XMin > m_oriData[i].X)
			XMin = m_oriData[i].X;

		if (XMax < m_oriData[i].X)
			XMax = m_oriData[i].X;

		if (YMin > m_oriData[i].Y)
			YMin = m_oriData[i].Y;

		if (YMax < m_oriData[i].Y)
			YMax = m_oriData[i].Y;

		if (ZMin > m_oriData[i].Z)
			ZMin = m_oriData[i].Z;

		if (ZMax < m_oriData[i].Z)
			ZMax = m_oriData[i].Z;
	}

	/*************************************************/
	//�Ƿ�Ҫ�ж�m_boder�����ݣ�20131112  ֮ǰ�Ѿ�����ôһ���ĸ�ֵ������m_OriBoder = m_Border;
	for (size_t i = 0; i<m_Border.size(); i++)
	{
		if (XMin > m_Border[i].X)
			XMin = m_Border[i].X;

		if (XMax < m_Border[i].X)
			XMax = m_Border[i].X;

		if (YMin > m_Border[i].Y)
			YMin = m_Border[i].Y;

		if (YMax < m_Border[i].Y)
			YMax = m_Border[i].Y;
	}
	/*************************************************/

	m_XMin = XMin;
	m_XMax = XMax;
	m_YMin = YMin;
	m_YMax = YMax;
	m_ZMin = ZMin;
	m_ZMax = ZMax;
}
void Jie_Ds::DataOpt()
{
	vector<Data> datatemp;
	for (int i = 0; i < m_oriData.size(); i++)
	{
		Data p;
		p.X = m_oriData[i].X-m_XMin;
		p.Y = m_oriData[i].Y-m_YMin;
		p.Z = m_oriData[i].Z;
		datatemp.push_back(p);
	}
	m_oriData.clear();
	m_oriData = datatemp;
}
void Jie_Ds::DataRec()
{
	vector<Data> datatemp;
	for (int i = 0; i < m_oriData.size(); i++)
	{
		Data p;
		p.X = m_oriData[i].X + m_XMin;
		p.Y = m_oriData[i].Y + m_YMin;
		p.Z = m_oriData[i].Z;
		datatemp.push_back(p);
	}
	m_oriData.clear();
	m_oriData = datatemp;
}

void Jie_Ds::CalcBorder()
{
	if (m_Border.size() < 3) // û�����ñ߽磬����һ���߽�
	{
		Data dt;
		dt.X = m_XMin; // ����
		dt.Y = m_YMax;
		m_Border.push_back(dt);

		dt.X = m_XMax; // ����
		dt.Y = m_YMax;
		m_Border.push_back(dt);

		dt.X = m_XMax; // ����
		dt.Y = m_YMin;
		m_Border.push_back(dt);

		dt.X = m_XMin; // ����
		dt.Y = m_YMin;
		m_Border.push_back(dt);
	}


	//m_Border.clear();//20131111
	//m_Border.resize(0);

	//for (int i = 0; i< m_Border.size(); i++)
	//{
	//	m_Border.push_back(m_Border[i]); // ����߽�
	//}

}
double Jie_Ds::GetDis(double x1, double y1, double x2, double y2)
{
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*((y1 - y2)));
}
void Jie_Ds::AddPt(vector<Data> &convexBag)
{
	int Num = (int)convexBag.size();
	double MaxD = 0;
	int Index = 0;
	for (int i = 0; i < Num - 1; i++)
	{
		Data Star = convexBag[i];
		Data End = convexBag[i + 1];
		double d = pow(Star.X - End.X, 2) + pow(Star.Y - End.Y, 2);
		if (d>MaxD)
		{
			MaxD = d;
			Index = i;
		}
	}
	Data Middle;
	Middle.X = 0.5*(convexBag[Index].X + convexBag[Index + 1].X);
	Middle.Y = 0.5*(convexBag[Index].Y + convexBag[Index + 1].Y);
	convexBag.insert(convexBag.begin() + Index + 1, Middle);
}
void Jie_Ds::deCasteljau()
{
	vector<point> bezierpoint;
	int n = control_point.size();

	for (double t = 0.0; t <= 1.0; t += 0.001 / n)//���������
	{
		for (int i = 1; i < n; i++)
		{
			for (int j = 0; j < n - i; j++)
			{
				if (i == 1)//i=1����֪���ƶ������
				{
					control_point[j].x = (1 - t) * input_vertice[j].x + t * input_vertice[j + 1].x;
					control_point[j].y = (1 - t) * input_vertice[j].y + t * input_vertice[j + 1].y;
					continue;
				}
				else//i!=1����һ�ε����������
				{
					control_point[j].x = (1 - t) * control_point[j].x + t * control_point[j + 1].x;
					control_point[j].y = (1 - t) * control_point[j].y + t * control_point[j + 1].y;
				}
			}
		}
		b_spline.push_back(control_point[0]);
	}
}
void Jie_Ds::deBoor()
{
	float t[maxn];
	int k = 3;
	int n = control_point.size() - 1;
	//׼����B����
	for (int i = 0; i < n + k; i++)
	{
		if (i <= k - 1) t[i] = 0;
		if (i >= k && i < n + 1) t[i] = t[i - 1] + 1.0 / (n - k + 2);
		if (i >= n + 1) t[i] = 1;
	}
	for (int j = k - 1; j <= n; j++)
	{
		for (double u = t[j]; u <= t[j + 1]; u += 0.02 / n)
		{
			for (int r = 1; r <= k - 1; r++)
			{
				for (int i = j; i >= j - k + r + 1; i--)
				{
					float x1 = u - t[i];
					float x2 = t[i + k - r] - t[i];
					float y1 = t[i + k - r] - u;
					float k1, k2;
					if (x1 == 0.0 && x2 == 0.0) k1 = 0;
					else k1 = x1 / x2;
					if (y1 == 0.0 && x2 == 0.0) k2 = 0;
					else k2 = y1 / x2;

					if (r == 1)//���Ƶ�
					{
						control_point[i].x = input_vertice[i].x * k1 + input_vertice[i - 1].x * k2;
						control_point[i].y = input_vertice[i].y * k1 + input_vertice[i - 1].y * k2;
						continue;
					}
					else
					{
						control_point[i].x = control_point[i].x * k1 + control_point[i - 1].x * k2;
						control_point[i].y = control_point[i].y * k1 + control_point[i - 1].y * k2;
					}
				}
			}
			b_spline.push_back(control_point[j]);//���Ƶ����һ��ĵ㣬��Ϊ��õĵ�
		}
	}


}
double Jie_Ds::bezier3funcX(double uu, Vector2D *controlP) {
	double part0 = controlP[0].x * uu * uu * uu;
	double part1 = 3 * controlP[1].x * uu * uu * (1 - uu);
	double part2 = 3 * controlP[2].x * uu * (1 - uu) * (1 - uu);
	double part3 = controlP[3].x * (1 - uu) * (1 - uu) * (1 - uu);
	return part0 + part1 + part2 + part3;
}
double Jie_Ds::bezier3funcY(double uu, Vector2D *controlP) {
	double part0 = controlP[0].y * uu * uu * uu;
	double part1 = 3 * controlP[1].y * uu * uu * (1 - uu);
	double part2 = 3 * controlP[2].y * uu * (1 - uu) * (1 - uu);
	double part3 = controlP[3].y * (1 - uu) * (1 - uu) * (1 - uu);
	return part0 + part1 + part2 + part3;
}
void Jie_Ds::createCurve()
{
	double scale = 0.3;
	int count = originPoint.size();
	//CvPoint midpoints[count];
	vector<Vector2D> midpoints;
	//�����е�       
	for (int i = 0; i < count; i++) {
		int nexti = (i + 1) % count;
		Vector2D p;
		p.x = (originPoint[i].x + originPoint[nexti].x) / 2.0;
		p.y = (originPoint[i].y + originPoint[nexti].y) / 2.0;
		midpoints.push_back(p);
	}

	//ƽ���е�  
	Vector2D extrapoints[maxn];
	for (int i = 0; i < count; i++) {
		int nexti = (i + 1) % count;
		int backi = (i + count - 1) % count;
		Vector2D midinmid;
		midinmid.x = (midpoints[i].x + midpoints[backi].x) / 2.0;
		midinmid.y = (midpoints[i].y + midpoints[backi].y) / 2.0;
		double offsetx = originPoint[i].x - midinmid.x;
		double offsety = originPoint[i].y - midinmid.y;
		int extraindex = 2 * i;
		extrapoints[extraindex].x = midpoints[backi].x + offsetx;
		extrapoints[extraindex].y = midpoints[backi].y + offsety;
		//�� originPoint[i]��������   
		double addx = (extrapoints[extraindex].x - originPoint[i].x) * scale;
		double addy = (extrapoints[extraindex].y - originPoint[i].y) * scale;
		extrapoints[extraindex].x = originPoint[i].x + addx;
		extrapoints[extraindex].y = originPoint[i].y + addy;

		int extranexti = (extraindex + 1) % (2 * count);
		extrapoints[extranexti].x = midpoints[i].x + offsetx;
		extrapoints[extranexti].y = midpoints[i].y + offsety;
		//�� originPoint[i]��������   
		addx = (extrapoints[extranexti].x - originPoint[i].x) * scale;
		addy = (extrapoints[extranexti].y - originPoint[i].y) * scale;
		extrapoints[extranexti].x = originPoint[i].x + addx;
		extrapoints[extranexti].y = originPoint[i].y + addy;

	}

	Vector2D controlPoint[4];
	//����4���Ƶ㣬��������������  
	for (int i = 0; i < count; i++) {
		controlPoint[0] = originPoint[i];
		int extraindex = 2 * i;
		controlPoint[1] = extrapoints[extraindex + 1];
		int extranexti = (extraindex + 2) % (2 * count);
		controlPoint[2] = extrapoints[extranexti];
		int nexti = (i + 1) % count;
		controlPoint[3] = originPoint[nexti];
		double u = 1;
		while (u >= 0) {
			double px = bezier3funcX(u, controlPoint);
			double py = bezier3funcY(u, controlPoint);
			//u�Ĳ����������ߵ�����  
			u -= 0.01;
			Vector2D tempP = Vector2D(px, py);
			//�������ߵ�   
			curvePoint.push_back(tempP);
		}
	}
}
void Jie_Ds::OptimizeBorderBezier(vector<Data> &convexBag,double e)
{
	std::ofstream outC("center_Bezier.obj");
	vector<Data> ConvexhullExp;
	double Sum_x = 0, Sum_y = 0, Aver_x = 0, Aver_y = 0;
	for (int j = 0; j < convexBag.size()-1; j++)
	{
		Sum_x += convexBag[j].X;
		Sum_y += convexBag[j].Y;
	}
	Aver_x = Sum_x / (convexBag.size() - 1);
	Aver_y = Sum_y / (convexBag.size() - 1);
	outC << "v " << fixed << setprecision(5) << Aver_x << " " << fixed << setprecision(5) << Aver_y << " " << "0" << endl;
	outC.close();
	for (int i = 0; i < convexBag.size(); i++)
	{
		Data cp;
		cp.X = convexBag[i].X;
		cp.Y = convexBag[i].Y;
		double d_v_x = convexBag[i].X - Aver_x;
		double d_v_y = convexBag[i].Y - Aver_y;
		double d_n_v_x = d_v_x / sqrt((d_v_x*d_v_x) + (d_v_y*d_v_y));
		double d_n_v_y = d_v_y / sqrt((d_v_x*d_v_x) + (d_v_y*d_v_y));
		cp.X += e* d_n_v_x;
		cp.Y += e* d_n_v_y;
		ConvexhullExp.push_back(cp);
	}

	for (int i = 0; i < ConvexhullExp.size()-1; i++)
	{
		Vector2D p;
		p.x = ConvexhullExp[i].X;
		p.y = ConvexhullExp[i].Y;
		originPoint.push_back(p);
	}
	createCurve();
	convexBag.clear();
	for (int i = 0; i < curvePoint.size(); i++)
	{
		Data d;
		d.X = curvePoint[i].x;
		d.Y = curvePoint[i].y;
		convexBag.push_back(d);
	}
	std::ofstream out101("data//fault_test//convex_exp.obj");
	for (int j = 0; j < ConvexhullExp.size(); j++)
	{
		Data qq;
		qq = ConvexhullExp[j];
		out101 << "v " << fixed << setprecision(5) << qq.X << " " << fixed << setprecision(5) << qq.Y << " " << "0" << endl;
	}
	int count_exp_ = 1;
	for (int j = 0; j < ConvexhullExp.size() - 1; j++)
	{
		out101 << "l " << count_exp_ << " " << count_exp_ + 1 << endl;
		count_exp_++;
	}
	out101.close();
	std::ofstream out2000("data//fault_test//convex_bezier.obj");
	int count_exp = 1;
	for (int j = 0; j < convexBag.size(); j++)
	{
		Data qq;
		qq = convexBag[j];
		out2000 << "v " << fixed << setprecision(5)<< qq.X << " " << fixed << setprecision(5)<<qq.Y << " " << "0" << endl;
	}
	count_exp_ = 1;
	for (int j = 0; j < convexBag.size() - 1; j++)
	{
		out2000 << "l " << count_exp << " " << count_exp + 1 << endl;
		count_exp++;
	}
	out2000 << "l " << convexBag.size() << " " << "1" << endl;
	out2000.close();


}
void Jie_Ds::OptimizeBoder(vector<Data> &convexBag,double e)
{
	vector<Data> ConvexhullExp;
	double Sum_x = 0, Sum_y = 0, Aver_x = 0, Aver_y = 0;
	for (int j = 0; j < convexBag.size()-1; j++)
	{
		Sum_x += convexBag[j].X;
		Sum_y += convexBag[j].Y;
	}
	Aver_x = Sum_x / (convexBag.size()-1);
	Aver_y = Sum_y / (convexBag.size()-1);
	for (int i = 0; i < convexBag.size(); i++)
	{
		Data cp;
		cp.X = convexBag[i].X;
		cp.Y = convexBag[i].Y;
		double d_v_x = convexBag[i].X - Aver_x;
		double d_v_y = convexBag[i].Y - Aver_y;
		double d_n_v_x = d_v_x / sqrt((d_v_x*d_v_x) + (d_v_y*d_v_y));
		double d_n_v_y = d_v_y / sqrt((d_v_x*d_v_x) + (d_v_y*d_v_y));
		cp.X += e* d_n_v_x;
		cp.Y += e* d_n_v_y;
		ConvexhullExp.push_back(cp);
	}



	for (int i = 0; i < ConvexhullExp.size(); i++)
	{
		point p;
		p.x = ConvexhullExp[i].X;
		p.y = ConvexhullExp[i].Y;
		input_vertice.push_back(p);
	}
	control_point = input_vertice;
	deBoor();
	//deCasteljau();
	convexBag.clear();
	for (int i = 0; i < b_spline.size(); i++)
	{
		Data d;
		d.X = b_spline[i].x;
		d.Y = b_spline[i].y;
		convexBag.push_back(d);
	}
	std::ofstream out100("convex_current_exp.obj");
	for (int j = 0; j < ConvexhullExp.size(); j++)
	{
		Data qq;
		qq = ConvexhullExp[j];
		out100 << "v " << fixed << setprecision(5) << qq.X << " " << fixed << setprecision(5) << qq.Y << " " << "0" << endl;
	}
	int count_exp = 1;
	for (int j = 0; j < ConvexhullExp.size() - 1; j++)
	{
		out100 << "l " << count_exp << " " << count_exp + 1 << endl;
		count_exp++;
	}
	out100.close();



	std::ofstream out200("convex_bezier.obj");
	count_exp = 1;
	for (int j = 0; j < convexBag.size(); j++)
	{
		Data qq;
		qq = convexBag[j];
		out200 << "v " << qq.X << " " << qq.Y << " " << "0" << endl;
	}
	count_exp = 1;
	for (int j = 0; j < convexBag.size() - 1; j++)
	{
		out200 << "l " << count_exp << " " << count_exp + 1 << endl;
		count_exp++;
	}
	out200 << "l " << convexBag.size() << " " << "1" << endl;
	out200.close();

	//if ((int)convexBag.size() == 1)
	//{
	//	//�����ı���
	//	m_XMin = m_XMin - 200;
	//	m_XMax = m_XMax + 200;
	//	m_YMin = m_YMin - 200;
	//	m_YMax = m_YMax + 200;

	//	convexBag.clear();

	//	Data p1, p2, p3, p4;
	//	p1.X = m_XMin;
	//	p1.Y = m_YMin;

	//	p2.X = m_XMin;
	//	p2.Y = m_YMax;

	//	p3.X = m_XMax;
	//	p3.Y = m_YMax;

	//	p4.X = m_XMax;
	//	p4.Y = m_YMin;

	//	convexBag.push_back(p1);
	//	convexBag.push_back(p2);
	//	convexBag.push_back(p3);
	//	convexBag.push_back(p4);//���Ͻ�Ϊ��1,1���㣬ʵ���ϵģ�0,0����
	//	convexBag.push_back(p1);
	//}
	//else if ((int)convexBag.size() == 2)
	//{
	//	//�����Ӧ�ľ���
	//	convexBag.clear();

	//	Data p1, p2, p3, p4;
	//	if (m_XMin == m_XMax)
	//	{
	//		m_XMin = m_XMin - 200;
	//	}
	//	if (m_YMin == m_YMax)
	//	{
	//		m_YMin = m_YMin - 200;
	//	}
	//	p1.X = m_XMin;
	//	p1.Y = m_YMin;

	//	p2.X = m_XMin;
	//	p2.Y = m_YMax;

	//	p3.X = m_XMax;
	//	p3.Y = m_YMax;

	//	p4.X = m_XMax;
	//	p4.Y = m_YMin;

	//	convexBag.push_back(p1);
	//	convexBag.push_back(p2);
	//	convexBag.push_back(p3);
	//	convexBag.push_back(p4);		 //���Ͻ�Ϊ��1,1���㣬ʵ���ϵģ�0,0����
	//	convexBag.push_back(p1);        //����Ϊɶ���Ͻ�������

	//}
	//else if ((int)convexBag.size() == 4)
	//{
	//	//���һ����
	//	AddPt(convexBag);
	//}

	//double dx = m_XMax - m_XMin;
	//double dy = m_YMax - m_YMin;
	//if (dy - 8 * dx > 0)
	//{
	//	//�޸�ͼ��Ϊ���Σ�
	//}
	//else if (dx - 8 * dy > 0)
	//{
	//	//�޸�ͼ��Ϊ���Σ�
	//}
}
double Jie_Ds::Angle(Data &p0, const Data &p1, const Data &p2)
{
	//���ڶ���������x�᷽������ƽ��һ����λ��ֵ����һ����
	p0.X = p1.X + 1;
	p0.Y = p1.Y;


	/*std::ofstream out("current_triangle_point.obj");
	out << "v " << p0.X << " " << p0.Y << " " << "0" << endl;
	out << "v " << p1.X << " " << p1.Y << " " << "0" << endl;
	out << "v " << p2.X << " " << p2.Y << " " << "0" << endl;

	out.close();*/

	double A, B, X, cross, angle;
	//���� ����
	A = sqrt((p1.X - p0.X) * (p1.X - p0.X) + (p1.Y - p0.Y) *(p1.Y - p0.Y));
	B = sqrt((p1.X - p2.X) * (p1.X - p2.X) + (p1.Y - p2.Y) *(p1.Y - p2.Y));

	if (A == 0 || B == 0)
		return 0;

	//����ֵ
	X = ((p0.X - p1.X) * (p2.X - p1.X) + (p0.Y - p1.Y) * (p2.Y - p1.Y)) / (A*B);

	//����ж���ʱ���Ƿ����180��
	//a = (x1, y1) b = (x2, y2)
	//a��b = x1y2 - x2y1,�����Ϊ����������b��a����ʱ�뷽��,����b��a��˳ʱ�뷽��,�����Ϊ0����a��b����
	cross = (p0.X - p1.X) * (p2.Y - p1.Y) - (p2.X - p1.X) * (p0.Y - p1.Y);

	if (X == 1 || X >1 || X < -1)
		return 0;

	double temp;
	double p = 3.14159265358979323846;
	if (X == -1)
	{
		angle = p;
	}
	else
	{
		if (cross < 0)//˳ʱ��
		{
			//cout << "˳ʱ��" << endl;
			//angle = atan2((p2.Y - p1.Y), (p2.X - p1.X));
			temp = (atan(-X / sqrt(-X * X + 1)) + 2 * atan(1.0));
			angle = p*2  - temp;
		}
		else//��ʱ��
		{
			//cout << "��ʱ��" << endl;
			//angle = atan2((p1.Y - p2.Y), (p2.X - p1.X));
			angle = (atan(-X / sqrt(-X * X + 1)) + 2 * atan(1.0));
		}
	}
	//if (angle > p)
	//	angle = angle - p;
	return angle;
}
void Jie_Ds::DividedFourParts(int n, vector<int>& FourPort)
{
	if (n>3)
	{
		FourPort[0] = n;
		FourPort[1] = (int)(n / 4);
		FourPort[2] = 2 * (int)(n / 4);
		FourPort[3] = n - (int)(n / 4);
	}
	else
	{
		FourPort[0] = 3;
		FourPort[1] = 1;
		FourPort[2] = 2;
		FourPort[3] = 2;
	}

}
void Jie_Ds::BordersPointDis(vector<int>& FourPart, vector<Data>& convexBag, vector<Data>& upLine, vector<Data>&downline, vector<Data>&leftLine, vector<Data>& rightLine,
	vector<Dis>& upDis, vector<Dis>&downDis, vector<Dis>&leftDis, vector<Dis>&rightDis)
{
	//�������������ʲô����
	downline.resize(FourPart[0] - FourPart[3] + 2);//
	leftLine.resize(FourPart[1] + 2);//
	upLine.resize(FourPart[2] - FourPart[1] + 2);//
	rightLine.resize(FourPart[3] - FourPart[2] + 2);//

	downDis.resize(downline.size());
	leftDis.resize(leftLine.size());
	upDis.resize(upLine.size());
	rightDis.resize(rightLine.size());

	bool JudgeMax = true;
	vector<int> side(4);
	//����ֵ
	side[0] = FourPart[3];
	side[1] = 0;
	side[2] = FourPart[1];
	side[3] = FourPart[2];

	int count_s0 = 0, count_s1 = 0, count_s2 = 0, count_s3 = 0;
	int j = 1, i;
	while (JudgeMax)
	{
		i = 0;

		//���±߼�����С�����ֵ
		if (side[0] <= FourPart[0])
		{
			count_s0++;
			//���㸳ֵ
			downline[j].X = convexBag[side[0]].X;
			downline[j].Y = convexBag[side[0]].Y;


			//��Ϊ�׵�ʱ����ľ��� Ĭ��Ϊ��(0,0)�ľ��� ��
			//Ϊ�׵�ʱ �����븳ֵ 0
			if (j == 1)
				downDis[j].dis = 0;
			else
			//����˵��˱��׵�ľ���
			downDis[j].dis = sqrt((downline[j].X - downline[j - 1].X) * (downline[j].X - downline[j - 1].X)
				+ (downline[j].Y - downline[j - 1].Y) * (downline[j].Y - downline[j - 1].Y)) + downDis[j - 1].dis;

			

			//���������
			side[0] = side[0] + 1;
			//��¼�жϵ�������
			i = i + 1;
			/*if (side[0] == FourPart[0])
			{
				int j_ = j + 1;
				downline[j_].X = convexBag[0].X;
				downline[j_].Y = convexBag[0].Y;
				downDis[j_].dis = sqrt((downline[j_].X - downline[j_ - 1].X) * (downline[j_].X - downline[j_ - 1].X)
					+ (downline[j_].Y - downline[j_ - 1].Y) * (downline[j_].Y - downline[j_ - 1].Y)) + downDis[j_ - 1].dis;
			}*/

		}

		//��߸�ֵ  ͬ��
		if (side[1] <= FourPart[1])
		{
			count_s1++;
			leftLine[j].X = convexBag[side[1]].X;
			leftLine[j].Y = convexBag[side[1]].Y;

			if (j == 1)
				leftDis[j].dis = 0;
			else
			leftDis[j].dis = sqrt((leftLine[j].X - leftLine[j - 1].X) * (leftLine[j].X - leftLine[j - 1].X)
				+ (leftLine[j].Y - leftLine[j - 1].Y) * (leftLine[j].Y - leftLine[j - 1].Y)) + leftDis[j - 1].dis;

			

			side[1] = side[1] + 1;
			i = i + 1;
		}
		//�ϱ߸�ֵ ͬ��
		if (side[2] <= FourPart[2])
		{
			count_s2++;
			upLine[j].X = convexBag[side[2]].X;
			upLine[j].Y = convexBag[side[2]].Y;
			if (j == 1)
				upDis[j].dis = 0;
			else
			upDis[j].dis = sqrt((upLine[j].X - upLine[j - 1].X) * (upLine[j].X - upLine[j - 1].X)
				+ (upLine[j].Y - upLine[j - 1].Y) * (upLine[j].Y - upLine[j - 1].Y)) + upDis[j - 1].dis;

			

			side[2] = side[2] + 1;
			i = i + 1;
		}

		//�ұ߸�ֵ ͬ��
		if (side[3] <= FourPart[3])
		{
			count_s3++;
			rightLine[j].X = convexBag[side[3]].X;
			rightLine[j].Y = convexBag[side[3]].Y;

			if (j == 1)
				rightDis[j].dis = 0;
			else
			rightDis[j].dis = sqrt((rightLine[j].X - rightLine[j - 1].X) * (rightLine[j].X - rightLine[j - 1].X)
				+ (rightLine[j].Y - rightLine[j - 1].Y) * (rightLine[j].Y - rightLine[j - 1].Y)) + rightDis[j - 1].dis;

			

			side[3] = side[3] + 1;
			i = i + 1;
		}

		//�ߵļ�¼�� ����
		j = j + 1;
		//���û������ i=0 ��˵���Ѿ��������� ���е��ϵĵ� ����ѭ��
		if (i == 0)
			JudgeMax = false;
	}
	std::ofstream outd("DP.obj");
	for (int i = 1; i < downline.size(); i++)
		outd << "v " << downline[i].X << " " << downline[i].Y << " " << "0" << endl;
	outd.close();
	std::ofstream outu("UP.obj");
	for (int i = 1; i < upLine.size(); i++)
		outu << "v " << upLine[i].X << " " << upLine[i].Y << " " << "0" << endl;
	outu.close();
	std::ofstream outl("LP.obj");
	for (int i = 1; i < leftLine.size(); i++)
		outl << "v " << leftLine[i].X << " " << leftLine[i].Y << " " << "0" << endl;
	outl.close();
	std::ofstream outr("RP.obj");
	for (int i = 1; i < rightLine.size(); i++)
		outr << "v " << rightLine[i].X << " " << rightLine[i].Y << " " << "0" << endl;
	outr.close();
	//cout << "j = :" << j << endl;
	//cout << "count_s0 = :" << count_s0 << endl;
	//cout << "count_s1 = :" << count_s1 << endl;
	//cout << "count_s2 = :" << count_s2 << endl;
	//cout << "count_s3 = :" << count_s3 << endl;
}
//����upLine��downline��leftLine��rightLine��Dis����ռ�ܵĳ��ȵİ�ֱ�
//����upDis��downdis��leftDis��rightDis��angle��������ǰһ���㹹�ɵ��߶���x��������ļн�
void Jie_Ds::BordersChar(vector<Data>& upLine, vector<Data>&downline, vector<Data>&leftLine, vector<Data>& rightLine,
	vector<Dis>& upDis, vector<Dis>&downdis, vector<Dis>&leftDis, vector<Dis>&rightDis)
{
	bool JudgeMax = true;
	Data triPoint[3];
	int i, j = 2;
	std::ofstream out22("2.obj");
	while (JudgeMax)
	{
		//һ���жϴ�ѭ���Ƿ��м���� ������ i>0˵����ѭ�������㣬����i=0 ����ѭ��
		//����ֵ
		i = 0;
		//����û�г��� �±� ����м���
		if (j < (int)downline.size())
		{
			//����˵�ռ�˱��ܳ��İٷ���
			//����downline.size()��downdis.size()��1��downsize�±��1��ʼ��downsize[1]=0;
			downdis[j].per = downdis[j].dis / downdis[downline.size() - 1].dis;
			//��ֵ��¼�˵���ǰһ����߶�
			triPoint[1] = downline[j - 1];
			triPoint[2] = downline[j];

			//������߶���x��������ĽǶ�,����ĽǶ���ʵ�Ǵ�x��������ʼ��ʱ����ת��ֱ����ת������ʱ����ת�ĽǶ�
			downdis[j].angle = Angle(triPoint[0], triPoint[1], triPoint[2]);
			//cout << downdis[j].angle << endl;
			//���� >0˵��������
			i = i + 1;
			
			out22 << "v " << downline[downline.size() - 1].X << " " << downline[downline.size() - 1].Y << " " << "0" << endl;
		}
		
		if (j < (int)leftLine.size())
		{
			//����˵�ռ�˱��ܳ��İٷ���
			leftDis[j].per = leftDis[j].dis / leftDis[leftLine.size() - 1].dis;
			//��ֵ��¼�˵���ǰһ����߶�
			triPoint[1] = leftLine[j - 1];
			triPoint[2] = leftLine[j];

			//������߶���x��������ĽǶ�
			leftDis[j].angle = Angle(triPoint[0], triPoint[1], triPoint[2]);
			//���� >0˵��������
			i = i + 1;
			
		}

		if (j < (int)upLine.size())
		{
			//����˵�ռ�˱��ܳ��İٷ���
			upDis[j].per = upDis[j].dis / upDis[upLine.size() - 1].dis;
			//��ֵ��¼�˵���ǰһ����߶�
			triPoint[1] = upLine[j - 1];
			triPoint[2] = upLine[j];

			//������߶���x��������ĽǶ�
			upDis[j].angle = Angle(triPoint[0], triPoint[1], triPoint[2]);
			//���� >0˵��������
			i = i + 1;
			
		}

		if (j < (int)rightLine.size())
		{
			//����˵�ռ�˱��ܳ��İٷ���
			rightDis[j].per = rightDis[j].dis / rightDis[rightLine.size() - 1].dis;
			//��ֵ��¼�˵���ǰһ����߶�
			triPoint[1] = rightLine[j - 1];
			triPoint[2] = rightLine[j];

			//������߶���x��������ĽǶ�
			rightDis[j].angle = Angle(triPoint[0], triPoint[1], triPoint[2]);
			//���� >0˵��������
			i = i + 1;
			
		}

		//�߽����������
		j = j + 1;
		//i=0��˵���˴�ѭ��û������ ������ѭ��
		if (i == 0)
			JudgeMax = false;
		out22.close();

	}
}
void Jie_Ds::BordersPoints(vector<int>& FourPart, vector<Data>& convexBag, vector<Data>& upLine, vector<Data>&downline, vector<Data>&leftLine, vector<Data>& rightLine,
	vector<Dis>& upDis, vector<Dis>&downdis, vector<Dis>&leftDis, vector<Dis>&rightDis,
	vector<Data>& DL, vector<Data>& DR, vector<Data>&DD, vector<Data>& DU,
	vector<TWOVALUE>& sim1, vector<TWOVALUE>& sim2, vector<TWOVALUE>& sim3, vector<TWOVALUE>& sim4, int XM, int YM)
{
	int i, j, m;
	double s, t, Tran;

	//*****************�ָ��㷨
	//ȷ���ĵ�����
	//�洢ӳ���ԭ������
	sim1[1].X = convexBag[FourPart[0]].X;//����Ӧ�ô洢����͹���߽������һ�����λ��

	//��Ӧ��һ���ߵ�x�����ֵ
	sim1[2].X = convexBag[FourPart[3]].X - convexBag[FourPart[0]].X;

	//��Ӧ�ڶ����ߵ�x�����ֵ
	sim1[3].X = convexBag[FourPart[1]].X - convexBag[FourPart[0]].X;

	sim1[4].X = convexBag[FourPart[0]].X - convexBag[FourPart[1]].X + convexBag[FourPart[2]].X - convexBag[FourPart[3]].X;

	//����ͬ�� ��Ӧy����ֵ
	sim1[1].Y = convexBag[FourPart[0]].Y;
	sim1[2].Y = convexBag[FourPart[3]].Y - convexBag[FourPart[0]].Y;
	sim1[3].Y = convexBag[FourPart[1]].Y - convexBag[FourPart[0]].Y;
	sim1[4].Y = convexBag[FourPart[0]].Y - convexBag[FourPart[1]].Y + convexBag[FourPart[2]].Y - convexBag[FourPart[3]].Y;

	/*cout << "=======================" << endl;
	cout << "=======================" << endl;
	for(int p=1;p<=4;p++)
	cout << "v " << sim1[p].X << " " << sim1[p].Y << " " << "0" << endl;*/



	sim3.resize(YM + 1);
	sim2.resize(XM + 1);
	sim4.resize(XM + 1);
	DL.resize(YM + 1);
	DR.resize(YM + 1);
	DD.resize(XM + 1);
	DU.resize(XM + 1);
	//ע�⣬��ʵ�����(XM-1),(YM-1)�������������������ʵ�ʸ���,XM,YM�����������������ĸ���
	for (j = 1; j <= YM; j++)
	{
		//�˷ָ�������ľ���ռ�˱߳��ȵİٷ���
		//t = double(j) / YM;
		//��������ı�ֵ
		t = double(j - 1) / (YM - 1);

							  //����
							 //��˵㵽�˱����ľ���
		sim3[j].X = sim1[3].X * t;
		sim3[j].Y = sim1[3].Y * t;

		//�����˵����������(�洢�㵽�ߵľ���,���˾���Ϊ�˱ߵİٷ���, �ʹ˵���ǰһ����߶κ�x��������ĽǶ�(ע������ǶȵĶ���))
		for (m = 2; m<(int)leftDis.size(); m++)
		{
			//���ָ�� �ٷ����� ͹������İٷ�����ͬʱ
			if (t == leftDis[m - 1].per) //��͹����Ϊ�ָ��ʱ
			{
				//�Ѵ�͹���㸳ֵ����߷ָ������
				DL[j].X = leftLine[m - 1].X;
				DL[j].Y = leftLine[m - 1].Y;
				//���ҵ��˷ָ���������
				break;
			}

			//���ָ��������˱ߵ�β��ʱ
			else if (t == 1)
			{
				//�Ѵ˱ߵ����һ���㸳ֵ�� �ָ������
				DL[j].X = convexBag[FourPart[1]].X;
				DL[j].Y = convexBag[FourPart[1]].Y;
				//���ҵ�������ѭ��
				break;
			}

			//���˷ָ����׵�ľ���ռ�İٷ��� Ϊ����͹������ʱ ��  t��ĳ���߶����˵�İٷ���֮��
			else if (leftDis[m - 1].per < t && t < leftDis[m].per)
			{
				//�˷ָ������͹�����ϵ�λ�õ���͹����һ�˵����ռ��͹���߳��ȵİٷ���
				Tran = leftDis[leftDis.size() - 1].dis* t - leftDis[m - 1].dis;
				//���ݴ˱ߵĽǶ���˵㵽һ�ε�ľ��� ��ֱ�߲������� �ɼ���˵��(x,y)����
				DL[j].X = leftLine[m - 1].X + Tran * cos(leftDis[m].angle);
				DL[j].Y = leftLine[m - 1].Y + Tran * sin(leftDis[m].angle);
				break;
			}
		}

		if ((int)rightDis.size()>1)
		{
			//ͬ�ϣ������ұ߽�ķָ�� ��������ֵ
			for (m = 2; m< (int)rightDis.size(); m++)
			{
				if ((1 - t) == rightDis[m - 1].per)  //��Ϊĳ���ڵ�ʱ
				{
					DR[j].X = rightLine[m - 1].X;
					DR[j].Y = rightLine[m - 1].Y;
					break;
				}
				else if ((1 - t) == 1)
				{
					DR[j].X = convexBag[FourPart[3]].X;
					DR[j].Y = convexBag[FourPart[3]].Y;
					break;
				}
				else if (rightDis[m - 1].per < (1 - t) && (1 - t) < rightDis[m].per)
				{
					Tran = rightDis[rightDis.size() - 1].dis * (1 - t) - rightDis[m - 1].dis;
					DR[j].X = rightLine[m - 1].X + Tran * cos(rightDis[m].angle);
					DR[j].Y = rightLine[m - 1].Y + Tran * sin(rightDis[m].angle);
					break;
				}
			}
		}
		//����������=3ʱ, ֻ��һ����
		else
		{
			//�˵�Ϊ��4����
			DR[j].X = convexBag[FourPart[3]].X;
			DR[j].Y = convexBag[FourPart[3]].Y;
		}
	}

	// ���ұ߽��Ϸָ��������

	//�������±߽�ķָ��
	for (i = 1; i <= XM; i++)
	{
		//����˵�ռ�ܱ߳��İٷ���
		s = double(i - 1) / (XM - 1);
		sim2[i].X = sim1[2].X * s;
		sim2[i].Y = sim1[2].Y * s;
		sim4[i].X = sim1[4].X * s;
		sim4[i].Y = sim1[4].Y * s;

		//���ϱ߽�ָ�㸳ֵ
		for (m = 2; m< (int)upDis.size(); m++)
		{
			//���˷ָ��İٷ�����͹����İٷ�����ͬʱ ͹����Ϊ�ָ��
			if (s == upDis[m - 1].per) //��Ϊĳ���ڵ�ʱ
			{
				//���ָ�㸳ֵ
				DU[i].X = upLine[m - 1].X;
				DU[i].Y = upLine[m - 1].Y;
				//����ʱ����ѭ��
				break;
			}

			//Ϊβ��ʱ
			else if (s == 1)
			{
				DU[i].X = convexBag[FourPart[2]].X;
				DU[i].Y = convexBag[FourPart[2]].Y;
				//����ѭ��
				break;

			}

			//��Ϊĳ��͹����ʱ
			else if (upDis[m - 1].per < s && s < upDis[m].per)
			{
				Tran = upDis[upDis.size() - 1].dis * s - upDis[m - 1].dis;
				DU[i].X = upLine[m - 1].X + Tran * cos(upDis[m].angle);
				DU[i].Y = upLine[m - 1].Y + Tran * sin(upDis[m].angle);
				break;
			}
		}

		//����ͬ�� �ɼ����±߽�ָ�������ֵ
		for (m = 2; m < (int)downdis.size(); m++)
		{
			if ((1 - s) == downdis[m - 1].per)  //��Ϊĳ���ڵ�ʱ
			{
				DD[i].X = downline[m - 1].X;
				DD[i].Y = downline[m - 1].Y;
				break;
			}

			else if ((1 - s) == 1)
			{
				DD[i].X = convexBag[FourPart[0]].X;
				DD[i].Y = convexBag[FourPart[0]].Y;
				break;
			}
			else if (downdis[m - 1].per <= 1 - s && 1 - s <= downdis[m].per)
			{
				Tran = downdis[downdis.size() - 1].dis * (1 - s) - downdis[m - 1].dis;
				DD[i].X = downline[m - 1].X + Tran * cos(downdis[m].angle);
				DD[i].Y = downline[m - 1].Y + Tran * sin(downdis[m].angle);
				break;
			}

		}

	}
	std::ofstream outDL("FGL.obj");
	for (int i = 1; i < DL.size(); i++)
		outDL << "v " << DL[i].X << " " << DL[i].Y << " " << "0" << endl;
	outDL.close();
	std::ofstream outDR("FGR.obj");
	for (int i = 1; i < DR.size(); i++)
		outDR << "v " << DR[i].X << " " << DR[i].Y << " " << "0" << endl;
	outDR.close();
	std::ofstream outDU("FGU.obj");
	for (int i = 1; i < DU.size(); i++)
		outDU << "v " << DU[i].X << " " << DU[i].Y << " " << "0" << endl;
	outDU.close();
	std::ofstream outDD("FGD.obj");
	for (int i = 1; i < DD.size(); i++)
		outDD << "v " << DD[i].X << " " << DD[i].Y << " " << "0" << endl;
	outDD.close();


}
void Jie_Ds::AddData(Data &t)
{
	bool m_b = false;
	for (int i = 0; i<(int)m_oriData.size(); i++)
	{
		if (m_oriData[i].X == t.X && m_oriData[i].Y == t.Y)
			m_b = true;
	}
	if (!m_b)
		m_oriData.push_back(t);
}
bool Jie_Ds::Inv(vector<vector<double>>&M)
{
	int i, j, k, n;
	double Temp;
	n = (int)M.size();
	vector<int> iw(n), jw(n);
	//��m�ұ�����һ����λ�󣬹���һ��m���������mm
	//double **mm = new double * [n];
	//vector<vector<double>> mm(n);
	//for(i = 0 ;i< n;i++)
	//{
	//	mm[i].resize(2 * n);
	//	for(int j = 0;j < n;j ++) 
	//	{
	//		mm[i][j] = M[i][j];
	//	}
	//}
	double **mm = new double *[n];		//���ݱȽϴ������Ƚ϶࣬vectorЧ�ʱ�ָ��ͺܶ�20150325
	for (i = 0; i< n; i++)
	{
		mm[i] = new double[2 * n];
		for (int j = 0; j < n; j++)
		{
			mm[i][j] = M[i][j];
		}
	}

	for (i = 0; i< n; i++)
	{
		for (j = n; j<2 * n; j++)
		{
			if (i == j - n)
				mm[i][j] = 1;
			else
				mm[i][j] = 0;
		}
	}
	//ͨ�������б任(����˹��ȥ��)ʹԭ�����Ϊ��λ�����ұߵĵ�λ����ԭ���������
	for (k = 0; k < n - 1; k++)
	{
		/*----------------------------------------*/
		//�ӵ� k �С��� k �п�ʼ�����½�������ѡȡ����ֵ����Ԫ�أ�����ס��Ԫ���ڵ��кź��кţ�
		//��ͨ���н������н���������������Ԫ��λ����.��һ����Ϊȫѡ��Ԫ  20140925
		iw[k] = k;
		jw[k] = k;
		if (abs(mm[k][k]) < 0.00000001)
		{
			for (int i = k + 1; i < n; i++)
			{
				if (abs(mm[i][k]) > 0.000001)
				{
					iw[k] = i;
					for (j = 0; j < n; j++)
					{
						Temp = mm[k][j];
						mm[k][j] = mm[iw[k]][j];
						mm[iw[k]][j] = Temp;
						//swap(mm[k][j], mm[iw[k]][j]);
					}
					break;
				}
			}
		}
		if (abs(mm[k][k]) < 0.00000001)
		{
			return false;
		}
		/*----------------------------------------*/
		for (i = k + 1; i < n; i++)
		{
			Temp = mm[i][k] / mm[k][k];
			for (j = 0; j < 2 * n; j++)
			{
				mm[i][j] = mm[i][j] - mm[k][j] * Temp;
			}
			mm[i][k] = 0.0;		//��ֹ�������20140929
		}
	}

	if (abs(mm[n - 1][n - 1]) < 0.00000001)
	{
		return false;
	}
	for (k = n - 1; k > 0; k--)
	{
		for (i = k - 1; i >= 0; i--)
		{
			Temp = mm[i][k] / mm[k][k];
			for (j = 2 * n - 1; j >= 0; j--)
				mm[i][j] = mm[i][j] - mm[k][j] * Temp;
		}
	}
	double s;
	for (i = 0; i<n; i++)
	{
		s = mm[i][i];
		for (j = 0; j < 2 * n; j++)
			mm[i][j] = mm[i][j] / s;
	}

	//����任����ұߵľ���
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
			M[i][j] = mm[i][j + n];
	}

	//������ȫѡ��Ԫ����������¼���С��н�������Ϣ���лָ����ָ���ԭ�����£���ȫѡ��Ԫ�����У�
	//�Ƚ������У��У�����лָ���ԭ�����У��У��������У��У��������ָ���
	for (k = n - 2; k >= 0; k--)
	{
		if (jw[k] != k)
		{
			for (i = 0; i < n; i++)
			{
				Temp = M[k][i];
				M[k][i] = M[jw[k]][i];
				M[jw[k]][i] = Temp;
				//swap(MInv[k][i], MInv[jw[k]][i]);
			}
		}

		if (iw[k] != k)
		{
			for (i = 0; i < n; i++)
			{
				Temp = M[i][k];
				M[i][k] = M[i][iw[k]];
				M[i][iw[k]] = Temp;
				//swap(MInv[i][k], MInv[i][iw[k]]);
			}
		}
	}
	return true;
}
void Jie_Ds::PreMatrix(vector<Data>& suV, vector<double>& ni)
{
	double b0 = 1;			//�ȼ���б��Ϊһ��ֵ
	double b1 = 0;			//��ؾ�Ϊһ��ֵ
	double RR = m_B;		//������ͺ��
	//cout << m_B << endl;
	int n = (int)suV.size();//��λ�����
	//vector<vector<double>> Arr(n + 1);
	MatrixXd m(n + 1, n + 1);
	MatrixXd m_inver(n + 1, n + 1);
	MatrixXd m_tran(n + 1, n + 1);
	MatrixXd m_temp(n + 1, n + 1);
	MatrixXd m_temp_inver(n + 1, n + 1);

	//********������ֵ
	//Arr[0].resize(n + 1);
	for (int i = 1; i <= n; i++)
	{
		//Arr[i].resize(n + 1);
		//Arr[i][i] = b1 + b0 * RR - b1;//�Խ�����ֵΪ����ͺ����
		m(i,i) = b1 + b0 * RR - b1;
		//Arr[i][n] = 1;
		m(i, n) = 1;
		//Arr[0][i] = 1;
		m(0, i) = 1;
		for (int j = 0; j <= n - 1; j++)
		{
			//���㵱ǰ���ʣ�����е�ľ���
			double d = sqrt((suV[i - 1].X - suV[j].X) * (suV[i - 1].X - suV[j].X) + (suV[i - 1].Y - suV[j].Y) * (suV[i - 1].Y - suV[j].Y));
			//����RR��ʵ�Ѿ����õ�ֵ�㹻���ˣ������������if��䲻���ȥ
			if (d > RR)
			{
				d = RR;			//20140422����ͺ����,������ͺ����ָ��Ӧ���Ǿ�λ��������������ֵ
			}
			//Arr[i][j] = abs(b0 * (d)-RR);//��ǰ������������ͺ�����ֵ�ľ���ֵ
			m(i,j)= abs(b0 * (d)-RR);
		}
	}
	//Arr[0][n] = 0;
	//Arr[0][0] = 1;
	m(0, n) = 0;
	m(0, 0) = 1;
	m_temp = m;
	double temp;
	for (int i = 0; i <= n; i++)
	{
		temp = m_temp(0,i);
		m_temp(0, i) = m_temp(n, i);
		m_temp(n, i) = temp;
	}
	//cout << m_temp;
//	cout << endl;
	//cout << "ԭ����" << endl;
	//cout << m << endl;
	//cout << endl;
	//Inv(Arr);//�������������
	m_tran = m.transpose();
	m_inver = m.inverse();
	m_temp_inver = m_temp.inverse();
	//cout << "ת�þ���" << endl;
	//cout << m_tran << endl;
	//cout << m_tran.inverse() << endl;
	//cout << endl;
	//cout << "�����" << endl;
	//cout << m_inver << endl;
	//cout << endl;
	//��ⷽ�̸�
	ni.resize(n + 1);
	Data d;
	d.X = 0; d.Y = 0; d.Z = 0;
	suV.push_back(d);
	//cout<<suV[n].Z << endl;
	for (int i = 0; i <= n; i++)
	{
		for (int j = 0; j <= n; j++)
		{
			//ni[i] = ni[i] + suV[j].Z * Arr[j][i];
			//֮ǰ�ȶ�m����һ��ת�ò���֮�������һЩ��
			//�κ�һ���߳�ֵ����ni��Ԫ�ص�������ϣ�����ni[0]ǰ���ϵ��Ϊ1
			//ʣ��ni[i]ǰ���ϵ����ŷ����þ��������ͺ����ı�ֵ�й�
			ni[i] = ni[i] + suV[j].Z * m_inver(j, i);
		}
	}
	//cout << endl;
	//for (int i = 0; i <= n; i++)
	//	cout << ni[i] << " ";
	//cout << endl;
	//cout << ni[0] <<" "<<ni[1]<<" "<<ni[2]<<" "<<ni[3]<< endl;
}

double Jie_Ds::InsertEst(vector<Data>& suV, TWOVALUE& D, vector<double>& ni)
{
	double dg = 0;
	double ZZ = m_B;//ֱ���ͱ��캯��Bֵ
	double b0 = 1;
	double b1 = 0;
	vector<double> cc(suV.size());
	cc[0] = 1;
	//cout << ni[0] << endl;
	dg = cc[0] * ni[0];
	double distanceVal = 0;
	//һ��Ҫע�������suV����һ�������ж����һ��Ԫ�أ�����
	for (int i = 1; i < (int)suV.size(); i++)
	{
		// distanceVal = 0;
		//��ǰ�����;�λ��ľ���
		distanceVal = sqrt((suV[i - 1].X - D.X) * (suV[i - 1].X - D.X) + (suV[i - 1].Y - D.Y) * (suV[i - 1].Y - D.Y));
		if (distanceVal > ZZ)
		{
			distanceVal = ZZ;	//20140422����ͺ����
		}
		cc[i] = b0 * (ZZ - distanceVal);
		dg = cc[i] * ni[i] + dg;
	}
	//cout << dg << " " << endl;
	return dg;
}
double Jie_Ds::DisInv(TWOVALUE D)
{
	double Z = 0;
	double Sum = 0;
	double Di = 0;
	Data TempWell;
	for (int i = 0; i < (int)m_suV.size(); i++)
	{
		TempWell = m_suV[i];
		double d = (pow(D.X - TempWell.X, 2) + pow(D.Y - TempWell.Y, 2));
		//Ĭ������2�ξ��뷴��
		//d = pow(d,1.5);		//�Ӵ���뷴�ȷ��Ĵ������Ա���ý��ĵ�Ӱ��������20131217
		//�������̫����ֱ�ӰѴ˵�ĸ߳�ֵ��ֵ����
		if (d < 0.00001)
		{
			return TempWell.Z;
		}
		/************************/
		//TWOVALUE temp;
		//temp.X = TempWell.X;
		//temp.Y = TempWell.Y;
		//if (IsChangeW(D,temp,m_vecFaultLines))
		//{
		//	d = d * 1000;								//���Ŷϲ�����СȨֵ20140904��
		//}
		/**************************/
		//d = pow(d,3) ;
		d = 1.0 / d;
		Sum += d;
		Di = Di + d * TempWell.Z;
	}
	Z = Di / Sum;
	return Z;
}
void Jie_Ds::EvaluateNoFault()
{
	if ((int)m_oriData.size() < 800)//20150323,800�ھ�����ʹ�ÿ���𷽷����ٶ�4s���ҿ��Խ���
	{
		m_IsK = true;				//��������ʹ�ÿ�����ֵ
	}
	else
	{
		m_IsK = false;				//����̫��ʹ���ݾ��뷴�ȹ�ֵ
	}

	if ((int)m_oriData.size() <= 1)
	{
		return;		//����ֻ��һ����λ������20131107
	}
	if (m_IsK)			//�Ƿ�ʹ�ÿ�����㷨��ֵ��׼ȷ�����ٶ�����201308163
	{
		vector<Data> Suv = m_oriData;
		vector<double> ni;
		PreMatrix(Suv, ni);
		TWOVALUE D;
		for (int i = 1; i <= m_XNum; i++)
		{
			for (int j = 1; j <= m_YNum; j++)
			{
				D.X = m_GridPoint[i][j].X;
				D.Y = m_GridPoint[i][j].Y;
				//���ƴ˵������ֵ    û�жϲ�Ĺ���ֵ + ����ֵ
				m_GridPoint[i][j].Z = InsertEst(Suv, D, ni);//ȫ����λ��Ŀ�����ֵ

															//m_GridPoint[i][j].Z = Well_Near_K(D);		//�ھ���Χ�ڵĿ�����ֵ20131212
															//m_GridPoint[i][j].Z = DisInv(D);			//�ھ���Χ�ڵľ��뷴�ȹ�ֵ20131212
				if (m_ZMin > m_GridPoint[i][j].Z)
					m_ZMin = m_GridPoint[i][j].Z;

				if (m_ZMax < m_GridPoint[i][j].Z)
					m_ZMax = m_GridPoint[i][j].Z;
			}
		}
		//m_suV = Suv;		//20131212
		//m_ni = ni;			//20130913 Ӧ�������Ʒ�
	}
	else
	{
		m_suV = m_oriData;
		TWOVALUE D;
		for (int i = 1; i <= m_XNum; i++)
		{
			for (int j = 1; j <= m_YNum; j++)
			{
				D.X = m_GridPoint[i][j].X;
				D.Y = m_GridPoint[i][j].Y;
				//���ƴ˵������ֵ    û�жϲ�Ĺ���ֵ + ����ֵ
				//m_GridPoint[i][j].Z = InsertEst(Suv, D, ni);//ȫ����λ��Ŀ�����ֵ
				//m_GridPoint[i][j].Z = Well_Near_K(D);		//�ھ���Χ�ڵĿ�����ֵ20131212
				m_GridPoint[i][j].Z = DisInv(D);			//�ھ���Χ�ڵľ��뷴�ȹ�ֵ20131212
				if (m_ZMin > m_GridPoint[i][j].Z)
					m_ZMin = m_GridPoint[i][j].Z;

				if (m_ZMax < m_GridPoint[i][j].Z)
					m_ZMax = m_GridPoint[i][j].Z;
			}

		}
	}

	//if ((int)m_oriData.size() <= 1)
	//{
	//	return ;		//����ֻ��һ����λ������20131107
	//}
	//vector<double> ni;
	//vector<Data> Suv = m_oriData; //20140421
	//PreMatrix(Suv,ni);
	//TWOVALUE D;
	//for (int i = 1;i <= m_XNum;i++)
	//{
	//	for(int j = 1;j <= m_YNum;j++)
	//	{
	//		D.X= m_GridPoint[i][j].X;
	//		D.Y = m_GridPoint[i][j].Y;
	//		//���ƴ˵������ֵ    û�жϲ�Ĺ���ֵ + ����ֵ
	//		m_GridPoint[i][j].Z = InsertEst(Suv, D, ni);
	//		if(m_ZMin > m_GridPoint[i][j].Z)
	//			m_ZMin = m_GridPoint[i][j].Z;

	//		if(m_ZMax < m_GridPoint[i][j].Z)
	//			m_ZMax = m_GridPoint[i][j].Z;
	//	}

	//}

	/************************************************/
	//CString Str = "zֵ \n";
	//for (int i = 1;i <= m_XNum;i++)
	//{
	//	for(int j = 1;j <= m_YNum;j++)
	//	{
	//		CString s;
	//		s.Format("    %f",m_GridPoint[i][j].Z);
	//		Str += s;
	//	}
	//	Str += "\n";
	//}
	//for (int i = 0 ; i < m_oriData.size() ; i ++)
	//{
	//	CString str;
	//	str.Format("%f,    %f,    %f��   %f     ��%f\n",m_oriData[i].X,m_oriData[i].Y,m_oriData[i].Z,m_ZMax,m_ZMin);
	//	Str += str;
	//}
	//AfxMessageBox(Str);
	/************************************************/
	//m_suV = Suv;
	//m_ni = ni;				//20130913 Ӧ�������Ʒ�
}
void Jie_Ds::SetGridXY(double exp)
{
	//�����m_Border��Ӧ�ô洢���Ǿ��α߽�
	m_OriBoder = m_Border;	//ԭʼ�߽���Ϣ��m_OriBoder  20140806
	vector<Data> convexBag;
	//Charact();				//��ȡ�����Сֵ

							/*-------------------------------------------*/
							//���ݶ����߽粻�պϵ����20170718
	int OriCount = (int)m_OriBoder.size();
	if (OriCount >= 3)
	{
		Data Pt0 = m_OriBoder[0];
		Data Pt1 = m_OriBoder[OriCount - 1];
		if (GetDis(Pt0.X, Pt0.Y, Pt1.X, Pt1.Y) >= 0.0001)
		{
			//�ñ߽�պ�
			m_OriBoder.push_back(Pt0);
		}
	}

	/*-------------------------------------------*/

	/*-------------------------------------------*/
	//���ݾ�λ�������������m_B��ֵ 20150205  ���������ʲô����
	double k = sqrt(pow(m_XMax - m_XMin, 2) + pow(m_YMax - m_YMin, 2));
	//cout << "������룺" << m_B << endl;
	if (m_B <= k)
	{
		m_B = k+2;		//��Ҫ�Ƿ�ֹ�����Ǳ��ر��ķ�Χ
	}
	/*-------------------------------------------*/

	//GetRectBoder();		//ʹ�þ��α߽磨�Լ�����߽磩20131025

	//�ж��Ƿ��б߽磻����б߽磬�ʹӱ߽����͹������������ݵ����͹��
	//20131111m_Border��һֱ����ֵ�������ǵ����ľ�λ���ݻ����˹�����
	//�����ݵ����͹���ļ���
	if ((int)m_Border.size() <= 2)
	{
		m_Border.clear();
		//convexBag = Convex(m_oriData);
		convexBag = Withershins(m_oriData);	//�����µ�͹������20131108 ����͹��
		//ʹ��B��������͹���߽���Ż�
		//OptimizeBoder(convexBag,200);			
		//ʹ�ñ��������߽���͹���߽���Ż�
		OptimizeBorderBezier(convexBag,exp);
		m_Border = convexBag;
		m_OriBoder = m_Border;	//ԭʼ�߽���Ϣ��m_OriBoder  20140806
	}
	//�ɱ߽����͹���ļ���
	else
	{
		//cout << "hello2" << endl;
		convexBag = Withershins(m_Border);  //����20131107  ����͹��
		//ʹ��B��������͹���߽���Ż�
		//OptimizeBoder(convexBag);
		//ʹ�ñ��������߽���͹���߽���Ż�
		OptimizeBorderBezier(convexBag,exp); 
		m_Border = convexBag;
	}

	std::ofstream out("convexhull_test_point.obj");
	//out << "v " << convexBag[379].X << " " << convexBag[376].Y << " " << "0" << endl;
	//out << "v " << convexBag[499].X << " " << convexBag[499].Y << " " << "0" << endl;
	/*out << "v " << convexBag[377].X << " " << convexBag[377].Y << " " << "0" << endl;
	out << "v " << convexBag[378].X << " " << convexBag[378].Y << " " << "0" << endl;
	out << "v " << convexBag[389].X << " " << convexBag[389].Y << " " << "0" << endl;
	out << "v " << convexBag[390].X << " " << convexBag[390].Y << " " << "0" << endl;
	out << "v " << convexBag[391].X << " " << convexBag[391].Y << " " << "0" << endl;*/
	//out << "v " << convexBag[0].X << " " << convexBag[0].Y << " " << "0" << endl;
	//out << "v " << convexBag[126].X << " " << convexBag[126].Y << " " << "0" << endl;
	//out << "v " << convexBag[252].X << " " << convexBag[252].Y << " " << "0" << endl;
	//out.close();
	vector<int> FourPort(4);
	

	DividedFourParts((int)(convexBag.size() - 1), FourPort);//�Ż��߽��͹�����Ϊ������������

	//cout << "=========================================" << endl;
	//cout << "���͹��������һЩ��Ϣ" << endl;
	//cout << "FourPort[0]= " << FourPort[0] << " " << "FourPort[1]= " << FourPort[1] << " " << "FourPort[2]= " << FourPort[2] << " " << "FourPort[3]= " << FourPort[3] << endl;

	vector<Data> upLine, downLine, leftLine, rightLine;

	vector<Dis> upDis, downdis, leftDis, rightDis;

	//upLine��downline��leftLine��rightLine�洢��͹���Ĳ��ֱ߽��ϵĵ��λ��
	//upDis��downDis��leftDis��rightDis�е�dis�����洢�˶�Ӧ�㵽��һ����ľ���
	BordersPointDis(FourPort, convexBag, upLine, downLine, leftLine, rightLine,
		upDis, downdis, leftDis, rightDis);

	//����upLine��downline��leftLine��rightLine��Dis����ռ�ܵĳ��ȵİ�ֱ�
	//����upDis��downdis��leftDis��rightDis��angle��������x��������ļн�
	BordersChar(upLine, downLine, leftLine, rightLine, upDis, downdis, leftDis, rightDis);

	vector<Data> DL, DR, DD, DU;
	vector<TWOVALUE> sim1(5), sim2, sim3, sim4;
	BordersPoints(FourPort, convexBag, upLine, downLine, leftLine, rightLine,
		upDis, downdis, leftDis, rightDis, DL, DR, DD, DU,
		sim1, sim2, sim3, sim4, m_XNum, m_YNum);

	double s, t;
	int XM = m_XNum, YM = m_YNum;

	int GridPoint_size = m_GridPoint.size();
	if (GridPoint_size >0)
	{
		for (int i = 0; i<GridPoint_size; i++)
		{
			vector <THRVALUE>().swap(m_GridPoint[i]);
			m_GridPoint[i].clear();
		}
	}
	vector <vector<THRVALUE>>().swap(m_GridPoint);
	m_GridPoint.clear();
	m_GridPoint.resize(m_XNum + 3);
	m_GridPoint[0].resize(m_XNum + 3);

	for (int i = 1; i <= m_XNum; i++)
	{
		m_GridPoint[i].resize(m_XNum + 3);
	}

	m_GridPoint[m_XNum + 1].resize(m_XNum + 3);
	m_GridPoint[m_XNum + 2].resize(m_XNum + 3);

	for (int i = 1; i <= XM; i++)
	{
		s = (double)(i - 1) / (XM - 1);
		for (int j = 1; j <= YM; j++)
		{
			t = (double)(j - 1) / (YM - 1);
			//ע������t��s��ʹ��
			m_GridPoint[i][j].X = (DL[j].X * (1 - s) + DR[j].X * s) + (DU[i].X * t + DD[i].X * (1 - t)) - (sim1[1].X + sim2[i].X + sim3[j].X + t * sim4[i].X);
			m_GridPoint[i][j].Y = (DL[j].Y * (1 - s) + DR[j].Y * s) + (DU[i].Y * t + DD[i].Y * (1 - t)) - (sim1[1].Y + sim2[i].Y + sim3[j].Y + t * sim4[i].Y);
			m_GridPoint[i][j].Z = 0.0;//20130814
		}
	}
	//����swap��������vector�ڴ���ͷ�
	DL.clear();
	vector <Data>().swap(DL);

	DR.clear();
	vector <Data>().swap(DR);

	DD.clear();
	vector <Data>().swap(DD);

	DU.clear();
	vector <Data>().swap(DU);

	sim2.clear();
	vector <TWOVALUE>().swap(sim2);

	sim3.clear();
	vector <TWOVALUE>().swap(sim3);

	sim4.clear();
	vector <TWOVALUE>().swap(sim4);

	sim1.clear();
	vector <TWOVALUE>().swap(sim1);

	upLine.clear();
	vector <Data>().swap(upLine);

	downLine.clear();
	vector <Data>().swap(downLine);

	leftLine.clear();
	vector <Data>().swap(leftLine);

	rightLine.clear();
	vector <Data>().swap(rightLine);

	upDis.clear();
	vector <Dis>().swap(upDis);

	downdis.clear();
	vector <Dis>().swap(downdis);

	leftDis.clear();
	vector <Dis>().swap(leftDis);

	rightDis.clear();
	vector <Dis>().swap(rightDis);
}
//�õ�һ������������
double Jie_Ds::GetMagnitude(double fNumber)
{
	//������
	double magnitudeValue = 1.0;

	if (fNumber == 0.0)
		return(0.0);

	//�Ƿ�Ϊ����
	bool bNegative;
	bNegative = (fNumber<0) ? true : false;

	double positiveNumber = abs(fNumber);
	if (positiveNumber == 1)
	{//����1	
		magnitudeValue = 1.0;
	}
	else if (positiveNumber<1.0)
	{//С��1
		while (positiveNumber<1.0)
		{
			positiveNumber *= 10.0;
			magnitudeValue /= 10.0;
		}
	}
	else
	{//����1
		while (positiveNumber>1.0)
		{
			positiveNumber /= 10.0;
			magnitudeValue *= 10.0;
		}
		magnitudeValue /= 10.0;
	}
	return magnitudeValue;
}
float Jie_Ds::FindStep(float StepMin, bool bUporDown)
{
	string str;
	float dStep, dStepOld, RetVal;
	if (StepMin == 0.0f)
		return(0.0f);

	//�Ƿ�Ϊ����
	bool bNegative = false;
	bNegative = (StepMin<0) ? true : false;

	if (bNegative)
	{//�����ľ���ֵ�Ĺ���������֮���������෴
		bUporDown = !bUporDown;
	}

	//���Ȱ�����������
	StepMin = (float)fabs(StepMin);     //��С��0,��ȡ����ֵ

	if (!bUporDown)
	{//���¹���
	 //���㽥��ĸֵ
		dStep = (float)GetMagnitude(double(StepMin));//�õ��������������
		dStep *= 10;

		while (StepMin < dStep)
		{
			dStepOld = dStep;
			dStep = dStep / 2.5f;
			if (StepMin < dStep)
			{
				dStepOld = dStep;
				dStep = dStep / 2;
			}
			if (StepMin < dStep)
			{
				dStepOld = dStep;
				dStep = dStep / 2.0f;
			}
		}
		RetVal = dStep;
	}
	else
	{//���Ϲ���
	 //���㽥��ĸֵ
		dStep = (float)GetMagnitude(double(StepMin));//�õ��������������
		while (StepMin > dStep)
		{
			dStep *= 2.5f;
			if (StepMin > dStep)
			{
				dStep *= 2.0f;
			}
			else
			{
				dStep /= 2.5f;
				dStep *= 2.0f;
			}
		}
		RetVal = dStep;
		//	RetVal = FindMaxFloatStep(StepMin);
	}

	//����������������ԭ�����
	if (bNegative)
	{
		RetVal *= -1;
	}
	return(RetVal);
}
void Jie_Ds::CalcSameArray()
{
	//��ֵ����С��ʾֵ
	m_Show_MinValue = FindStep((float)m_ZMin, false);//���¹���Сֵ

	//��ֵ�������ʾֵ
	m_Show_MaxValue = FindStep((float)(m_ZMax), true);//���Ϲ�����ֵ

																//��ֵ�߼��
	m_Show_JianGeValue = (m_Show_MaxValue - m_Show_MinValue) / 2;

	m_sameArray.RemoveAll();
	double minvalue = m_Show_MinValue;
	while (minvalue < m_Show_MaxValue)
	{
		m_sameArray.Add(minvalue);
		minvalue += m_Show_JianGeValue;
	}

	if (minvalue != m_Show_MaxValue)
	{
		m_sameArray.Add(m_Show_MaxValue);
	}
}
void Jie_Ds::SetTrackValue(vector<double> Track)
{
	m_TrackValue.clear();
	for (int i = 0; i < (int)Track.size(); i++)
	{
		m_TrackValue.push_back(Track[i]);
	}
	if ((int)m_TrackValue.size() > 2)
	{
		m_valuedis = abs(m_TrackValue[1] - m_TrackValue[0]);
	}
	else
	{
		m_valuedis = 1.0;
	}
}

//�õ�����A������ĵ�20140916
void Jie_Ds::GetMinDis(THRVALUE A, vector<THRVALUE>Pts, THRVALUE &Point)
{
	double Distance = pow(Pts[0].X - A.X, 2) + pow(Pts[0].Y - A.Y, 2);
	Point = Pts[0];
	for (int i = 1; i < (int)Pts.size(); i++)
	{
		double d = pow(Pts[i].X - A.X, 2) + pow(Pts[i].Y - A.Y, 2);
		if (d < Distance)
		{
			Distance = d;
			Point = Pts[i];
		}
	}
}

//X�����Ϻ��ϲ�ĵĵ�ֵ��201301015
void Jie_Ds::X_FalutEquivalent(int X_index, int Y_index, double Value, vector<TWOVALUE>&VirtualIJ)
{
	THRVALUE Star = m_GridPoint[X_index][Y_index];
	THRVALUE End = m_GridPoint[X_index + 1][Y_index];

	TWOVALUE A, B;
	A.X = Star.X;
	A.Y = Star.Y;

	B.X = End.X;
	B.Y = End.Y;
	/*-----------------------------------------------------------------*/
	//20140916
	vector<THRVALUE>TempPts = m_XFaultPts[X_index][Y_index].FLine;
	if ((int)TempPts.size() <= 0)
	{
		return;
	}
	//vector<TWOVALUE> TempPts;
	//for (int i = 0 ; i < (int)m_vecFaultLines.size() ; i ++)
	//{
	//	FalutLine LineOne = m_vecFaultLines[i];
	//	for (int j = 0 ; j < (int)LineOne.FLine.size() - 1 ; j ++)
	//	{
	//		TWOVALUE p1,p2,Point;
	//		p1 = LineOne.FLine[j];
	//		p2 = LineOne.FLine[j+1];
	//		if (L2L_Intersect(A,B,p1,p2,Point))
	//		{
	//			TempPts.push_back(Point);
	//		}
	//	}
	//}
	/*-----------------------------------------------------------------*/
	//if (!IsInside(A))
	if (!m_GridInFault[X_index][Y_index])
	{
		/*-----------------------------------------------------------------*/
		//20140916
		//TWOVALUE Pt;
		//GetMinDis(A,TempPts,Pt);	//�õ�����A������ĵ�
		//THRVALUE Dataij;
		//Dataij.X = Pt.X ;
		//Dataij.Y = Pt.Y ;
		//Dataij.Z = 0;
		//GetValueByK(Dataij);
		THRVALUE Dataij;
		GetMinDis(Star, TempPts, Dataij);
		/*-----------------------------------------------------------------*/
		if ((Dataij.Z - Value) * (Star.Z - Value) < 0)
		{
			double t = abs((Value - Star.Z) / (Dataij.Z - Star.Z));
			double d1 = sqrt(pow(Dataij.X - Star.X, 2) + pow(Dataij.Y - Star.Y, 2));

			double d2 = sqrt(pow(End.X - Star.X, 2) + pow(End.Y - Star.Y, 2));

			TWOVALUE P;
			P.X = X_index + t * d1 / d2;
			P.Y = Y_index;
			VirtualIJ.push_back(P);
		}
	}

	//if (!IsInside(B))
	if (!m_GridInFault[X_index + 1][Y_index])
	{
		/*-----------------------------------------------------------------*/
		//20140916
		//TWOVALUE Pt;
		//GetMinDis(B,TempPts,Pt);	//�õ�����B������ĵ�
		//THRVALUE Dataij;
		//Dataij.X = Pt.X ;
		//Dataij.Y = Pt.Y ;
		//Dataij.Z = 0;
		//GetValueByK(Dataij);
		THRVALUE Dataij;
		GetMinDis(End, TempPts, Dataij);
		/*-----------------------------------------------------------------*/
		if ((Dataij.Z - Value) * (End.Z - Value) < 0)
		{
			double t = abs((End.Z - Value) / (End.Z - Dataij.Z));
			double d1 = sqrt(pow(Dataij.X - End.X, 2) + pow(Dataij.Y - End.Y, 2));

			double d2 = sqrt(pow(End.X - Star.X, 2) + pow(End.Y - Star.Y, 2));

			TWOVALUE P;
			P.X = X_index + 1 - t * d1 / d2;
			P.Y = Y_index;
			VirtualIJ.push_back(P);
		}
	}
}


//Y�����Ϻ��ϲ�ĵĵ�ֵ��201301015
void Jie_Ds::Y_FalutEquivalent(int X_index, int Y_index, double Value, vector<TWOVALUE>&VirtualIJ)
{
	THRVALUE Star = m_GridPoint[X_index][Y_index];
	THRVALUE End = m_GridPoint[X_index][Y_index + 1];

	TWOVALUE A, B;
	A.X = Star.X;
	A.Y = Star.Y;

	B.X = End.X;
	B.Y = End.Y;

	/*-----------------------------------------------------------------*/
	//20140916
	vector<THRVALUE> TempPts = m_YFaultPts[X_index][Y_index].FLine;
	//for (int i = 0; i < TempPts.size(); i++)
	//	cout << "v " << TempPts[i].X << " " << TempPts[i].Y << " " << "0" << endl;
	if ((int)TempPts.size() <= 0)
	{
		return;
	}
	//vector<TWOVALUE> TempPts;
	//for (int i = 0 ; i < (int)m_vecFaultLines.size() ; i ++)
	//{
	//	FalutLine LineOne = m_vecFaultLines[i];
	//	for (int j = 0 ; j < (int)LineOne.FLine.size() - 1 ; j ++)
	//	{
	//		TWOVALUE p1,p2,Point;
	//		p1 = LineOne.FLine[j];
	//		p2 = LineOne.FLine[j+1];
	//		if (L2L_Intersect(A,B,p1,p2,Point))
	//		{
	//			TempPts.push_back(Point);
	//		}
	//	}
	//}
	/*-----------------------------------------------------------------*/
	//if (!IsInside(A))
	//������㲻���ڶϲ��ڲ�
	if (!m_GridInFault[X_index][Y_index])
	{
		/*-----------------------------------------------------------------*/
		//20140916
		//TWOVALUE Pt;
		//GetMinDis(A,TempPts,Pt);	//�õ�����A������ĵ�
		//THRVALUE Dataij;
		//Dataij.X = Pt.X ;
		//Dataij.Y = Pt.Y ;
		//Dataij.Z = 0;
		//GetValueByK(Dataij);
		THRVALUE Dataij;
		GetMinDis(Star, TempPts, Dataij);
		/*-----------------------------------------------------------------*/
		if ((Dataij.Z - Value) * (Star.Z - Value) < 0)
		{
			double t = abs((Value - Star.Z) / (Dataij.Z - Star.Z));
			double d1 = sqrt(pow(Dataij.X - Star.X, 2) + pow(Dataij.Y - Star.Y, 2));
			double d2 = sqrt(pow(End.X - Star.X, 2) + pow(End.Y - Star.Y, 2));

			TWOVALUE P;
			P.X = X_index;
			P.Y = Y_index + t * d1 / d2;
			VirtualIJ.push_back(P);
		}
	}

	//if (!IsInside(B))
	if (!m_GridInFault[X_index][Y_index + 1])
	{
		/*-----------------------------------------------------------------*/
		//20140916
		//TWOVALUE Pt;
		//GetMinDis(B,TempPts,Pt);	//�õ�����B������ĵ�
		//THRVALUE Dataij;
		//Dataij.X = Pt.X ;
		//Dataij.Y = Pt.Y ;
		//Dataij.Z = 0;
		//GetValueByK(Dataij);
		THRVALUE Dataij;
		GetMinDis(End, TempPts, Dataij);
		/*-----------------------------------------------------------------*/
		if ((Dataij.Z - Value) * (End.Z - Value) < 0)
		{
			double t = abs((End.Z - Value) / (End.Z - Dataij.Z));
			double d1 = sqrt(pow(Dataij.X - End.X, 2) + pow(Dataij.Y - End.Y, 2));
			double d2 = sqrt(pow(End.X - Star.X, 2) + pow(End.Y - Star.Y, 2));

			TWOVALUE P;
			P.X = X_index;
			P.Y = Y_index + 1 - t * d1 / d2;
			VirtualIJ.push_back(P);
		}
	}
}
//�õ����еĵ�ֵ��
//void Jie_Ds::EquivalentPoints(double Value, vector<THRVALUE>&Jie_VirtualIJK)
void Jie_Ds::EquivalentPoints(double Value, vector<TWOVALUE>&VirtualIJ)
{
	IsoLine temp_Line;
	VirtualIJ.clear();
	for (int i = 1; i <= m_XNum; i++)//�����ݱ��ϵĵ�ֵ��
	{
		for (int j = 1; j < m_YNum; j++)
		{
			if (m_YFault[i][j])
			{
				//���������н��� 20130913
				Y_FalutEquivalent(i, j, Value, VirtualIJ);//�õ�����ĵ�ֵ�� 20131015
				continue;
			}
			//�жϴ˱����Ƿ��е�ֵ��
			if ((m_GridPoint[i][j].Z - Value) * (m_GridPoint[i][j + 1].Z - Value) < 0)
			{
				TWOVALUE P;
				P.X = i;
				//�����tֵһ�������ģ���������
				double t = ((Value - m_GridPoint[i][j].Z) / (m_GridPoint[i][j + 1].Z - m_GridPoint[i][j].Z));
				t = abs(t);
				P.Y = j + t;
				//if (t >= 1.0)
				//{
				//	int sk = 1;
				//	ASSERT(sk);
				//	CString str;
				//	str.Format("xt���� %f,%f,%f", Value, m_GridPoint[i][j].Z, m_GridPoint[i][j + 1].Z);
				//	//AfxMessageBox(str);
				//}
				TWOVALUE RealyPoint;
				RealyPoint.X = m_GridPoint[i][j].X + t*(m_GridPoint[i][j + 1].X - m_GridPoint[i][j].X);
				RealyPoint.Y = m_GridPoint[i][j].Y + t*(m_GridPoint[i][j + 1].Y - m_GridPoint[i][j].Y);
				/*if (IsInside(RealyPoint))
				{
					continue;
				}
				else*/
				VirtualIJ.push_back(P);
				//Jie_VirtualIJK.push_back(P1);
					temp_Line.Logic.push_back(RealyPoint);
					//temp_Line.Value = Value;
					//Jie_IsoLine.push_back(temp_Line);
					//VirtualIJ1.push_back(RealyPoint);
			}
			else if ((m_GridPoint[i][j].Z - Value) * (m_GridPoint[i][j + 1].Z - Value) == 0)
			{
				//cout << "�ݱߵ�ֵ��" << endl;
				if (m_GridPoint[i][j].Z == Value)
				{
					if (m_GridPoint[i][j].Z < m_GridPoint[i][j + 1].Z)
						m_GridPoint[i][j].Z -= thea;
					else
						m_GridPoint[i][j].Z += thea;
					TWOVALUE P;
					P.X = i;
					//�����tֵһ�������ģ���������
					double t = ((Value - m_GridPoint[i][j].Z) / (m_GridPoint[i][j + 1].Z - m_GridPoint[i][j].Z));
					t = abs(t);
					P.Y = j + t;
					TWOVALUE RealyPoint;
					RealyPoint.X = m_GridPoint[i][j].X + t*(m_GridPoint[i][j + 1].X - m_GridPoint[i][j].X);
					RealyPoint.Y = m_GridPoint[i][j].Y + t*(m_GridPoint[i][j + 1].Y - m_GridPoint[i][j].Y);
					VirtualIJ.push_back(P);
					temp_Line.Logic.push_back(RealyPoint);
				}
				else
				{
					if (m_GridPoint[i][j].Z < m_GridPoint[i][j + 1].Z)
						m_GridPoint[i][j+1].Z += thea;
					else
						m_GridPoint[i][j+1].Z -= thea;
					TWOVALUE P;
					P.X = i;
					//�����tֵһ�������ģ���������
					double t = ((Value - m_GridPoint[i][j].Z) / (m_GridPoint[i][j + 1].Z - m_GridPoint[i][j].Z));
					t = abs(t);
					P.Y = j + t;
					TWOVALUE RealyPoint;
					RealyPoint.X = m_GridPoint[i][j].X + t*(m_GridPoint[i][j + 1].X - m_GridPoint[i][j].X);
					RealyPoint.Y = m_GridPoint[i][j].Y + t*(m_GridPoint[i][j + 1].Y - m_GridPoint[i][j].Y);
					VirtualIJ.push_back(P);
					temp_Line.Logic.push_back(RealyPoint);
				}
			}
		}
	}
	for (int j = 1; j <= m_YNum; j++)//�����ϵĵ�ֵ��
	{
		for (int i = 1; i < m_XNum; i++)
		{
			if (m_XFault[i][j])
			{
				//���������н��� 20130913
				X_FalutEquivalent(i, j, Value, VirtualIJ);//�õ�����ĵ�ֵ�� 20131015
				continue;
			}
			if ((m_GridPoint[i][j].Z - Value) * (m_GridPoint[i + 1][j].Z - Value) < 0)
			{
				TWOVALUE P;
				double t = ((Value - m_GridPoint[i][j].Z) / (m_GridPoint[i + 1][j].Z - m_GridPoint[i][j].Z));
				t = abs(t);
				P.X = i + t;
				P.Y = j;
				if (t >= 1.0 || t<0.0)
				{
					//AfxMessageBox("yt����");
				}
				TWOVALUE RealyPoint;
				RealyPoint.X = m_GridPoint[i][j].X + t*(m_GridPoint[i + 1][j].X - m_GridPoint[i][j].X);
				RealyPoint.Y = m_GridPoint[i][j].Y + t*(m_GridPoint[i + 1][j].Y - m_GridPoint[i][j].Y);
				/*if (IsInside(RealyPoint))
				{
					continue;
				}*/
				VirtualIJ.push_back(P);
				//Jie_VirtualIJK.push_back(P1);
				//VirtualIJ1.push_back(RealyPoint);
				temp_Line.Logic.push_back(RealyPoint);
				
			}
			else if ((m_GridPoint[i][j].Z - Value) * (m_GridPoint[i+1][j].Z - Value) == 0)
			{
				//cout << "��ߵ�ֵ��" << endl;
				if (m_GridPoint[i][j].Z == Value)
				{
					if (m_GridPoint[i][j].Z < m_GridPoint[i+1][j].Z)
						m_GridPoint[i][j].Z -= thea;
					else
						m_GridPoint[i][j].Z += thea;
					TWOVALUE P;
					P.X = i;
					//�����tֵһ�������ģ���������
					double t = ((Value - m_GridPoint[i][j].Z) / (m_GridPoint[i+1][j].Z - m_GridPoint[i][j].Z));
					t = abs(t);
					P.Y = j + t;
					TWOVALUE RealyPoint;
					RealyPoint.X = m_GridPoint[i][j].X + t*(m_GridPoint[i+1][j].X - m_GridPoint[i][j].X);
					RealyPoint.Y = m_GridPoint[i][j].Y + t*(m_GridPoint[i+1][j].Y - m_GridPoint[i][j].Y);
					VirtualIJ.push_back(P);
					temp_Line.Logic.push_back(RealyPoint);
				}
				else
				{
					if (m_GridPoint[i+1][j].Z < m_GridPoint[i][j].Z)
						m_GridPoint[i+1][j].Z -= thea;
					else
						m_GridPoint[i+1][j].Z += thea;
					TWOVALUE P;
					P.X = i;
					//�����tֵһ�������ģ���������
					double t = ((Value - m_GridPoint[i][j].Z) / (m_GridPoint[i+1][j].Z - m_GridPoint[i][j].Z));
					t = abs(t);
					P.Y = j + t;
					TWOVALUE RealyPoint;
					RealyPoint.X = m_GridPoint[i][j].X + t*(m_GridPoint[i+1][j].X - m_GridPoint[i][j].X);
					RealyPoint.Y = m_GridPoint[i][j].Y + t*(m_GridPoint[i+1][j].Y - m_GridPoint[i][j].Y);
					VirtualIJ.push_back(P);
					temp_Line.Logic.push_back(RealyPoint);
				}
			}
		}
	}
	temp_Line.Value = Value;
	Jie_IsoLine.push_back(temp_Line);
	//std::cout << "================================================" << endl;
	//std::cout << "================================================" << endl;
	//for (int i = 0; i < VirtualIJ1.size(); i++)
	//{
	//	std::cout << "v " << VirtualIJ1[i].X << " " << VirtualIJ1[i].Y << " " << "0" << endl;
    //}
	//std::cout << "================================================" << endl;
	//std::cout << "================================================" << endl;
}

//����Ҫע�⣬����ͨ��vector<vector>����Ķ�ά���飬��һ��ֵ�����Ͻǣ��������½�
void Jie_Ds::SignBorder(vector<vector<double>>& biaoji1, vector<vector<double>>& biaoji2)
{
	biaoji1.clear();
	biaoji2.clear();
	biaoji1.resize(m_XNum + 1);
	biaoji2.resize(m_XNum + 2);

	for (int i = 0; i <= m_XNum; i++)
	{
		biaoji1[i].resize(m_YNum + 2);
		biaoji2[i].resize(m_YNum + 1);
		biaoji1[i][0] = 1;
		biaoji1[i][m_YNum + 1] = 1;
		biaoji2[i][0] = 1;
		biaoji2[i][m_YNum] = 1;
	}
	
	biaoji2[m_XNum + 1].resize(m_YNum + 1);
	biaoji2[m_XNum + 1][0] = 1;
	biaoji2[m_XNum + 1][m_YNum] = 1;

	for (int i = 1; i<m_YNum; i++)
	{
		biaoji2[0][i] = 1;
		biaoji2[m_XNum + 1][i] = 1;
		biaoji1[0][i] = 1;
		biaoji1[m_XNum][i] = 1;
	}
	biaoji1[0][m_YNum] = 1;
	biaoji1[m_XNum][m_YNum] = 1;
}
//ת��һ����
void Jie_Ds::GetReallyPoint(TWOVALUE A, TWOVALUE &B)
{
	double X1 = A.X;
	double Y1 = A.Y;
	int X0 = (int)X1;
	int Y0 = (int)Y1;
	double Mark = 0, C, D;
	//˵����һ�������
	if (X1 - X0 == 0 && Y1 - Y0 == 0)
	{
		C = m_GridPoint[X0][Y0].X;
		D = m_GridPoint[X0][Y0].Y;
	}
	//˵���ں���
	else if (X1 - X0 == 0 && Y1 - Y0 != 0)
	{
		Mark = abs(Y1 - Y0);
		C = (1 - Mark) * m_GridPoint[X0][Y0].X + Mark * (m_GridPoint[X0][Y0 + 1].X);//���������Բ�ֵ
		D = (1 - Mark) * m_GridPoint[X0][Y0].Y + Mark * (m_GridPoint[X0][Y0 + 1].Y);
	}
	else if (X1 - X0 != 0 && Y1 - Y0 == 0)
	{
		Mark = abs(X1 - X0);
		C = (1 - Mark) * m_GridPoint[X0][Y0].X + Mark * (m_GridPoint[X0 + 1][Y0].X);
		D = (1 - Mark) * m_GridPoint[X0][Y0].Y + Mark * (m_GridPoint[X0 + 1][Y0].Y);
	}
	B.X = C;
	B.Y = D;

}
//���Ƿ���ֱ���� 
bool Jie_Ds::IsinLineK(THRVALUE Star, THRVALUE End, THRVALUE Point)
{
	End.X = End.X - Star.X;
	End.Y = End.Y - Star.Y;
	Point.X = Point.X - Star.X;
	Point.Y = Point.Y - Star.Y;
	Star.X = 0;
	Star.Y = 0;					//ƽ�ƣ����ж��Ƿ�һ��ֱ�߲����Ӱ�졤����������ΪС��~����߼��㾫��
	if (Point.X *(Point.X - End.X) >0 && Point.Y *(Point.Y - End.Y) > 0)
	{
		return false;
	}
	double A = Star.Y - End.Y;
	double B = End.X - Star.X;
	//double C = -A * Star.X- B * Star.Y;
	double C = 0;
	double D = sqrt(pow(A, 2) + pow(B, 2));
	double d = A * (Point.X - Star.X) + B * (Point.Y - Star.Y);
	d = d / D;
	if (abs(d)<0.00001)
	{
		return true;
	}
	return false;

}
bool Jie_Ds::IsinLineK(THRVALUE Star, THRVALUE End, TWOVALUE Point)
{
	End.X = End.X - Star.X;
	End.Y = End.Y - Star.Y;
	Point.X = Point.X - Star.X;
	Point.Y = Point.Y - Star.Y;
	Star.X = 0;
	Star.Y = 0;					//ƽ�ƣ����ж��Ƿ�һ��ֱ�߲����Ӱ�졤����������ΪС��~����߼��㾫��
	if (Point.X *(Point.X - End.X) >0 && Point.Y *(Point.Y - End.Y) > 0)
	{
		return false;
	}
	double A = Star.Y - End.Y;
	double B = End.X - Star.X;
	//double C = -A * Star.X- B * Star.Y;
	double C = 0;
	double D = sqrt(pow(A, 2) + pow(B, 2));
	double d = A * (Point.X - Star.X) + B * (Point.Y - Star.Y);
	d = d / D;
	if (abs(d)<0.00001)
	{
		return true;
	}
	return false;

}
//�����߶��еı��� 20140915
double Jie_Ds::GetIndex(THRVALUE Star, THRVALUE End, THRVALUE Point)
{
	double t = 0.0;
	if (abs(Star.X - End.X) > abs(Star.Y - End.Y))
	{
		t = (Point.X - Star.X) / (End.X - Star.X);
	}
	else
	{
		t = (Point.Y - Star.Y) / (End.Y - Star.Y);
	}
	return t;

}
//�ж϶ϲ�����β�����ڵ��������20140724
vector<FLOValue> Jie_Ds::GetFalutindex(THRVALUE GridIJ[6], GridFL Lines)
{
	vector<FLOValue> Index;			//�ȼ����׵�λ�ã��ټ�ĩ��λ��
	for (int i = 0; i < (int)Lines.FLines.size(); i++)
	{
		SingleFalut Falt = Lines.FLines[i];

		/*--------------------------------------------*/
		//20140915
		//TWOVALUE Star = Falt.FLine[0];
		//TWOVALUE End  = Falt.FLine[(int)Falt.FLine.size() - 1];
		//��ǰ�ϲ��ߵ���β��
		THRVALUE Star = Falt.FLine[0];
		THRVALUE End = Falt.FLine[(int)Falt.FLine.size() - 1];
		/*--------------------------------------------*/
		FLOValue  OneFalut;
		OneFalut.FLines = Falt;
		//�׵����
		for (int j = 0; j < 5; j++)
		{
			THRVALUE StarA = GridIJ[j];
			THRVALUE EndA = GridIJ[j + 1];
			if (IsinLineK(StarA, EndA, Star))
			{
				OneFalut.Star = GetIndex(StarA, EndA, Star) + j;
			}
		}
		//β�����
		for (int j = 0; j < 5; j++)
		{
			THRVALUE StarA = GridIJ[j];
			THRVALUE EndA = GridIJ[j + 1];
			if (IsinLineK(StarA, EndA, End))
			{
				OneFalut.End = GetIndex(StarA, EndA, End) + j;
			}
		}
		Index.push_back(OneFalut);
	}
	return Index;

}
//��star�㿪ʼ����һ����ֵ��ȷ����������뵽����  20140724
void Jie_Ds::GetCrossMe(THRVALUE GridIJ[6], vector<FLOValue> &FLIndex, double &Star, vector<THRVALUE> &FlautArae)
{
	int Colc = 0;			//�׵�Ϊ��Сֵ
	int index = 0;			//�յ�Ϊ��Сֵ
	double minx = 100;		//star��ĵ�һ���ϲ��
	double next = 100;		//�ϲ��Ӧ�����ӵ�
	//cout << FLIndex.size() << endl;
	//cout << FLIndex[0].FLines.FLine.size() << endl;
	//for (int i = 0; i < FLIndex[0].FLines.FLine.size(); i++)
	//	cout << "v " << FLIndex[0].FLines.FLine[i].X << " " << FLIndex[0].FLines.FLine[i].Y << " " << "0" << endl;;
	for (int i = 0; i < (int)FLIndex.size(); i++)	//Ѱ��Star��ĵ�һ���ϲ��
	{
		FLOValue FlO = FLIndex[i];
		//cout << Star << endl;
		//cout << FlO.Star << endl;
		//cout << FlO.End << endl;
		if (FlO.Star > Star && FlO.Star < minx)
		{
			//cout << "hello1" << endl;
			Colc = 0;
			index = i;
			minx = FlO.Star;
			next = FlO.End;
		}
		if (FlO.End > Star && FlO.End < minx)
		{
			//cout << "hello2" << endl;
			Colc = 1;
			index = i;
			minx = FlO.End;
			next = FlO.Star;
		}
	}

	//cout << endl;
	//cout << "hello3" << endl;
	//cout << endl;

	//forѭ��û�н��в���
	if (minx == 100)
	{
		//cout << "hello4" << endl;
		FLIndex.erase(FLIndex.begin());	//ɾ�����õ�����
		return;
	}
	if (minx >= (int)Star + 1)
	{
		for (int i = (int)Star + 1; i < minx; i++)
		{
			FlautArae.push_back(GridIJ[i]);
		}
	}
	FLOValue FlO = FLIndex[index];
	int Num = FlO.FLines.FLine.size();
	/*--------------------------------------------*/
	//20140915
	//vector<TWOVALUE> PtLine = FlO.FLines.FLine;
	vector<THRVALUE> PtLine = FlO.FLines.FLine;
	/*--------------------------------------------*/
	if (Colc == 0)							//˳�����ϲ�����
	{
		for (int i = 0; i < Num; i++)
		{
			/*-----------------------------------*/
			//20140915
			//THRVALUE Pt;
			//Pt.X = PtLine[i].X;
			//Pt.Y = PtLine[i].Y;
			//Pt.Z = 0.0;
			THRVALUE Pt = PtLine[i];
			/*-----------------------------------*/

			FlautArae.push_back(Pt);
		}
	}
	else									//�������ϲ�����
	{
		for (int i = Num - 1; i >= 0; i--)
		{
			/*-----------------------------------*/
			//20140915
			//THRVALUE Pt;
			//Pt.X = PtLine[i].X;
			//Pt.Y = PtLine[i].Y;
			//Pt.Z = 0.0;
			THRVALUE Pt = PtLine[i];
			/*-----------------------------------*/
			FlautArae.push_back(Pt);
		}
	}
	Star = next;
	FLIndex.erase(FLIndex.begin() + index);	//ɾ�����õ�����,��ǰ�Ķϲ����Ѿ��������
}

//�õ����ڹ�ֵ������20140724
vector<THRVALUE> Jie_Ds::GetFalutAera(THRVALUE GridIJ[6], GridFL Lines)
{
	vector<THRVALUE> FlautLine;
	FlautLine.push_back(GridIJ[0]);//�Ѿ��ѵ�ֵ����뵽������

	//�õ�ÿһ�������ڶϲ��ߵ���Ϣ���Լ�ÿһ���ϲ��ߵ��׵��β���ڴ������е�λ�ñ��
	vector<FLOValue> FLIndex = GetFalutindex(GridIJ, Lines);
	double Star = 0;
	//for (int i = 0 ; i < 6 ; i ++)
	//{
	//	GetCrossMe(GridIJ,Lines,i,FlautLine);
	//}
	for (double i = 0; i < 6; )
	{
		GetCrossMe(GridIJ, FLIndex, i, FlautLine);
		if (FLIndex.size() < 1)
		{
			//cout << "yes" << endl;
			for (int k = i + 1; k < 6; k++)
			{
				//cout << k << endl;
				FlautLine.push_back(GridIJ[k]);
			}
			break;
		}
	}
	return FlautLine;
}
//���Ƿ���������20140724
BOOL Jie_Ds::IsOnGrid(THRVALUE GridIJ[6], TWOVALUE PT)
{
	for (int i = 0; i < 5; i++)
	{
		THRVALUE P1 = GridIJ[i];
		THRVALUE P2 = GridIJ[i + 1];
		if (IsinLineK(P1, P2, PT))
		{
			return TRUE;
		}
	}
	return FALSE;
}

//�õ���ֵ���������������ֵ  GridIJ�Ե�ֵ��Ϊ���ı߽����� Lines�����ڲ��Ķϲ�������   
BOOL Jie_Ds::GetVirtualValue(THRVALUE GridIJ[6], GridFL Lines, TWOVALUE &Point)
{
	vector<THRVALUE>TempPoint;
	vector<BOOL>Mark;			//�洢��Ϊ�ϲ�㻹������㣬TUREΪ�ϲ��
	vector<THRVALUE>TestPoint;
	//int index = 0;
	//GetFirstLine(GridIJ,Lines,index);	//�õ��ϲ����߶εĵ�һ������
	//for (int i = 0 ; i < index + 1; i ++)
	//{
	//	TempPoint.push_back(GridIJ[i]);
	//	Mark.push_back(TRUE);			//TUREΪ�����
	//	TestPoint.push_back(GridIJ[i]);
	//}
	//if (index < 5)						//�õ������Ӧ�Ķϲ�������
	//{
	//	GetCrossFault(GridIJ[index],GridIJ[index + 1],Lines,TempPoint,index);
	//}
	//int Num = Mark.size() ;
	//for (int i = Num ; i < (int)TempPoint.size() ; i ++)
	//{
	//	Mark.push_back(FALSE);
	//}
	//Num = (int)TempPoint.size() - 1;
	//THRVALUE EndP = TempPoint[Num];
	//GetLastLine(GridIJ,EndP,index);		//�õ������Ӧ���߶Σ����һ�ϲ���λ��
	//for (int i = index ; i < 6 ; i++)
	//{
	//	TempPoint.push_back(GridIJ[i]);
	//	Mark.push_back(TRUE);
	//}
	//vector<TWOVALUE> CrossPoint;
	//vector<BOOL> CrossMark;
	//double Z = GridIJ[0].Z;//�׵�Ϊ��ֵ��
	//for (int i = 0 ; i < (int)TempPoint.size() - 1 ;i++)	//�������һ�ں����һ���߶�
	//{
	//	double A = TempPoint[i].Z;
	//	double B = TempPoint[i+1].Z;
	//	if ((A-Z)*(B-Z)<0)
	//	{
	//		TWOVALUE P;
	//		P.X = (Z-A)*(TempPoint[i+1].X - TempPoint[i].X)/(B-A) + TempPoint[i].X;
	//		P.Y = (Z-A)*(TempPoint[i+1].Y - TempPoint[i].Y)/(B-A) + TempPoint[i].Y;
	//		CrossPoint.push_back(P);
	//		if (!Mark[i] && !Mark[i+1])
	//		{
	//			CrossMark.push_back(FALSE);
	//		}
	//		else
	//		{
	//			CrossMark.push_back(TRUE);
	//		}
	//	}
	//}
	//m_GridFault.push_back(TempPoint);//20130918
	//double mindis = 1000000000;//pow(StarA.X - EndA.X,2) + pow(StarA.Y - EndA.Y,2);
	//BOOL Judge = FALSE;
	//if ((int)CrossPoint.size()<1)
	//{
	//	AfxMessageBox("1");
	//	return TRUE;
	//}
	//for (int i = 0 ; i < (int)CrossPoint.size();i++)
	//{
	//	TWOVALUE PointB = CrossPoint[i];
	//	double d = pow(GridIJ[0].X - PointB.X,2) + pow(GridIJ[0].Y - PointB.Y,2);
	//	if (d < mindis)
	//	{
	//		mindis = d;
	//		Point = PointB; //�����׵�����Ľ���
	//		Judge = CrossMark[i];
	//	}
	//}
	//return Judge;

	/****************************************************/
	//ʵ���·�����20140724
	//if (GridIJ[0].Z > 1749 && abs(627138.65 - GridIJ[0].X) < 1 && abs(152986.58 - GridIJ[0].Y) < 1)
	//if (abs(GridIJ[0].Z - (-450)) < 1 && abs(625410.48 - GridIJ[0].X) < 1 && abs(150759.19 - GridIJ[0].Y) < 1)
	//if (abs(GridIJ[0].Z - (149)) < 1 && abs(627138.6 - GridIJ[0].X) < 1 && abs(152091.1 - GridIJ[0].Y) < 1)
	//if (abs(GridIJ[0].Z - (-450)) < 1 && abs(625417.48 - GridIJ[0].X) < 1 && abs(150753.19 - GridIJ[0].Y) < 1)
	//if (abs(GridIJ[0].Z - (-50)) < 1 && abs(626533.7 - GridIJ[0].X) < 1 && abs(153259.5 - GridIJ[0].Y) < 1)
	//{
	//	int sk = 0;
	//	AfxMessageBox("2");
	//}
	TempPoint = GetFalutAera(GridIJ, Lines);
	//for (int i = 0; i < TempPoint.size(); i++)
	//	cout << "v " << TempPoint[i].X << " " << TempPoint[i].Y << " " << "0" << endl;
	//20140915���˴�֮ǰ�Ѿ���ֵ��
	//for (int i = 0 ; i < TempPoint.size()  ; i ++)
	//{
	//	THRVALUE TP = TempPoint[i];
	//	TP.Z = 0;
	//	GetValueByK(TP);
	//	TempPoint[i] = TP;
	//}
	/*cout << "v " << GridIJ[0].X << " " << GridIJ[0].Y << " " << "0" << endl;
	cout << "v " << GridIJ[1].X << " " << GridIJ[1].Y << " " << "0" << endl;
	cout << "v " << GridIJ[2].X << " " << GridIJ[2].Y << " " << "0" << endl;
	cout << endl;*/
	TempPoint[0] = GridIJ[0];//��֤�׵�Ϊ��ֵ��
	TempPoint[(int)TempPoint.size() - 1] = GridIJ[0];//��֤�յ�Ϊ��ֵ��
	//for (int i = 0; i < TempPoint.size(); i++)
	//	cout << "v " << TempPoint[i].X << " " << TempPoint[i].Y << " " << "0" << endl;
	/****************************************************/
	vector<TWOVALUE> CrossPoint;
	vector<BOOL> CrossMark;
	double Z = GridIJ[0].Z;//�׵�Ϊ��ֵ�㣬��ǰҪ׷�ٵ�����ֵ
	//������Ҫ��ȷһ�㣬��ֻ��������и߳�ֵ��֮ǰ�ڼ���ϲ��ߺ�����ߵĽ���ʱ����ÿһ�������Լ������ڲ��Ķϲ�㶼�����˵����Ĺ�ֵ
	for (int i = 0; i < (int)TempPoint.size() - 1; i++)	//��ʵ���ﲻ��Ҫ�������һ�ں����һ���߶�
	{
		double A = TempPoint[i].Z;
		double B = TempPoint[i + 1].Z;
		if ((A - Z)*(B - Z)<0)
		{
			TWOVALUE P;
			//�õ���ֵ�������ֵ
			P.X = (Z - A)*(TempPoint[i + 1].X - TempPoint[i].X) / (B - A) + TempPoint[i].X;
			P.Y = (Z - A)*(TempPoint[i + 1].Y - TempPoint[i].Y) / (B - A) + TempPoint[i].Y;
			CrossPoint.push_back(P);
			//�жϵ�ǰ�ĵ�ֵ���Ƿ����������
			if (!IsOnGrid(GridIJ, P))
			{
				CrossMark.push_back(FALSE);
			}
			else
			{
				CrossMark.push_back(TRUE);
			}
		}
	}
	m_GridFault.push_back(TempPoint);//20130918
	double mindis = 1000000000;//pow(StarA.X - EndA.X,2) + pow(StarA.Y - EndA.Y,2);
	BOOL Judge = FALSE;
	//������ʵ�Ǻܿ����ֵ��׼ȷ�Եģ�Ҫ�ǲ�ֵ��Ч���ã���������޵�ֵ������
	if ((int)CrossPoint.size()<1)//˵���޵�ֵ��
	{
		//cout << "no no no" << endl;
		//AfxMessageBox("1");
		return TRUE;
	}
	for (int i = 0; i < (int)CrossPoint.size(); i++)
	{
		TWOVALUE PointB = CrossPoint[i];
		//������׵�ľ��룬������׵���ʵ���ǽ�������ߵĵ�ֵ��
		double d = pow(GridIJ[0].X - PointB.X, 2) + pow(GridIJ[0].Y - PointB.Y, 2);
		if (d < mindis)
		{
			mindis = d;
			Point = PointB; //�����׵�����Ľ��㣬�������˼�������ж����ֵ��ʱ��ѡȡ���׵�����ĵ�ֵ��
			Judge = CrossMark[i];
		}
	}
	return Judge;
}
//�õ��ϲ��Χ��������������һ����
void Jie_Ds::GetPtPs(THRVALUE GridIJ[6], THRVALUE Point, int &Index)
{
	for (int i = 0; i < 5; i++)
	{
		THRVALUE Star = GridIJ[i];
		THRVALUE End = GridIJ[i + 1];
		TWOVALUE StarA, EndA, TemPoint;
		StarA.X = Star.X;
		StarA.Y = Star.Y;
		EndA.X = End.X;
		EndA.Y = End.Y;
		TemPoint.X = Point.X;
		TemPoint.Y = Point.Y;
		if (IsinLineK(StarA, EndA, TemPoint))
		{
			Index = i;
			return;
		}
	}
}
//�õ�����
void Jie_Ds::GetT(THRVALUE A, THRVALUE B, THRVALUE C, double &t)
{
	double dx = B.X - A.X;
	double dy = B.Y - A.Y;
	if (abs(dx) > abs(dy))
	{
		t = (C.X - A.X) / dx;
	}
	else
	{
		t = (C.Y - A.Y) / dy;
	}
	if (t > 1.0 || t < 0.0)
	{
		int sk = 0;
	}
}
//׷�ٴ��ϲ���ҷ�
void Jie_Ds::TrackRightFault(double Value, IsoLine &Line, IsoLine &IsoReal, vector<vector<double>>&X_Used, vector<vector<double>>&Y_Used)
{
	int Num = (int)Line.Logic.size();
	int i = (int)Line.Logic[Num - 1].X;
	int j = (int)Line.Logic[Num - 1].Y;
	int index = m_NoFault[i][j];
	GridFL Lines = m_vecGridFL[index];


	TWOVALUE StarA;
	double Mark = abs(Line.Logic[Num - 1].Y - j);
	StarA.X = (1 - Mark) * m_GridPoint[i][j].X + Mark * (m_GridPoint[i][j + 1].X);
	StarA.Y = (1 - Mark) * m_GridPoint[i][j].Y + Mark * (m_GridPoint[i][j + 1].Y);

	THRVALUE Star;
	Star.X = StarA.X;
	Star.Y = StarA.Y;
	Star.Z = Value;
	//ʹ�÷ֿ鷽��  20130917
	TWOVALUE Point;
	THRVALUE GridFault[6];
	GridFault[0] = Star;
	GridFault[1] = m_GridPoint[i][j];
	GridFault[2] = m_GridPoint[i + 1][j];
	GridFault[3] = m_GridPoint[i + 1][j + 1];
	GridFault[4] = m_GridPoint[i][j + 1];
	GridFault[5] = Star;

	if (!GetVirtualValue(GridFault, Lines, Point))
	{
		IsoReal.Logic.push_back(Point);
		return;
	}

	int indexi = 0;
	THRVALUE tempt;
	tempt.X = Point.X;
	tempt.Y = Point.Y;
	tempt.Z = Value;
	GetPtPs(GridFault, tempt, indexi);		//�õ������Ӧ���߶Σ����һ�ϲ���λ��
	if (indexi == 0)
	{
		//IsoReal.Logic.push_back(Point);
		return;
	}
	if (indexi == 1)
	{
		if (X_Used[i][j] == 1)
			return;
		double t = 0;
		GetT(m_GridPoint[i][j], m_GridPoint[i + 1][j], tempt, t);
		TWOVALUE tmpV;
		tmpV.X = i + t;
		tmpV.Y = j;
		Line.Logic.push_back(tmpV);
		X_Used[i][j] = 1;

		IsoReal.Logic.push_back(Point);

		TrackDown(Value, Line, IsoReal, X_Used, Y_Used);
	}
	else if (indexi == 2)
	{
		if (Y_Used[i + 1][j] == 1)
			return;
		double t = 0;
		GetT(m_GridPoint[i + 1][j], m_GridPoint[i + 1][j + 1], tempt, t);
		TWOVALUE tmpV;
		tmpV.X = i + 1;
		tmpV.Y = j + t;
		Line.Logic.push_back(tmpV);
		Y_Used[i + 1][j] = 1;

		IsoReal.Logic.push_back(Point);
		TrackRight(Value, Line, IsoReal, X_Used, Y_Used);
	}
	else if (indexi == 3)
	{
		if (X_Used[i][j + 1] == 1)
			return;
		double t = 0;
		GetT(m_GridPoint[i][j + 1], m_GridPoint[i + 1][j + 1], tempt, t);
		TWOVALUE tmpV;
		tmpV.X = i + t;
		tmpV.Y = j + 1;
		Line.Logic.push_back(tmpV);
		X_Used[i][j + 1] = 1;

		IsoReal.Logic.push_back(Point);

		TrackUp(Value, Line, IsoReal, X_Used, Y_Used);
	}
	else
	{
		//AfxMessageBox("wrong");
	}
	//TODO:���Ʒ���ֵ	20130913
	//THRVALUE GridIJ[5];
	//GridIJ[0] = m_GridPoint[i][j];
	//GridIJ[1] = m_GridPoint[i+1][j];
	//GridIJ[2] = m_GridPoint[i+1][j+1];
	//GridIJ[3] = m_GridPoint[i][j+1];
	//THRVALUE TempPoint;
	//TempPoint.X = StarA.X;
	//TempPoint.Y = StarA.Y;
	//TempPoint.Z = Value;
	//GetVirtualValue(TempPoint, GridIJ, Lines);

	//double A = GridIJ[0].Z;
	//double B = GridIJ[1].Z;
	//double C = GridIJ[2].Z;
	//double D = GridIJ[3].Z;			//ʹ�����Ʒ��õ�������ֵ

	////double A = m_GridPoint[i][j].Z;
	////double B = m_GridPoint[i+1][j].Z;
	////double C = m_GridPoint[i+1][j+1].Z;
	////double D = m_GridPoint[i][j+1].Z;
	//double Z = (A + B + C + D) / 4;

	//if ((A - Value) * (B - Value) < 0 && (D - Value) * (C - Value) < 0)   //׷������
	//{
	//	if((A - Value) * (B - Value) < 0 &&((A - Value) * (Z - Value) < 0 || ((C - Value) * (Z - Value) < 0)))
	//	{
	//		Mark = abs((Value - A) / (B - A));
	//		EndA.X = (1 - Mark) * m_GridPoint[i][j].X + Mark * (m_GridPoint[i+1][j].X);
	//		EndA.Y = (1 - Mark) * m_GridPoint[i][j].Y + Mark * (m_GridPoint[i+1][j].Y);
	//		//TODO:�õ����㣬���н��㣬�򽫽�����뵽������
	//		TWOVALUE Point;
	//		if (GetCrossFault(StarA,EndA,Lines,Point))
	//		{
	//			IsoReal.Logic.push_back(Point);
	//			return;
	//		}
	//		if(X_Used[i][j] == 1)
	//			return;

	//		TWOVALUE tmpV;
	//		tmpV.X = i + abs((Value - A) / (A - B)) ;
	//		tmpV.Y = j ;	
	//		Line.Logic.push_back(tmpV);
	//		X_Used[i][j] = 1;
	//		//ʵ��ֵ
	//		TWOVALUE tmpR;
	//		GetReallyPoint(tmpV,tmpR);
	//		IsoReal.Logic.push_back(tmpR);

	//		//׷���±�
	//		TrackDown(Value,Line,IsoReal,X_Used,Y_Used);

	//	}
	//	else if((D - Value) * (C - Value) < 0 && ((D - Value) * (Z - Value) < 0 || ((B - Value) * (Z - Value) < 0)))
	//	{
	//		Mark = abs((Value - D) / (C - D));
	//		EndA.X = (1 - Mark) * m_GridPoint[i][j+1].X + Mark * (m_GridPoint[i+1][j+1].X);
	//		EndA.Y = (1 - Mark) * m_GridPoint[i][j+1].Y + Mark * (m_GridPoint[i+1][j+1].Y);
	//		//TODO:�õ����㣬���н��㣬�򽫽�����뵽������
	//		TWOVALUE Point;
	//		if (GetCrossFault(StarA,EndA,Lines,Point))
	//		{
	//			IsoReal.Logic.push_back(Point);
	//			return;
	//		}
	//		if(X_Used[i][j+1] == 1)
	//			return;

	//		TWOVALUE tmpV;
	//		tmpV.X = i + abs((Value - D) / (C - D)) ;
	//		tmpV.Y = j + 1;	
	//		Line.Logic.push_back(tmpV);
	//		X_Used[i][j+1] = 1;
	//		//ʵ��ֵ
	//		TWOVALUE tmpR;
	//		GetReallyPoint(tmpV,tmpR);
	//		IsoReal.Logic.push_back(tmpR);

	//		//׷���ϱ�
	//		TrackUp(Value,Line,IsoReal,X_Used,Y_Used);

	//	}

	//}
	//else    //׷��һ��
	//{
	//	if((A - Value) * (B - Value) < 0 )
	//	{
	//		Mark = abs((Value - A) / (B - A));
	//		EndA.X = (1 - Mark) * m_GridPoint[i][j].X + Mark * (m_GridPoint[i+1][j].X);
	//		EndA.Y = (1 - Mark) * m_GridPoint[i][j].Y + Mark * (m_GridPoint[i+1][j].Y);
	//		//TODO:�õ����㣬���н��㣬�򽫽�����뵽������
	//		TWOVALUE Point;
	//		if (GetCrossFault(StarA,EndA,Lines,Point))
	//		{
	//			IsoReal.Logic.push_back(Point);
	//			return;
	//		}
	//		if(X_Used[i][j] == 1)
	//			return;

	//		TWOVALUE tmpV;
	//		tmpV.X = i + abs((Value - A) / (A - B)) ;
	//		tmpV.Y = j ;	
	//		Line.Logic.push_back(tmpV);
	//		X_Used[i][j] = 1;

	//		//ʵ��ֵ
	//		TWOVALUE tmpR;
	//		GetReallyPoint(tmpV,tmpR);
	//		IsoReal.Logic.push_back(tmpR);

	//		//׷���±�
	//		TrackDown(Value,Line,IsoReal,X_Used,Y_Used);

	//	}
	//	else if((D - Value) * (C - Value) < 0 )
	//	{
	//		Mark = abs((Value - D) / (C - D));
	//		EndA.X = (1 - Mark) * m_GridPoint[i][j+1].X + Mark * (m_GridPoint[i+1][j+1].X);
	//		EndA.Y = (1 - Mark) * m_GridPoint[i][j+1].Y + Mark * (m_GridPoint[i+1][j+1].Y);
	//		//TODO:�õ����㣬���н��㣬�򽫽�����뵽������
	//		TWOVALUE Point;
	//		if (GetCrossFault(StarA,EndA,Lines,Point))
	//		{
	//			IsoReal.Logic.push_back(Point);
	//			return;
	//		}
	//		if(X_Used[i][j+1] == 1)
	//			return;

	//		TWOVALUE tmpV;
	//		tmpV.X = i + abs((Value - D) / (C - D)) ;
	//		tmpV.Y = j + 1;	
	//		Line.Logic.push_back(tmpV);
	//		X_Used[i][j+1] = 1;
	//		//ʵ��ֵ
	//		TWOVALUE tmpR;
	//		GetReallyPoint(tmpV,tmpR);
	//		IsoReal.Logic.push_back(tmpR);

	//		//׷���ϱ�
	//		TrackUp(Value,Line,IsoReal,X_Used,Y_Used);

	//	}
	//	else if ((B - Value)*(C- Value) < 0)
	//	{
	//		Mark = abs((Value - B) / (B - C));
	//		EndA.X = (1 - Mark) * m_GridPoint[i+1][j].X + Mark * (m_GridPoint[i+1][j+1].X);
	//		EndA.Y = (1 - Mark) * m_GridPoint[i+1][j].Y + Mark * (m_GridPoint[i+1][j+1].Y);
	//		//TODO:�õ����㣬���н��㣬�򽫽�����뵽������
	//		TWOVALUE Point;
	//		if (GetCrossFault(StarA,EndA,Lines,Point))
	//		{
	//			IsoReal.Logic.push_back(Point);
	//			return;
	//		}
	//		if(Y_Used[i+1][j] == 1)
	//			return;
	//		TWOVALUE tmpV;
	//		tmpV.X = i + 1;
	//		tmpV.Y = j + abs((Value - B)/(C - B));
	//		Line.Logic.push_back(tmpV);
	//		Y_Used[i+1][j] = 1;
	//		//ʵ��ֵ
	//		TWOVALUE tmpR;
	//		GetReallyPoint(tmpV,tmpR);
	//		IsoReal.Logic.push_back(tmpR);

	//		//����׷��
	//		TrackRight(Value,Line,IsoReal,X_Used,Y_Used);

	//	}
	//}
}
//����׷�٣������ں��жϲ�
void Jie_Ds::TrackDownFault(double Value, IsoLine &Line, IsoLine &IsoReal, vector<vector<double>>&X_Used, vector<vector<double>>&Y_Used)
{
	int Num = (int)Line.Logic.size();
	int i = (int)Line.Logic[Num - 1].X;
	int j = (int)Line.Logic[Num - 1].Y;
	int index = m_NoFault[i][j - 1];
	GridFL Lines = m_vecGridFL[index];

	TWOVALUE StarA;
	double Mark = abs(Line.Logic[Num - 1].X - i);
	StarA.X = (1 - Mark) * m_GridPoint[i][j].X + Mark * (m_GridPoint[i + 1][j].X);
	StarA.Y = (1 - Mark) * m_GridPoint[i][j].Y + Mark * (m_GridPoint[i + 1][j].Y);
	THRVALUE Star;
	Star.X = StarA.X;
	Star.Y = StarA.Y;
	Star.Z = Value;
	//ʹ�÷ֿ鷽��  20130917
	TWOVALUE Point;
	THRVALUE GridFault[6];
	//
	GridFault[0] = Star;
	GridFault[1] = m_GridPoint[i][j];
	GridFault[2] = m_GridPoint[i][j - 1];
	GridFault[3] = m_GridPoint[i + 1][j - 1];
	GridFault[4] = m_GridPoint[i + 1][j];
	GridFault[5] = Star;
	//�����ж��Ƿ�׷�ٵ������ڵĶϲ����ϣ�����׷�ٵ��ϲ����ϣ���ֹ׷��
	if (!GetVirtualValue(GridFault, Lines, Point))
	{
		IsoReal.Logic.push_back(Point);
		return;
	}
	int indexi = 0;
	THRVALUE tempt;
	tempt.X = Point.X;
	tempt.Y = Point.Y;
	tempt.Z = Value;
	GetPtPs(GridFault, tempt, indexi);		//�õ������Ӧ���߶Σ����һ�ϲ���λ��
	if (indexi == 0)
	{
		//IsoReal.Logic.push_back(Point);
		return;
	}
	if (indexi == 1)
	{
		if (Y_Used[i][j - 1] == 1)
			return;
		double t = 0;
		GetT(m_GridPoint[i][j], m_GridPoint[i][j - 1], tempt, t);
		TWOVALUE tmpV;
		tmpV.X = i;
		tmpV.Y = j - t;
		Line.Logic.push_back(tmpV);
		Y_Used[i][j - 1] = 1;

		IsoReal.Logic.push_back(Point);

		TrackLeft(Value, Line, IsoReal, X_Used, Y_Used);
	}
	else if (indexi == 2)
	{
		if (X_Used[i][j - 1] == 1)
			return;
		double t = 0;
		GetT(m_GridPoint[i][j - 1], m_GridPoint[i + 1][j - 1], tempt, t);
		TWOVALUE tmpV;
		tmpV.X = i + t;
		tmpV.Y = j - 1;
		Line.Logic.push_back(tmpV);
		X_Used[i][j - 1] = 1;

		IsoReal.Logic.push_back(Point);
		TrackDown(Value, Line, IsoReal, X_Used, Y_Used);
	}
	else if (indexi == 3)
	{
		if (Y_Used[i + 1][j - 1] == 1)
			return;
		double t = 0;
		GetT(m_GridPoint[i + 1][j], m_GridPoint[i + 1][j - 1], tempt, t);
		TWOVALUE tmpV;
		tmpV.X = i + 1;
		tmpV.Y = j - t;
		Line.Logic.push_back(tmpV);
		Y_Used[i + 1][j - 1] = 1;

		IsoReal.Logic.push_back(Point);

		TrackRight(Value, Line, IsoReal, X_Used, Y_Used);
	}
	////TODO:���Ʒ���ֵ	20130913
	//THRVALUE GridIJ[5];
	//GridIJ[0] = m_GridPoint[i][j];
	//GridIJ[1] = m_GridPoint[i+1][j];
	//GridIJ[2] = m_GridPoint[i+1][j-1];
	//GridIJ[3] = m_GridPoint[i][j-1];
	//THRVALUE TempPoint;
	//TempPoint.X = StarA.X;
	//TempPoint.Y = StarA.Y;
	//TempPoint.Z = Value;
	//GetVirtualValue(TempPoint, GridIJ, Lines);

	//double A = GridIJ[0].Z;
	//double B = GridIJ[1].Z;
	//double C = GridIJ[2].Z;
	//double D = GridIJ[3].Z;			//ʹ�����Ʒ��õ�������ֵ

	////double A = m_GridPoint[i][j].Z;
	////double B = m_GridPoint[i+1][j].Z;
	////double C = m_GridPoint[i+1][j-1].Z;
	////double D = m_GridPoint[i][j-1].Z;
	//double Z = (A + B + C + D) / 4;

	//if ((A - Value) * (D - Value) < 0  && (B - Value) * (C - Value) < 0)  //׷������
	//{
	//	if((A - Value) * (D - Value) < 0  && ((A - Value) * (Z - Value) < 0 || ((C - Value) * (Z - Value) < 0)))
	//	{

	//		Mark = abs((Value - D) / (D - A));
	//		EndA.X = (1 - Mark) * m_GridPoint[i][j-1].X + Mark * (m_GridPoint[i][j].X);
	//		EndA.Y = (1 - Mark) * m_GridPoint[i][j-1].Y + Mark * (m_GridPoint[i][j].Y);
	//		//TODO:�õ����㣬���н��㣬�򽫽�����뵽������
	//		TWOVALUE Point;
	//		if (GetCrossFault(StarA,EndA,Lines,Point))
	//		{
	//			IsoReal.Logic.push_back(Point);
	//			return;
	//		}

	//		if(Y_Used[i][j-1] == 1)
	//			return;
	//		TWOVALUE tmpV;
	//		tmpV.X = i;
	//		tmpV.Y = j - abs((Value - A) / (D - A));	
	//		Line.Logic.push_back(tmpV);
	//		Y_Used[i][j-1] = 1;

	//		//ʵ��ֵ
	//		TWOVALUE tmpR;
	//		GetReallyPoint(tmpV,tmpR);
	//		IsoReal.Logic.push_back(tmpR);

	//		//׷�����
	//		TrackLeft(Value,Line,IsoReal,X_Used,Y_Used);
	//	}
	//	else if((B - Value) * (C - Value) < 0  && ((B - Value) * (Z - Value) < 0 || ((D - Value) * (Z - Value) < 0)))
	//	{
	//		Mark = abs((Value - C) / (C - B));
	//		EndA.X = (1 - Mark) * m_GridPoint[i+1][j-1].X + Mark * (m_GridPoint[i+1][j].X);
	//		EndA.Y = (1 - Mark) * m_GridPoint[i+1][j-1].Y + Mark * (m_GridPoint[i+1][j].Y);
	//		//TODO:�õ����㣬���н��㣬�򽫽�����뵽������
	//		TWOVALUE Point;
	//		if (GetCrossFault(StarA,EndA,Lines,Point))
	//		{
	//			IsoReal.Logic.push_back(Point);
	//			return;
	//		}

	//		if(Y_Used[i+1][j-1] == 1)
	//			return;
	//		TWOVALUE tmpV;
	//		tmpV.X = i + 1;
	//		tmpV.Y = j - abs((Value - B) / (C - B));	
	//		Line.Logic.push_back(tmpV);
	//		Y_Used[i+1][j-1] = 1;

	//		//ʵ��ֵ
	//		TWOVALUE tmpR;
	//		GetReallyPoint(tmpV,tmpR);
	//		IsoReal.Logic.push_back(tmpR);

	//		//׷���ұ�
	//		TrackRight(Value,Line,IsoReal,X_Used,Y_Used);
	//	}
	//}
	//else   //׷��һ��
	//{
	//	if((A - Value) * (D - Value) < 0 )
	//	{

	//		Mark = abs((Value - D) / (D - A));
	//		EndA.X = (1 - Mark) * m_GridPoint[i][j-1].X + Mark * (m_GridPoint[i][j].X);
	//		EndA.Y = (1 - Mark) * m_GridPoint[i][j-1].Y + Mark * (m_GridPoint[i][j].Y);
	//		//TODO:�õ����㣬���н��㣬�򽫽�����뵽������
	//		TWOVALUE Point;
	//		if (GetCrossFault(StarA,EndA,Lines,Point))
	//		{
	//			IsoReal.Logic.push_back(Point);
	//			return;
	//		}

	//		if(Y_Used[i][j-1] == 1)
	//			return;
	//		TWOVALUE tmpV;
	//		tmpV.X = i;
	//		tmpV.Y = j - abs((Value - A) / (D - A));	
	//		Line.Logic.push_back(tmpV);
	//		Y_Used[i][j-1] = 1;

	//		//ʵ��ֵ
	//		TWOVALUE tmpR;
	//		GetReallyPoint(tmpV,tmpR);
	//		IsoReal.Logic.push_back(tmpR);

	//		//׷�����
	//		TrackLeft(Value,Line,IsoReal,X_Used,Y_Used);
	//	}
	//	else if((B - Value) * (C - Value) < 0  )
	//	{
	//		Mark = abs((Value - C) / (C - B));
	//		EndA.X = (1 - Mark) * m_GridPoint[i+1][j-1].X + Mark * (m_GridPoint[i+1][j].X);
	//		EndA.Y = (1 - Mark) * m_GridPoint[i+1][j-1].Y + Mark * (m_GridPoint[i+1][j].Y);
	//		//TODO:�õ����㣬���н��㣬�򽫽�����뵽������
	//		TWOVALUE Point;
	//		if (GetCrossFault(StarA,EndA,Lines,Point))
	//		{
	//			IsoReal.Logic.push_back(Point);
	//			return;
	//		}

	//		if(Y_Used[i+1][j-1] == 1)
	//			return;
	//		TWOVALUE tmpV;
	//		tmpV.X = i + 1;
	//		tmpV.Y = j - abs((Value - B) / (C - B));	
	//		Line.Logic.push_back(tmpV);
	//		Y_Used[i+1][j-1] = 1;

	//		//ʵ��ֵ
	//		TWOVALUE tmpR;
	//		GetReallyPoint(tmpV,tmpR);
	//		IsoReal.Logic.push_back(tmpR);

	//		//׷���ұ�
	//		TrackRight(Value,Line,IsoReal,X_Used,Y_Used);
	//	}
	//	else if ((D - Value)*(C- Value) < 0)
	//	{
	//		Mark = abs((Value - D) / (C - D));
	//		EndA.X = (1 - Mark) * m_GridPoint[i][j-1].X + Mark * (m_GridPoint[i+1][j-1].X);
	//		EndA.Y = (1 - Mark) * m_GridPoint[i][j-1].Y + Mark * (m_GridPoint[i+1][j-1].Y);
	//		//TODO:�õ����㣬���н��㣬�򽫽�����뵽������
	//		TWOVALUE Point;
	//		if (GetCrossFault(StarA,EndA,Lines,Point))
	//		{
	//			IsoReal.Logic.push_back(Point);
	//			return;
	//		}

	//		if(X_Used[i][j-1] == 1)
	//		{
	//			return;
	//		}

	//		TWOVALUE tmpV;
	//		tmpV.X = i + abs((Value - D)/(C - D));
	//		tmpV.Y = j - 1;
	//		Line.Logic.push_back(tmpV);
	//		X_Used[i][j-1] = 1;

	//		//ʵ��ֵ
	//		TWOVALUE tmpR;
	//		GetReallyPoint(tmpV,tmpR);
	//		IsoReal.Logic.push_back(tmpR);

	//		//����׷��
	//		TrackDown(Value,Line,IsoReal,X_Used,Y_Used);
	//	}
	//}
}
//׷�ٴ��ϲ���·�
void Jie_Ds::TrackLeftFault(double Value, IsoLine &Line, IsoLine &IsoReal, vector<vector<double>>&X_Used, vector<vector<double>>&Y_Used)
{
	int Num = (int)Line.Logic.size();
	int i = (int)Line.Logic[Num - 1].X;
	int j = (int)Line.Logic[Num - 1].Y;

	TWOVALUE StarA;
	double Mark = abs(Line.Logic[Num - 1].Y - j);
	StarA.X = (1 - Mark) * m_GridPoint[i][j].X + Mark * (m_GridPoint[i][j + 1].X);
	StarA.Y = (1 - Mark) * m_GridPoint[i][j].Y + Mark * (m_GridPoint[i][j + 1].Y);
	THRVALUE Star;
	Star.X = StarA.X;
	Star.Y = StarA.Y;
	Star.Z = Value;
	int index = m_NoFault[i - 1][j];
	GridFL Lines = m_vecGridFL[index];
	//ʹ�÷ֿ鷽��  20130917
	TWOVALUE Point;
	THRVALUE GridFault[6];
	GridFault[0] = Star;
	GridFault[1] = m_GridPoint[i][j];
	GridFault[2] = m_GridPoint[i - 1][j];
	GridFault[3] = m_GridPoint[i - 1][j + 1];
	GridFault[4] = m_GridPoint[i][j + 1];
	GridFault[5] = Star;
	if (!GetVirtualValue(GridFault, Lines, Point))
	{
		IsoReal.Logic.push_back(Point);
		return;
	}
	int indexi = 0;
	THRVALUE tempt;
	tempt.X = Point.X;
	tempt.Y = Point.Y;
	tempt.Z = Value;
	//TODO�����㲻һ���ھ���֮�䡤�������ڶϲ��������֮�䡤������Ҫ����GetVirtualValue���ص�Point�����ж�20140724
	GetPtPs(GridFault, tempt, indexi);		//�õ������Ӧ���߶Σ����һ�ϲ���λ��
	if (indexi == 0)
	{
		//IsoReal.Logic.push_back(Point);
		return;
	}
	if (indexi == 1)
	{
		if (X_Used[i - 1][j] == 1)
			return;
		double t = 0;
		GetT(m_GridPoint[i][j], m_GridPoint[i - 1][j], tempt, t);
		TWOVALUE tmpV;
		tmpV.X = i - t;
		tmpV.Y = j;
		Line.Logic.push_back(tmpV);
		X_Used[i - 1][j] = 1;

		IsoReal.Logic.push_back(Point);

		TrackDown(Value, Line, IsoReal, X_Used, Y_Used);
	}
	else if (indexi == 2)
	{
		if (Y_Used[i - 1][j] == 1)
			return;
		double t = 0;
		GetT(m_GridPoint[i - 1][j], m_GridPoint[i - 1][j + 1], tempt, t);
		TWOVALUE tmpV;
		tmpV.X = i - 1;
		tmpV.Y = j + t;
		Line.Logic.push_back(tmpV);
		Y_Used[i - 1][j] = 1;

		IsoReal.Logic.push_back(Point);
		TrackLeft(Value, Line, IsoReal, X_Used, Y_Used);
	}
	else if (indexi == 3)
	{
		if (X_Used[i - 1][j + 1] == 1)
			return;
		double t = 0;
		GetT(m_GridPoint[i][j + 1], m_GridPoint[i - 1][j + 1], tempt, t);
		TWOVALUE tmpV;
		tmpV.X = i - t;
		tmpV.Y = j + 1;
		Line.Logic.push_back(tmpV);
		X_Used[i - 1][j + 1] = 1;

		IsoReal.Logic.push_back(Point);

		TrackUp(Value, Line, IsoReal, X_Used, Y_Used);
	}
}
//׷�ٴ��ϲ���Ϸ�
void Jie_Ds::TrackUPFault(double Value, IsoLine &Line, IsoLine &IsoReal, vector<vector<double>>&X_Used, vector<vector<double>>&Y_Used)
{
	int Num = (int)Line.Logic.size();
	int i = (int)Line.Logic[Num - 1].X;
	int j = (int)Line.Logic[Num - 1].Y;
	//�����ڶϲ��ߵ��ߺ�
	int index = m_NoFault[i][j];
	//���ߺŵõ��ϲ�������
	GridFL Lines = m_vecGridFL[index];

	TWOVALUE StarA;
	//���������ϵĵ�ֵ���ƫ����
	double Mark = abs(Line.Logic[Num - 1].X - i);
	//�ڴ��������Ͻ������Բ�ֵ�õ���ֵ��
	StarA.X = (1 - Mark) * m_GridPoint[i][j].X + Mark * (m_GridPoint[i + 1][j].X);
	StarA.Y = (1 - Mark) * m_GridPoint[i][j].Y + Mark * (m_GridPoint[i + 1][j].Y);
	THRVALUE Star;
	Star.X = StarA.X;
	Star.Y = StarA.Y;
	Star.Z = Value;

	//ʹ�÷ֿ鷽��  20130917
	TWOVALUE Point;
	THRVALUE GridFault[6];
	GridFault[0] = Star;
	GridFault[1] = m_GridPoint[i][j];
	GridFault[2] = m_GridPoint[i][j + 1];
	GridFault[3] = m_GridPoint[i + 1][j + 1];
	GridFault[4] = m_GridPoint[i + 1][j];
	GridFault[5] = Star;
	//�������˼��׷�ٵ������ڵĶϲ������棬��ֹ׷�٣�����Ҫ��ȥ�ж����ĸ��������׷��
	if (!GetVirtualValue(GridFault, Lines, Point))
	{
		IsoReal.Logic.push_back(Point);
		return;
	}
	int indexi = 0;
	//Ҫ׷�ٵĵ�ֵ��
	THRVALUE tempt;
	tempt.X = Point.X;
	tempt.Y = Point.Y;
	tempt.Z = Value;
	GetPtPs(GridFault, tempt, indexi);		//�õ������Ӧ���߶Σ����һ�ϲ���λ��
	//����Ӧ�þ��ǵ�ֵ��������ߣ����Ǵ�������ڶϲ��ڲ��������������ڻ�Ҫ������
	if (indexi == 0)
	{
		//IsoReal.Logic.push_back(Point);
		return;
	}
	if (indexi == 1)
	{
		if (Y_Used[i][j] == 1)
			return;
		double t = 0;
		GetT(m_GridPoint[i][j], m_GridPoint[i][j + 1], tempt, t);
		TWOVALUE tmpV;
		tmpV.X = i;
		tmpV.Y = j + t;
		Line.Logic.push_back(tmpV);
		Y_Used[i][j] = 1;

		IsoReal.Logic.push_back(Point);

		TrackLeft(Value, Line, IsoReal, X_Used, Y_Used);
	}
	else if (indexi == 2)
	{
		if (X_Used[i][j + 1] == 1)
			return;
		double t = 0;
		GetT(m_GridPoint[i][j + 1], m_GridPoint[i + 1][j + 1], tempt, t);
		TWOVALUE tmpV;
		tmpV.X = i + t;
		tmpV.Y = j + 1;
		Line.Logic.push_back(tmpV);
		X_Used[i][j + 1] = 1;

		IsoReal.Logic.push_back(Point);
		TrackUp(Value, Line, IsoReal, X_Used, Y_Used);
	}
	else if (indexi == 3)
	{
		if (Y_Used[i + 1][j] == 1)
			return;
		double t = 0;
		THRVALUE A = m_GridPoint[i + 1][j];
		THRVALUE B = m_GridPoint[i + 1][j + 1];
		GetT(A, B, tempt, t);
		//GetT(m_GridPoint[i+1][j],m_GridPoint[i+1][j+1],tempt,t);
		TWOVALUE tmpV;
		tmpV.X = i + 1;
		tmpV.Y = j + t;
		Line.Logic.push_back(tmpV);
		Y_Used[i + 1][j] = 1;

		IsoReal.Logic.push_back(Point);

		TrackRight(Value, Line, IsoReal, X_Used, Y_Used);
	}

	////TODO:���Ʒ���ֵ	20130913
	//THRVALUE GridIJ[5];
	//GridIJ[0] = m_GridPoint[i][j];
	//GridIJ[1] = m_GridPoint[i+1][j];
	//GridIJ[2] = m_GridPoint[i+1][j+1];
	//GridIJ[3] = m_GridPoint[i][j+1];
	//THRVALUE TempPoint;
	//TempPoint.X = StarA.X;
	//TempPoint.Y = StarA.Y;
	//TempPoint.Z = Value;
	//GetVirtualValue(TempPoint, GridIJ, Lines);

	//double A = GridIJ[0].Z;
	//double B = GridIJ[1].Z;
	//double C = GridIJ[2].Z;
	//double D = GridIJ[3].Z;			//ʹ�����Ʒ��õ�������ֵ

	////double A = m_GridPoint[i][j].Z;
	////double B = m_GridPoint[i+1][j].Z;
	////double C = m_GridPoint[i+1][j+1].Z;
	////double D = m_GridPoint[i][j+1].Z;
	//double Z = (A + B + C + D) / 4;

	//if ((A - Value) * (D - Value) < 0 && (B - Value) * (C - Value) < 0)//׷������
	//{
	//	if((A - Value) * (D - Value) < 0  && ((A - Value) * (Z - Value) < 0 || ((C - Value) * (Z - Value) < 0)))
	//	{

	//		Mark = abs((Value - A) / (D - A));
	//		EndA.X = (1 - Mark) * m_GridPoint[i][j].X + Mark * (m_GridPoint[i][j+1].X);
	//		EndA.Y = (1 - Mark) * m_GridPoint[i][j].Y + Mark * (m_GridPoint[i][j+1].Y);
	//		//TODO:�õ����㣬���н��㣬�򽫽�����뵽������
	//		TWOVALUE Point;
	//		if (GetCrossFault(StarA,EndA,Lines,Point))
	//		{
	//			IsoReal.Logic.push_back(Point);
	//			return;
	//		}

	//		if(Y_Used[i][j] == 1)
	//			return;
	//		TWOVALUE tmpV;
	//		tmpV.X = i ;
	//		tmpV.Y = j + abs((Value - A) / (D - A));	
	//		Line.Logic.push_back(tmpV);	
	//		Y_Used[i][j] = 1;

	//		//ʵ��ֵ
	//		TWOVALUE tmpR;
	//		GetReallyPoint(tmpV,tmpR);
	//		IsoReal.Logic.push_back(tmpR);

	//		//׷�����
	//		TrackLeft(Value,Line,IsoReal,X_Used,Y_Used);

	//	}
	//	else if((B - Value) * (C - Value) < 0  && ((B - Value) * (Z - Value) < 0 || ((D - Value) * (Z - Value) < 0)))
	//	{
	//		Mark = abs((Value - B) / (C - B));
	//		EndA.X = (1 - Mark) * m_GridPoint[i+1][j].X + Mark * (m_GridPoint[i+1][j+1].X);
	//		EndA.Y = (1 - Mark) * m_GridPoint[i+1][j].Y + Mark * (m_GridPoint[i+1][j+1].Y);
	//		//TODO:�õ����㣬���н��㣬�򽫽�����뵽������
	//		TWOVALUE Point;
	//		if (GetCrossFault(StarA,EndA,Lines,Point))
	//		{
	//			IsoReal.Logic.push_back(Point);
	//			return;
	//		}

	//		if(Y_Used[i+1][j] == 1)
	//			return;
	//		TWOVALUE tmpV;
	//		tmpV.X = i + 1;
	//		tmpV.Y = j + abs((Value - B) / (C - B));	
	//		Line.Logic.push_back(tmpV);
	//		Y_Used[i+1][j] = 1;

	//		//ʵ��ֵ
	//		TWOVALUE tmpR;
	//		GetReallyPoint(tmpV,tmpR);
	//		IsoReal.Logic.push_back(tmpR);

	//		//׷���ұ�
	//		TrackRight(Value,Line,IsoReal,X_Used,Y_Used);
	//	}
	//}
	//else			//׷��һ��
	//{
	//	if((A - Value) * (D - Value) < 0 )
	//	{

	//		Mark = abs((Value - A) / (D - A));
	//		EndA.X = (1 - Mark) * m_GridPoint[i][j].X + Mark * (m_GridPoint[i][j+1].X);
	//		EndA.Y = (1 - Mark) * m_GridPoint[i][j].Y + Mark * (m_GridPoint[i][j+1].Y);
	//		//TODO:�õ����㣬���н��㣬�򽫽�����뵽������
	//		TWOVALUE Point;
	//		if (GetCrossFault(StarA,EndA,Lines,Point))
	//		{
	//			IsoReal.Logic.push_back(Point);
	//			return;
	//		}

	//		if(Y_Used[i][j] == 1)
	//			return;
	//		TWOVALUE tmpV;
	//		tmpV.X = i ;
	//		tmpV.Y = j + abs((Value - A) / (D - A));	
	//		Line.Logic.push_back(tmpV);	
	//		Y_Used[i][j] = 1;

	//		//ʵ��ֵ
	//		TWOVALUE tmpR;
	//		GetReallyPoint(tmpV,tmpR);
	//		IsoReal.Logic.push_back(tmpR);

	//		//׷�����
	//		TrackLeft(Value,Line,IsoReal,X_Used,Y_Used);

	//	}
	//	else if((B - Value) * (C - Value) < 0  )
	//	{
	//		Mark = abs((Value - B) / (C - B));
	//		EndA.X = (1 - Mark) * m_GridPoint[i+1][j].X + Mark * (m_GridPoint[i+1][j+1].X);
	//		EndA.Y = (1 - Mark) * m_GridPoint[i+1][j].Y + Mark * (m_GridPoint[i+1][j+1].Y);
	//		//TODO:�õ����㣬���н��㣬�򽫽�����뵽������
	//		TWOVALUE Point;
	//		if (GetCrossFault(StarA,EndA,Lines,Point))
	//		{
	//			IsoReal.Logic.push_back(Point);
	//			return;
	//		}

	//		if(Y_Used[i+1][j] == 1)
	//			return;
	//		TWOVALUE tmpV;
	//		tmpV.X = i + 1;
	//		tmpV.Y = j + abs((Value - B) / (C - B));	
	//		Line.Logic.push_back(tmpV);
	//		Y_Used[i+1][j] = 1;

	//		//ʵ��ֵ
	//		TWOVALUE tmpR;
	//		GetReallyPoint(tmpV,tmpR);
	//		IsoReal.Logic.push_back(tmpR);

	//		//׷���ұ�
	//		TrackRight(Value,Line,IsoReal,X_Used,Y_Used);
	//	}
	//	else if ((D - Value)*(C- Value) < 0)
	//	{
	//		Mark = abs((Value - D) / (C - D));
	//		EndA.X = (1 - Mark) * m_GridPoint[i][j+1].X + Mark * (m_GridPoint[i+1][j+1].X);
	//		EndA.Y = (1 - Mark) * m_GridPoint[i][j+1].Y + Mark * (m_GridPoint[i+1][j+1].Y);
	//		//TODO:�õ����㣬���н��㣬�򽫽�����뵽������
	//		TWOVALUE Point;
	//		if (GetCrossFault(StarA,EndA,Lines,Point))
	//		{
	//			IsoReal.Logic.push_back(Point);
	//			return;
	//		}

	//		if(X_Used[i][j+1] == 1)
	//			return;
	//		TWOVALUE tmpV;
	//		tmpV.X = i + abs((Value - D)/(C - D));
	//		tmpV.Y = j + 1;
	//		Line.Logic.push_back(tmpV);
	//		X_Used[i][j+1] = 1;

	//		//ʵ��ֵ
	//		TWOVALUE tmpR;
	//		GetReallyPoint(tmpV,tmpR);
	//		IsoReal.Logic.push_back(tmpR);

	//		//����׷��
	//		TrackUp(Value,Line,IsoReal,X_Used,Y_Used);
	//	}
	//}
}

void Jie_Ds::TrackRight(double Value, IsoLine &Line, IsoLine &IsoReal, vector<vector<double>>&X_Used, vector<vector<double>>&Y_Used)
{
	int Num = (int)Line.Logic.size();
	int i = (int)Line.Logic[Num - 1].X;
	int j = (int)Line.Logic[Num - 1].Y;
	double A = m_GridPoint[i][j].Z;
	double B = m_GridPoint[i + 1][j].Z;
	double C = m_GridPoint[i + 1][j + 1].Z;
	double D = m_GridPoint[i][j + 1].Z;
	double Z = (A + B + C + D) / 4;
	if (m_NoFault[i][j]>-1)					//�������жϲ�
	{
		TrackRightFault(Value, Line, IsoReal, X_Used, Y_Used);//׷��������
		return;
	}
	//����׷��
	if ((A - Value) * (B - Value) < 0 && (D - Value) * (C - Value) < 0) //׷��������ֵ��
	{
		if (X_Used[i][j] == 0 && ((A - Value) * (Z - Value) < 0 || ((C - Value) * (Z - Value) < 0)))
		{
			TWOVALUE tmpV;
			tmpV.X = i + abs((Value - A) / (A - B));
			tmpV.Y = j;
			Line.Logic.push_back(tmpV);
			X_Used[i][j] = 1;

			//ʵ��ֵ
			TWOVALUE tmpR;
			GetReallyPoint(tmpV, tmpR);
			IsoReal.Logic.push_back(tmpR);

			//׷���±�
			TrackDown(Value, Line, IsoReal, X_Used, Y_Used);
		}
		else if (X_Used[i][j + 1] == 0 && ((D - Value) * (Z - Value) < 0 || ((B - Value) * (Z - Value) < 0)))
		{
			TWOVALUE tmpV;
			tmpV.X = i + abs((Value - D) / (C - D));
			tmpV.Y = j + 1;
			Line.Logic.push_back(tmpV);
			X_Used[i][j + 1] = 1;

			//ʵ��ֵ
			TWOVALUE tmpR;
			GetReallyPoint(tmpV, tmpR);
			IsoReal.Logic.push_back(tmpR);

			//׷���ϱ�
			TrackUp(Value, Line, IsoReal, X_Used, Y_Used);
		}

	}
	else  //׷��һ����ֵ��
	{
		if ((A - Value) * (B - Value) < 0 && X_Used[i][j] == 0)
		{
			TWOVALUE tmpV;
			tmpV.X = i + abs((Value - A) / (A - B));
			tmpV.Y = j;
			Line.Logic.push_back(tmpV);
			X_Used[i][j] = 1;

			//ʵ��ֵ
			TWOVALUE tmpR;
			GetReallyPoint(tmpV, tmpR);
			IsoReal.Logic.push_back(tmpR);

			//׷���±�
			TrackDown(Value, Line, IsoReal, X_Used, Y_Used);
		}
		else if ((D - Value) * (C - Value) < 0 && X_Used[i][j + 1] == 0)
		{
			TWOVALUE tmpV;
			tmpV.X = i + abs((Value - D) / (C - D));
			tmpV.Y = j + 1;
			Line.Logic.push_back(tmpV);
			X_Used[i][j + 1] = 1;

			//ʵ��ֵ
			TWOVALUE tmpR;
			GetReallyPoint(tmpV, tmpR);
			IsoReal.Logic.push_back(tmpR);

			//׷���ϱ�
			TrackUp(Value, Line, IsoReal, X_Used, Y_Used);
		}
		else if (Y_Used[i + 1][j] == 0 && (B - Value)*(C - Value) < 0)
		{
			TWOVALUE tmpV;
			tmpV.X = i + 1;
			tmpV.Y = j + abs((Value - B) / (C - B));
			Line.Logic.push_back(tmpV);
			Y_Used[i + 1][j] = 1;

			//ʵ��ֵ
			TWOVALUE tmpR;
			GetReallyPoint(tmpV, tmpR);
			IsoReal.Logic.push_back(tmpR);

			//����׷��
			TrackRight(Value, Line, IsoReal, X_Used, Y_Used);
		}
	}
}
void Jie_Ds::TrackDown(double Value, IsoLine &Line, IsoLine &IsoReal, vector<vector<double>>&X_Used, vector<vector<double>>&Y_Used)
{
	int Num = (int)Line.Logic.size();
	int i = (int)Line.Logic[Num - 1].X;
	int j = (int)Line.Logic[Num - 1].Y;
	double A = m_GridPoint[i][j].Z;
	double B = m_GridPoint[i + 1][j].Z;
	double C = m_GridPoint[i + 1][j - 1].Z;
	double D = m_GridPoint[i][j - 1].Z;
	double Z = (A + B + C + D) / 4;
	if (m_NoFault[i][j - 1]>-1)					//�������жϲ�
	{
		TrackDownFault(Value, Line, IsoReal, X_Used, Y_Used);//׷��������
		return;
	}
	//����׷��
	if ((A - Value) * (D - Value) < 0 && (B - Value) * (C - Value) < 0) //׷��������ֵ��
	{
		if (Y_Used[i][j - 1] == 0 && ((A - Value) * (Z - Value) < 0 || ((C - Value) * (Z - Value) < 0)))
		{
			TWOVALUE tmpV;
			tmpV.X = i;
			tmpV.Y = j - abs((Value - A) / (D - A));//�����൱��tmpV.Y = j -1+ abs((Value - D) / (A - D));��Ϊ-1+abs((Value - D) / (A - D))=-abs((Value - A) / (D - A))
			Line.Logic.push_back(tmpV);
			Y_Used[i][j - 1] = 1;

			//ʵ��ֵ
			TWOVALUE tmpR;
			GetReallyPoint(tmpV, tmpR);
			IsoReal.Logic.push_back(tmpR);

			//׷�����
			TrackLeft(Value, Line, IsoReal, X_Used, Y_Used);
		}
		else if (Y_Used[i + 1][j - 1] == 0 && ((B - Value) * (Z - Value) < 0 || ((D - Value) * (Z - Value) < 0)))
		{
			TWOVALUE tmpV;
			tmpV.X = i + 1;
			tmpV.Y = j - abs((Value - B) / (C - B));
			Line.Logic.push_back(tmpV);
			Y_Used[i + 1][j - 1] = 1;

			TWOVALUE tmpR;
			GetReallyPoint(tmpV, tmpR);
			IsoReal.Logic.push_back(tmpR);
			//׷���ұ�
			TrackRight(Value, Line, IsoReal, X_Used, Y_Used);
		}
	}
	else  //׷��һ����ֵ��
	{
		if ((A - Value) * (D - Value) < 0 && Y_Used[i][j - 1] == 0)
		{
			TWOVALUE tmpV;
			tmpV.X = i;
			tmpV.Y = j - abs((Value - A) / (D - A));
			Line.Logic.push_back(tmpV);
			Y_Used[i][j - 1] = 1;

			//ʵ��ֵ
			TWOVALUE tmpR;
			GetReallyPoint(tmpV, tmpR);
			IsoReal.Logic.push_back(tmpR);

			//׷�����
			TrackLeft(Value, Line, IsoReal, X_Used, Y_Used);
		}
		else if ((B - Value) * (C - Value) < 0 && Y_Used[i + 1][j - 1] == 0)
		{
			TWOVALUE tmpV;
			tmpV.X = i + 1;
			tmpV.Y = j - abs((Value - B) / (C - B));
			Line.Logic.push_back(tmpV);
			Y_Used[i + 1][j - 1] = 1;

			TWOVALUE tmpR;
			GetReallyPoint(tmpV, tmpR);
			IsoReal.Logic.push_back(tmpR);
			//׷���ұ�
			TrackRight(Value, Line, IsoReal, X_Used, Y_Used);
		}
		else if (X_Used[i][j - 1] == 0 && (D - Value)*(C - Value) < 0)
		{
			TWOVALUE tmpV;
			tmpV.X = i + abs((Value - D) / (C - D));
			tmpV.Y = j - 1;
			Line.Logic.push_back(tmpV);
			X_Used[i][j - 1] = 1;
			//ʵ��ֵ
			TWOVALUE tmpR;
			GetReallyPoint(tmpV, tmpR);
			IsoReal.Logic.push_back(tmpR);
			//����׷��
			TrackDown(Value, Line, IsoReal, X_Used, Y_Used);
		}
	}
}
void Jie_Ds::TrackLeft(double Value, IsoLine &Line, IsoLine &IsoReal, vector<vector<double>>&X_Used, vector<vector<double>>&Y_Used)
{
	int Num = (int)Line.Logic.size();
	int i = (int)Line.Logic[Num - 1].X;
	int j = (int)Line.Logic[Num - 1].Y;
	double A = m_GridPoint[i][j].Z;
	double B = m_GridPoint[i - 1][j].Z;
	double C = m_GridPoint[i - 1][j + 1].Z;
	double D = m_GridPoint[i][j + 1].Z;
	//cout << "v " << m_GridPoint[i][j].X << " " << m_GridPoint[i][j].Y << " " << "0" << endl;
	//cout << "v " << m_GridPoint[i-1][j].X << " " << m_GridPoint[i-1][j].Y << " " << "0" << endl;
	//cout << "v " << m_GridPoint[i-1][j+1].X << " " << m_GridPoint[i-1][j+1].Y << " " << "0" << endl;
	//cout << "v " << m_GridPoint[i][j+1].X << " " << m_GridPoint[i][j+1].Y << " " << "0" << endl;
	double Z = (A + B + C + D) / 4;
	if (m_NoFault[i - 1][j]>-1)					//�������жϲ�
	{
		TrackLeftFault(Value, Line, IsoReal, X_Used, Y_Used);//׷��������
		return;
	}
	//����׷��
	if ((A - Value) * (B - Value) < 0 && (D - Value) * (C - Value) < 0) //׷��������
	{
		if (X_Used[i - 1][j] == 0 && ((A - Value) * (Z - Value) < 0 || ((C - Value) * (Z - Value) < 0)))
		{
			TWOVALUE tmpV;
			tmpV.X = i - abs((Value - A) / (B - A));//�����൱��tmpV.X = i -1+ abs((Value - B) / (A - B));��Ϊ-1+abs((Value - B) / (A - B))=-abs((Value - A) / (B - A))
			tmpV.Y = j;
			Line.Logic.push_back(tmpV);
			X_Used[i - 1][j] = 1;

			//ʵ��ֵ
			TWOVALUE tmpR;
			GetReallyPoint(tmpV, tmpR);
			IsoReal.Logic.push_back(tmpR);

			//׷���±�
			TrackDown(Value, Line, IsoReal, X_Used, Y_Used);
		}
		else if (X_Used[i - 1][j + 1] == 0 && ((B - Value) * (Z - Value) < 0 || ((D - Value) * (Z - Value) < 0)))
		{
			TWOVALUE tmpV;
			tmpV.X = i - abs((Value - D) / (C - D));
			tmpV.Y = j + 1;
			Line.Logic.push_back(tmpV);
			X_Used[i - 1][j + 1] = 1;

			//ʵ��ֵ
			TWOVALUE tmpR;
			GetReallyPoint(tmpV, tmpR);
			IsoReal.Logic.push_back(tmpR);

			//׷���ϱ�
			TrackUp(Value, Line, IsoReal, X_Used, Y_Used);
		}
	}
	else    //׷��һ����
	{
		if ((A - Value) * (B - Value) < 0 && X_Used[i - 1][j] == 0)
		{
			TWOVALUE tmpV;
			tmpV.X = i - abs((Value - A) / (B - A));
			tmpV.Y = j;
			Line.Logic.push_back(tmpV);
			X_Used[i - 1][j] = 1;

			//ʵ��ֵ
			TWOVALUE tmpR;
			GetReallyPoint(tmpV, tmpR);
			IsoReal.Logic.push_back(tmpR);

			//׷���±�
			TrackDown(Value, Line, IsoReal, X_Used, Y_Used);
		}
		else if ((D - Value) * (C - Value) < 0 && X_Used[i - 1][j + 1] == 0)
		{
			TWOVALUE tmpV;
			tmpV.X = i - abs((Value - D) / (C - D));
			tmpV.Y = j + 1;
			Line.Logic.push_back(tmpV);
			X_Used[i - 1][j + 1] = 1;

			//ʵ��ֵ
			TWOVALUE tmpR;
			GetReallyPoint(tmpV, tmpR);
			IsoReal.Logic.push_back(tmpR);

			//׷���ϱ�
			TrackUp(Value, Line, IsoReal, X_Used, Y_Used);
		}
		else if (Y_Used[i - 1][j] == 0 && (B - Value)*(C - Value) < 0)
		{
			TWOVALUE tmpV;
			tmpV.X = i - 1;
			tmpV.Y = j + abs((Value - B) / (C - B));
			Line.Logic.push_back(tmpV);
			Y_Used[i - 1][j] = 1;

			//ʵ��ֵ
			TWOVALUE tmpR;
			GetReallyPoint(tmpV, tmpR);
			IsoReal.Logic.push_back(tmpR);

			//����׷��
			//TrackRight(Value, Line, IsoReal, X_Used, Y_Used);
			//�޸ģ�����׷��
			TrackLeft(Value, Line, IsoReal, X_Used, Y_Used);
		}
	}
}
void Jie_Ds::TrackUp(double Value, IsoLine &Line, IsoLine &IsoReal, vector<vector<double>>&X_Used, vector<vector<double>>&Y_Used)
{
	int Num = (int)Line.Logic.size();
	//��ǰ���һ����ֵ�����������
	int i = (int)Line.Logic[Num - 1].X;
	int j = (int)Line.Logic[Num - 1].Y;
	double A = m_GridPoint[i][j].Z;
	double B = m_GridPoint[i + 1][j].Z;
	double C = m_GridPoint[i + 1][j + 1].Z;
	double D = m_GridPoint[i][j + 1].Z;
	//cout << "v " << m_GridPoint[i][j].X << " " << m_GridPoint[i][j].Y << " " << "0" << endl;
	//cout << "v " << m_GridPoint[i+1][j].X << " " << m_GridPoint[i+1][j].Y << " " << "0" << endl;
	//cout << "v " << m_GridPoint[i+1][j+1].X << " " << m_GridPoint[i+1][j+1].Y << " " << "0" << endl;
	//cout << "v " << m_GridPoint[i][j+1].X << " " << m_GridPoint[i][j+1].Y << " " << "0" << endl;
	double Z = (A + B + C + D) / 4;
	if (m_NoFault[i][j]>-1)					//�������жϲ�
	{
		TrackUPFault(Value, Line, IsoReal, X_Used, Y_Used);//׷��������
		return;
	}

	if ((A - Value) * (D - Value) < 0 && (B - Value) * (C - Value) < 0)//׷��������ֵ��
	{
		if (Y_Used[i][j] == 0 && ((A - Value) * (Z - Value) < 0 || ((C - Value) * (Z - Value) < 0)))
		{
			TWOVALUE tmpV;
			tmpV.X = i;
			tmpV.Y = j + abs((Value - A) / (D - A));
			Line.Logic.push_back(tmpV);
			Y_Used[i][j] = 1;

			//ʵ��ֵ
			TWOVALUE tmpR;
			GetReallyPoint(tmpV, tmpR);
			IsoReal.Logic.push_back(tmpR);

			//׷�����
			TrackLeft(Value, Line, IsoReal, X_Used, Y_Used);
		}
		else if (Y_Used[i + 1][j] == 0 && ((B - Value) * (Z - Value) < 0 || ((D - Value) * (Z - Value) < 0)))
		{
			TWOVALUE tmpV;
			tmpV.X = i + 1;
			tmpV.Y = j + abs((Value - B) / (C - B));
			Line.Logic.push_back(tmpV);
			Y_Used[i + 1][j] = 1;

			//ʵ��ֵ
			TWOVALUE tmpR;
			GetReallyPoint(tmpV, tmpR);
			IsoReal.Logic.push_back(tmpR);

			//׷���ұ�
			TrackRight(Value, Line, IsoReal, X_Used, Y_Used);
		}
	}
	else	//ֻ��׷��һ����ֵ�� 
	{

		if (Y_Used[i][j] == 0 && (A - Value) * (D - Value) < 0)
		{
			TWOVALUE tmpV;
			tmpV.X = i;
			tmpV.Y = j + abs((Value - A) / (D - A));
			Line.Logic.push_back(tmpV);
			Y_Used[i][j] = 1;

			//ʵ��ֵ
			TWOVALUE tmpR;
			GetReallyPoint(tmpV, tmpR);
			IsoReal.Logic.push_back(tmpR);

			//׷�����
			TrackLeft(Value, Line, IsoReal, X_Used, Y_Used);
		}
		else if (Y_Used[i + 1][j] == 0 && (B - Value) * (C - Value) < 0)
		{
			TWOVALUE tmpV;
			tmpV.X = i + 1;
			tmpV.Y = j + abs((Value - B) / (C - B));
			Line.Logic.push_back(tmpV);
			Y_Used[i + 1][j] = 1;

			//ʵ��ֵ
			TWOVALUE tmpR;
			GetReallyPoint(tmpV, tmpR);
			IsoReal.Logic.push_back(tmpR);

			//׷���ұ�
			TrackRight(Value, Line, IsoReal, X_Used, Y_Used);
		}
		else if (X_Used[i][j + 1] == 0 && (D - Value)*(C - Value) < 0)
		{
			TWOVALUE tmpV;
			tmpV.X = i + abs((Value - D) / (C - D));
			tmpV.Y = j + 1;
			Line.Logic.push_back(tmpV);
			X_Used[i][j + 1] = 1;

			//ʵ��ֵ
			TWOVALUE tmpR;
			GetReallyPoint(tmpV, tmpR);
			IsoReal.Logic.push_back(tmpR);

			//����׷��
			TrackUp(Value, Line, IsoReal, X_Used, Y_Used);
		}
	}
}
void Jie_Ds::TrackX(TWOVALUE A, double Value, IsoLine &Line, IsoLine &IsoReal, vector<vector<double>>&X_Used, vector<vector<double>>&Y_Used)
{

	int i = (int)A.X;
	int j = (int)A.Y;
	if (j > 1 && j < m_YNum)					//�Ǳ߽��,���ڵ�һ�������һ��֮��
	{
		//����ֵ
		IsoLine LineA;
		IsoLine LineB;
		LineA.Logic.push_back(A);
		LineB.Logic.push_back(A);


		IsoLine RLineA;
		IsoLine RLineB;


		//ʵ��ֵ
		TWOVALUE tmpR;
		//�õ���ʵ�ĵ�ֵ�������ֵ
		GetReallyPoint(A, tmpR);
		RLineA.Logic.push_back(tmpR);
		RLineB.Logic.push_back(tmpR);
		TrackUp(Value, LineA, RLineA, X_Used, Y_Used);

		int Num = RLineA.Logic.size() - 1;
		if (RLineA.Logic[0].X == RLineA.Logic[Num].X && RLineA.Logic[0].Y == RLineA.Logic[Num].Y)
		{
			IsoReal = RLineA;
			X_Used[i][j] = 1;
			return;
		}
		X_Used[i][j] = 1;
		TrackDown(Value, LineB, RLineB, X_Used, Y_Used);

		//TODO:����������ϵ����
		int NumA = (int)LineA.Logic.size();
		if (NumA > 1)
		{
			for (int k = NumA - 1; k > 0; k--) //�������׵�
			{
				TWOVALUE PointA = LineA.Logic[k];
				Line.Logic.push_back(PointA);
			}
		}
		for (int k = 0; k < (int)LineB.Logic.size(); k++)
		{
			TWOVALUE PointA = LineB.Logic[k];
			Line.Logic.push_back(PointA);
		}

		//ʵ����������
		NumA = (int)RLineA.Logic.size();
		if (NumA > 1)
		{
			for (int k = NumA-1; k > 0; k--) //�������׵�
			{
				TWOVALUE PointA = RLineA.Logic[k];
				IsoReal.Logic.push_back(PointA);
			}
		}
		for (int k = 0; k < (int)RLineB.Logic.size(); k++)
		{
			TWOVALUE PointA = RLineB.Logic[k];
			IsoReal.Logic.push_back(PointA);
		}

	}
	else if (j == 1)//��һ��
	{

		Line.Logic.push_back(A);
		TWOVALUE tmpR;
		GetReallyPoint(A, tmpR);
		IsoReal.Logic.push_back(tmpR);
		TrackUp(Value, Line, IsoReal, X_Used, Y_Used);
		X_Used[i][j] = 1;
	}
	else if (j == m_YNum)//���һ��
	{

		Line.Logic.push_back(A);
		TWOVALUE tmpR;
		GetReallyPoint(A, tmpR);
		IsoReal.Logic.push_back(tmpR);
		TrackDown(Value, Line, IsoReal, X_Used, Y_Used);
		X_Used[i][j] = 1;
	}
	//X_Used[i][j] = 1;
}
void Jie_Ds::TrackY(TWOVALUE A, double Value, IsoLine &Line, IsoLine &IsoReal, vector<vector<double>>&X_Used, vector<vector<double>>&Y_Used)
{

	int i = (int)A.X;
	int j = (int)A.Y;
	if (i > 1 && i < m_XNum)					//�Ǳ߽�㣬���ڵ�һ�������һ��֮��
	{
		IsoLine LineA;
		IsoLine LineB;
		LineA.Logic.push_back(A);
		LineB.Logic.push_back(A);

		IsoLine RLineA;
		IsoLine RLineB;

		//ʵ��ֵ
		TWOVALUE tmpR;
		GetReallyPoint(A, tmpR);
		RLineA.Logic.push_back(tmpR);
		RLineB.Logic.push_back(tmpR);
		TrackLeft(Value, LineA, RLineA, X_Used, Y_Used);
		int Num = RLineA.Logic.size() - 1;
		if (RLineA.Logic[0].X == RLineA.Logic[Num].X && RLineA.Logic[0].Y == RLineA.Logic[Num].Y)
		{
			IsoReal = RLineA;
			Y_Used[i][j] = 1;
			return;
		}
		Y_Used[i][j] = 1;
		TrackRight(Value, LineB, RLineB, X_Used, Y_Used);

		//������������
		int NumA = (int)LineA.Logic.size();
		if (NumA > 1)
		{
			for (int k = NumA - 1; k > 0; k--) //�������׵�
			{
				TWOVALUE PointA = LineA.Logic[k];
				Line.Logic.push_back(PointA);
			}
		}
		for (int k = 0; k < (int)LineB.Logic.size(); k++)
		{
			TWOVALUE PointA = LineB.Logic[k];
			Line.Logic.push_back(PointA);
		}
		//ʵ����������
		NumA = (int)RLineA.Logic.size();
		if (NumA > 1)
		{
			for (int k = NumA - 1; k > 0; k--) //�������׵�
			{
				TWOVALUE PointA = RLineA.Logic[k];
				IsoReal.Logic.push_back(PointA);
			}
		}
		for (int k = 0; k < (int)RLineB.Logic.size(); k++)
		{
			TWOVALUE PointA = RLineB.Logic[k];
			IsoReal.Logic.push_back(PointA);
		}
	}
	else if (i == 1)//��һ��
	{
		Line.Logic.push_back(A);
		TWOVALUE tmpR;
		GetReallyPoint(A, tmpR);
		IsoReal.Logic.push_back(tmpR);
		TrackRight(Value, Line, IsoReal, X_Used, Y_Used);
		Y_Used[i][j] = 1;
	}
	else if (i == m_XNum)  //���һ��
	{
		Line.Logic.push_back(A);
		TWOVALUE tmpR;
		GetReallyPoint(A, tmpR);
		IsoReal.Logic.push_back(tmpR);
		TrackLeft(Value, Line, IsoReal, X_Used, Y_Used);
		Y_Used[i][j] = 1;
	}
	//Y_Used[i][j] = 1;
}
//�ӳ�ʼֵ׷�ٵ�ֵ��
void Jie_Ds::TrackPoint(double Value, IsoLine &Line, IsoLine &IsoReal, vector<vector<double>>&X_Used, vector<vector<double>>&Y_Used, TWOVALUE First)
{
	//����֮����Ҫ������һ���жϣ�����Ϊ�����൱��һ��ȡ���Ĳ���
	//double t = abs((Value - m_GridPoint[i][j].Z) / (m_GridPoint[i][j + 1].Z - m_GridPoint[i][j].Z));
	//double t = abs((Value - m_GridPoint[i][j].Z) / (m_GridPoint[i+1][j].Z - m_GridPoint[i][j].Z));
	//�����t�൱������������Բ�ֵǰ���ϵ����������֤t������һ��������
	int i = int(First.X);
	int j = (int)First.Y;
	//˵����ֵ���������
	if (i == First.X && j == First.Y)
	{
		//AfxMessageBox("��ֵ���������");			//���ڲ���
	}

	if (j == First.Y)								//����
	{
		if (X_Used[i][j] == 0)						//û�б�ʹ�ù�
		{
			//cout << "hello1" << endl;
			//TODO:׷�ٺ���
			//X_Used[int(First.X)][j] = 1;			//�׵㲻���жϣ������Ժ�׷�ٵ��׵�
			TrackX(First, Value, Line, IsoReal, X_Used, Y_Used);	//׷�ٺ��
		}
	}
	else											//����
	{
		if (Y_Used[i][j] == 0)						//û�б�ʹ�ù�
		{
			//cout << i << " " << j << endl;
			//cout << "hello2" << endl;
			//TODO:׷������
			//Y_Used[int(First.X)][j] = 1;			//�׵㲻���жϣ������Ժ�׷�ٵ��׵�
			TrackY(First, Value, Line, IsoReal, X_Used, Y_Used);	//׷���ݱ�
		}
	}
}
void Jie_Ds::TrackOneValue(double Value)
{
	vector<TWOVALUE> VirtualIJ;
	vector<THRVALUE> Jie_VirtualIJK;
	EquivalentPoints(Value, VirtualIJ);//�õ����еĵ�ֵ��
	vector<vector<double>> X_Used;
	vector<vector<double>> Y_Used;
    //����Ǹ�ֵ,����Ҫע������������ı�����ʽ
	SignBorder(X_Used, Y_Used);						
	//cout << "======================================================" << endl;
	//cout << "�����ǹ��ڱ������������Ϣ��" << endl;
	//for (int i = 0; i < X_Used.size(); i++)
	//{
	//	for (int j = 0; j < X_Used[i].size(); j++)
	//	{
	//		cout << X_Used[i][j] << " ";
	//		//flag_x[i][j] = X_Used[i][j];
	//	}
	//	cout << endl;
	//}
	//cout << endl;
	//for (int i = 0; i < Y_Used.size(); i++)
	//{
	//	for (int j = 0; j < Y_Used[i].size(); j++)
	//	{
	//		cout << Y_Used[i][j] << " ";
	//	}
	//	cout << endl;
	//}
	//cout << "======================================================" << endl;													
	//��ֵ��׷��
	//cout << (int)VirtualIJ.size() << " ";
	for (int i = 0; i < (int)VirtualIJ.size(); i++)
	{
		IsoLine ISOLineOne;	//�洢��������
		IsoLine IsoReal;	//�洢ʵ������
		TWOVALUE First = VirtualIJ[i];
		TrackPoint(Value, ISOLineOne, IsoReal, X_Used, Y_Used, First);
		if (IsoReal.Logic.size() > 30)
		{
			ISOLineOne.Value = Value;
			IsoReal.Value = Value;
			m_IsoLine.push_back(ISOLineOne);
			m_IsoRealLine.push_back(IsoReal); //20131008���Զϲ��ڲ���������
		}
	}
	/*cout << "======================================================" << endl;
	cout << "������ִ�к���ڱ������������Ϣ��" << endl;
	for (int i = 0; i < X_Used.size(); i++)
	{
		for (int j = 0; j < X_Used[i].size(); j++)
		{
			cout <<  fixed << setprecision(0)<< X_Used[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl;
	for (int i = 0; i < Y_Used.size(); i++)
	{
		for (int j = 0; j < Y_Used[i].size(); j++)
		{
			cout << fixed << setprecision(0) << Y_Used[i][j] << " ";
		}
		cout << endl;
	}
	cout << "======================================================" << endl;*/
	
}
void Jie_Ds::IsolineTracking()
{

	//�ڴ˴���յ�ֵ��������
	m_IsoLine.clear();
	m_IsoRealLine.clear();	//20131111
	for (int i = 0; i < (int)m_TrackValue.size(); i++)
	{
		double Value = m_TrackValue[i];
		if (Value > m_ZMax || Value < m_ZMin)
		{
			continue;
		}
		else
		{
			TrackOneValue(Value);
		}
		std::ofstream out300("current_isoline.obj");
		for (int i = 0; i < m_IsoRealLine.size(); i++)
		{
			for (int j = 0; j < m_IsoRealLine[i].Logic.size(); j++)
				out300 << "v " << fixed << setprecision(5) << m_IsoRealLine[i].Logic[j].X << " " << fixed << setprecision(5) << m_IsoRealLine[i].Logic[j].Y << " " << "0" << endl;
		}
	}
}
void Jie_Ds::IsolineOpt()
{
	for (int i = 0; i < m_IsoRealLine.size(); i++)
	{
		originPoint.clear();
		curvePoint.clear();
		Vector2D p;
		for (int j = 0; j < m_IsoRealLine[i].Logic.size(); j++)
		{
			p.x = m_IsoRealLine[i].Logic[j].X;
			p.y = m_IsoRealLine[i].Logic[j].Y;
			originPoint.push_back(p);
		}
		createCurve();
		IsoLine iso;
		for (int j = 0; j < curvePoint.size(); j++)
		{	
			TWOVALUE t;
			t.X = curvePoint[j].x;
			t.Y = curvePoint[j].y;
			iso.Logic.push_back(t);
		}
		opt_IsoRealLine.push_back(iso);
		std::ofstream out("data//isoline_current.obj");
		for (int k = 0; k < iso.Logic.size(); k++)
			out << "v " << fixed << setprecision(10) << iso.Logic[k].X << " " << fixed << setprecision(10) << iso.Logic[k].Y << " " << "0" << endl;
		out.close();
	}
}
void Jie_Ds::CreateIsoLine()
{
	if (m_ZMax != m_ZMin)
	{
		vector<HwIsoLine> vec;
		m_lsoLines.swap(vec);

		int iso_size = int(m_IsoLine.size());


		for (int i = 0; i < iso_size; i++)
		{
			IsoLine il = m_IsoLine[i];

			HwIsoLine line;
			line.zValue = il.Value;
			line.ptn = il.Logic.size();
			line.pts = new HwIsoPt[line.ptn];

			for (int k = 0; k < il.Logic.size(); k++)
			{
				HwIsoPt hpt;
				hpt.x = il.Logic[k].X;
				hpt.y = il.Logic[k].Y;

				line.pts[k].x = hpt.x;
				line.pts[k].y = hpt.y;
			}

			m_lsoLines.push_back(line);
		}

	}

}



void Jie_Ds::SetOriBoder(vector<TWOVALUE> OriBoder)
{
	Jie_OriBoder.clear();
	for (int i = 0; i < (int)OriBoder.size(); i++)
	{
		Jie_OriBoder.push_back(OriBoder[i]);
	}
}
void Jie_Ds::SetOriISOLine(vector<IsoLine> IsoRealLine)
{
	//for (int i = 0; i < IsoRealLine.size(); i++)
	//	cout << IsoRealLine[i].Logic[0].X << " ";





	//IsoLine L;
	//Jie_IsoRealLine.clear();
	for (int i = 0; i < (int)IsoRealLine.size(); i++)
	{
		/*L.direction0 = IsoRealLine[i].direction0;
		L.direction1 = IsoRealLine[i].direction1;
		L.Index = IsoRealLine[i].Index;
		cout << IsoRealLine[i].Logic.size();
		for (int j = 0; j < IsoRealLine[i].Logic.size(); j++)
		{
			cout << IsoRealLine[i].Logic.size();
			cout << IsoRealLine[i].Logic[j].X << " ";
		}
			
		
		L.Value = IsoRealLine[i].Value;
		cout << "value = ;" << L.Value << endl;*/
		Jie_IsoRealLine.push_back(IsoRealLine[i]);
	}
}
bool Jie_Ds::IsinLineK(TWOVALUE Star, TWOVALUE End, TWOVALUE Point)
{
	End.X = End.X - Star.X;
	End.Y = End.Y - Star.Y;
	Point.X = Point.X - Star.X;
	Point.Y = Point.Y - Star.Y;
	Star.X = 0;
	Star.Y = 0;					//ƽ�ƣ����ж��Ƿ�һ��ֱ�߲����Ӱ�졤����������ΪС��~����߼��㾫��
	if (Point.X *(Point.X - End.X) >0 && Point.Y *(Point.Y - End.Y) > 0)
	{
		return false;
	}
	double A = Star.Y - End.Y;
	double B = End.X - Star.X;
	//double C = -A * Star.X- B * Star.Y;
	double C = 0;
	double D = sqrt(pow(A, 2) + pow(B, 2));
	double d = A * (Point.X - Star.X) + B * (Point.Y - Star.Y);
	d = d / D;
	if (abs(d)<0.00001)
	{
		return true;
	}
	return false;

}
//�߶��Ƿ��ཻ,index�������
bool Jie_Ds::ISIntersect(TWOVALUE p1_s, TWOVALUE p1_e, TWOVALUE p2_s, TWOVALUE p2_e, double &index)
{
	double Xmax_1, Xmax_2, Xmin_1, Xmin_2, Ymax_1, Ymax_2, Ymin_1, Ymin_2;
	double V1, V2, V3, V4;

	if (p1_s.X > p1_e.X)
	{
		Xmax_1 = p1_s.X;
		Xmin_1 = p1_e.X;
	}
	else
	{
		Xmax_1 = p1_e.X;
		Xmin_1 = p1_s.X;
	}


	if (p1_s.Y > p1_e.Y)
	{
		Ymax_1 = p1_s.Y;
		Ymin_1 = p1_e.Y;
	}
	else
	{
		Ymax_1 = p1_e.Y;
		Ymin_1 = p1_s.Y;
	}


	if (p2_s.X > p2_e.X)
	{
		Xmax_2 = p2_s.X;
		Xmin_2 = p2_e.X;
	}
	else
	{
		Xmax_2 = p2_e.X;
		Xmin_2 = p2_s.X;
	}

	if (p2_s.Y > p2_e.Y)
	{
		Ymax_2 = p2_s.Y;
		Ymin_2 = p2_e.Y;
	}
	else
	{
		Ymax_2 = p2_e.Y;
		Ymin_2 = p2_s.Y;
	}


	if (Xmax_1 < Xmin_2 || Xmin_1 > Xmax_2 || Ymin_1 > Ymax_2 || Ymax_1 < Ymin_2)   //���߶���С���β��ཻ���ó����߶β��ཻ
		return FALSE;
	else
	{
		V1 = (p1_e.X - p1_s.X) * (p2_s.Y - p1_s.Y) - (p2_s.X - p1_s.X) * (p1_e.Y - p1_s.Y);
		V2 = (p1_e.X - p1_s.X) * (p2_e.Y - p1_s.Y) - (p2_e.X - p1_s.X) * (p1_e.Y - p1_s.Y);
		V3 = (p2_e.X - p2_s.X) * (p1_s.Y - p2_s.Y) - (p1_s.X - p2_s.X) * (p2_e.Y - p2_s.Y);
		V4 = (p2_e.X - p2_s.X) * (p1_e.Y - p2_s.Y) - (p1_e.X - p2_s.X) * (p2_e.Y - p2_s.Y);
		//�����������ֱ����ʱ��   20131015
		BOOL Mark = FALSE;
		TWOVALUE TempPoint;
		if (abs(V1)< 0.00000001)
		{
			TempPoint = p2_s;
			Mark = TRUE;
		}
		else if (abs(V2)< 0.00000001)
		{
			TempPoint = p2_e;
			Mark = TRUE;
		}
		else if (abs(V3)< 0.00000001)
		{
			TempPoint = p1_s;
			Mark = TRUE;
		}
		else if (abs(V4)< 0.00000001)
		{
			TempPoint = p1_e;
			Mark = TRUE;
		}
		if (Mark)
		{
			double t = 0;
			if (abs(TempPoint.X - p1_s.X)<abs(TempPoint.Y - p1_s.Y))
			{
				t = (TempPoint.Y - p1_s.Y) / (p1_e.Y - p1_s.Y);/////////////////////
			}
			else
			{
				t = (TempPoint.X - p1_s.X) / (p1_e.X - p1_s.X);
			}
			index = index + t;
			return TRUE;
		}
		if (V3 * V4 <= 0 && V1 * V2 <= 0)
		{
			double dy = p1_e.Y - p1_s.Y;
			double dx = p1_e.X - p1_s.X;
			double t = 0;
			if (abs(p2_e.X - p2_s.X)<0.00000000001)
			{
				t = (p2_e.X - p1_s.X) / dx;/////////////////////
			}
			else
			{
				double k = (p2_s.Y - p2_e.Y) / (p2_s.X - p2_e.X);
				t = ((p2_s.Y - p1_s.Y) - k*p2_s.X + k*p1_s.X) / (dy - k*dx);
			}
			index = index + t;
			return TRUE;
		}
		else
			return FALSE;
	}
}
//��A�Ƿ���Line�ڲ�
bool Jie_Ds::IsInside(TWOVALUE A, vector<TWOVALUE> Line)
{
	//cout << "hhh" << endl;
	for (int i = 0; i < (int)Line.size() - 1; i++)
	{
		if (IsinLineK(Line[i], Line[i + 1], A))		//���ڱ߽���  20131023
		{
			return TRUE;
		}
	}
	double Xmax = Line[0].X;
	double Xmin = Line[0].X;
	double Ymax = Line[0].Y;
	double Ymin = Line[0].Y;
	for (int i = 1; i < (int)Line.size(); i++)
	{
		if (Line[i].X > Xmax)
		{
			Xmax = Line[i].X;
		}
		else if (Line[i].X < Xmin)
		{
			Xmin = Line[i].X;
		}

		if (Line[i].Y > Ymax)
		{
			Ymax = Line[i].Y;
		}
		else if (Line[i].Y < Ymin)
		{
			Ymin = Line[i].Y;
		}
	}
	if (A.X > Xmax || A.X <Xmin || A.Y>Ymax || A.Y < Ymin)
	{
		return FALSE;
	}
	//�����ж��Ƿ����ڲ�
	TWOVALUE B = A;
	B.X = m_XMin - 200.0;     //AB����
	int Sum = 0;
	for (int i = 0; i < (int)Line.size() - 1; i++)
	{
		double t = 0;
		TWOVALUE LineA = Line[i];
		TWOVALUE LineB = Line[i + 1];
		if (ISIntersect(LineA, LineB, A, B, t))
		{
			Sum = Sum + 1;
		}
	}
	if (Sum % 2 == 1)//����Ϊ�����������ڲ�
	{
		return TRUE;
	}
	return FALSE;
}
bool Jie_Ds::L2L_Intersect(TWOVALUE p1_s, TWOVALUE p1_e, TWOVALUE p2_s, TWOVALUE p2_e, TWOVALUE &Point)
{
	double Xmax_1, Xmax_2, Xmin_1, Xmin_2, Ymax_1, Ymax_2, Ymin_1, Ymin_2;
	double V1, V2, V3, V4;

	if (p1_s.X > p1_e.X)
	{
		Xmax_1 = p1_s.X;
		Xmin_1 = p1_e.X;
	}
	else
	{
		Xmax_1 = p1_e.X;
		Xmin_1 = p1_s.X;
	}


	if (p1_s.Y > p1_e.Y)
	{
		Ymax_1 = p1_s.Y;
		Ymin_1 = p1_e.Y;
	}
	else
	{
		Ymax_1 = p1_e.Y;
		Ymin_1 = p1_s.Y;
	}


	if (p2_s.X > p2_e.X)
	{
		Xmax_2 = p2_s.X;
		Xmin_2 = p2_e.X;
	}
	else
	{
		Xmax_2 = p2_e.X;
		Xmin_2 = p2_s.X;
	}

	if (p2_s.Y > p2_e.Y)
	{
		Ymax_2 = p2_s.Y;
		Ymin_2 = p2_e.Y;
	}
	else
	{
		Ymax_2 = p2_e.Y;
		Ymin_2 = p2_s.Y;
	}


	if (Xmax_1 < Xmin_2 || Xmin_1 > Xmax_2 || Ymin_1 > Ymax_2 || Ymax_1 < Ymin_2)   //���߶���С���β��ཻ���ó����߶β��ཻ
		return false;
	else   //���������Ĳ�����ʣ�������һ���߶ε������˵�����һ���߶ε�ͬһ��ʱ�����ཻ�������ཻ��
		   //( Q1 - P1 )��( P2 - P1) * ( P2 - P1)��(Q2 - P1) >= 0��
		   //����ļ��㹫ʽΪ:  P1 X P2 = x1y2 - x2y1;
	{
		V1 = (p1_e.X - p1_s.X) * (p2_s.Y - p1_s.Y) - (p2_s.X - p1_s.X) * (p1_e.Y - p1_s.Y);
		V2 = (p1_e.X - p1_s.X) * (p2_e.Y - p1_s.Y) - (p2_e.X - p1_s.X) * (p1_e.Y - p1_s.Y);
		V3 = (p2_e.X - p2_s.X) * (p1_s.Y - p2_s.Y) - (p1_s.X - p2_s.X) * (p2_e.Y - p2_s.Y);
		V4 = (p2_e.X - p2_s.X) * (p1_e.Y - p2_s.Y) - (p1_e.X - p2_s.X) * (p2_e.Y - p2_s.Y);
		//�����������ֱ����ʱ��   20131015
		if (abs(V1)< 0.00000001)
		{
			Point = p2_s;
			return true;
		}
		else if (abs(V2)< 0.00000001)
		{
			Point = p2_e;
			return true;
		}
		else if (abs(V3)< 0.00000001)
		{
			Point = p1_s;
			return true;
		}
		else if (abs(V4)< 0.00000001)
		{
			Point = p1_e;
			return true;
		}

		else if (V3 * V4 <= 0 && V1 * V2 <= 0)
		{
			double dy = p1_e.Y - p1_s.Y;
			double dx = p1_e.X - p1_s.X;
			double t = 0;
			if (abs(p2_e.X - p2_s.X)<0.00000000001)
			{
				t = (p2_e.X - p1_s.X) / dx;/////////////////////
				Point.X = p2_s.X;
				Point.Y = t*dy + p1_s.Y;
			}
			else
			{
				double k = (p2_s.Y - p2_e.Y) / (p2_s.X - p2_e.X);
				t = ((p2_s.Y - p1_s.Y) - k*p2_s.X + k*p1_s.X) / (dy - k*dx);
				Point.X = t * dx + p1_s.X;
				Point.Y = t * dy + p1_s.Y;
			}
			return true;
		}
	}
	return false;

}
bool Jie_Ds::GetCrossPt(TWOVALUE Star, TWOVALUE End, vector<TWOVALUE> Line, TWOVALUE &A)
{
	for (int j = 0; j < (int)Jie_OriBoder.size() - 1; j++)
	{
		TWOVALUE C = Jie_OriBoder[j];
		TWOVALUE B = Jie_OriBoder[j + 1];
		if (L2L_Intersect(C, B, Star, End, A))
		{
			return TRUE;
		}
	}
	return FALSE;
}
void Jie_Ds::DleaIso(IsoLine &OneIso, vector<IsoLine> &NewIso)
{
	IsoLine TempLine;
	TempLine.Value = OneIso.Value;
	//cout << "Jie_OriBorder size is :" << Jie_OriBoder.size() << endl;
	//cout << "��ǰ�����꣺" << OneIso.Logic[0].X << " " << OneIso.Logic[0].X << endl;
	if (OneIso.Logic.size() > 0)
	{
		bool flag = IsInside(OneIso.Logic[0], Jie_OriBoder);
		if (!flag)//�׵㲻��ԭʼ�߽��ڣ�ɾ����ǰ�ĵ㣬������߽��
		{
			for (int i = 1; i < (int)OneIso.Logic.size(); i++)
			{
				if (IsInside(OneIso.Logic[i], Jie_OriBoder))
				{
					TWOVALUE Star = OneIso.Logic[i - 1];
					TWOVALUE End = OneIso.Logic[i];
					TWOVALUE A;
					if (GetCrossPt(Star, End, Jie_OriBoder, A))//�õ��߽���������㹹�ɵ��߶εĽ���
					{
						TempLine.Logic.push_back(A);//�˽�����Ϊ��ֵ�ߵ����е�һ��
					}
					else
					{
						int sk = 0;
						//AfxMessageBox("111");
					}
					for (int j = i; j < (int)OneIso.Logic.size(); j++)
					{
						TempLine.Logic.push_back(OneIso.Logic[j]);//��������
					}
					break;
				}
			}
		}
		else
		{
			TempLine = OneIso;
		}
		if ((int)TempLine.Logic.size() <= 1)	//��ȫ����ԭʼ�߽�����
		{
			return;
		}
		IsoLine StarLine;
		StarLine.Value = TempLine.Value;
		StarLine.Logic.push_back(TempLine.Logic[0]);
		OneIso.Logic.clear();
		for (int i = 1; i < (int)TempLine.Logic.size(); i++)  //�ӵڶ����㿪ʼ��������
		{
			if (IsInside(TempLine.Logic[i], Jie_OriBoder))
			{
				StarLine.Logic.push_back(TempLine.Logic[i]);
			}
			else
			{
				//CString str;
				//str.Format("�׸��߽��%d,%f     %f",i,TempLine.Logic[0].X,TempLine.Logic[0].Y);
				//AfxMessageBox(str);

				TWOVALUE Star = TempLine.Logic[i - 1];
				TWOVALUE End = TempLine.Logic[i];
				TWOVALUE A;
				if (GetCrossPt(Star, End, Jie_OriBoder, A))
				{
					StarLine.Logic.push_back(A);
				}
				else
				{
					int sk = 0;
					//AfxMessageBox("111");
				}

				for (int j = i; j < (int)TempLine.Logic.size(); j++)
				{
					OneIso.Logic.push_back(TempLine.Logic[j]);
				}
				break;
			}
		}
		NewIso.push_back(StarLine);
		if (OneIso.Logic.size() > 0)//������滹�����ݣ�˵����ֵ���м䲿��Ҳ���߽���з���
		{
			DleaIso(OneIso, NewIso);
		}
	}
}

void Jie_Ds::DleaIso()
{

	vector<IsoLine> NewIso;
	for (int i = 0; i < (int)Jie_IsoRealLine.size(); i++)
	{
		vector<IsoLine> TempLines;
		IsoLine LineOne = Jie_IsoRealLine[i];
	
		DleaIso(LineOne, TempLines);
		for (int j = 0; j < (int)TempLines.size(); j++)
		{
			NewIso.push_back(TempLines[j]);
		}
	}

	Jie_IsoRealLine.clear();
	Jie_IsoRealLine = NewIso;//���߽��и��ĵ�ֵ��
}
vector<double> Jie_Ds::Randnum(int k)
{
	vector<double> temp;
	srand((int)time(0));
	for (int i = 1; i <= k / 2; i++)
	{
		double temp_ = random(10);
		temp.push_back(temp_);
	}
	return temp;
}