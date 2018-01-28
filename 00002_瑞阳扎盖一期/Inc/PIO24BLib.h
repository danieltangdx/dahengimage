// PIO24BLib.h: interface for the CPIO24BLib class.
//////////////////////////////////////////////////////////////////////

// *** Usage Example ***
//
// CPIO24BLib pio24b; //Defined as class member
//
// if(!pio24b.openCard(0)) {
//   ERROR;
// }
// pio24b.hardReset();
// if(!pio24b.initRegisters("PIO24B_reg_init.txt", "PIO24B")) {
//   ERROR;
// }
// pio24b.softReset();
// pio24b.enable(true);
// pio24b.softReset(); //need reset twice
// pio24b.Show_PIO24B_DebugDialog(NULL);

#if !defined(AFX_PIO24BLIB_H__725CC319_FDC7_44B6_8F4F_C0FC827D5359__INCLUDED_)
#define AFX_PIO24BLIB_H__725CC319_FDC7_44B6_8F4F_C0FC827D5359__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma pack(1)

/*
struct __declspec(dllexport) _PLX_DEVICE_OBJECT;  
#ifndef __PLX_TYPES_H
typedef _PLX_DEVICE_OBJECT PLX_DEVICE_OBJECT;
#endif
*/

#define BUFFER_SIZE_PIO24B 5000

#define PIO24B_CONST_OUTPUT_VALUE_NOCHANGE 2
#define PIO24B_CONST_OUTPUT_VALUE_NEGATE 3

class __declspec(dllexport) CPIO24BLib  
{
public:
	CPIO24BLib();
	~CPIO24BLib();
	enum DLL_USER_TYPE {USER_CLIENT=0, USER_PIO24CRELOAD=1};

	//Initialization
	bool openCard(unsigned char deviceNum, unsigned char deviceType=0, DLL_USER_TYPE dll_user=USER_CLIENT);
	bool closeCard();
	int currentCardQuantity();
	bool initRegisters(const char* pIniFileName, const char* pIniAppName);
	int readIniFileReg(int regIndex, int section); //暂不使用
	void writeIniFileReg(int keyName, int keyValue, int section);

	//Register Operation
	void writeRegister(unsigned int addr, unsigned short data, unsigned char PCI_space=2, unsigned char BitSize=16); //用地址定位寄存器
	void writeBuffer(unsigned int addr, void *pBuffer, int maxNum, unsigned char PCI_space=2, unsigned char BitSize=16); //用地址定位寄存器
	void writeParam(int paramIndex, unsigned short data);  //需手工避免Functions下面的寄存器访问
	int readRegister(unsigned int addr, unsigned char PCI_space=2, unsigned char BitSize=16); //用地址定位寄存器
	void readBuffer(unsigned int addr, void *pBuffer, int maxNum, unsigned char PCI_space=2, unsigned char BitSize=16); //用地址定位寄存器
	int readParam(int paramIndex);
	int readCounter(int counterIndex);
	int readPicSn(int counterIndex);
	int readStatus(int counterIndex);
	void pedgeParamBit(int paramNum, int bit);  //虽然加锁，但需手工避免Functions下面的寄存器访问
	void negateParamBit(int paramNum, int bit);
	void clrParamBit(int paramNum, int bit);
	void setParamBit(int paramNum, int bit);
	void pedgeRegisterBit(int regAddr, int bit, unsigned char PCI_space=2, unsigned char BitSize=16); //用地址定位寄存器
	void negateRegisterBit(int regAddr, int bit, unsigned char PCI_space=2, unsigned char BitSize=16); //用地址定位寄存器
	void clrRegisterBit(int regAddr, int bit, unsigned char PCI_space=2, unsigned char BitSize=16); //用地址定位寄存器
	void setRegisterBit(int regAddr, int bit, unsigned char PCI_space=2, unsigned char BitSize=16); //用地址定位寄存器

	//PIO24B's Functions
	void sendChckRslt(unsigned short bySignEnable, unsigned short pictureSN, unsigned short picChannel, unsigned short Bias=0);
	void setCardOutput(unsigned short portIndex, int outValue, bool isDsPort=false); //outValue:0(关闭),1(开启),2(内部输出),3(求反)
	void softReset();
	void hardReset();
	void enable(bool enableCard);
	int check_sensor_distance(int);

	//Useful Functions
	static bool getCurrentAppPath(char* pStr, int length);
	static int hexStrToInt(char *hexStr);
	int Show_PIO24B_DebugDialog(void *pParentWnd=NULL);
	int Show_PIO24C_ProgramDialog(int factoryFlag=0, void *pParentWnd=NULL);
	int get_trigger_pedge_info(int searchedSN);
	int get_trigger_nedge_info(int searchedSN);

	//Internal Status
	bool hasCard, hasInitFile, isDialogOpen;
	char iniFullFileName[BUFFER_SIZE_PIO24B], iniAppName[BUFFER_SIZE_PIO24B];
	int read_status_var(int index);
	void write_status_var(int index, int content);
	int status[4095];
	void *pVars;
};




#pragma  pack() 
#endif // !defined(AFX_PIO24BLIB_H__725CC319_FDC7_44B6_8F4F_C0FC827D5359__INCLUDED_)
