#pragma once
class CImageOfSys
{
public:
	CImageOfSys();
	~CImageOfSys();
};

/*************************************<图像处理类>************************************[9/6/2017 ConanSteve]*/
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
enum e_ShowModeFlip	//翻转显示方式
{
	NoFlip = 0,					//不翻转显示
	FlipVertical,				//垂直翻转显示
	FlipHorizintal,				//水平翻转显示
	Flip						//全翻转显示
};	//翻转显示方式

enum e_ShowModeStretch	//拉伸显示方式
{
	NoStretch = 0,				//不拉伸显示
	Stretch						//拉伸显示
};	//拉伸显示方式

enum eAutoSaveErrorInfo
{
	GrabAllFaultError = -9,
	ParamTypeError,				// 参数2保存类型错误错误 [8/19/2009 GZ]
	ParamLPElementError,			// 参数1错误元素指针为空 [8/19/2009 GZ]
	ListCountError,					// 获得的元素中的链表个数为零或负数 [8/19/2009 GZ]
	SaveBMPError,					// 函数SaveBMPFile()未成功 [8/19/2009 GZ]
	CreatePathError,				// 创建路径错误
	FileNameError,					// 文件名错误
	ImageWHBITError,				// 图片长宽位数错误
	ImageBufferError				// 图像数据区错误
};
class CImgHandle
{
	//**[9/6/2017 ConanSteve]**:  对图像做镜像
	static bool MirrorSingleImage(BYTE* pImageBuff, int lImageWidth, int lImageHeight, int lImageBitCount, e_ShowModeFlip  eShowMode);
};

