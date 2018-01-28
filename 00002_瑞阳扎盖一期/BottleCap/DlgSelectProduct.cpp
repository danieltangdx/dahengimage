// SelectProductDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BottleCap.h"
#include "MainFrm.h"
#include "DlgSelectProduct.h"
#include "BottleCapVIEW.h"
#include "DlgNewModel.h"
#include "DlgTestCipher.h"
#include "..\\Inc\\ObjAlloc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectProduct dialog


CDlgSelectProduct::CDlgSelectProduct(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelectProduct::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelectProduct)
	m_strEditNew = _T("");
	//}}AFX_DATA_INIT
	m_strAlgIniPath=_T("");
	m_nCount=0;
	m_nCameraSN=0;

}


void CDlgSelectProduct::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelectProduct)
	DDX_Control(pDX, IDC_LIST_MODEL, m_ctrList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelectProduct, CDialog)
	//{{AFX_MSG_MAP(CDlgSelectProduct)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectProduct message handlers

// 获取本执行程序所在的路径
CString CDlgSelectProduct::GetCurrentAppPath()
{
	char str[MAX_PATH], drive[MAX_PATH], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
	CString AppPath;
	GetModuleFileName(NULL, str, sizeof(str));
	_splitpath(str, drive, dir, fname, ext);
	strcat(drive, dir);
	AppPath = drive;
	return AppPath;
}

VOID OP_Directory(LPCSTR lpsrcPath, LPCSTR lpdstPath, int type,int flag)
{   
	SHFILEOPSTRUCT FileOp;   
	
	FileOp.hwnd   =   NULL;   
	FileOp.wFunc   =   type;   
	FileOp.pFrom   =   lpsrcPath;   
	FileOp.pTo   =   lpdstPath;   
	FileOp.fFlags   =   flag;//FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR|FOF_NOERRORUI|FOF_SILENT;   
	FileOp.hNameMappings   =   NULL;   
	FileOp.lpszProgressTitle   =   NULL;   
	
	int hr=0;
	hr = SHFileOperation(&FileOp);

	CString str1;
	if(hr!=0)
	{
		str1.Format("删除文件夹%s失败!",lpsrcPath);
		AfxMessageBox(str1);
	}
}

BOOL CDlgSelectProduct::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_strAppPath=GetCurrentAppPath();
#ifdef __SGGrabber_                                // 模拟采集
	m_strAppPath+="\\Config\\CheckSystemConfig-sim.ini";
#else
	m_strAppPath+="\\Config\\CheckSystemConfig.ini";
#endif
	RefreshList();

// 	CString strName;
// 	m_ctrList.GetText(m_ctrList.GetCurSel(),strName);
// 	WritePrivateProfileString("系统配置", "上一次使用模板名称", strName, m_strAppPath);
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgSelectProduct::OnDestroy() 
{
	CDialog::OnDestroy();
// 	m_vtProductName.clear();	
}

void CDlgSelectProduct::RefreshList()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CString  strModelName = pFrame->m_sModelName;
	int iNum=0,iModeNameNum=0;

	m_ctrList.ResetContent();
	CFileFind fd;
	CString str;
	
	BOOL bFind=fd.FindFile(m_strAlgIniPath+"\\*.*");
	
	while(bFind)
	{
		//打开子目录，读出文件夹名称
		bFind=fd.FindNextFile();
		if(fd.IsDirectory()&&!fd.IsDots())
		{
			if (fd.GetFileName()!="mod1")
			{
				m_ctrList.AddString(fd.GetFileName());
				iNum++;
			}	
			
			CString str= fd.GetFileName();
			
			if(fd.GetFileName()==strModelName)
			{
				
				iModeNameNum=iNum;
			}	
		}
	}

	if (m_ctrList.GetCount()>0)
	{
//		m_ctrList.SetCurSel(0);
		m_ctrList.SetCurSel(iModeNameNum-1);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	}
}

// 新建模板
void CDlgSelectProduct::OnButtonNew() 
{
	// TODO: Add your control notification handler code here

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CBottleCapView* pView = (CBottleCapView*) pFrame->GetActiveView();
	
// 	if(FALSE==pFrame->m_bAdvMode)
// 	{
// 		CDlgTestCipher dlg;
// 		if(dlg.DoModal() == IDOK && dlg.m_bOK == TRUE)
// 		{
// 			pFrame->m_bAdvMode = TRUE;
// 			pView->GetDlgItem(IDC_ADV_MODE)->ShowWindow(SW_SHOW);
// 		}
// 		else
// 		{
// 			return;
// 		}
// 	}

	UpdateData();

	GetDlgItemText(IDC_EDIT_NEW_MODEL_NAME,m_strEditNew);

	if( m_strEditNew.Find(".")>=0||
		m_strEditNew.Find("\\")>=0||
		m_strEditNew.Find("/")>=0||
		m_strEditNew.Find("?")>=0||
		m_strEditNew.Find("*")>=0||
		m_strEditNew.Find(":")>=0||
		m_strEditNew.Find("|")>=0||
		m_strEditNew.Find("\"")>=0||
		m_strEditNew.Find("<")>=0||
		m_strEditNew.Find(">")>=0||
		m_strEditNew.Find("&")>=0)
	{
		AfxMessageBox("输入的字符含有非法字符，请重新输入！");
		return;
	}

	if(m_strEditNew=="")
	{
		AfxMessageBox("您没有输入任何内容");
		return ;
	}

	CString strProductName,str,strNewAlgPath,strIniPath,strSamplePath;
	CString strPath1,strPath2,str1;

	int i=0;
	for (i=0;i<m_ctrList.GetCount();i++)
	{
		CString str;
		m_ctrList.GetText(i,str);
		if(str==m_strEditNew)
		{
			AfxMessageBox("新建模板与已有模板同名!");
			return;
		}
	}
	strProductName=m_strEditNew;
	
	// 创建算法配置文件夹路径
	strNewAlgPath.Format("%s\\ModelInfo\\%s",pFrame->m_sWorkPath,strProductName);
	CreateDirectory(strNewAlgPath,NULL);
		
// 	for(i=0;i<ALLELEMENT_COUNT;i++)
// 	{
// 		str.Format("%s\\%d",strNewAlgPath,i+1);
// 		if(FALSE==CreateDirectory(str,NULL))
// 		{
// 			AfxMessageBox("创建目录失败，文件夹名字可能含有非法字符!");
// 			return;
// 		}
// 		CString strName;
// 		strName.Format("%s\\ModelParas.ini",str);
// 		CFile fileT;
// 		fileT.Open(strName,CFile::modeCreate|CFile::modeReadWrite|CFile::shareDenyNone);
// 		fileT.Close();
// 	}

// 	// 新建模板名字
// 	strPath1.Format("%s\\ModelInfo\\%s\\1\\ModelParas.ini",pFrame->m_sWorkPath,m_strEditNew);
// 	strPath2.Format("%s\\ModelInfo\\%s\\2\\ModelParas.ini",pFrame->m_sWorkPath,m_strEditNew);

	UpdateData(FALSE);
	RefreshList();

	// 拷贝相机配置文件
	CString strDest;
	for(i=0;i<ALLELEMENT_COUNT;i++)
	{
		strDest.Format("%s\\%s",strNewAlgPath,pFrame->m_sCamConfigName[i]);
		BOOL bRet = CopyFile(pFrame->m_sCamConfigPath[i],strDest,false);

		if(FALSE==bRet)
		{
			AfxMessageBox("拷贝相机配置文件失败!");
			return;
		}
	}

	
	CString strDemo;
	CString strVirtualDiskPath;
	
	strDemo.Format("%s\\DEMO.exe",pFrame->m_sWorkPath);	
	strVirtualDiskPath.Format("%s\\Config\\AlgConfig.ini",pFrame->m_sWorkPath);
	WritePrivateProfileString("DEMO Controls","DefaultModel",strProductName,strVirtualDiskPath);
	strVirtualDiskPath.Format("%s\\Config\\CheckSystemConfig.ini",pFrame->m_sWorkPath);
	WritePrivateProfileString("系统配置","上一次使用算法模板名称",strProductName,strVirtualDiskPath);
	
	HINSTANCE hRet= ShellExecute(NULL,"open",strDemo,NULL,NULL,SW_SHOWNORMAL);		
}

//删除模板
void CDlgSelectProduct::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame= (CMainFrame*)AfxGetMainWnd();
	CBottleCapView* pView = (CBottleCapView*) pFrame->GetActiveView();
	
// 	if(FALSE==pFrame->m_bAdvMode)
// 	{
// 		CDlgTestCipher dlg;
// 		if(dlg.DoModal() == IDOK && dlg.m_bOK == TRUE)
// 		{
// 			pFrame->m_bAdvMode = TRUE;
// 			pView->GetDlgItem(IDC_ADV_MODE)->ShowWindow(SW_SHOW);
// 		}
// 		else
// 		{
// 			return;
// 		}
// 	}


	if(m_ctrList.GetCurSel()<0)
		return;
	CString str,strMsg,strSample;
	CString tstr;
	m_ctrList.GetText(m_ctrList.GetCurSel(),tstr);
	str.Format("%s\\%s",m_strAlgIniPath,tstr);
	strSample.Format("%s\\正样本图片\\%s",pFrame->m_sWorkPath,tstr);

	char ch[255],chSample[255];
	memset(ch,0,255);
	memset(chSample,0,255);
	memcpy(ch,str.GetBuffer(str.GetLength()),str.GetLength());
	memcpy(chSample,strSample.GetBuffer(strSample.GetLength()),strSample.GetLength());

	if(pFrame->m_sModelName==tstr)
	{
		strMsg.Format("当前正在使用模板 '%s', 不能删除该模板!",tstr);
		AfxMessageBox(strMsg);
		return;
	}

	// 执行删除操作
	OP_Directory(ch,NULL,FO_DELETE,0);
	OP_Directory(chSample,NULL,FO_DELETE,FOF_NOCONFIRMATION );

	RefreshList();
}

// 加载模板
void CDlgSelectProduct::OnOK() 
{
	// TODO: Add extra validation here

	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CBottleCapView* pView=(CBottleCapView*)pFrame->GetActiveView();

	//提示正在加载对话框

	CString strListContext,strOldModelName,strNewModelName;
	CString strDesc,strBatchModify,strOriBatchCode,strCurrBatchCode;
	CFileFind find;

	// 已经获取模板名称,从配置文件中读取产品批次信息
	CString strModelInfoIniFile;
	CString strCode,strSize,strFormat,strAluminumMaterial,strBrand,strColor,strMouldNum;

	m_ctrList.GetText(m_ctrList.GetCurSel(),strListContext);

	strNewModelName = strListContext;
	strOldModelName = pFrame->m_sModelName;

	BOOL bAlg1=FALSE;
	s_AlgModelInput sAlgModel;
	s_Status sRet;
	
	ZeroMemory(&sAlgModel,sizeof(s_AlgModelInput));
	
	// 告诉算法同时加载配置参数和模板
	CString strAlgModel;
	strAlgModel.Format("%s\\Config\\AlgConfig.ini",pFrame->m_sWorkPath);
	WritePrivateProfileString("检测控制","bReadModel","1",strAlgModel);

	if(TRUE==pFrame->m_bOpenAlgCheck)
	{

		// 需要读参数
		s_Status sRet;
		s_AlgModelInput sModelInfo;
		ZeroMemory(&sModelInfo,sizeof(s_AlgModelInput));
		ZeroMemory(&sRet,sizeof(s_Status));
		
		s_DetectModelInput sParam;
		ZeroMemory(&sParam,sizeof(s_DetectModelInput));
		
		CString strModelPath;
		strModelPath.Format("%s\\ModelInfo\\%s",pFrame->m_sWorkPath,strNewModelName);
		memcpy(sParam.chPath,strModelPath.GetBuffer(0),strModelPath.GetLength());
		
		CString strLogPath;
		strLogPath.Format("%s\\MendParamLog",pFrame->m_sLogFilePath);
		memcpy(sParam.chLog,strLogPath.GetBuffer(0),strLogPath.GetLength());
		
		ZeroMemory(sParam.chImageBufferPath,MAX_LENGTH);
		
		int* pModelParam[ALLELEMENT_COUNT];
		
		
		for(int i=0;i<ALLELEMENT_COUNT;i++)
		{
			CString strAlgImages;
			
			strAlgImages.Format("%s:\\SaveImages\\AlgImages\\Camera%d",pFrame->m_sVirtualDisk,i+1);
			memcpy(sParam.chAbnormalPath,strAlgImages.GetBuffer(0),strAlgImages.GetLength());
			
			
			sModelInfo.iCheckSN=i;
			sModelInfo.iOperSig=0;
			
			pModelParam[i] = (int*)&sParam;
			
			sModelInfo.pParam=&pModelParam[i];
			
			sRet = pFrame->m_InspectionObj.Set(sModelInfo);

			//如果加载失败，弹出算法错误描述
			if(sRet.iStatusType != 0)
			{
				pFrame->m_bCalAlg = FALSE;
				bAlg1 = FALSE;
				break;
			}
			else
			{
				bAlg1 = TRUE;
			}

		}

			
		if(bAlg1==TRUE)
		{
			// 加载成功会调用算法检测，否则不调用算法检测
			WritePrivateProfileString("系统配置", "上一次使用算法模板名称", strListContext, m_strAppPath);
			pFrame->m_bCalAlg=TRUE;	
		}
		
		
		// 更新界面模板名称
		pFrame->m_sModelName = strListContext;
		pView->m_ModelNameData.Display(strListContext.GetBuffer(strListContext.GetLength()),"",0,FALSE);
		
		if(strListContext!=strOldModelName)
		{
			pFrame->SoftResetIOCard(strOldModelName);
		}
		else
		{
			if(strOriBatchCode!=strCurrBatchCode)
			{
				pFrame->SoftResetIOCard(strOldModelName);
			}
		}
		
		
		CString strMsg;
		if(bAlg1)
		{
			strMsg.Format("加载模板'%s'成功!",strNewModelName);
#ifdef __RealGrabber_
		pFrame->LoadCamConfig(strNewModelName);
#endif
			AfxMessageBox(strMsg);
		}
		else
		{
			strMsg.Format("加载模板'%s'失败!\n目前只能采集图像，不能检测图像!",strNewModelName);
			AfxMessageBox(strMsg);
		}
	}

	CDialog::OnOK();
}

void CDlgSelectProduct::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}








