// 高级命令字
#ifndef _ROCKEY4_ND_32_
#define _ROCKEY4_ND_32_

//#define FUNCTION_ROCKEY4ND //使用RockeyND代替原函数名Rockey
#define FUNCTION_ROCKEY    //函数名使用Rockey


//函数id
/* 基本格式:
(1) 查找锁
    输入参数:
    function = 0
    *p1 = pass1
    *p2 = pass2
    *p3 = pass3
    *p4 = pass4
    返回:
    *lp1 为锁的硬件 ID
    返回为 0 表示成功, 其它为错误码

(2) 查找下一锁
    输入参数:
    function = 1
    *p1 = pass1
    *p2 = pass2
    *p3 = pass3
    *p4 = pass4
    返回:
    *lp1 为锁的硬件 ID
    返回为 0 表示成功, 其它为错误码

(3) 打开锁
    输入参数:
    function = 2
    *p1 = pass1
    *p2 = pass2
    *p3 = pass3
    *p4 = pass4
    *lp1 = 硬件 ID
    返回:
    *handle 为锁的句柄
    返回为 0 表示成功, 其它为错误码

(4) 关闭锁
    输入参数:
    function = 3
    *handle = 锁的句柄
    返回:
    返回为 0 表示成功, 其它为错误码

(5) 读锁
    输入参数:
    function = 4
    *handle = 锁的句柄
    *p1 = pos
    *p2 = length
    buffer = 缓冲区的指针
    返回:
    buffer 中添入读入的内容
    返回为 0 表示成功, 其它为错误码

(6) 写锁
    function = 5
    *handle = 锁的句柄
    *p1 = pos
    *p2 = length
    buffer = 缓冲区的指针
    返回:
    返回为 0 表示成功, 其它为错误码

(7) 随机数
    function = 6
    *handle = 锁的句柄
    返回:
    *p1,*p2,*p3,*p4 = 随机数
    返回为 0 表示成功, 其它为错误码
    
(8) 种子码
    function = 7
    *handle = 锁的句柄
    *lp2 = 种子码
    返回:
    *p1 = 返回码1
    *p2 = 返回码2
    *p3 = 返回码3
    *p4 = 返回码4
    返回为 0 表示成功, 其它为错误码

(9) 写用户 ID [*]
    function = 8
    *handle = 锁的句柄
    *lp1 = 用户 ID
    返回:
    返回为 0 表示成功, 其它为错误码

(10) 读用户 ID
     function = 9
     *handle = 锁的句柄
     返回:
     *lp1 = 用户 ID
     返回为 0 表示成功, 其它为错误码

(11) 设置模块 [*]
     function = 10
     *handle = 锁的句柄
     *p1 = 模块号
     *p2 = 用户模块密码
     *p3 = 是否允许递减 (1 = 允许, 0 = 不允许)
     返回:
     返回为 0 表示成功, 其它为错误码

(12) 检查模块是否有效
     function = 11
     *handle = 锁的句柄
     *p1 = 模块号
     返回:
     *p2 = 1 表示此模块有效
     *p3 = 1 表示此模块可以递减
     返回为 0 表示成功, 其它为错误码

(13) 写算法 [*]
     function = 12
     *handle = 锁的句柄
     *p1 = pos
     buffer = 算法指令串
     返回:
     返回为 0 表示成功, 其它为错误码
     
(14) 计算1 (模块字, ID 高位, ID 低位, 随机数)
     function = 13
     *handle = 锁的句柄
     *lp1 = 计算起始点
     *lp2 = 模块号
     *p1 = 输入值1
     *p2 = 输入值2
     *p3 = 输入值3
     *p4 = 输入值4
     返回:
     *p1 = 返回值1
     *p2 = 返回值2
     *p3 = 返回值3
     *p4 = 返回值4
     返回为 0 表示成功, 其它为错误码

(15) 计算2
     function = 14
     *handle = 锁的句柄
     *lp1 = 计算起始点
     *lp2 = 种子码
     *p1 = 输入值1
     *p2 = 输入值2
     *p3 = 输入值3
     *p4 = 输入值4
     返回:
     *p1 = 返回值1
     *p2 = 返回值2
     *p3 = 返回值3
     *p4 = 返回值4
     返回为 0 表示成功, 其它为错误码

(16) 计算3
     function = 15
     *handle = 锁的句柄
     *lp1 = 计算起始点
     *lp2 = 密码起始地址
     *p1 = 输入值1
     *p2 = 输入值2
     *p3 = 输入值3
     *p4 = 输入值4
     返回:
     *p1 = 返回值1
     *p2 = 返回值2
     *p3 = 返回值3
     *p4 = 返回值4
     返回为 0 表示成功, 其它为错误码

(17) 递减
     function = 16
     *handle = 锁的句柄
     *p1 = 模块号
     返回为 0 表示成功, 其它为错误码
*/

#define  RY_FIND                        1		//找锁
#define  RY_FIND_NEXT					2		//找下一锁
#define  RY_OPEN                        3		//打开锁
#define  RY_CLOSE                       4		//关闭锁
#define  RY_READ                        5		//读锁
#define  RY_WRITE                       6		//写锁
#define  RY_RANDOM                      7		//产生随机数
#define  RY_SEED                        8		//产生种子码
#define  RY_WRITE_USERID				9		//写用户 ID
#define  RY_READ_USERID					10		//读用户 ID
#define  RY_SET_MOUDLE					11		//设置模块字
#define  RY_CHECK_MOUDLE				12		//检查模块状态
#define  RY_WRITE_ARITHMETIC            13		//写算法
#define  RY_CALCULATE1					14		//计算 1
#define  RY_CALCULATE2					15		//计算 2
#define  RY_CALCULATE3					16		//计算 3
#define  RY_DECREASE					17		//递减模块单元
#define  RY_CALLNET                     18      //网络锁算法

// 错误码
#define ERR_SUCCESS				0 //没有错误
#define ERR_NO_ROCKEY			3 //没有ROCKEY
#define ERR_INVALID_PASSWORD	4 //有ROCKEY锁，但基本密码错
#define ERR_INVALID_PASSWORD_OR_ID	5 //错误的密码或硬件 ID
#define ERR_SETID				6 //设置硬件 ID 错
#define ERR_INVALID_ADDR_OR_SIZE	7 //读写地址或长度有误
#define ERR_UNKNOWN_COMMAND		8 //没有此命令
#define ERR_NOTBELEVEL3			9 //内部错误
#define ERR_READ				10 //读数据错
#define ERR_WRITE				11 //写数据错
#define ERR_RANDOM				12 //随机数错
#define ERR_SEED				13 //种子码错
#define ERR_CALCULATE			14 //计算错
#define ERR_NO_OPEN				15 //在操作前没有打开锁
#define ERR_OPEN_OVERFLOW		16 //打开的锁太多(>16)
#define ERR_NOMORE				17 //找不到更多的锁
#define ERR_NEED_FIND			18 //没有 Find 直接用了 FindNext
#define ERR_DECREASE			19 //递减错
#define ERR_AR_BADCOMMAND		20 //算法指令错
#define ERR_AR_UNKNOWN_OPCODE	21 //算法运算符错
#define ERR_AR_WRONGBEGIN		22 //算法第一条指令含有常数
#define ERR_AR_WRONG_END		23 //算法最后一条指令含有常数
#define ERR_AR_VALUEOVERFLOW	24 //算法中常数值 > 63
#define ERR_TOOMUCHTHREAD		25 //同一个进程中打开锁的线程数 > 100
#define ERR_RECEIVE_NULL		0x100 //接收不到
#define ERR_UNKNOWN_SYSTEM		0x102 //未知的操作系统
#define ERR_UNKNOWN				0xffff //未知错误

#ifdef FUNCTION_ROCKEY4ND
	EXTERN_C __declspec(dllexport) WORD WINAPI RockeyND(WORD function, WORD* handle, DWORD* lp1,  DWORD* lp2, WORD* p1, WORD* p2, WORD* p3, WORD* p4, BYTE* buffer);
#else
 	EXTERN_C __declspec(dllexport) WORD WINAPI Rockey(WORD function, WORD* handle, DWORD* lp1,  DWORD* lp2, WORD* p1, WORD* p2, WORD* p3, WORD* p4, BYTE* buffer);
#endif

#endif
