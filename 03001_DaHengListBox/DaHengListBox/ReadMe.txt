2015-08-31 修改增加对灯检机文件夹的支持
// 灯检机Circle文件夹发送
// E:\SaveImages\Good\2015-08-21-22-22\Circle123 [8/31/2015 Administrator]
BOOL CDHFileBox::DirOrBmp(const CString& strFilePath)
{
	//////////////////////////////////////////////////////////////////////////
	int index = strFilePath.ReverseFind('\\');
	CString strAJDZ=strFilePath.Right(strFilePath.GetLength()-index-1);
	if (strAJDZ.Find("Circle") >=0)//文件夹的名字中包含Circle
	{
		//判断这个文件夹下面是否含有bmp文件
		CFileFind finder;
		BOOL bRet = finder.FindFile(strFilePath+"\\*.bmp",0);
		if (bRet)
		{
			//灯检机Circle文件夹
			return TRUE;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (strFilePath.Right(4)==".bmp" || strFilePath.Right(4)==".BMP")
		return TRUE;
	else
		return FALSE;
}

Date: 2017-06-08 version：2, 2, 1, 1 ModifiedBy:ConanSteve
//增加ModelListBox过滤显示功能 

Date:2017-06-21		Version：2, 2, 1, 2		 ModifiedBy:张凡
	//1、修复第一个模板不能使用右键弹出菜单的问题
	//2、开放重命名模板功能