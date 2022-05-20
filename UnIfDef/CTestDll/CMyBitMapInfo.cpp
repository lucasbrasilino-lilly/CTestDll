#include "stdafx.h"
#include "CMyBitMapInfo.h"
#include "DebugUtilities.h"



CMyBitMapInfo::CMyBitMapInfo(ImageDefinitionStructEx& rImageDef)
{
	static const int MONO_COLOR_TABLE_SIZE = 256;

	FDB(1, "CMyBitMapInfo::CMyBitMapInfo - Start\n");
	pBitmapInfo = CreateBitmapInfoStruct(rImageDef);
	_ASSERT(pBitmapInfo);

	switch (rImageDef.eImageFormat)
	{
	case  SVImageFormatMono8:
	{
		for (int i = 0; i < MONO_COLOR_TABLE_SIZE; i++)
		{
			RGBQUAD rgb = { (BYTE)i,(BYTE)i,(BYTE)i,0 };
			pBitmapInfo->bmiColors[0] = rgb;
		}
		break;
	}
	case SVImageFormatBGR888X:
	{
		break;
	}
	case SVImageFormatUnknown:
	{
		break;
	}
	default:
	{
		break;
	}
	}

	pBits = NULL;
	hbm = ::CreateDIBSection(NULL, pBitmapInfo, DIB_RGB_COLORS, &pBits, NULL, 0);
	FDB(1, "CMyBitMapInfo::CMyBitMapInfo - End\n");
}

CMyBitMapInfo::~CMyBitMapInfo()
{
	FDB(1, "CMyBitMapInfo::~CMyBitMapInfo - Start\n");
	DeleteObject(hbm);
	delete[](pBitmapInfo);
	FDB(1, "CMyBitMapInfo::~CMyBitMapInfo - End\n");
}

HBITMAP CMyBitMapInfo::getBitmapHandle()
{
	return hbm;
}

/// Create a Infostruct based on the referenced Image Definitions
/// 
/// takes the information 
/// \param rImageDef:  
BITMAPINFO* CMyBitMapInfo::CreateBitmapInfoStruct(ImageDefinitionStructEx& rImageDef)
{
	constexpr WORD numberOfBitsForMono8 = 8;
	constexpr WORD numberOfBitsForBGR888X = 32;

	BITMAPINFO* pbmi;
	WORD wClrBits;

	// Allocate memory for the BITMAPINFO structure. (This structure 
	// contains a BITMAPINFOHEADER structure and an array of RGBQUAD 
	// data structures.) 
	if (rImageDef.eImageFormat == SVImageFormatBGR888X)
	{
		wClrBits = numberOfBitsForBGR888X;
		// There is no RGBQUAD array for the RGB8888 format. 
		pbmi = (PBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER)];
	}
	else // numberOfBitsForMono8
	{
		wClrBits = numberOfBitsForMono8;
		pbmi = (PBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * static_cast<DWORD>(1 << wClrBits)];
	}

	// Initialize the fields in the BITMAPINFO structure. 
	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = rImageDef.lWidth;
	pbmi->bmiHeader.biHeight = -rImageDef.lHeight;
	pbmi->bmiHeader.biPlanes = 1; //?
	pbmi->bmiHeader.biBitCount = wClrBits;
	if (wClrBits < numberOfBitsForBGR888X)
	{
		pbmi->bmiHeader.biClrUsed = (1 << wClrBits);
	}

	// If the bitmap is not compressed, set the BI_RGB flag. 
	pbmi->bmiHeader.biCompression = BI_RGB;

	// Compute the number of bytes in the array of color 
	// indices and store the result in biSizeImage. 
	// For Windows NT, the width must be DWORD aligned unless 
	// the bitmap is RLE compressed. This example shows this. 
	// For Windows 95/98/Me, the width must be WORD aligned unless the 
	// bitmap is RLE compressed.
	pbmi->bmiHeader.biSizeImage = ((abs(pbmi->bmiHeader.biWidth) * wClrBits + 31) & ~31) / 8 * abs(pbmi->bmiHeader.biHeight);

	// Set biClrImportant to 0, indicating that all of the 
	// device colors are important. 
	pbmi->bmiHeader.biClrImportant = 0;

	_ASSERT(pbmi);
	return pbmi;
}// end CreateBitmapInfoStruct( ImageDefinitionStruct& rImageDef )