////////////////////////////////////////////////////////////////////////////////
// Module for converting videodata.
// c. kuehnel, intek, 12.5.2001
////////////////////////////////////////////////////////////////////////////////

#include <convert.h>
#include <string.h>

UINT32 VTable[256];
UINT32 UTable[256];

UINT8  TablesInitialized=FALSE;

////////////////////////////////////////////////////////////////////////////////
// Init lookup-tables.
// We use the following formulas:
// R = Y + 1.4022V
// G = Y - 0.3457U - 0.7144V
// B = Y + 1.771U
////////////////////////////////////////////////////////////////////////////////

void InitTables()
{
  SINT16  Vr,Vg,Ug,Ub;
  UINT32  i;
  SINT16  U,V;

  if(TablesInitialized)
   return;

  // Initialize Assemblertables
  for(i=0;i<256;i++)
  {
    U=(SINT16)i-(SINT16)128;
    V=(SINT16)i-(SINT16)128;

    Vr=(SINT16)(1.4022*V);
    Vg=(SINT16)(-0.7144*V);
    Ug=(SINT16)(-0.3457*U);
    Ub=(SINT16)(1.7710*U);

    UTable[i]=(((UINT32)(UINT16)Ub)<<16)|(UINT32)(UINT16)Ug;
    VTable[i]=(((UINT32)(UINT16)Vr)<<16)|(UINT32)(UINT16)Vg;
  }

  TablesInitialized=TRUE;
}

////////////////////////////////////////////////////////////////////////////////
// Convert RGB to BGR in mode RGB.
// Is: R G B R G B ...
// Windows BITMAP-Format is B G R B G R ...
////////////////////////////////////////////////////////////////////////////////

void ConvertRGB(UINT32 XSize,UINT32 YSize,UINT8 *pBuf,UINT8 *pBGR)
{
  UINT32 Cnt;

  Cnt=XSize*YSize;
  while(Cnt--)
  {
    pBGR[2]=pBuf[0];
    pBGR[1]=pBuf[1];
    pBGR[0]=pBuf[2];

    pBuf+=3;
    pBGR+=3;
  }
}

////////////////////////////////////////////////////////////////////////////////
// Convert RGB16 to BGR in mode RGB.
// Is: R16 G16 B16 R16 G16 B16 ...
// Windows BITMAP-Format is B G R B G R ...
////////////////////////////////////////////////////////////////////////////////

void ConvertRGB16(UINT32 XSize,UINT32 YSize,UINT8 *pBuf,UINT8 *pBGR,UINT8 Swap)
{
  UINT32 Cnt;

  Cnt=XSize*YSize;
  if(!Swap)
  {
    while(Cnt--)
    {
      pBGR[2]=pBuf[0];
      pBGR[1]=pBuf[2];
      pBGR[0]=pBuf[4];

      pBuf+=6;
      pBGR+=3;
    }
  }
  else
  {
    while(Cnt--)
    {
      pBGR[2]=pBuf[1];
      pBGR[1]=pBuf[3];
      pBGR[0]=pBuf[5];

      pBuf+=6;
      pBGR+=3;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// Convert YUV to RGB in mode 4:4:4.
// Is: U Y V U Y V U Y V ...
// Windows BITMAP-Format is B G R B G R ...
////////////////////////////////////////////////////////////////////////////////

void Convert444(UINT32 XSize,UINT32 YSize,UINT8 *pBuf,UINT8 *pBGR)
{
  UINT32  Cnt;

  Cnt=(XSize*YSize);

#ifdef __SC__
  asm
#else
  __asm
#endif
  {
        mov     edx,Cnt

        mov     esi,pBuf
        mov     edi,pBGR

lbl0:   movzx   eax,byte ptr [esi]              // Get U
        mov     eax,DWORD ptr UTable[eax*4]     // Load Ub and Ug

        movzx   ebx,byte ptr 2[esi]             // Get V
        mov     ebx,DWORD ptr VTable[ebx*4]     // Load Vr and Vg

        add     ax,bx                           // Build -0.3457U -0.7144V in ax

        movzx   cx,byte ptr 1[esi]              // Get Y0

        add     cx,ax                           // Build G0
        or      ch,ch
        je      lbl1
        mov     cl,ch
        neg     cl
lbl1:   mov     1[edi],cl                       // Store G0

        shr     eax,16                          // Put Ub to AX
        shr     ebx,16                          // Put Vr to bx

        movzx   cx,byte ptr 1[esi]              // Get Y0
        add     cx,ax
        or      ch,ch
        je      lbl3
        mov     cl,ch
        neg     cl
lbl3:   mov     [edi],cl                        // Store B0

        movzx   cx,byte ptr 1[esi]              // Get Y0
        add     cx,bx
        or      ch,ch
        je      lbl5
        mov     cl,ch
        neg     cl
lbl5:   mov     2[edi],cl                       // Store R0

        add     esi,3
        add     edi,3

        dec     edx
        jnz     lbl0
  }
}

////////////////////////////////////////////////////////////////////////////////
// Convert YUV to RGB in mode 4:2:2.
// Is: U Y V Y U Y V Y ...
// R* = Y            + 1.4022 V
// G* = Y - 0.3457 U - 0.7144 V
// B* = Y + 1.7710 U
////////////////////////////////////////////////////////////////////////////////

void Convert422(UINT32 XSize,UINT32 YSize,UINT8 *pBuf,UINT8 *pBGR)
{
  UINT32  Cnt;

  Cnt=(XSize*YSize)/2;

#ifdef __SC__
  asm
#else
  __asm
#endif
  {
        mov     edx,Cnt

        mov     esi,pBuf
        mov     edi,pBGR

lbl0:   movzx   eax,byte ptr [esi]              // Get U
        mov     eax,DWORD ptr UTable[eax*4]     // Load Ub and Ug

        movzx   ebx,byte ptr 2[esi]             // Get V
        mov     ebx,DWORD ptr VTable[ebx*4]     // Load Vr and Vg

        add     ax,bx                           // Build -0.3457U -0.7144V in ax

        mov     bl,byte ptr 1[esi]              // Load Y0
        mov     bh,byte ptr 3[esi]              // Load Y1

        movzx   cx,bl                           // Get Y0

        add     cx,ax                           // Build G0
        or      ch,ch
        je      lbl1
        mov     cl,ch
        neg     cl
lbl1:   mov     1[edi],cl                       // Store G0

        movzx   cx,bh                           // Get Y1
        add     cx,ax                           // Build G1
        or      ch,ch
        je      lbl2
        mov     cl,ch
        neg     cl
lbl2:   mov     4[edi],cl                       // Store G1

        shr     eax,16                          // Put Ub to AX

        movzx   cx,bl                           // Build B0
        add     cx,ax
        or      ch,ch
        je      lbl3
        mov     cl,ch
        neg     cl
lbl3:   mov     [edi],cl                        // Store B0

        movzx   cx,bh                           // Build B1
        add     cx,ax
        or      ch,ch
        je      lbl4
        mov     cl,ch
        neg     cl
lbl4:   mov     3[edi],cl                       // Store B1

        mov     eax,ebx                         // Put Vr to ax
        shr     eax,16

        movzx   cx,bl                           // Build R0
        add     cx,ax
        or      ch,ch
        je      lbl5
        mov     cl,ch
        neg     cl
lbl5:   mov     2[edi],cl                       // Store R0

        movzx   cx,bh                           // Build R1
        add     cx,ax
        or      ch,ch
        je      lbl6
        mov     cl,ch
        neg     cl
lbl6:   mov     5[edi],cl                       // Store R1

        add     esi,4
        add     edi,6

        dec     edx
        jnz     lbl0
  }
}

////////////////////////////////////////////////////////////////////////////////
// Convert YUV to RGB in mode 4:1:1.
// Is: U Y Y V Y Y U Y Y V Y Y ...
////////////////////////////////////////////////////////////////////////////////

void Convert411(UINT32 XSize,UINT32 YSize,UINT8 *pBuf,UINT8 *pBGR)
{
  UINT32  Cnt;

  Cnt=(XSize*YSize)/4;

#ifdef __SC__
  asm
#else
  __asm
#endif
  {
        mov     edx,Cnt

        mov     esi,pBuf
        mov     edi,pBGR

lbl0:   movzx   eax,byte ptr [esi]              // Get U
        mov     eax,DWORD ptr UTable[eax*4]     // Load Ub and Ug

        movzx   ebx,byte ptr 3[esi]             // Get V
        mov     ebx,DWORD ptr VTable[ebx*4]     // Load Vr and Vg

        add     ax,bx                           // Build -0.3457U -0.7144V in ax

        movzx   cx,byte ptr 1[esi]              // Get Y0
        add     cx,ax                           // Build G0
        or      ch,ch
        je      lbl1
        mov     cl,ch
        neg     cl
lbl1:   mov     1[edi],cl                       // Store G0

        movzx   cx,byte ptr 2[esi]              // Get Y1
        add     cx,ax                           // Build G1
        or      ch,ch
        je      lbl2
        mov     cl,ch
        neg     cl
lbl2:   mov     4[edi],cl                       // Store G1

        movzx   cx,byte ptr 4[esi]              // Get Y2
        add     cx,ax                           // Build G2
        or      ch,ch
        je      lbl3
        mov     cl,ch
        neg     cl
lbl3:   mov     7[edi],cl                       // Store G2

        movzx   cx,byte ptr 5[esi]              // Get Y3
        add     cx,ax                           // Build G3
        or      ch,ch
        je      lbl4
        mov     cl,ch
        neg     cl
lbl4:   mov     10[edi],cl                      // Store G3

        shr     eax,16                          // Put Ub to AX
        shr     ebx,16                          // Put Vr to bx

        movzx   cx,byte ptr 1[esi]              // Get Y0
        add     cx,ax
        or      ch,ch
        je      lbl5
        mov     cl,ch
        neg     cl
lbl5:   mov     [edi],cl                        // Store B0

        movzx   cx,byte ptr 2[esi]              // Get Y1
        add     cx,ax
        or      ch,ch
        je      lbl6
        mov     cl,ch
        neg     cl
lbl6:   mov     3[edi],cl                       // Store B1

        movzx   cx,byte ptr 4[esi]              // Get Y2
        add     cx,ax
        or      ch,ch
        je      lbl7
        mov     cl,ch
        neg     cl
lbl7:   mov     6[edi],cl                       // Store B2

        movzx   cx,byte ptr 5[esi]              // Get Y3
        add     cx,ax
        or      ch,ch
        je      lbl8
        mov     cl,ch
        neg     cl
lbl8:   mov     9[edi],cl                       // Store B3

        movzx   cx,byte ptr 1[esi]              // Get Y0
        add     cx,bx
        or      ch,ch
        je      lbl9
        mov     cl,ch
        neg     cl
lbl9:   mov     2[edi],cl                       // Store R0

        movzx   cx,byte ptr 2[esi]              // Get Y1
        add     cx,bx
        or      ch,ch
        je      lbla
        mov     cl,ch
        neg     cl
lbla:   mov     5[edi],cl                       // Store R1

        movzx   cx,byte ptr 4[esi]              // Get Y2
        add     cx,bx
        or      ch,ch
        je      lblb
        mov     cl,ch
        neg     cl
lblb:   mov     8[edi],cl                       // Store R2

        movzx   cx,byte ptr 5[esi]              // Get Y3
        add     cx,bx
        or      ch,ch
        je      lblc
        mov     cl,ch
        neg     cl
lblc:   mov     11[edi],cl                      // Store R3

        add     esi,6
        add     edi,12

        dec     edx
        jnz     lbl0
  }
}

////////////////////////////////////////////////////////////////////////////////
// Convert Y16 to RGB.
// Data area is Low0 High0 Low1 High1 ...
////////////////////////////////////////////////////////////////////////////////

void ConvertY16(UINT32 XSize,UINT32 YSize,UINT8 *pBuf,UINT8 *pBGR)
{
  UINT32 Cnt;

  Cnt=XSize*YSize;

#ifdef __SC__
  asm
#else
  __asm
#endif
  {
        mov     ecx,Cnt

        mov     esi,pBuf
        mov     edi,pBGR

lbl0:   mov     ah,0[esi]               ; Big endian
        mov     al,1[esi]

        add     esi,2

        cmp     ah,0xFF
        jnz     lbl1
        xor     al,al

lbl1:   mov     bl,ah                   ; copy High Byte to bl

        test    al,0x40                 ; Bit6 increases Blue
        jz      lbl2
        add     ah,1

lbl2:   mov     byte ptr [edi],ah
        mov     byte ptr 1[edi],bl

        test    al,0x80                 ; Bit7 increases Red
        jz      lbl3
        inc     bl

lbl3:   mov     byte ptr 2[edi],bl

        add     edi,3

        dec     ecx
        jnz     lbl0
  }
}

////////////////////////////////////////////////////////////////////////////////
// Convert Y8 to RGB buffer.
////////////////////////////////////////////////////////////////////////////////

void ConvertY8(UINT32 XSize,UINT32 YSize,UINT8 *pBuf,UINT8 *pBGR)
{
  const UINT32 cnt=XSize*YSize;
  UINT32 i;
  
  for(i=0;i<cnt;++i)
  {
    *pBGR = *pBuf; ++pBGR;
    *pBGR = *pBuf; ++pBGR;
    *pBGR = *pBuf; ++pBGR;
    ++pBuf;
  }
}

////////////////////////////////////////////////////////////////////////////////
// Convert Y16 to RGB byte reversed order.
// Data area is Low0 High0 Low1 High1 ...
////////////////////////////////////////////////////////////////////////////////

void ConvertY16a(UINT32 XSize,UINT32 YSize,UINT8 *pBuf,UINT8 *pBGR)
{
  UINT32 Cnt;

  Cnt=XSize*YSize;

#ifdef __SC__
  asm
#else
  __asm
#endif
  {
        mov     ecx,Cnt

        mov     esi,pBuf
        mov     edi,pBGR

lbl0:   mov     ax,word ptr [esi]      ; Little Endian

        add     esi,2

        cmp     ah,0xFF
        jnz     lbl1
        xor     al,al

lbl1:   mov     bl,ah                   ; copy High Byte to bl

        test    al,0x40                 ; Bit6 increases Blue
        jz      lbl2
        add     ah,1

lbl2:   mov     byte ptr [edi],ah
        mov     byte ptr 1[edi],bl

        test    al,0x80                 ; Bit7 increases Red
        jz      lbl3
        inc     bl

lbl3:   mov     byte ptr 2[edi],bl

        add     edi,3

        dec     ecx
        jnz     lbl0
  }
}

////////////////////////////////////////////////////////////////////////////////
// Convert raw mode with 8 bit values pattern RG GB.
////////////////////////////////////////////////////////////////////////////////

void ConvertRawY8RGGB(UINT32 XSize,UINT32 YSize,UINT8 *pBuf,UINT8 *pBGR)
{
  UINT8 *pR,*pB,*pG0,*pG1;
  UINT32 i,j;
  struct pdst
  {
    UINT8 B;
    UINT8 G;
    UINT8 R;
  }*pDst;

  pDst=(struct pdst*)pBGR;

  for(i=0;i<YSize-1;i++)
  {
    if(i&1)
    {
      pG1=pBuf+i*XSize;
      pB=pG1+1;
      pR=pG1+XSize;
      pG0=pR+1;
    }
    else
    {
      pR=pBuf+i*XSize;
      pG0=pR+1;
      pG1=pR+XSize;
      pB=pG1+1;
    }

    // Go through all pixels
    for(j=0;j<XSize-1;j++)
    {
      pDst->B=*pB;
      pDst->G=(UINT8)((*pG0+*pG1)/2);
      pDst->R=*pR;

      pDst++;

      if(j&1)
      {
        pB+=2;
        pG0+=2;
      }
      else
      {
        pR+=2;
        pG1+=2;
      }
    }
    pDst->B=0;
    pDst->G=0;
    pDst->R=0;

    pDst++;
  }

  memset(pBGR+(XSize*(YSize-1))*3,0,XSize*3);
}

////////////////////////////////////////////////////////////////////////////////
// Convert raw mode with 8 bit values pattern GR BG.
////////////////////////////////////////////////////////////////////////////////

void ConvertRawY8GRBG(UINT32 XSize,UINT32 YSize,UINT8 *pBuf,UINT8 *pBGR)
{
  UINT8 *pR,*pB,*pG0,*pG1;
  UINT32 i,j;
  struct pdst
  {
    UINT8 B;
    UINT8 G;
    UINT8 R;
  }*pDst;

  pDst=(struct pdst*)pBGR;

  for(i=0;i<YSize-1;i++)
  {
    if(i&1)
    {
      pB=pBuf+i*XSize;
      pG1=pB+1;
      pG0=pB+XSize;
      pR=pG0+1;
    }
    else
    {
      pG0=pBuf+i*XSize;
      pR=pG0+1;
      pB=pG0+XSize;
      pG1=pB+1;
    }

    // Go through all pixels
    for(j=0;j<XSize-1;j++)
    {
      pDst->B=*pB;
      pDst->G=(UINT8)((*pG0+*pG1)/2);
      pDst->R=*pR;

      pDst++;

      if(j&1)
      {
        pR+=2;
        pG1+=2;
      }
      else
      {
        pG0+=2;
        pB+=2;
      }
    }

    pDst->B=0;
    pDst->G=0;
    pDst->R=0;

    pDst++;
  }

  memset(pBGR+(XSize*(YSize-1))*3,0,XSize*3);
}

////////////////////////////////////////////////////////////////////////////////
// Convert raw mode with 8 bit values pattern BG GR.
////////////////////////////////////////////////////////////////////////////////

void ConvertRawY8BGGR(UINT32 XSize,UINT32 YSize,UINT8 *pBuf,UINT8 *pBGR)
{
  UINT8 *pR,*pB,*pG0,*pG1;
  UINT32 i,j;
  struct pdst
  {
    UINT8 B;
    UINT8 G;
    UINT8 R;
  }*pDst;

  pDst=(struct pdst*)pBGR;

  for(i=0;i<YSize-1;i++)
  {
    if(i&1)
    {
      pG1=pBuf+i*XSize;
      pR=pG1+1;
      pB=pG1+XSize;
      pG0=pB+1;
    }
    else
    {
      pB=pBuf+i*XSize;
      pG0=pB+1;
      pG1=pB+XSize;
      pR=pG1+1;
    }

    // Go through all pixels
    for(j=0;j<XSize-1;j++)
    {
      pDst->B=*pB;
      pDst->G=(UINT8)((*pG0+*pG1)/2);
      pDst->R=*pR;

      pDst++;

      if(j&1)
      {
        pG0+=2;
        pR+=2;
      }
      else
      {
        pB+=2;
        pG1+=2;
      }
    }
    pDst->B=0;
    pDst->G=0;
    pDst->R=0;

    pDst++;
  }

  memset(pBGR+(XSize*(YSize-1))*3,0,XSize*3);
}

////////////////////////////////////////////////////////////////////////////////
// Convert raw mode with 8 bit values pattern GB RG.
////////////////////////////////////////////////////////////////////////////////

void ConvertRawY8GBRG(UINT32 XSize,UINT32 YSize,UINT8 *pBuf,UINT8 *pBGR)
{
  UINT8 *pR,*pB,*pG0,*pG1;
  UINT32 i,j;
  struct pdst
  {
    UINT8 B;
    UINT8 G;
    UINT8 R;
  }*pDst;

  pDst=(struct pdst*)pBGR;

  for(i=0;i<YSize-1;i++)
  {
    if(i&1)
    {
      pR=pBuf+i*XSize;
      pG1=pR+1;
      pG0=pR+XSize;
      pB=pG0+1;
    }
    else
    {
      pG0=pBuf+i*XSize;
      pB=pG0+1;
      pR=pG0+XSize;
      pG1=pR+1;
    }

    // Go through all pixels
    for(j=0;j<XSize-1;j++)
    {
      pDst->B=*pB;
      pDst->G=(UINT8)((*pG0+*pG1)/2);
      pDst->R=*pR;

      pDst++;

      if(j&1)
      {
        pB+=2;
        pG1+=2;
      }
      else
      {
        pG0+=2;
        pR+=2;
      }
    }

    pDst->B=0;
    pDst->G=0;
    pDst->R=0;

    pDst++;
  }

  memset(pBGR+(XSize*(YSize-1))*3,0,XSize*3);
}

////////////////////////////////////////////////////////////////////////////////
// Convert raw mode with 8 bit values.
////////////////////////////////////////////////////////////////////////////////

void ConvertRawY8(UINT32 XSize,UINT32 YSize,UINT8 *pBuf,UINT8 *pBGR,
                  UINT8 BayerPattern)
{
  switch(BayerPattern)
  {
    case 0: ConvertRawY8RGGB(XSize,YSize,pBuf,pBGR);
            break;

    case 1: ConvertRawY8GRBG(XSize,YSize,pBuf,pBGR);
            break;

    case 2: ConvertRawY8BGGR(XSize,YSize,pBuf,pBGR);
            break;

    case 3: ConvertRawY8GBRG(XSize,YSize,pBuf,pBGR);
            break;
  }
}

////////////////////////////////////////////////////////////////////////////////
// Convert raw mode with 16 bit values pattern RG GB.
////////////////////////////////////////////////////////////////////////////////

void ConvertRawY16RGGB(UINT32 XSize,UINT32 YSize,UINT8 *pBuf,UINT8 *pBGR)
{
  UINT16 *pR,*pB,*pG0,*pG1;
  UINT32 i,j;
  UINT16 *pMem;
  struct pdst
  {
    UINT8 B;
    UINT8 G;
    UINT8 R;
  }*pDst;

  pDst=(struct pdst*)pBGR;
  pMem=(UINT16*)pBuf;

  for(i=0;i<YSize-1;i++)
  {
    if(i&1)
    {
      pG1=pMem+i*XSize;
      pB=pG1+1;
      pR=pG1+XSize;
      pG0=pR+1;
    }
    else
    {
      pR=pMem+i*XSize;
      pG0=pR+1;
      pG1=pR+XSize;
      pB=pG1+1;
    }

    // Go through all pixels
    for(j=0;j<XSize-1;j++)
    {
      // We only use one byte
      pDst->B=(UINT8)(*(UINT8*)pB);
      pDst->G=(UINT8)(((*(UINT8*)pG0+*(UINT8*)pG1)/2));
      pDst->R=(UINT8)(*(UINT8*)pR);

      pDst++;

      if(j&1)
      {
        pB+=2;
        pG0+=2;
      }
      else
      {
        pR+=2;
        pG1+=2;
      }
    }
    pDst->B=0;
    pDst->G=0;
    pDst->R=0;

    pDst++;
  }

  memset(pBGR+(XSize*(YSize-1))*3,0,XSize*3);
}

////////////////////////////////////////////////////////////////////////////////
// Convert raw mode with 16 bit values pattern GR BG.
////////////////////////////////////////////////////////////////////////////////

void ConvertRawY16GRBG(UINT32 XSize,UINT32 YSize,UINT8 *pBuf,UINT8 *pBGR)
{
  UINT16 *pR,*pB,*pG0,*pG1;
  UINT32 i,j;
  UINT16 *pMem;
  struct pdst
  {
    UINT8 B;
    UINT8 G;
    UINT8 R;
  }*pDst;

  pDst=(struct pdst*)pBGR;
  pMem=(UINT16*)pBuf;

  for(i=0;i<YSize-1;i++)
  {
    if(i&1)
    {
      pB=pMem+i*XSize;
      pG1=pB+1;
      pG0=pB+XSize;
      pR=pG0+1;
    }
    else
    {
      pG0=pMem+i*XSize;
      pR=pG0+1;
      pB=pG0+XSize;
      pG1=pB+1;
    }

    // Go through all pixels
    for(j=0;j<XSize-1;j++)
    {
      // We only use one MSB (it the lower one)
      pDst->B=(UINT8)(*(UINT8*)pB);
      pDst->G=(UINT8)(((*(UINT8*)pG0+*(UINT8*)pG1)/2));
      pDst->R=(UINT8)(*(UINT8*)pR);

      pDst++;

      if(j&1)
      {
        pR+=2;
        pG1+=2;
      }
      else
      {
        pG0+=2;
        pB+=2;
      }
    }

    pDst->B=0;
    pDst->G=0;
    pDst->R=0;

    pDst++;
  }

  memset(pBGR+(XSize*(YSize-1))*3,0,XSize*3);
}

////////////////////////////////////////////////////////////////////////////////
// Convert raw mode with 16 bit values pattern BG GR.
////////////////////////////////////////////////////////////////////////////////

void ConvertRawY16BGGR(UINT32 XSize,UINT32 YSize,UINT8 *pBuf,UINT8 *pBGR)
{
  UINT16 *pR,*pB,*pG0,*pG1;
  UINT32 i,j;
  UINT16 *pMem;
  struct pdst
  {
    UINT8 B;
    UINT8 G;
    UINT8 R;
  }*pDst;

  pDst=(struct pdst*)pBGR;
  pMem=(UINT16*)pBuf;

  for(i=0;i<YSize-1;i++)
  {
    if(i&1)
    {
      pG1=pMem+i*XSize;
      pR=pG1+1;
      pB=pG1+XSize;
      pG0=pB+1;
    }
    else
    {
      pB=pMem+i*XSize;
      pG0=pB+1;
      pG1=pB+XSize;
      pR=pG1+1;
    }

    // Go through all pixels
    for(j=0;j<XSize-1;j++)
    {
      // We only use one MSB (it the lower one)
      pDst->B=(UINT8)(*(UINT8*)pB);
      pDst->G=(UINT8)(((*(UINT8*)pG0+*(UINT8*)pG1)/2));
      pDst->R=(UINT8)(*(UINT8*)pR);

      pDst++;

      if(j&1)
      {
        pG0+=2;
        pR+=2;
      }
      else
      {
        pB+=2;
        pG1+=2;
      }
    }
    pDst->B=0;
    pDst->G=0;
    pDst->R=0;

    pDst++;
  }

  memset(pBGR+(XSize*(YSize-1))*3,0,XSize*3);
}

////////////////////////////////////////////////////////////////////////////////
// Convert raw mode with 16 bit values pattern GB RG.
////////////////////////////////////////////////////////////////////////////////

void ConvertRawY16GBRG(UINT32 XSize,UINT32 YSize,UINT8 *pBuf,UINT8 *pBGR)
{
  UINT16 *pR,*pB,*pG0,*pG1;
  UINT32 i,j;
  UINT16 *pMem;
  struct pdst
  {
    UINT8 B;
    UINT8 G;
    UINT8 R;
  }*pDst;

  pDst=(struct pdst*)pBGR;
  pMem=(UINT16*)pBuf;

  for(i=0;i<YSize-1;i++)
  {
    if(i&1)
    {
      pR=pMem+i*XSize;
      pG1=pR+1;
      pG0=pR+XSize;
      pB=pG0+1;
    }
    else
    {
      pG0=pMem+i*XSize;
      pB=pG0+1;
      pR=pG0+XSize;
      pG1=pR+1;
    }

    // Go through all pixels
    for(j=0;j<XSize-1;j++)
    {
      // We only use one MSB (it the lower one)
      pDst->B=(UINT8)(*(UINT8*)pB);
      pDst->G=(UINT8)(((*(UINT8*)pG0+*(UINT8*)pG1)/2));
      pDst->R=(UINT8)(*(UINT8*)pR);

      pDst++;

      if(j&1)
      {
        pB+=2;
        pG1+=2;
      }
      else
      {
        pG0+=2;
        pR+=2;
      }
    }

    pDst->B=0;
    pDst->G=0;
    pDst->R=0;

    pDst++;
  }

  memset(pBGR+(XSize*(YSize-1))*3,0,XSize*3);
}

////////////////////////////////////////////////////////////////////////////////
// Convert raw mode with 16 bit values.
////////////////////////////////////////////////////////////////////////////////

void ConvertRawY16(UINT32 XSize,UINT32 YSize,UINT8 *pBuf,UINT8 *pBGR,
                   UINT8 BayerPattern)
{
  switch(BayerPattern)
  {
    case 0: ConvertRawY16RGGB(XSize,YSize,pBuf,pBGR);
            break;

    case 1: ConvertRawY16GRBG(XSize,YSize,pBuf,pBGR);
            break;

    case 2: ConvertRawY16BGGR(XSize,YSize,pBuf,pBGR);
            break;

    case 3: ConvertRawY16GBRG(XSize,YSize,pBuf,pBGR);
            break;
  }
}

////////////////////////////////////////////////////////////////////////////////
// Convert raw mode with 16 bit invers values pattern RG GB.
////////////////////////////////////////////////////////////////////////////////

void ConvertRawY16aRGGB(UINT32 XSize,UINT32 YSize,UINT8 *pBuf,UINT8 *pBGR)
{
  UINT16 *pR,*pB,*pG0,*pG1;
  UINT32 i,j;
  UINT16 *pMem;
  struct pdst
  {
    UINT8 B;
    UINT8 G;
    UINT8 R;
  }*pDst;

  pDst=(struct pdst*)pBGR;
  pMem=(UINT16*)pBuf;

  for(i=0;i<YSize-1;i++)
  {
    if(i&1)
    {
      pG1=pMem+i*XSize;
      pB=pG1+1;
      pR=pG1+XSize;
      pG0=pR+1;
    }
    else
    {
      pR=pMem+i*XSize;
      pG0=pR+1;
      pG1=pR+XSize;
      pB=pG1+1;
    }

    // Go through all pixels
    for(j=0;j<XSize-1;j++)
    {
      // We only use one byte
      pDst->B=(UINT8)(*((UINT8*)pB+1));
      pDst->G=(UINT8)(((*((UINT8*)pG0+1)+*((UINT8*)pG1+1))/2));
      pDst->R=(UINT8)(*((UINT8*)pR+1));

      pDst++;

      if(j&1)
      {
        pB+=2;
        pG0+=2;
      }
      else
      {
        pR+=2;
        pG1+=2;
      }
    }
    pDst->B=0;
    pDst->G=0;
    pDst->R=0;

    pDst++;
  }

  memset(pBGR+(XSize*(YSize-1))*3,0,XSize*3);
}

////////////////////////////////////////////////////////////////////////////////
// Convert raw mode with 16 bit invers values pattern GR BG.
////////////////////////////////////////////////////////////////////////////////

void ConvertRawY16aGRBG(UINT32 XSize,UINT32 YSize,UINT8 *pBuf,UINT8 *pBGR)
{
  UINT16 *pR,*pB,*pG0,*pG1;
  UINT32 i,j;
  UINT16 *pMem;
  struct pdst
  {
    UINT8 B;
    UINT8 G;
    UINT8 R;
  }*pDst;

  pDst=(struct pdst*)pBGR;
  pMem=(UINT16*)pBuf;

  for(i=0;i<YSize-1;i++)
  {
    if(i&1)
    {
      pB=pMem+i*XSize;
      pG1=pB+1;
      pG0=pB+XSize;
      pR=pG0+1;
    }
    else
    {
      pG0=pMem+i*XSize;
      pR=pG0+1;
      pB=pG0+XSize;
      pG1=pB+1;
    }

    // Go through all pixels
    for(j=0;j<XSize-1;j++)
    {
      // We only use one byte
      pDst->B=(UINT8)(*((UINT8*)pB+1));
      pDst->G=(UINT8)(((*((UINT8*)pG0+1)+*((UINT8*)pG1+1))/2));
      pDst->R=(UINT8)(*((UINT8*)pR+1));

      pDst++;

      if(j&1)
      {
        pR+=2;
        pG1+=2;
      }
      else
      {
        pG0+=2;
        pB+=2;
      }
    }

    pDst->B=0;
    pDst->G=0;
    pDst->R=0;

    pDst++;
  }

  memset(pBGR+(XSize*(YSize-1))*3,0,XSize*3);
}

////////////////////////////////////////////////////////////////////////////////
// Convert raw mode with 16 bit invers values pattern BG GR.
////////////////////////////////////////////////////////////////////////////////

void ConvertRawY16aBGGR(UINT32 XSize,UINT32 YSize,UINT8 *pBuf,UINT8 *pBGR)
{
  UINT16 *pR,*pB,*pG0,*pG1;
  UINT32 i,j;
  UINT16 *pMem;
  struct pdst
  {
    UINT8 B;
    UINT8 G;
    UINT8 R;
  }*pDst;

  pDst=(struct pdst*)pBGR;
  pMem=(UINT16*)pBuf;

  for(i=0;i<YSize-1;i++)
  {
    if(i&1)
    {
      pG1=pMem+i*XSize;
      pR=pG1+1;
      pB=pG1+XSize;
      pG0=pB+1;
    }
    else
    {
      pB=pMem+i*XSize;
      pG0=pB+1;
      pG1=pB+XSize;
      pR=pG1+1;
    }

    // Go through all pixels
    for(j=0;j<XSize-1;j++)
    {
      // We only use one byte
      pDst->B=(UINT8)(*((UINT8*)pB+1));
      pDst->G=(UINT8)(((*((UINT8*)pG0+1)+*((UINT8*)pG1+1))/2));
      pDst->R=(UINT8)(*((UINT8*)pR+1));

      pDst++;

      if(j&1)
      {
        pG0+=2;
        pR+=2;
      }
      else
      {
        pB+=2;
        pG1+=2;
      }
    }
    pDst->B=0;
    pDst->G=0;
    pDst->R=0;

    pDst++;
  }

  memset(pBGR+(XSize*(YSize-1))*3,0,XSize*3);
}

////////////////////////////////////////////////////////////////////////////////
// Convert raw mode with 16 bit invers values pattern GB RG.
////////////////////////////////////////////////////////////////////////////////

void ConvertRawY16aGBRG(UINT32 XSize,UINT32 YSize,UINT8 *pBuf,UINT8 *pBGR)
{
  UINT16 *pR,*pB,*pG0,*pG1;
  UINT32 i,j;
  UINT16 *pMem;
  struct pdst
  {
    UINT8 B;
    UINT8 G;
    UINT8 R;
  }*pDst;

  pDst=(struct pdst*)pBGR;
  pMem=(UINT16*)pBuf;

  for(i=0;i<YSize-1;i++)
  {
    if(i&1)
    {
      pR=pMem+i*XSize;
      pG1=pR+1;
      pG0=pR+XSize;
      pB=pG0+1;
    }
    else
    {
      pG0=pMem+i*XSize;
      pB=pG0+1;
      pR=pG0+XSize;
      pG1=pR+1;
    }

    // Go through all pixels
    for(j=0;j<XSize-1;j++)
    {
      // We only use one byte
      pDst->B=(UINT8)(*((UINT8*)pB+1));
      pDst->G=(UINT8)(((*((UINT8*)pG0+1)+*((UINT8*)pG1+1))/2));
      pDst->R=(UINT8)(*((UINT8*)pR+1));

      pDst++;

      if(j&1)
      {
        pB+=2;
        pG1+=2;
      }
      else
      {
        pG0+=2;
        pR+=2;
      }
    }

    pDst->B=0;
    pDst->G=0;
    pDst->R=0;

    pDst++;
  }

  memset(pBGR+(XSize*(YSize-1))*3,0,XSize*3);
}

////////////////////////////////////////////////////////////////////////////////
// Convert raw mode with 16 bit inverted values.
////////////////////////////////////////////////////////////////////////////////

void ConvertRawY16a(UINT32 XSize,UINT32 YSize,UINT8 *pBuf,UINT8 *pBGR,
                    UINT8 BayerPattern)
{
  switch(BayerPattern)
  {
    case 0: ConvertRawY16aRGGB(XSize,YSize,pBuf,pBGR);
            break;

    case 1: ConvertRawY16aGRBG(XSize,YSize,pBuf,pBGR);
            break;

    case 2: ConvertRawY16aBGGR(XSize,YSize,pBuf,pBGR);
            break;

    case 3: ConvertRawY16aGBRG(XSize,YSize,pBuf,pBGR);
            break;
  }
}

///////////////////////////////////////////////////////////////////////////////
// Plane-Convert YUV to RGB in mode 4:4:4.
// Is: U Y V U Y V U Y V ...
////////////////////////////////////////////////////////////////////////////////

void XPlane444(UINT32 XSize,UINT32 YSize,UINT8 *pBuf,UINT8 *pR,UINT8 *pG,UINT8 *pB)
{
  UINT32  Cnt;

  Cnt=(XSize*YSize);

#ifdef __SC__
  asm
#else
  __asm
#endif
  {
        mov     edx,Cnt
        mov     esi,pBuf

lbl0:
        movzx   eax,byte ptr [esi]              // Get U
        mov     eax,DWORD ptr UTable[eax*4]     // Load Ub and Ug

        movzx   ebx,byte ptr 2[esi]             // Get V
        mov     ebx,DWORD ptr VTable[ebx*4]     // Load Vr and Vg

        add     ax,bx                           // Build -0.3457U -0.7144V in ax

        movzx   cx,byte ptr 1[esi]              // Get Y0

        add     cx,ax                           // Build G0
        or      ch,ch
        je      lbl1
        mov     cl,ch
        neg     cl

lbl1:   mov     edi,pG
        mov     [edi],cl                        // Store G0
        add     edi,1
        mov     pG,edi

        shr     eax,16                          // Put Ub to AX
        shr     ebx,16                          // Put Vr to bx

        movzx   cx,byte ptr 1[esi]              // Get Y0
        add     cx,ax
        or      ch,ch
        je      lbl3
        mov     cl,ch
        neg     cl

lbl3:   mov     edi,pB
        mov     [edi],cl                        // Store B0
        add     edi,1
        mov     pB,edi

        movzx   cx,byte ptr 1[esi]              // Get Y0
        add     cx,bx
        or      ch,ch
        je      lbl5
        mov     cl,ch
        neg     cl

lbl5:   mov     edi,pR                          // Store R0
        mov     [edi],cl
        add     edi,1
        mov     pR,edi

        add     esi,3
        add     edi,3

        dec     edx
        jnz     lbl0
  }
}

////////////////////////////////////////////////////////////////////////////////
// Plane Convert YUV to RGB in mode 4:2:2.
// Is: U Y V Y U Y V Y ...
// R* = Y            + 1.4022 V
// G* = Y - 0.3457 U - 0.7144 V
// B* = Y + 1.7710 U
////////////////////////////////////////////////////////////////////////////////

void XPlane422(UINT32 XSize,UINT32 YSize,UINT8 *pBuf,UINT8 *pR,UINT8 *pG,UINT8 *pB)
{
  UINT32  Cnt;

  Cnt=(XSize*YSize)/2;

#ifdef __SC__
  asm
#else
  __asm
#endif
  {
        mov     edx,Cnt

        mov     esi,pBuf

lbl0:   movzx   eax,byte ptr [esi]              // Get U
        mov     eax,DWORD ptr UTable[eax*4]     // Load Ub and Ug

        movzx   ebx,byte ptr 2[esi]             // Get V
        mov     ebx,DWORD ptr VTable[ebx*4]     // Load Vr and Vg

        add     ax,bx                           // Build -0.3457U -0.7144V in ax

        mov     bl,byte ptr 1[esi]              // Load Y0
        mov     bh,byte ptr 3[esi]              // Load Y1

        movzx   cx,bl                           // Get Y0

        add     cx,ax                           // Build G0
        or      ch,ch
        je      lbl1
        mov     cl,ch
        neg     cl

lbl1:   mov     edi,pG
        mov     [edi],cl                        // Store G0

        movzx   cx,bh                           // Get Y1
        add     cx,ax                           // Build G1
        or      ch,ch
        je      lbl2
        mov     cl,ch
        neg     cl

lbl2:   mov     1[edi],cl                       // Store G1
        add     edi,2
        mov     pG,edi

        shr     eax,16                          // Put Ub to AX

        movzx   cx,bl                           // Build B0
        add     cx,ax
        or      ch,ch
        je      lbl3
        mov     cl,ch
        neg     cl

lbl3:   mov     edi,pB
        mov     [edi],cl                        // Store B0

        movzx   cx,bh                           // Build B1
        add     cx,ax
        or      ch,ch
        je      lbl4
        mov     cl,ch
        neg     cl

lbl4:   mov     1[edi],cl                       // Store B1
        add     edi,2
        mov     pB,edi

        mov     eax,ebx                         // Put Vr to ax
        shr     eax,16

        movzx   cx,bl                           // Build R0
        add     cx,ax
        or      ch,ch
        je      lbl5
        mov     cl,ch
        neg     cl

lbl5:   mov     edi,pR
        mov     [edi],cl                        // Store R0

        movzx   cx,bh                           // Build R1
        add     cx,ax
        or      ch,ch
        je      lbl6
        mov     cl,ch
        neg     cl

lbl6:   mov     1[edi],cl                       // Store R1
        add     edi,2
        mov     pR,edi

        add     esi,4
        add     edi,6

        dec     edx
        jnz     lbl0
  }
}

////////////////////////////////////////////////////////////////////////////////
// Plane Convert YUV to RGB in mode 4:1:1.
// Is: U Y Y V Y Y U Y Y V Y Y ...
////////////////////////////////////////////////////////////////////////////////

void XPlane411(UINT32 XSize,UINT32 YSize,UINT8 *pBuf,UINT8 *pR,UINT8 *pG,UINT8 *pB)
{
  UINT32  Cnt;

  Cnt=(XSize*YSize)/4;

#ifdef __SC__
  asm
#else
  __asm
#endif
  {
        mov     edx,Cnt
        mov     esi,pBuf

lbl0:   movzx   eax,byte ptr [esi]              // Get U
        mov     eax,DWORD ptr UTable[eax*4]     // Load Ub and Ug

        movzx   ebx,byte ptr 3[esi]             // Get V
        mov     ebx,DWORD ptr VTable[ebx*4]     // Load Vr and Vg

        add     ax,bx                           // Build -0.3457U -0.7144V in ax

        movzx   cx,byte ptr 1[esi]              // Get Y0
        add     cx,ax                           // Build G0
        or      ch,ch
        je      lbl1
        mov     cl,ch
        neg     cl

lbl1:   mov     edi,pG
        mov     [edi],cl                        // Store G0

        movzx   cx,byte ptr 2[esi]              // Get Y1
        add     cx,ax                           // Build G1
        or      ch,ch
        je      lbl2
        mov     cl,ch
        neg     cl

lbl2:   mov     1[edi],cl                       // Store G1

        movzx   cx,byte ptr 4[esi]              // Get Y2
        add     cx,ax                           // Build G2
        or      ch,ch
        je      lbl3
        mov     cl,ch
        neg     cl

lbl3:   mov     2[edi],cl                       // Store G2

        movzx   cx,byte ptr 5[esi]              // Get Y3
        add     cx,ax                           // Build G3
        or      ch,ch
        je      lbl4
        mov     cl,ch
        neg     cl

lbl4:   mov     3[edi],cl                      // Store G3
        add     edi,4
        mov     pG,edi

        shr     eax,16                          // Put Ub to AX
        shr     ebx,16                          // Put Vr to bx

        movzx   cx,byte ptr 1[esi]              // Get Y0
        add     cx,ax
        or      ch,ch
        je      lbl5
        mov     cl,ch
        neg     cl

lbl5:   mov     edi,pB
        mov     [edi],cl                        // Store B0

        movzx   cx,byte ptr 2[esi]              // Get Y1
        add     cx,ax
        or      ch,ch
        je      lbl6
        mov     cl,ch
        neg     cl

lbl6:   mov     1[edi],cl                       // Store B1

        movzx   cx,byte ptr 4[esi]              // Get Y2
        add     cx,ax
        or      ch,ch
        je      lbl7
        mov     cl,ch
        neg     cl

lbl7:   mov     2[edi],cl                       // Store B2

        movzx   cx,byte ptr 5[esi]              // Get Y3
        add     cx,ax
        or      ch,ch
        je      lbl8
        mov     cl,ch
        neg     cl

lbl8:   mov     3[edi],cl                       // Store B3
        add     edi,4
        mov     pB,edi

        movzx   cx,byte ptr 1[esi]              // Get Y0
        add     cx,bx
        or      ch,ch
        je      lbl9
        mov     cl,ch
        neg     cl

lbl9:   mov     edi,pR
        mov     [edi],cl                        // Store R0

        movzx   cx,byte ptr 2[esi]              // Get Y1
        add     cx,bx
        or      ch,ch
        je      lbla
        mov     cl,ch
        neg     cl

lbla:   mov     1[edi],cl                       // Store R1

        movzx   cx,byte ptr 4[esi]              // Get Y2
        add     cx,bx
        or      ch,ch
        je      lblb
        mov     cl,ch
        neg     cl

lblb:   mov     2[edi],cl                       // Store R2

        movzx   cx,byte ptr 5[esi]              // Get Y3
        add     cx,bx
        or      ch,ch
        je      lblc
        mov     cl,ch
        neg     cl

lblc:   mov     3[edi],cl                       // Store R3
        add     edi,4
        mov     pR,edi

        add     esi,6
        add     edi,12

        dec     edx
        jnz     lbl0
  }
}

