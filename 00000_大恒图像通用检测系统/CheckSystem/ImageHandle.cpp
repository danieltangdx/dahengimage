#include "stdafx.h"
#include "ImageHandle.h"


CImageOfSys::CImageOfSys()
{
}


CImageOfSys::~CImageOfSys()
{
}

bool CImgHandle::MirrorSingleImage(BYTE* pImageBuff, int lImageWidth, int lImageHeight, int lImageBitCount, e_ShowModeFlip  eShowMode) //只有水平和垂直镜像
{
	if (pImageBuff == NULL)
	{
		return ImageBufferError;
	}
	if (lImageWidth < 0 || lImageHeight < 0 || lImageBitCount < 0)
	{
		return ImageWHBITError;
	}

	if (lImageBitCount == 8)
	{
		lImageWidth = WIDTHBYTES(lImageWidth * 8);
		if (eShowMode == FlipHorizintal) //水平翻转
		{
			BYTE  tem;
			for (int i = 0; i < lImageHeight; i++)
			{
				for (int j = 0; j < lImageWidth / 2; j++)
				{
					tem = *(pImageBuff + i*lImageWidth + j);
					*(pImageBuff + i*lImageWidth + j) = *(pImageBuff + i*lImageWidth + lImageWidth - 1 - j);
					*(pImageBuff + i*lImageWidth + lImageWidth - 1 - j) = tem;
				}
			}
			return TRUE;
		}
		if (eShowMode == FlipVertical) //垂直翻转
		{
			BYTE  tem;
			for (int i = 0; i < lImageHeight / 2; i++)
			{
				for (int j = 0; j < lImageWidth; j++)
				{
					tem = *(pImageBuff + i*lImageWidth + j);
					*(pImageBuff + i*lImageWidth + j) = *(pImageBuff + (lImageHeight - 1 - i)*lImageWidth + j);
					*(pImageBuff + (lImageHeight - 1 - i)*lImageWidth + j) = tem;
				}
			}
			return TRUE;
		}
	}
	if (lImageBitCount == 24)//24位镜像
	{
		lImageWidth = WIDTHBYTES(lImageWidth * 8);

		if (eShowMode == FlipHorizintal) //水平翻转
		{
			BYTE  tem1, tem2, tem3;
			for (int i = 0; i < lImageHeight; i++)
			{
				for (int j = 0; j < lImageWidth / 2; j++)
				{
					tem1 = *(pImageBuff + i*lImageWidth * 3 + 3 * j);
					tem2 = *(pImageBuff + i*lImageWidth * 3 + 3 * j + 1);
					tem3 = *(pImageBuff + i*lImageWidth * 3 + 3 * j + 2);
					*(pImageBuff + i*lImageWidth * 3 + 3 * j) = *(pImageBuff + i*lImageWidth * 3 + 3 * (lImageWidth - 1 - j));
					*(pImageBuff + i*lImageWidth * 3 + 3 * j + 1) = *(pImageBuff + i*lImageWidth * 3 + 3 * (lImageWidth - 1 - j) + 1);
					*(pImageBuff + i*lImageWidth * 3 + 3 * j + 2) = *(pImageBuff + i*lImageWidth * 3 + 3 * (lImageWidth - 1 - j) + 2);
					*(pImageBuff + i*lImageWidth * 3 + 3 * (lImageWidth - 1 - j)) = tem1;
					*(pImageBuff + i*lImageWidth * 3 + 3 * (lImageWidth - 1 - j) + 1) = tem2;
					*(pImageBuff + i*lImageWidth * 3 + 3 * (lImageWidth - 1 - j) + 2) = tem3;
				}
			}
			return TRUE;
		}
		if (eShowMode == FlipVertical) //垂直翻转
		{
			BYTE  tem1, tem2, tem3;
			for (int i = 0; i < lImageHeight / 2; i++)
			{
				for (int j = 0; j < lImageWidth; j++)
				{
					tem1 = *(pImageBuff + i*lImageWidth * 3 + 3 * j);
					tem2 = *(pImageBuff + i*lImageWidth * 3 + 3 * j + 1);
					tem3 = *(pImageBuff + i*lImageWidth * 3 + 3 * j + 2);
					*(pImageBuff + i*lImageWidth * 3 + 3 * j) = *(pImageBuff + (lImageHeight - 1 - i)*lImageWidth * 3 + 3 * j);
					*(pImageBuff + i*lImageWidth * 3 + 3 * j + 1) = *(pImageBuff + (lImageHeight - 1 - i)*lImageWidth * 3 + 3 * j + 1);
					*(pImageBuff + i*lImageWidth * 3 + 3 * j + 2) = *(pImageBuff + (lImageHeight - 1 - i)*lImageWidth * 3 + 3 * j + 2);
					*(pImageBuff + (lImageHeight - 1 - i)*lImageWidth * 3 + 3 * j) = tem1;
					*(pImageBuff + (lImageHeight - 1 - i)*lImageWidth * 3 + 3 * j + 1) = tem2;
					*(pImageBuff + (lImageHeight - 1 - i)*lImageWidth * 3 + 3 * j + 2) = tem3;
				}
			}
			return TRUE;
		}
	}
	return TRUE;
}
