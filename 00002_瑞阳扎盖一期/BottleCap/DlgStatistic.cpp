// DlgStatistic.cpp : implementation file
//

#include "stdafx.h"
#include "BottleCap.h"
#include "MainFrm.h"
#include "DlgStatistic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgStatistic dialog


CDlgStatistic::CDlgStatistic(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStatistic::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgStatistic)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	AFX_MANAGE_STATE(AfxGetModuleState());        //以下两句一定要加
	AfxGetModuleState()->m_dwVersion = 0x0601;

	m_iTotalCount = 0;
	m_iTotalGood = 0;
	m_iTotalBad = 0;
	m_iFieldCount = 7;


}


void CDlgStatistic::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgStatistic)
	DDX_Control(pDX, IDC_LIST1, m_listData);
	DDX_Control(pDX, IDC_COMBO_MODEL_NAME, m_Model);
	DDX_Control(pDX, IDC_STOP_TIME, m_StopTime);
	DDX_Control(pDX, IDC_START_TIME, m_StartTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgStatistic, CDialog)
	//{{AFX_MSG_MAP(CDlgStatistic)
	ON_NOTIFY(MCN_SELECT, IDC_START_TIME, OnSelectStartTime)
	ON_NOTIFY(MCN_SELCHANGE, IDC_START_TIME, OnSelchangeStartTime)
	ON_BN_CLICKED(IDC_BTN_EXCEL, OnBtnExcel)
	ON_BN_CLICKED(IDC_BTN_QUERY, OnBtnQuery)
	ON_NOTIFY(MCN_SELCHANGE, IDC_STOP_TIME, OnSelchangeStopTime)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgStatistic message handlers

void CDlgStatistic::OnSelectStartTime(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CString str;
	SYSTEMTIME syTime;

	if(0!=m_StartTime.GetCurSel(&syTime))
	{
		str.Format("%d",syTime.wDay);

	}
	*pResult = 0;
}

void CDlgStatistic::OnSelchangeStartTime(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	SYSTEMTIME syTime;


	if(0!=m_StartTime.GetCurSel(&syTime))
	{
		m_sStartYear.Format("%d",syTime.wYear);
		m_sStartMonth.Format("%d",syTime.wMonth);
		m_sStartDay.Format("%d",syTime.wDay);
	}

	*pResult = 0;
}

void CDlgStatistic::OnSelchangeStopTime(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	SYSTEMTIME syTime;
	
	if(0!=m_StopTime.GetCurSel(&syTime))
	{
		m_sStopYear.Format("%d",syTime.wYear);
		m_sStopMonth.Format("%d",syTime.wMonth);
		m_sStopDay.Format("%d",syTime.wDay);		
	}
	*pResult = 0;
}

BOOL CDlgStatistic::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	int iWidth=0;
	CRect rcList;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CTime sTime = CTime::GetCurrentTime();
	m_sAlgModelPath = pFrame->m_sAlgIniFilePath;
	m_sModelName = pFrame->m_sModelName;

	m_sStartYear.Format("%d",sTime.GetYear());
	m_sStartMonth.Format("%d",sTime.GetMonth()) ;
	m_sStartDay.Format("%d",sTime.GetDay());
	m_sStopYear = m_sStartYear;
	m_sStopMonth = m_sStartMonth;
	m_sStopDay = m_sStartDay;

	InitComboProduct();
	GetDlgItem(IDC_BTN_EXCEL)->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_DATE))->SetCheck(TRUE);

	iWidth = 70;
	m_listData.GetWindowRect(&rcList);
	m_listData.DeleteAllItems();
	m_listData.ModifyStyle(NULL,LVS_REPORT);
	m_listData.SetExtendedStyle(LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_listData.InsertColumn(0, "用户名称", LVCFMT_LEFT,iWidth*2);	
	m_listData.InsertColumn(1, "开检时间", LVCFMT_LEFT,iWidth*2);	
	m_listData.InsertColumn(2, "停检时间", LVCFMT_LEFT,iWidth*2);
	m_listData.InsertColumn(3, "模板名称", LVCFMT_LEFT,(int)(iWidth*1.5));	
	m_listData.InsertColumn(4, "检测总数", LVCFMT_LEFT,(int)(iWidth*1.5));
	m_listData.InsertColumn(5, "合格品数", LVCFMT_LEFT,(int)(iWidth*1.5));
	m_listData.InsertColumn(6, "不合格数", LVCFMT_LEFT,(int)(iWidth*1.5));
	m_listData.InsertColumn(7, "不合格率", LVCFMT_LEFT,(int)(iWidth*1.5));

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgStatistic::OnBtnExcel() 
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
	strFilePath.Format("%s\\MP.csv", pFrame->m_sWorkPath);
	BOOL bRet=OpenCsvFile(strFilePath);
	
	if(bRet==FALSE)
	{
		return;
	}
	
	strTemp.Format("%s","标签检测结果统计\n\n");
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
	
	strTemp.Format("%s,%s,%s,%s,%s,%s,%s,%s,%s\n","序号", "用户名称", "开检时间", "停检时间", "模板名称","检测总数", "合格品数","不合格数","不合格率");
	m_stdFile.WriteString(strTemp);
	
	nListCount=m_listData.GetItemCount()-5;
	nListColumn=m_listData.GetHeaderCtrl()->GetItemCount();

	strTemp1="123";
	
	
	for(i=0;i<nListCount;i++)
	{
		strTemp.Format("%d,", i+1);
		for(j=0;j<nListColumn;j++)
		{
			strTemp1.Format("%s",m_listData.GetItemText(i,j));
//			strTemp.Format("%s%s,", strTemp, strTemp1);
			strTemp+=strTemp1+",";
		}
		
		strTemp+="\n";
		m_stdFile.WriteString(strTemp);
	}

	// 写入空行
	strTemp="";
	strTemp+="\n";
	m_stdFile.WriteString(strTemp);

	// 写入合计
	i+=1;
	j=0;

	strTemp1.Format("%s",m_listData.GetItemText(i,j));
	strTemp.Format("%s\n",strTemp1);
	m_stdFile.WriteString(strTemp);

	// 写入检测总数
	i+=1;
	j=0;
	strTemp1.Format("%s",m_listData.GetItemText(i,j));
	strTemp.Format("%s,",strTemp1);
	j++;
	strTemp1.Format("%s",m_listData.GetItemText(i,j));
	strTemp.Format("%s%s\n",strTemp,strTemp1);
	m_stdFile.WriteString(strTemp);


	// 写入好品总数
	i+=1;
	j=0;
	strTemp1.Format("%s",m_listData.GetItemText(i,j));
	strTemp.Format("%s,",strTemp1);
	j++;
	strTemp1.Format("%s",m_listData.GetItemText(i,j));
	strTemp.Format("%s%s\n",strTemp,strTemp1);
	m_stdFile.WriteString(strTemp);	
	
	// 写入坏品总数
	i+=1;
	j=0;
	strTemp1.Format("%s",m_listData.GetItemText(i,j));
	strTemp.Format("%s,",strTemp1);
	j++;
	strTemp1.Format("%s",m_listData.GetItemText(i,j));
	strTemp.Format("%s%s\n",strTemp,strTemp1);
	m_stdFile.WriteString(strTemp);
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

void CDlgStatistic::OnBtnQuery() 
{
	// TODO: Add your control notification handler code here

	CString strSQL;

	UpdateData(TRUE);
	m_bCheckDate = FALSE;
	m_bCheckModel= FALSE;

	m_bCheckDate = ((CButton*)GetDlgItem(IDC_CHECK_DATE))->GetCheck();
	m_bCheckModel = ((CButton*)GetDlgItem(IDC_CHECK_MODEL_NAME))->GetCheck();
	GetDlgItemText(IDC_COMBO_MODEL_NAME,m_sModelName);
	m_listData.DeleteAllItems();

	m_sStartTime.Format("%s-%s-%s 00:00:00",m_sStartYear,m_sStartMonth,m_sStartDay);
	m_sStopTime.Format("%s-%s-%s 23:59:59",m_sStopYear,m_sStopMonth,m_sStopDay);

	if(FALSE==m_bCheckDate&&FALSE==m_bCheckModel)
	{
		strSQL.Format("Select * From %s", "All_Check_Info");
	}
	else if(TRUE==m_bCheckDate && FALSE==m_bCheckModel)
	{
		strSQL.Format("Select * From %s where StartTime>=#%s# and StartTime<=#%s#", "All_Check_Info",m_sStartTime,m_sStopTime);
	}
	else if(FALSE==m_bCheckDate && TRUE==m_bCheckModel)
	{
		strSQL.Format("Select * From %s where ModelName='%s'", "All_Check_Info",m_sModelName);
	}
	else if(TRUE==m_bCheckDate && TRUE==m_bCheckModel)
	{
		strSQL.Format("Select * From %s where StartTime>=#%s# and StartTime<=#%s# and ModelName='%s'", "All_Check_Info",
			m_sStartTime,m_sStopTime,m_sModelName);
	}
	
	GetRecordSet(strSQL);
	OutputRecord();

}

void CDlgStatistic::InitComboProduct()
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
				m_Model.AddString(fd.GetFileName());
			}	
		}
	}

	m_Model.SetWindowText(m_sModelName);

	UpdateData(FALSE);
}

void CDlgStatistic::Init(_ConnectionPtr pConn, _RecordsetPtr pRecord)
{
	m_Connect = pConn;
	m_RecordSet = pRecord;
}

BOOL CDlgStatistic::GetRecordSet(CString sSQL)
{
	CString strTemp("");
	int nTotalCount=0;
	int nRecordCount=0;

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

			// 【2017-6-14 zhy】
			var=m_RecordSet->GetCollect("User");
			if(var.vt==VT_NULL||var.vt==VT_EMPTY)
			{
				strTemp="";
			}
			else
			{
				strTemp = (LPCSTR)_bstr_t(var);
			}
			
			pStrList->AddTail(strTemp);
			///////////////////////////////////////
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
			m_iTotalCount=nTotalCount;


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
			m_iTotalGood=nTotalCount;


			
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
			m_iTotalBad=nTotalCount;

	
			var=m_RecordSet->GetCollect("TotalBadRatio");
			if(var.vt==VT_NULL||var.vt==VT_EMPTY)
			{
				strTemp="";
			}
			else
			{
				strTemp = (LPCSTR)_bstr_t(var);
			}

			pStrList->AddTail(strTemp);
			
	
			m_PtrList.AddTail(pStrList);
			nRecordCount++;
		}
		
		m_RecordSet->MoveNext();
	}

	m_RecordSet->Close();

	return TRUE;
}
BOOL CDlgStatistic::OutputRecord()
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
	
	
	m_listData.InsertItem(nRow,"");
	
	nRow++;
	m_listData.InsertItem(nRow,"合计:");
	
	nRow+=1;
	strTemp.Format("%d",m_iTotalCount);
	m_listData.InsertItem(nRow,"");
	m_listData.SetItemText(nRow,0,"检测总数:");
	m_listData.SetItemText(nRow,1,strTemp);
	
	nRow+=1;
	strTemp.Format("%d",m_iTotalGood);
	m_listData.InsertItem(nRow,"");
	m_listData.SetItemText(nRow,0,"好品总数:");
	m_listData.SetItemText(nRow,1,strTemp);
	
	nRow+=1;
	strTemp.Format("%d",m_iTotalBad);
	m_listData.InsertItem(nRow,"");
	m_listData.SetItemText(nRow,0,"坏品总数:");
	m_listData.SetItemText(nRow,1,strTemp);


	return TRUE;
}

BOOL CDlgStatistic::OpenCsvFile(CString strFilePath)
{
	if(!m_stdFile.Open(strFilePath, CFile::modeCreate|CFile::modeReadWrite|CFile::typeText))
	{
		AfxMessageBox("MP.csv 打开失败，注意MP.csv是否已经打开!");
		return FALSE;
	}
	
	return TRUE;
}

void CDlgStatistic::CloseCsvFile()
{
	m_stdFile.Close();
}

