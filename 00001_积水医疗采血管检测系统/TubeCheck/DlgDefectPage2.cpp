// DlgDefectPage2.cpp : implementation file
//

#include "stdafx.h"
#include "TubeCheck.h"
#include "DlgDefectPage2.h"
#include "MainFrm.h"
#include "TubeCheckVIEW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMainFrame*			g_pFrame; //全局指针
extern CTubeCheckView*     g_pView;


/////////////////////////////////////////////////////////////////////////////
// CDlgDefectPage2 dialog


CDlgDefectPage2::CDlgDefectPage2(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDefectPage2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDefectPage2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgDefectPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDefectPage2)
	DDX_Control(pDX, IDC_LIST1, m_listData);
	DDX_Control(pDX, IDC_STOP_DATE, m_StopDate);
	DDX_Control(pDX, IDC_START_DATE, m_StartDate);
	DDX_Control(pDX, IDC_COMBO_MODEL_NAME, m_cmbModelName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDefectPage2, CDialog)
	//{{AFX_MSG_MAP(CDlgDefectPage2)
	ON_BN_CLICKED(IDC_BTN_INQUIRY, OnBtnInquiry)
	ON_BN_CLICKED(IDC_BTN_EXCEL, OnBtnExcel)
	ON_NOTIFY(MCN_SELCHANGE, IDC_START_DATE, OnSelchangeStartDate)
	ON_NOTIFY(MCN_SELCHANGE, IDC_STOP_DATE, OnSelchangeStopDate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDefectPage2 message handlers

BOOL CDlgDefectPage2::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	int iWidth=0;
	int i=0,j=0,k=0;
	CRect rcList;
	CString strTemp;
	CMainFrame* g_pFrame = (CMainFrame*)AfxGetMainWnd();
	CTime sTime = CTime::GetCurrentTime();
	m_sAlgModelPath = g_pFrame->m_sAlgIniFilePath;
	m_sModelName = g_pFrame->m_sModelName;
	
	m_sStartYear.Format("%d",sTime.GetYear());
	m_sStartMonth.Format("%d",sTime.GetMonth()) ;
	m_sStartDay.Format("%d",sTime.GetDay());
	m_sStopYear = m_sStartYear;
	m_sStopMonth = m_sStartMonth;
	m_sStopDay = m_sStartDay;

	// 初始化模板名称
	InitComboProduct();
	GetDlgItem(IDC_BTN_EXCEL)->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_DATE))->SetCheck(TRUE);

	iWidth=70;
	m_listData.GetWindowRect(&rcList);
	m_listData.DeleteAllItems();
	m_listData.ModifyStyle(NULL,LVS_REPORT);
	m_listData.SetExtendedStyle(LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	strTemp="开检时间";
	m_listData.InsertColumn(0, strTemp, LVCFMT_LEFT,iWidth*2);	
	m_listColName.AddTail(strTemp);

	strTemp="停检时间";
	m_listData.InsertColumn(1, strTemp, LVCFMT_LEFT,iWidth*2);
	m_listColName.AddTail(strTemp);

	strTemp="模板名称";
	m_listData.InsertColumn(2, strTemp, LVCFMT_LEFT,(int)(iWidth*1.5));	
	m_listColName.AddTail(strTemp);

	strTemp="检测总数";
	m_listData.InsertColumn(3, strTemp, LVCFMT_LEFT,(int)(iWidth*1.5));
	m_listColName.AddTail(strTemp);

	strTemp="合格品数";
	m_listData.InsertColumn(4, strTemp, LVCFMT_LEFT,(int)(iWidth*1.5));
	m_listColName.AddTail(strTemp);

	strTemp="不合格数";
	m_listData.InsertColumn(5, strTemp, LVCFMT_LEFT,(int)(iWidth*1.5));
	m_listColName.AddTail(strTemp);

	strTemp="工位1检测总数";
	m_listData.InsertColumn(6, strTemp, LVCFMT_LEFT,(int)(iWidth*2));
	m_listColName.AddTail(strTemp);

	strTemp="工位1不合格数";
	m_listData.InsertColumn(7, strTemp, LVCFMT_LEFT,(int)(iWidth*2));
	m_listColName.AddTail(strTemp);

	strTemp="工位2检测总数";
	m_listData.InsertColumn(8, strTemp, LVCFMT_LEFT,(int)(iWidth*2));
	m_listColName.AddTail(strTemp);

	strTemp="工位2不合格数";
	m_listData.InsertColumn(9, strTemp, LVCFMT_LEFT,(int)(iWidth*2));
	m_listColName.AddTail(strTemp);


	for(i=0;i<c_AllElementCount;i++)
	{
		for(j=0;j<g_pFrame->m_iDefectTypePerWS[i];j++)
		{
			if(""!=g_pFrame->m_sDefectInfo[i][j])
			{
				k++;
				strTemp=g_pFrame->m_sDefectInfo[i][j];
				m_listColName.AddTail(strTemp);
				m_listData.InsertColumn(9+k,strTemp,LVCFMT_LEFT,(int)(iWidth*2));
			}
		}
	}

	m_iFieldCount = 9+k+1;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDefectPage2::InitComboProduct()
{
	CFileFind fd;
	BOOL bFind=fd.FindFile(m_sAlgModelPath+"\\*.*");
	CString strLeft,strTemp;
	int iLeft=0;
	
	while(bFind)
	{
		//打开子目录，读出文件夹名称
		bFind=fd.FindNextFile();
		if(fd.IsDirectory()&&!fd.IsDots())
		{
			if (fd.GetFileName()!="mod1")
			{		
				m_cmbModelName.AddString(fd.GetFileName());
			}	
		}
	}
	
	m_cmbModelName.SetWindowText(m_sModelName);
	
	UpdateData(FALSE);
}

BOOL CDlgDefectPage2::GetRecordSet(CString sSQL)		// 获得记录集
{
	CString strTemp(""),strName("");
	int nTotalCount=0;
	int nRecordCount=0;
	int i=0,j=0,k=0;

	if(sSQL.IsEmpty())
	{
		return TRUE;
	}

	try
	{
		m_RecordSet->Open(sSQL.GetBuffer(sSQL.GetLength()),
			_variant_t((IDispatch*)(m_Connect),true),
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch (_com_error* e)
	{
		AfxMessageBox(e->ErrorMessage());
		return FALSE;
	}

	_variant_t var;
	
	if(!m_RecordSet->ADOEOF)
	{
		m_RecordSet->MoveFirst();
	}
	else
	{
		m_RecordSet->Close();
		return TRUE;
	}
	
	while(!m_RecordSet->ADOEOF)
	{
		var=(m_RecordSet)->GetCollect("TotalCount");
		if(var.vt==VT_NULL||var.vt==VT_EMPTY)
		{
			strTemp="";
		}
		else
		{
			strTemp = (LPCSTR)_bstr_t(var);
		}

		nTotalCount=atoi(strTemp);
		
		if(nTotalCount>0)
		{
			CStringList* pStrList=new CStringList;
			var=m_RecordSet->GetCollect("StartTime");
			if(var.vt==VT_NULL||var.vt==VT_EMPTY)
			{
				strTemp="";
			}
			else
			{
				strTemp = (LPCSTR)_bstr_t(var);
			}

			pStrList->AddTail(strTemp);
			
			var=m_RecordSet->GetCollect("StopTime");
			if(var.vt==VT_NULL||var.vt==VT_EMPTY)
			{
				strTemp="";
			}
			else
			{
				strTemp = (LPCSTR)_bstr_t(var);
			}

			pStrList->AddTail(strTemp);
			
			var=m_RecordSet->GetCollect("ModelName");
			if(var.vt==VT_NULL||var.vt==VT_EMPTY)
			{
				strTemp="";
			}
			else
			{
				strTemp = (LPCSTR)_bstr_t(var);
			}
			pStrList->AddTail(strTemp);
			
			var=m_RecordSet->GetCollect("TotalCount");
			if(var.vt==VT_NULL||var.vt==VT_EMPTY)
			{
				strTemp="";
			}
			else
			{
				strTemp = (LPCSTR)_bstr_t(var);
			}

			pStrList->AddTail(strTemp);
			nTotalCount=atoi(strTemp);
			m_iTotalCount+=nTotalCount;


 			var=m_RecordSet->GetCollect("TotalGood");
			if(var.vt==VT_NULL||var.vt==VT_EMPTY)
			{
				strTemp="";
			}
			else
			{
 				strTemp = (LPCSTR)_bstr_t(var);
			}

			pStrList->AddTail(strTemp);
			nTotalCount=atoi(strTemp);
			m_iTotalGood+=nTotalCount;


			
			var=m_RecordSet->GetCollect("TotalBad");
			if(var.vt==VT_NULL||var.vt==VT_EMPTY)
			{
				strTemp="";
			}
			else
			{
				strTemp = (LPCSTR)_bstr_t(var);
			}

			pStrList->AddTail(strTemp);
			nTotalCount=atoi(strTemp);
			m_iTotalBad+=nTotalCount;

	
			var=m_RecordSet->GetCollect("Total1");
			if(var.vt==VT_NULL||var.vt==VT_EMPTY)
			{
				strTemp="";
			}
			else
			{
				strTemp = (LPCSTR)_bstr_t(var);
			}

			pStrList->AddTail(strTemp);

			var=m_RecordSet->GetCollect("Bad1");
			if(var.vt==VT_NULL||var.vt==VT_EMPTY)
			{
				strTemp="";
			}
			else
			{
				strTemp = (LPCSTR)_bstr_t(var);
			}
			
			pStrList->AddTail(strTemp);

			var=m_RecordSet->GetCollect("Total2");
			if(var.vt==VT_NULL||var.vt==VT_EMPTY)
			{
				strTemp="";
			}
			else
			{
				strTemp = (LPCSTR)_bstr_t(var);
			}
			
			pStrList->AddTail(strTemp);


			var=m_RecordSet->GetCollect("Bad2");
			if(var.vt==VT_NULL||var.vt==VT_EMPTY)
			{
				strTemp="";
			}
			else
			{
				strTemp = (LPCSTR)_bstr_t(var);
			}
			
			pStrList->AddTail(strTemp);


			for(i=0;i<c_AllElementCount;i++)
			{
				for(j=0;j<g_pFrame->m_iDefectTypePerWS[i];j++)
				{
					k=0;
					if(""!=g_pFrame->m_sDefectInfo[i][j])
					{
						k++;
						strName.Format("Type%d",i*MAX_DEFECT_TYPE_COUNT+j+1);
						var=m_RecordSet->GetCollect(_variant_t(strName));
						if(var.vt==VT_NULL||var.vt==VT_EMPTY)
						{
							strTemp="";
						}
						else
						{
							strTemp = (LPCSTR)_bstr_t(var);
						}
						pStrList->AddTail(strTemp);
					}
				}
			}

			m_PtrList.AddTail(pStrList);
			nRecordCount++;
		}
		
		m_RecordSet->MoveNext();
	}

	m_RecordSet->Close();


	return TRUE;
}
BOOL CDlgDefectPage2::OutputRecord()					// 输出记录集
{
	CString strTemp("");
	int nRow=0, nColumn=0, i=0;
	
	if(m_PtrList.IsEmpty())
	{
		GetDlgItem(IDC_BTN_EXCEL)->EnableWindow(FALSE);
		return TRUE;
	}
	else
	{
		GetDlgItem(IDC_BTN_EXCEL)->EnableWindow(TRUE);
	}
	
	while(!m_PtrList.IsEmpty())
	{
		CStringList* pStrList=(CStringList*)m_PtrList.GetHead();
		m_listData.InsertItem(nRow,pStrList->GetHead());
		
		for(i=0;i<m_iFieldCount;i++)
		{
			strTemp=pStrList->GetHead();
			m_listData.SetItemText(nRow, nColumn++, pStrList->GetHead());
			pStrList->RemoveHead();
		}
		
		m_PtrList.RemoveHead();
		delete pStrList;
		pStrList=NULL;
		
		nRow++;
		nColumn=0;
	}
	
	return TRUE;
}
BOOL CDlgDefectPage2::CloseCsvFile()					// 关闭Excel文件
{
	m_stdFile.Close();

	return TRUE;
}
BOOL CDlgDefectPage2::OpenCsvFile(CString strFilePath)	// 打开Excel文件
{
	if(!m_stdFile.Open(strFilePath, CFile::modeCreate|CFile::modeReadWrite|CFile::typeText))
	{
		AfxMessageBox("MP.csv 打开失败，注意MP.csv是否已经打开!");
		return FALSE;
	}
	

	return TRUE;
}

void CDlgDefectPage2::OnBtnInquiry() 
{
	// TODO: Add your control notification handler code here
	CString strSQL;
	CString strTableName;
	strTableName="Defect_Statistic_Info";
	
	UpdateData(TRUE);
	m_bCheckDate = FALSE;
	m_bCheckModel= FALSE;
	
	m_bCheckDate = ((CButton*)GetDlgItem(IDC_CHECK_DATE))->GetCheck();
	m_bCheckModel = ((CButton*)GetDlgItem(IDC_CHECK_MODEL_NAME))->GetCheck();
	GetDlgItemText(IDC_COMBO_MODEL_NAME,m_sModelName);
	m_listData.DeleteAllItems();
	
	m_sStartTime.Format("%s-%s-%s 00:00:00",m_sStartYear,m_sStartMonth,m_sStartDay);
	m_sStopTime.Format("%s-%s-%s 23:59:59",m_sStopYear,m_sStopMonth,m_sStopDay);
	
	if(FALSE==m_bCheckDate && FALSE==m_bCheckModel)
	{
		strSQL.Format("Select * From %s", strTableName);
	}
	else if(TRUE==m_bCheckDate && FALSE==m_bCheckModel)
	{
		strSQL.Format("Select * From %s where StartTime>=#%s# and StartTime<=#%s#", strTableName,m_sStartTime,m_sStopTime);
	}
	else if(FALSE==m_bCheckDate && TRUE==m_bCheckModel)
	{
		strSQL.Format("Select * From %s where ModelName='%s'", strTableName,m_sModelName);
	}
	else if(TRUE==m_bCheckDate && TRUE==m_bCheckModel)
	{
		strSQL.Format("Select * From %s where StartTime>=#%s# and StartTime<=#%s# and ModelName='%s'", strTableName,
			m_sStartTime,m_sStopTime,m_sModelName);
	}
	
	GetRecordSet(strSQL);
	OutputRecord();
}

void CDlgDefectPage2::OnBtnExcel() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	if(pFrame->m_bIsDetecting==TRUE)
	{
		AfxMessageBox("正在检测，需停止检测方可查询!");
		return;
	}

	int nListCount=0, nListColumn=0, i=0, j=0;
	CString strFilePath, strTemp, strTemp1;	
	strFilePath.Format("%s\\MP_Defect.csv", pFrame->m_sWorkPath);
	BOOL bRet=OpenCsvFile(strFilePath);
	
	if(bRet==FALSE)
	{
		return;
	}
	
	strTemp.Format("%s","标签缺陷结果统计\n\n");
	m_stdFile.WriteString(strTemp);
	
	strTemp.Format("%s","筛选条件:\n");
	m_stdFile.WriteString(strTemp);
	
	if(m_bCheckDate==TRUE)
	{
		strTemp.Format(",%s,%s至%s\n", "生产日期: ", m_sStartTime, m_sStopTime);
	}
	else
	{
		strTemp.Format(",%s,%s\n","生产日期: ","不限");
	}
	m_stdFile.WriteString(strTemp);
	
	
	if(m_bCheckModel==TRUE)
	{
		strTemp.Format(",%s,%s\n","模板名称: ",m_sModelName);
	}
	else
	{
		strTemp.Format(",%s,%s\n","模板名称: ","不限");
	}
	m_stdFile.WriteString(strTemp);
	
	
	strTemp.Format("%s","\n\n");
	m_stdFile.WriteString(strTemp);

	strTemp.Format("%s","序号");

	POSITION pos;
	for(i=0;i<m_iFieldCount;i++)
	{
		pos = m_listColName.FindIndex(i);
		strTemp1.Format(",%s",m_listColName.GetAt(pos));
		strTemp+=strTemp1;
	}
	
	strTemp+="\n";
//	strTemp.Format("%s,%s,%s,%s,%s,%s,%s,%s\n","序号", "开检时间", "停检时间", "模板名称","检测总数", "合格品数","不合格数","不合格率");
	m_stdFile.WriteString(strTemp);
	
	nListCount=m_listData.GetItemCount();
	nListColumn=m_listData.GetHeaderCtrl()->GetItemCount()+1;

	strTemp1="123";
	
	
	for(i=0;i<nListCount;i++)
	{
		strTemp.Format("%d,", i+1);
		for(j=0;j<nListColumn;j++)
		{
			strTemp1.Format("%s",m_listData.GetItemText(i,j));
			strTemp+=strTemp1+",";
		}
		
		strTemp+="\n";
		m_stdFile.WriteString(strTemp);
	}

	CloseCsvFile();
	
	// 执行打开Bottle.csv文件到桌面

	// 执行复制并打开
	
	CString strDesPath,strSrcPath;
	CTime CurrTime = CTime::GetCurrentTime();

	strSrcPath = strFilePath;
	strDesPath.Format("%s\\%02d-%02d-%02d",pFrame->m_sDataFilePath,CurrTime.GetHour(),CurrTime.GetMinute(),CurrTime.GetSecond());

	if(FALSE==CreateDirectory(strDesPath,NULL))
	{
		AfxMessageBox("create directory failed!");
		return;
	}


	strSrcPath+='\0';
	bRet = pFrame->OP_Directory(strSrcPath,strDesPath,FO_COPY,FOF_NOCONFIRMATION);

	CString str ;
	str.Format("Src:%s-Ori:%s  复制文件出错!",strSrcPath,strDesPath);

	if(FALSE==bRet)
	{
		AfxMessageBox(str);
	}


	HINSTANCE h=ShellExecute(pFrame->m_hWnd,"open", strDesPath, NULL, NULL, SW_SHOWNORMAL);

}

void CDlgDefectPage2::OnSelchangeStartDate(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	SYSTEMTIME syTime;
	
	if(0!=m_StartDate.GetCurSel(&syTime))
	{
		m_sStartYear.Format("%d",syTime.wYear);
		m_sStartMonth.Format("%d",syTime.wMonth);
		m_sStartDay.Format("%d",syTime.wDay);
	}
	*pResult = 0;
}

void CDlgDefectPage2::OnSelchangeStopDate(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	SYSTEMTIME syTime;
	
	if(0!=m_StopDate.GetCurSel(&syTime))
	{
		m_sStopYear.Format("%d",syTime.wYear);
		m_sStopMonth.Format("%d",syTime.wMonth);
		m_sStopDay.Format("%d",syTime.wDay);		
	}

	*pResult = 0;
}
