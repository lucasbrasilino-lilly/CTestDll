#pragma once

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TOOLDLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TOOLDLL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef TOOLDLL_EXPORTS
#define TOOLDLL_API extern "C" __declspec(dllexport)
#else
#define TOOLDLL_API __declspec(dllimport)
#endif

#include <map>

#include "CDllTool.h"
#include "Guid.h"
#include "defines.h"
#include <mutex>
#include <atomic>

HINSTANCE hReporterModule;
typedef std::map< GUID, CDllTool* > MapToolsType;
MapToolsType g_DllTools;

std::mutex  g_DllToolsMutex;
// variable to store SVO Interface Version
// this is set in SVSimpleTest
std::atomic<long> g_svo_interface_version = 0;

/// This lookup for the tool
/// \param guidTool the gui id
/// \param rpDllTool [out] the tool reference
/// \return hresult value. S_OK if tool is found
HRESULT LookupTool(GUID guidTool, CDllTool*& rpDllTool);

/// Basic Check the input image definitions
HRESULT CheckInputImageDefinitions(const ImageDefinitionStruct* const pInputImageDefinitions);

/// Basic Check of input Value definitions
HRESULT CheckInputValueDefinitions(const std::array<InputValueDefinitionStructEx, NUM_INPUT_VALUES>  &rInputDef);

/// Basic Check of result  Value definitions
HRESULT CheckResultValueDefinitions(const std::array<ResultValueDefinitionStructEx, NUM_RESULT_VALUES>  &rResultDef);

// exports

/// This function receives two long parameters and should return the second parameter. 
/// This simple test will show that addresses are lined up correctly and should be able to test calling convention.
/// \param lValue1 is now also used to pass the SVObserver interface Version.
TOOLDLL_API long    __stdcall SVSimpleTest(long lValue1, long lValue2);

/// This function is called immediately after the DLL is loaded and verified.
TOOLDLL_API HRESULT __stdcall SVStartup();

/// Retrieve the name of the generic tool as it is desired to be referenced. 
/// The characters that are allowed are A-Z, a-z, 0-9, underscore ("_").
/// The tool will allocate the BSTR and SVObserver will deallocate it when finished with it.
TOOLDLL_API HRESULT __stdcall SVGetToolName(BSTR* pbstrToolName);

/// The parameter is an unsigned long that represents the tool version. 
/// It does not matter what the format of the version number is within the unsigned long, 
/// but SVObserver will base earlier/later version decisions on the values of the unsigned long.
TOOLDLL_API HRESULT __stdcall SVGetToolVersion(long* plVersionNumber);

/// There is an input image limit of 4.
TOOLDLL_API HRESULT __stdcall SVGetNumberOfInputImages(long* plNumberOfInputImages);

/// This command executes the tool. 
/// The tool should perform its processing based on the input images and input values, 
/// and prepare to provide the result images and result values.
TOOLDLL_API HRESULT __stdcall SVRunTool(GUID tool, long* plStatus);

/// This function is called prior to unloading the DLL.
TOOLDLL_API HRESULT __stdcall SVShutDown();

/// SVInitializeRun is called whenever the tool gets initialized/re-initialized. 
/// paStructs is a pointer to an array of ImageDefinitionStructs, 
/// pavInputValues is a pointer to an array of VARIANTs. 
/// The DLL should allocate result HBITMAPS in this function.
TOOLDLL_API HRESULT __stdcall SVInitializeRun(GUID tool,
	long lImageArraySize, ImageDefinitionStruct* paStructs,
	long lValueArraySize, VARIANT* pavInputValues);

/// This function is called whenever a specific instance of the DLL tool is removed from a toolset (IP) 
/// and before a tool is re-initialized with SVInitializeRun.
TOOLDLL_API HRESULT __stdcall SVUninitializeRun(GUID tool);

/// Retrieves the number of values to be used as inputs.
/// There is a limit of 50 input values.
/// These values will appear within the Result Picker, SIAC, and to other tools as 
/// "Input 01", "Input 02", "Input 03", etc.
/// There is a corresponding "Input 01 Name", etc., value object in SVObserver containing the Display Name of each value so that 
/// it is externally accessible to the user of the DLL.
/// This function must store the number of elements in the array in plArraySize, 
/// and store the pointer to an allocated array in ppaInputValueDefinitions.
/// This array must be deallocated in SVDestroyInputValueDefinitionStructures.
TOOLDLL_API HRESULT __stdcall SVGetInputValueDefinitions(long* plArraySize,
	InputValueDefinitionStruct** ppaStructs);

/// Setting bit 30 of the HRESULT indicates an invalid parameter. 
/// Bit 31 indicates an error condition. The low order 16 bits should indicate the reason. 
/// SVGetErrorMessageString should be able to get text associated with the reason.
/// Any HRESULT without bit 30 or 31 set indicates a valid parameter.
/// If the VARIANT contains a BSTR, SVObserver is responsible for destroying the BSTR.
TOOLDLL_API HRESULT __stdcall SVValidateValueParameter(GUID tool, long lParameterNumber, VARIANT vParameterValue);

/// Deletes the array allocated in SVGetResultValueDefinitions
TOOLDLL_API HRESULT __stdcall SVDestroyResultValueDefinitionStructures(ResultValueDefinitionStruct* paStructs);

/// Deletes the arrays allocated in SVGetInputImageDefinitions or SVGetResultImageDefinitions.
TOOLDLL_API HRESULT __stdcall SVDestroyImageDefinitionStructure(ImageDefinitionStruct* paStructs);

/// Deletes the array allocated in SVGetInputValueDefinitions
TOOLDLL_API HRESULT __stdcall SVDestroyInputValueDefinitionStructures(InputValueDefinitionStruct* paStructs);

/// SVObserver will allocate an array of empty VARIANTs, and pass a pointer to this array into the function. 
/// The DLL will copy the results into these variants, allocating BSTRs as necessary. 
/// SVObserver is responsible for destroying any BSTRs that were associated with the return values.
TOOLDLL_API HRESULT __stdcall SVGetResultValues(GUID tool, long lArraySize, VARIANT* paResultValues);

/// This function should extract the low order 16 bits of the error number input parameter, and return a message to be associated with those low order 16 bits. 
/// The high order bits should be acceptable with any setting and should be masked out within this function.
/// The DLL will allocate the BSTR and SVObserver will destroy the BSTR when it is finished using it.
TOOLDLL_API HRESULT __stdcall SVGetErrorMessageString(unsigned long ulErrorNumber, BSTR* pbstrErrorMessage);

/// Retrieves the number of values to be used as results.The results are VARIANTs(see section 3.1).
/// There is a limit of 50 result values.
///  These values will appear within the Result Picker, SIAC, and to other tools as "Result 01", "Result 02", "Result 03", etc.There is a corresponding "Result 01 Name", etc., value object in SVObserver containing the bstrDisplayName of each value so that it is externally accessible to the user of the DLL.
///  This function must store the number of elements in the array in plArraySize, and store the pointer to an allocated array in ppaResultValueDefinitions.This array is deallocated in SVDestroyResultValueDefinitionStructures.
TOOLDLL_API HRESULT __stdcall SVGetResultValueDefinitions(long* plArraySize, ResultValueDefinitionStruct** ppaResultValues);



/// There is a result image limit of 4.
/// This function should retrieve image size data to allow SVObserver to pre-allocate image buffers to be the correct size.
/// This function must store the number of elements in the array in plArraySize, 
/// and store the pointer to an allocated array in ppaImageDefinitions.
/// This array is deallocated in SVDestroyImageDefinitionStructures.
TOOLDLL_API HRESULT __stdcall SVGetResultImageDefinitions(GUID tool,
	long* plArraySize, ImageDefinitionStruct** ppaImageDefinitions);

/// The values passed into the DLL by this function, specifically the BSTRs, 
/// may not be manipulated by the DLL.This means that the DLL may not delete, destroy, re-allocate, 
/// or write to the referenced BSTR. SVObserver will create and destroy these BSTRs.
/// This is also valid for Arrays and Tables
TOOLDLL_API HRESULT __stdcall SVSetInputValues(GUID tool,
	long lArraySize, VARIANT* paInputValues);

#ifdef NOMIL
/// SVObserver will allocate an array of NULL HBITMAP handles and pass a pointer to this array to the function. 
/// The DLL will be responsible for providing the HBITMAP handles for the Windows Objects we SVObserver will reference. 
/// SVObserver will not close the handles.
TOOLDLL_API HRESULT __stdcall SVGetHBITMAPResultImages(GUID tool, long lArraySize, HBITMAP* paHandles);

/// Called when HBITMAP handles are used. The DLL must not write into these images or deallocate them. 
/// The array size is the same as the number of input images reported by the DLL.
TOOLDLL_API HRESULT __stdcall SVSetHBITMAPInputImages(GUID tool, HBITMAP* paHandles);

#else //MIl images
/// When using MIL IDs, SVObserver is responsible for allocating and destroying all input image buffers. 
/// SVObserver tells the DLL which MIL buffer to use by passing the MIL IDs via the SVSetMILInputImages function 
/// before the SVRunTool function. 
TOOLDLL_API HRESULT __stdcall SVSetMILInputImages(GUID tool, long lArraySize, long* paMILhandles);

/// When using MIL IDs, SVObserver is responsible for allocating and destroying all result image buffers. 
/// SVObserver tells the DLL which MIL buffer to use by passing the MIL IDs via the SVSetMILIDResultImages function 
/// before the SVRunTool function. Then after the tool has executed,
/// SVObserver assumes that those MIL IDs have been populated with the new result images.
TOOLDLL_API HRESULT __stdcall SVSetMILResultImages(GUID tool, long lArraySize, long* paMILhandles);
#endif

// New Interface from SVObserver 8.20 and 10.00


/* SVObserver call order MIL (8.20)
===== SVSimpleTest
===== SVStartup
===== SVGetToolName
===== SVGetToolVersion
===== SVGetNumberOfInputImages
===== SVGetInputImageInformation
===== SVGetInputValueDefinitions
===== SVDestroyInputValueDefinitionStructures
===== SVInitializeRun
===== SVGetResultValueDefinitions
===== GetResultValuesMaxArraySize
===== SVDestroyResultValueDefinitionStructures
===== GetResultTableDefinitions
===== GetResultTablesMaxRowSize
===== DestroyResultTableDefinitionStructures
===== SVSetMILInputImages
===== SVGetResultImageDefinitions
===== SVDestroyImageDefinitionStructure
===== SVSetMILResultImages
===== SVGetResultValues
===== GetResultTables

===== SVSetInputValues
===== SVSetMILInputImages
===== SVSetMILResultImages
===== SVRunTool
===== SVGetResultValues
===== GetResultTables

===== SVSetInputValues
===== SVSetMILInputImages
===== SVSetMILResultImages
===== SVRunTool
===== SVGetResultValues
===== GetResultTables
*/