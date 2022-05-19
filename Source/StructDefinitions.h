#pragma once 

#pragma pack( push, 1 )

struct DefinitionStructEx
{
	long vt{ VT_EMPTY };
	_bstr_t Name;
	_bstr_t HelpText;
	_bstr_t Group;
	_variant_t Reseved1; // not used at this time
	_variant_t Reserved2; // not used at this time
};

struct InputValueDefinitionStructEx : public DefinitionStructEx
{
	_variant_t vDefaultValue;
};

struct ResultValueDefinitionStructEx : public DefinitionStructEx
{
	long ArraySize{ 2 }; // not used in the template
};

struct ResultTableDefinitionStructEx : public DefinitionStructEx
{
	long ColoumnCount{ 1 };
	long RowCount{ 1 };
	_variant_t ColumnNames; //smart array of bstr with names

};

struct InputValueDefinitionStruct
{
	long lVT;
	BSTR bstrDisplayName;
	BSTR bstrHelpText;
	BSTR bstrGroup;
	_variant_t vDefaultValue;  //Changed from VARIANT ?
};

///AllowedImageTypes is meant to contain flags that specify which image types are acceptable.
/// currently, only two bits are used: 
///		bit 0 allows black and white images
///		bit 32 allows color images
/// later, bits 1 to 31 could specify specific black and white image types and
/// later, bits 33 to 63 could specify specific color image types
struct InputImageInformationStruct
{
	_bstr_t DisplayName;
	_bstr_t HelpText;
	uint64_t AllowedImageTypes;
};

struct ResultValueDefinitionStruct
{
	long lVT;
	BSTR bstrDisplayName;	// not used at this time
};

enum SVImageFormatEnum
{
	SVImageFormatUnknown = 0,
	SVImageFormatMono8 = 10,
	SVImageFormatBGR888X = 35 // clarification SVImageFormatRGB8888 = 35. This Format ist always 32 bit, packed. 
};

struct ImageDefinitionStruct
{
	BSTR bstrDisplayName;
	long lHeight;
	long lWidth;
	SVImageFormatEnum eImageFormat;
};

struct ImageDefinitionStructEx
{
	_bstr_t bstrDisplayName;
	long lHeight;
	long lWidth;
	SVImageFormatEnum eImageFormat;
	ImageDefinitionStructEx& operator=(const ImageDefinitionStruct& rhs)
	{
		bstrDisplayName = rhs.bstrDisplayName;
		lHeight = rhs.lHeight;
		lWidth = rhs.lWidth;
		eImageFormat = rhs.eImageFormat;
		return *this;
	}
};



struct ResultTableDefinitionStruct
{
	long lVT{ VT_EMPTY };
	_bstr_t bstrDisplayName;	// not used at this time
	DWORD type{ 0 }; 			// not used at this time
	long ColoumnCount{ 0 };		
	long RowCount{ 0 };			
	_variant_t ColumnNames;		//smart array of bstr with names

};

#pragma pack(pop) 
