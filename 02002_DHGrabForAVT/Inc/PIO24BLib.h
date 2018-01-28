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

class __declspec(dllexport) CPIO24BLib  
{
public:
	CPIO24BLib();
	~CPIO24BLib();

	//Initialization
	bool openCard(unsigned char DeviceNum);
	bool initRegisters(const char* pIniFileName, const char* pIniAppName);
	UINT readIniFileReg(int regIndex, int section); //暂不使用
	void writeIniFileReg(int keyName, int keyValue, int section);

	//Register Operation
	void writeRegister(unsigned int addr, unsigned short data); //用地址定位寄存器
	void writeParam(int paramIndex, unsigned short data);
	unsigned short readRegister(unsigned int addr); //用地址定位寄存器
	unsigned short readParam(int paramIndex);
	unsigned short readCounter(int counterIndex);
	unsigned short readStatus(int counterIndex);
	void pedgeParamBit(int paramNum, int bit);
	void negateParamBit(int paramNum, int bit);
	void pedgeRegisterBit(int regAddr, int bit); //用地址定位寄存器
	void negateRegisterBit(int regAddr, int bit); //用地址定位寄存器

	//PIO24B's Functions
	void sendChckRslt(unsigned short bySignEnable, unsigned short pictureSN, unsigned short picChannel, unsigned short Bias=0);
	void setCardOutput(unsigned short portIndex, bool outValue, BOOL isDsPort=false);
	void softReset();
	void hardReset();
	void enable(bool enableCard);

	//Useful Functions
	static bool getCurrentAppPath(char* pStr, int length);
	static int hexStrToInt(char *hexStr);
	int Show_PIO24B_DebugDialog(void *pParentWnd=NULL);

	//Internal Status
	bool hasCard, hasInitFile, isDialogOpen;
	char iniFullFileName[BUFFER_SIZE_PIO24B], iniAppName[BUFFER_SIZE_PIO24B];
	void *pVars;
};

#pragma  pack() 
#endif // !defined(AFX_PIO24BLIB_H__725CC319_FDC7_44B6_8F4F_C0FC827D5359__INCLUDED_)
