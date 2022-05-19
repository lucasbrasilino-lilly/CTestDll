///****************************************************************************
/// \copyright COPYRIGHT (c) 2020,2020 by Seidenader Maschinenbau GmbH. 
/// All Rights Reserved 
/// \file CMyBitMapInfo.h
/// This Class includes functionality to deal with HBitmap Info 
///*****************************************************************************

#pragma once

#include "StructDefinitions.h"

//// This class is used to pass informations on Result Images to SVObserver
class CMyBitMapInfo
{
public:
	/// Default constructor
	CMyBitMapInfo() { hbm = 0; pBits = 0; pBitmapInfo = 0; pBitmapHeader = 0; }
public:
	CMyBitMapInfo(ImageDefinitionStructEx& rImageDef);
	~CMyBitMapInfo();

public:
	HBITMAP getBitmapHandle();
	BITMAPINFO* getBitmapInfo() const { return pBitmapInfo; };
	BITMAPINFOHEADER* getBitmapHeader() const { return pBitmapHeader; };
	void* getBitsPointer() const { return pBits; };

private:
	BITMAPINFO* CreateBitmapInfoStruct(ImageDefinitionStructEx& rImageDef);

private:
	HBITMAP hbm;
	void* pBits;
	BITMAPINFO* pBitmapInfo;
	BITMAPINFOHEADER* pBitmapHeader;
};
