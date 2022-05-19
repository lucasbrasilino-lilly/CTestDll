#pragma once


// Defines used in conjunction with UnIfDef Tool
/// These are the defines of External Tool functionality.
// single defines can be deleted if set using UnIfDef 
#define USE_INPUT_VALUES
#define USE_INPUT_IMAGES
#define USE_RESULT_VALUES
//not usable for SVO_INTERFACE_001_ONLY
#define USE_RESULT_ARRAYS
#define USE_RESULT_IMAGES
//not usable for SVO_INTERFACE_001_ONLY
#define USE_RESULT_TABLES

// TODO: undefine this define 'EXAMPLECODE' to hide example code
// The EXAMPLECODE is based on SVO_INTERFACE_820_OR_HIGHER!
#define EXAMPLECODE


// TODO: define which SVO Interface Versions should be supported, or if there are limitations for this tool to apply

/// To demand at least SVObserver 10.00 to properly allow tables and arrays
//#define SVO_INTERFACE_1000_OR_HIGHER

/// To demand at least SVObserver 8.20 to use with color images and/or tables and array
#define SVO_INTERFACE_820_OR_HIGHER

/// to use only functionality already present in SVObserver 
/// (no color images, no tables and arrays)
/// External DLLs compiled with this option are the most compatible ones.
//#define SVO_INTERFACE_001_ONLY

#if defined(USE_INPUT_IMAGES) || defined(USE_RESULT_IMAGES) 
/// for the time being all gray scale images are considered to be in SVImageFormatMono8,
/// all color images are considered to be in SVImageFormatRGB8888
namespace AllowedImageType
{
	constexpr uint64_t c_Gray{ 0x0000000000000001ULL };
#if !defined(SVO_INTERFACE_001_ONLY)
	constexpr uint64_t c_Color{ 0x0000000100000000ULL };
	constexpr uint64_t c_GrayOrColor{ c_Gray | c_Color };
#endif
}
#endif


//TODO: set all defines and static parameters as required
#define	DLLVERSION				0,10
#define TOOLNAME				"TemplateTool"
#define	DLLVERSIONSTRING		"DLL-V 0.10 (14.02.2020)"
#define DEBUGFILE				"C:\\SVIMDLLDebug_" TOOLNAME ".txt"
static const char	static_strToolName[512] = TOOLNAME;
static const long	static_iToolVersion = 010;

// TODO: add enums for your inputs and result (DO NOT assign numbers) and remove the sample enum values provided here afterwards

#ifdef USE_INPUT_VALUES
/// Enum List of all Input Values to be used by the ExternalTool. 
/// 
/// Tables must always have two entries after each other. First is the array for values, the next one must be a BSTR Array
///	for the table columns names.
/// ATTENTION: Do not assign numbers to the enums. The NUM_* entries must be the last entry of every enum.
///	If only the NUM_* defined, this input or result is zero.

enum InputValuesEnum
{
#ifdef EXAMPLECODE
	InputValue_LONG,
	InputValue_BOOL,
	InputValue_DOUBLE,
	InputValue_BSTR,
#if !defined(SVO_INTERFACE_001_ONLY)
	InputValue_DOUBLE_ARRAY,
	InputValue_LONG_ARRAY,
	InputValue_RGBVAL_ARRAY,
	InputValue_TABLE_ARRAY,
	InputValue_TABLE_NAMES,
	InputValue_LONG_TABLE_SELECT,
#endif
#endif
	NUM_INPUT_VALUES  // The number of input values
};
#endif 

#ifdef USE_INPUT_IMAGES
/// Enum List of all Input Images to be used by the ExternalTool. 
///
/// ATTENTION: Do not assign numbers to the enums. The NUM_* entries must be the last entry of every enum.
///	If only the NUM_* defined, this input or result is zero.
enum InputImagesEnum
{
#ifdef EXAMPLECODE
	InputImage_Gray,
#if !defined(SVO_INTERFACE_001_ONLY)
	InputImage_Color,
	InputImage_GrayOrColor,
#endif
#endif
	NUM_INPUT_IMAGES
};
#endif

#ifdef USE_INPUT_IMAGES
// TODO: Set the types of the input Images in the same order as defined in enum InputImagesEnum
const uint64_t AllowedInputImageTypes[NUM_INPUT_IMAGES] =
{
#ifdef EXAMPLECODE
	AllowedImageType::c_Gray
#if !defined(SVO_INTERFACE_001_ONLY)
	, AllowedImageType::c_Color
	, AllowedImageType::c_GrayOrColor
#endif
#endif
};
#endif

#ifdef USE_RESULT_VALUES
//ATTENTION: Do not assign numbers to the enums. The NUM_* entries must be the last entry of every enum.
//			 If only the NUM_* defined, this input or result is zero.
enum ResultValuesEnum
{
#ifdef EXAMPLECODE
	ResultValue_LONG,
	ResultValue_BOOL,
	ResultValue_DOUBLE,
	ResultValue_BSTR,
#if !defined(SVO_INTERFACE_001_ONLY)
	ResultValue_DOUBLE_ARRAY,
	ResultValue_LONG_ARRAY,
	ResultValue_LONG_ROWCOUNT,
	RESULTVALUE_BSTR_ROWNAME,
	RESULTVALUE_DOUBLE_ARRAY_ROW,
#endif
#endif
	NUM_RESULT_VALUES
};
#endif

#ifdef USE_RESULT_TABLES
//ATTENTION: Do not assign numbers to the enums. The NUM_* entries must be the last entry of every enum.
//			 If only the NUM_* defined, this input or result is zero.
enum ResultTablesEnum
{
#ifdef EXAMPLECODE
	FirstResultTable,
	ResultTable_ColImageRGB,
#endif
	NUM_RESULT_TABLES
};
#endif

#ifdef EXAMPLECODE
//Example defines used for Example Tables
#define ColumnCountA 6
#define RowCountA 9
#define ColumnCount_RGB 3
#define RowCount_RGB 500 // Max ?
#define MAX_COLS 250
#endif


#ifdef USE_RESULT_IMAGES
//ATTENTION: Do not assign numbers to the enums. The NUM_* entries must be the last entry of every enum.
//			 If only the NUM_* defined, this input or result is zero.
enum ResultImagesEnum
{
#ifdef EXAMPLECODE
	ResultImage_Gray,
#if !defined(SVO_INTERFACE_001_ONLY)
	ResultImage_Color,
#endif
#endif
	NUM_RESULT_IMAGES
};
#endif


#pragma region errorcodes
//ATTENTION: S_FALSE is 1L, the last digit should not be increased.
//TODO: add your error codes
#ifdef EXAMPLECODE
#define ERRORCODE_INPUTVALUE_LONG_INVALID				(S_FALSE|(HRESULT) 0x0010)
#define ERRORCODE_INPUTVALUE_BOOL_INVALID				(S_FALSE|(HRESULT) 0x0020)
#define ERRORCODE_INPUTVALUE_DOUBLE_ARRAY_WRONGTYPE		(S_FALSE|(HRESULT) 0x0030)
#define ERRORCODE_INPUTVALUE_DOUBLE_ARRAY_TOOSMALL		(S_FALSE|(HRESULT) 0x0040)
#define	ERRORCODE_INPUTVALUE_INT_ARRAY_WRONGTYPE		(S_FALSE|(HRESULT) 0x0050)
#endif

// Common Errorcodes used in other modules, these can be also used in functions
#pragma region common errorcodes
#define ERRORCODE_IMAGEFORMAT_INVALID					(S_FALSE|(HRESULT) 0x8010)
#define ERRORCODE_INITIALIZATION_INCOMPLETE				(S_FALSE|(HRESULT) 0x8020)
#define ERRORCODE_INPUTVALUE_TYPE_INVALID				(S_FALSE|(HRESULT) 0x8030)
#define ERRORCODE_INPUTVALUE_DEFAULTTYPE_INVALID 		(S_FALSE|(HRESULT) 0x8040)
#define ERRORCODE_INPUTVALUE_ARRAY_TO_BIG		 		(S_FALSE|(HRESULT) 0x8050)
#define ERRORCODE_INPUTVALUE_ARRAY_TO_SMALL				(S_FALSE|(HRESULT) 0x8060)
#define ERRORCODE_RESULTVALUE_TYPE_INVALID				(S_FALSE|(HRESULT) 0x8070)
#define ERRORCODE_USER_EXCEPTION						(S_FALSE|(HRESULT) 0xFF00)
#pragma endregion common errorcodes
#pragma endregion errorcodes


// definition checks
#if defined SVO_INTERFACE_1000_OR_HIGHER && defined SVO_INTERFACE_001_ONLY
static_assert(false, "SVO_INTERFACE_1000_OR_HIGHER && SVO_INTERFACE_001_ONLY cannot be defined at the same time!");
#endif

#if defined SVO_INTERFACE_1000_OR_HIGHER && defined SVO_INTERFACE_820_OR_HIGHER
static_assert(false, "SVO_INTERFACE_1000_OR_HIGHER && SVO_INTERFACE_820_OR_HIGHER cannot be defined at the same time!");
#endif


#if defined SVO_INTERFACE_820_OR_HIGHER && defined SVO_INTERFACE_001_ONLY
static_assert(false, "SVO_INTERFACE_820_OR_HIGHER && SVO_INTERFACE_001_ONLY cannot be defined at the same time!");
#endif

#if defined MIL9 && defined MIL10
static_assert(false, "MIL9 && MIL10 cannot be defined at the same time!");
#endif

#if defined NOMIL && defined MIL9
static_assert(false, "NOMIL && MIL9 cannot be defined at the same time!");
#endif

#if defined NOMIL && defined MIL10
static_assert(false, "NOMIL && MIL10 cannot be defined at the same time!");
#endif
