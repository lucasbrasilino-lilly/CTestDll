// ExternalDll.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"

//#include <sstream> 

#include "ExternalDll.h"
#include "DebugUtilities.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static_assert(NUM_INPUT_IMAGES <= 4, "There must not be more then 4 input images!");
static_assert(NUM_RESULT_IMAGES <= 4, "There must not be more then 4 result images!");

static_assert(NUM_INPUT_VALUES <= 50, "There must not be more then 50 input values!");

static_assert(NUM_RESULT_VALUES <= 50, "There must not be more then 50 result values!");


extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
#if _DEBUG
		start_fdb(1);
#endif		
		FDB(0, "DLL_PROCESS_ATTACH\n");

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		FDB(0, "DLL_PROCESS_ATTACH Vers %d  <%s>\n", static_iToolVersion, static_strToolName);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		FDB(0, "DLL_PROCESS_DETACH; Tool Version: %d  Tool Name: <%s>\n", static_iToolVersion, static_strToolName);
		MapToolsType::iterator iter;
		for (iter = g_DllTools.begin(); iter != g_DllTools.end(); iter++)
		{
			delete iter->second;
		}
		g_DllTools.clear();
	}
	return 1;   // 1 = ok

}

HRESULT LookupTool(GUID guidTool, CDllTool*& rpDllTool)
{
	HRESULT hRet = S_OK;

	std::lock_guard<std::mutex> Lock(g_DllToolsMutex);


	MapToolsType::iterator iter;
	if ((iter = g_DllTools.find(guidTool)) == g_DllTools.end())
	{
		FDB(1, "LookupTool did not find the tool\n");
		hRet = S_FALSE;
	}
	else	// use existing
	{
		rpDllTool = iter->second;
	}

	return hRet;
}

// Check all Input Image Definitions for the format. 
// This is called by the initRun function.
HRESULT CheckInputImageDefinitions(const ImageDefinitionStruct* const pInputImageDefinitions)
{
	HRESULT hr = S_OK;
	SVTRY;
	{
		FDB(2, "CheckInputImageDefinitions - Enter\n");

		for (int i = 0; i < NUM_INPUT_IMAGES; i++)
		{
			FDB(2, "CheckInputImageDefinitions - Image %d\n", i);
			if (pInputImageDefinitions[i].eImageFormat != SVImageFormatMono8)
			{
				return ERRORCODE_IMAGEFORMAT_INVALID;
			}
		}
		FDB(2, "CheckInputImageDefinitions - Exit OK\n");
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);

	return hr;
}

HRESULT CheckInputValueDefinitions(const std::array<InputValueDefinitionStructEx, NUM_INPUT_VALUES>  &rInputDef)
{
	FDB(2, "CheckInputValueDefinitions - Enter\n");
	HRESULT hr = S_OK;
	SVTRY;
	{
		bool check_interface_1_compatibility = false;
		if (g_svo_interface_version == 1)
		{
			check_interface_1_compatibility = true;
		}
		check_interface_1_compatibility = true;
		for (int i = 0; i < NUM_INPUT_VALUES; i++)
		{
			if (check_interface_1_compatibility)
			{
				switch (rInputDef[i].vt)
				{
				case VT_I4:
				case VT_R8:
				case VT_BOOL:
				case VT_BSTR:
					// these are allowed
					break;
				default:
					FDBERROR("Input Value %d: InputValue Type not allowed\n", i);
					return ERRORCODE_INPUTVALUE_TYPE_INVALID;
					break;
				}
			} // check_interface_1_compatibility

			// generall checks for all SVObserver Interface Versions
			switch (rInputDef[i].vt)
			{
			case VT_I4:
			case VT_R8:
			case VT_BOOL:
			case VT_BSTR:
			case VT_ARRAY | VT_I4:
			case VT_ARRAY | VT_R8:
			case VT_ARRAY | VT_BSTR:
				// these are allowed
				break;
			default:
				FDBERROR("Input Value %d:  Type not allowed\n", i);
				return ERRORCODE_INPUTVALUE_TYPE_INVALID;
				break;
			}

			if (rInputDef[i].vt != rInputDef[i].vDefaultValue.vt)
			{
				FDBERROR("Input Value %d: Type does not match default value type\n", i);
				return ERRORCODE_INPUTVALUE_DEFAULTTYPE_INVALID;

			}
		}
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);
	FDB(2, "CheckInputValueDefinitions - Exit\n");
	return hr;
}

HRESULT CheckResultValueDefinitions(const std::array<ResultValueDefinitionStructEx, NUM_RESULT_VALUES>  &rResultDef)
{
	FDB(2, "CheckResultValueDefinitions - Enter\n");
	HRESULT hr = S_OK;
	SVTRY;
	{
		bool check_interface_1_compatibility = false;
		if (g_svo_interface_version == 1)
		{
			check_interface_1_compatibility = true;
		}
		for (int i = 0; i < NUM_RESULT_VALUES; i++)
		{
			if (check_interface_1_compatibility)
			{
				switch (rResultDef[i].vt)
				{
				case VT_I4:
				case VT_R8:
				case VT_BOOL:
				case VT_BSTR:
					// these are allowed
					break;
				default:
					FDB(2, "Result Value %d: Type not allowed\n", i);
					return ERRORCODE_RESULTVALUE_TYPE_INVALID;
					break;
				}
			} // check_interface_1_compatibility

			// generall checks for all SVObserver Interface Versions
			switch (rResultDef[i].vt)
			{
			case VT_I4:
			case VT_R8:
			case VT_BOOL:
			case VT_BSTR:
			case VT_ARRAY | VT_I4:
			case VT_ARRAY | VT_R8:
				// case VT_ARRAY | VT_BSTR:
				// these are allowed
				break;
			default:
				FDB(2, "Result Value %d: Type not allowed\n", i);
				return ERRORCODE_RESULTVALUE_TYPE_INVALID;
				break;
			}
		}
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);
	FDB(2, "CheckResultValueDefinitions - Exit\n");
	return hr;
}



// ******* Simple Test function to check that the calling convention is correct.
TOOLDLL_API long __stdcall SVSimpleTest(long lValue1, long lValue2)
{
	FDB(0, "===== SVSimpleTest - lValue1=%ld lValue2=%ld\n", lValue1, lValue2);
	g_svo_interface_version = lValue1;
	FDB(1, "Interface Version: %ld\n", g_svo_interface_version);
	return lValue2;
}

// Gets the name of the DLL Tool
TOOLDLL_API HRESULT __stdcall SVGetToolName(BSTR* pbstrToolName)
{
	HRESULT hr = S_OK;
	SVTRY;
	{
		FDB(0, "===== SVGetToolName - Enter - pbstrToolName=<%s> <%s>\n", static_strToolName, DLLVERSIONSTRING);

		char strTemp[1024] = "";
		strcpy_s(strTemp, static_strToolName);
		strcat_s(strTemp, "  ");
		strcat_s(strTemp, DLLVERSIONSTRING);
		strcat_s(strTemp, "  ");

		long interfaceVersion = g_svo_interface_version.load();
		char g_svo_interface_version_string[12];
		sprintf_s(g_svo_interface_version_string, 12, "%d", interfaceVersion);
		strcat_s(strTemp, " SVO Interface ");
		strcat_s(strTemp, g_svo_interface_version_string);
		strcat_s(strTemp, "  ");





		FDB(1, "<%s>\n", strTemp);
		BSTR bstrNew = A2BSTR(strTemp);
		if (bstrNew == NULL) return S_FALSE;
		*pbstrToolName = bstrNew;
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);
	FDB(0, "===== SVGetToolName - Exit\n");
	return hr;
}

// Tool Version 
TOOLDLL_API HRESULT __stdcall SVGetToolVersion(long* plVersionNumber)
{
	FDB(0, "===== SVGetToolVersion - Enter - static_iToolVersion=%d\n", static_iToolVersion);
	*plVersionNumber = static_iToolVersion;
	FDB(0, "===== SVGetToolVersion - Exit\n");
	return S_OK;
}



// Run Tool 
TOOLDLL_API HRESULT __stdcall SVRunTool(GUID guidTool, long* plStatus)
{
	FDB(0, "===== SVRunTool - Enter\n");
	HRESULT hr{ S_OK };
	SVTRY;
	{
		CDllTool* pTool = NULL;
		hr = LookupTool(guidTool, pTool);
		if (hr == S_OK && pTool != NULL)
		{
			hr = pTool->run();
		}
		//int iRes=0;
		*plStatus = 0;

		FDB(0, "===== SVRunTool - Exit - hr=0x%X\n", hr);
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);
	return hr;
}

TOOLDLL_API HRESULT __stdcall SVStartup()
{
	FDB(0, "===== SVStartup\n");
	return S_OK;
}

TOOLDLL_API HRESULT __stdcall SVShutDown()
{
	FDB(0, "===== SVShutDown\n");
	return S_OK;
}

TOOLDLL_API HRESULT __stdcall SVInitializeRun(GUID guidTool, long lImageArraySize,
	ImageDefinitionStruct* pInputImageDefinitions, long lValueArraySize, VARIANT* pavInputValues)
{
	HRESULT hr = S_OK;
	SVTRY;
	{

		FDB(0, "===== SVInitializeRun - Enter - lImageArraySize %d  lValueArraySize %d\n", lImageArraySize, lValueArraySize);

		if (lImageArraySize != NUM_INPUT_IMAGES)
		{
			FDBERROR("SVInitializeRun Failed Input sizes\n");
			return S_FALSE;
		}

		if (lValueArraySize != NUM_INPUT_VALUES)
		{
			FDBERROR("SVInitializeRun Failed Input sizes\n");
			return S_FALSE;
		}

		CDllTool* pDllTool = NULL;

		// Check if this tool guid exists
		HRESULT hrLookup = LookupTool(guidTool, pDllTool);
		if (hrLookup != S_OK)
		{	// create new tool
			pDllTool = new CDllTool;
			if (pDllTool == NULL)
			{
				FDB(1, "SVInitializeRun - Creation of new tool failed\n");
				return S_FALSE;
			}
			FDB(1, "SVInitializeRun - New Tool created\n");

			pDllTool->setGUID(guidTool);

			// store in map
			{
				std::lock_guard<std::mutex> Lock(g_DllToolsMutex);
				g_DllTools[guidTool] = pDllTool;
			}
		}

		FDB(1, "SVInitializeRun - CheckInputImageDefinitions\n");
		if (hr = CheckInputImageDefinitions(pInputImageDefinitions) != S_OK)
		{
			FDB(1, "SVInitializeRun - CheckInputImageDefinitions Failed\n");
			return hr;
		}
		FDB(1, "SVInitializeRun -  initRun\n");

		for (long i = 0; i < NUM_INPUT_VALUES; ++i)
		{
			hr = pDllTool->validateInputValue(i, pavInputValues[i]);
			if (hr != S_OK)
			{
				_bstr_t errorMessage = CDllTool::getErrorMessageString(hr);
				FDB(1, "Input Value %d is NOT OK: Message %s\n", i, errorMessage);
			}
		}
		using VariantArray = const std::array<VARIANT, NUM_INPUT_VALUES>;

		using ImageDefinitionArray = const std::array<ImageDefinitionStruct, NUM_INPUT_IMAGES>;
		using ImageDefinitionArrayEx = const std::array<ImageDefinitionStructEx, NUM_INPUT_IMAGES>;

		ImageDefinitionStructEx* pIMageDefinitionEx = new ImageDefinitionStructEx[NUM_INPUT_IMAGES];

		for (int i = 0; i < NUM_INPUT_IMAGES; i++)
		{
			pIMageDefinitionEx[i] = pInputImageDefinitions[i];
		}
		hr = pDllTool->initRun(*(ImageDefinitionArrayEx*)pIMageDefinitionEx, *(VariantArray*)pavInputValues);
		delete[] pIMageDefinitionEx;

		FDB(0, "===== SVInitializeRun - Exit - hr=0x%X\n", hr);
		return hr;

	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);
	return hr;

}

TOOLDLL_API HRESULT __stdcall SVUninitializeRun(GUID guidTool)
{
	FDB(0, "===== SVUninitializeRun - Enter\n");

	HRESULT hr = S_OK;

	CDllTool* pDllTool = NULL;

	{
		std::lock_guard<std::mutex> Lock(g_DllToolsMutex);

		MapToolsType::iterator iter;
		if ((iter = g_DllTools.find(guidTool)) == g_DllTools.end())
		{
			FDBERROR("Tool not found\n");
			hr = S_FALSE;
		}
		else
		{
			pDllTool = iter->second;
			g_DllTools.erase(iter);
		}

	}

	if (pDllTool)
	{
		delete pDllTool;// do this outside critical section
	}
	else
	{
		FDB(0, "===== SVUninitializeRun - DO NOT DELETE DLL-TOOL - Nr=0x%X\n", guidTool);
	}
	FDB(0, "===== SVUninitializeRun - Exit - hr=0x%X\n", hr);
	return hr;
}


// cppcheck-suppress unusedFunction



// cppcheck-suppress unusedFunction SVDestroyInputImageInformation


// cppcheck-suppress unusedFunction

// cppcheck-suppress unusedFunction
TOOLDLL_API HRESULT __stdcall SVSetInputValues(GUID guidTool, long lArraySize,
	VARIANT* paInputValues)
{
	HRESULT hr = S_FALSE;
	SVTRY;
	{
		FDB(0, "===== SVSetInputValues - Enter - lArraySize=%d = NUM_INPUT_VALUES=%d\n", lArraySize, NUM_INPUT_VALUES);
		if (lArraySize == NUM_INPUT_VALUES)
		{
			CDllTool* pTool = NULL;
			hr = LookupTool(guidTool, pTool);

			if (hr == S_OK)
			{	// found tool
				using VariantArray = std::array<VARIANT, NUM_INPUT_VALUES>;
				hr = pTool->setInputValues(*(VariantArray*)paInputValues);
			}
		}
		else
		{
			FDB(1, "SVSetInputValues - Number of parameters are different!\n");
		}
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);
	FDB(0, "===== SVSetInputValues - Exit - hr=0x%X\n", hr);
	return hr;
}

// cppcheck-suppress unusedFunction
TOOLDLL_API HRESULT __stdcall SVGetResultValues(GUID guidTool, long lArraySize,
	VARIANT* paResultValues)
{

	HRESULT hr = S_FALSE;
	SVTRY;
	{
		FDB(0, "===== SVGetResultValues - Enter - lArraySize=%d = NUM_RESULT_VALUES=%d\n", lArraySize, NUM_RESULT_VALUES);
		if (lArraySize == NUM_RESULT_VALUES)
		{
			CDllTool* pTool = NULL;
			hr = LookupTool(guidTool, pTool);

			if (hr == S_OK)
			{
				using VariantArray = std::array<VARIANT, NUM_RESULT_VALUES>;
				hr = pTool->getResultValues(*(VariantArray*)(paResultValues));
			}
		}
		FDB(0, "===== SVGetResultValues - Exit - hr=0x%X\n", hr);
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);
	return hr;
}

// cppcheck-suppress unusedFunction
TOOLDLL_API HRESULT __stdcall SVGetErrorMessageString(unsigned long ulErrorNumber,
	BSTR* pbstrErrorMessage)
{
	FDB(0, "===== SVGetErrorMessageString - Enter: ErrorCode: 0x%X\n", ulErrorNumber);

	HRESULT hr{ S_OK };
	SVTRY;
	{
		_bstr_t errorMessage = CDllTool::getErrorMessageString(ulErrorNumber);
		if (errorMessage.length() == 0)
		{
			FDB(0, "===== SVGetErrorMessageString - Exit E_NOTIMPL\n");
			return E_NOTIMPL;
		}
		else
		{
			*pbstrErrorMessage = errorMessage.Detach();
			FDB(0, "===== SVGetErrorMessageString - Exit S_OK\n");
			return S_OK;
		}
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);
	return hr;
}

// cppcheck-suppress unusedFunction
TOOLDLL_API HRESULT __stdcall SVValidateValueParameter(GUID guidTool, long lParameterNumber,
	VARIANT vParameterValue)
{
	HRESULT hr{ S_OK };
	FDB(0, "===== SVValidateValueParameter - Enter - PNum %d\n", lParameterNumber);
	SVTRY;
	{
		hr = CDllTool::validateInputValue(lParameterNumber, vParameterValue);
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);
	FDB(0, "===== SVValidateValueParameter - Exit - hr=0x%X\n", hr);
	return hr;
}






// cppcheck-suppress unusedFunction



TOOLDLL_API HRESULT __stdcall SVGetInputValueDefinitions(long* plArraySize,
	InputValueDefinitionStruct** ppInputValueDefinition)
{
	HRESULT hr = S_OK;
	SVTRY;
	{
		FDB(0, "===== SVGetInputValueDefinitions - Enter - NumInputValues %d\n", NUM_INPUT_VALUES);
		typedef std::basic_string<WCHAR> wstring;

		if (NUM_INPUT_VALUES > 0)
			*ppInputValueDefinition = new InputValueDefinitionStruct[NUM_INPUT_VALUES];
		else
			*ppInputValueDefinition = NULL;

		//### check alloc failed
		if (ppInputValueDefinition == NULL)
		{
			FDBERROR("SVGetInputValueDefinitions - allocation InputValueDefinitionStruct failed\n");
			return S_FALSE;
		}
		else
		{
			FDB(1, "SVGetInputValueDefinitions - allocation InputValueDefinitionStruct OK\n");
		}

		int i;
		for (i = 0; i < NUM_INPUT_VALUES; i++)
		{
			::VariantInit(&((*ppInputValueDefinition)[i].vDefaultValue));
		}

		*plArraySize = NUM_INPUT_VALUES;

		std::array<InputValueDefinitionStructEx, NUM_INPUT_VALUES> InputsEx;
		CDllTool::getInputValuesDefinition(InputsEx);
		hr = CheckInputValueDefinitions(InputsEx);
		if (hr != S_OK)
		{
			FDB(0, "===== SVGetInputValueDefinitions - Exit - hr=0x%X\n", hr);
			return hr;
		}

		for (int index = 0; index < NUM_INPUT_VALUES; index++)
		{
			(*ppInputValueDefinition)[index].bstrDisplayName = InputsEx[index].Name.Detach();
			(*ppInputValueDefinition)[index].lVT = InputsEx[index].vt;
			(*ppInputValueDefinition)[index].bstrHelpText = InputsEx[index].HelpText.Detach();
			(*ppInputValueDefinition)[index].bstrGroup = InputsEx[index].Group.Detach();
			(*ppInputValueDefinition)[index].vDefaultValue.Attach(InputsEx[index].vDefaultValue.Detach());
		}
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);
	FDB(0, "===== SVGetInputValueDefinitions - Exit - hr=0x%X\n", hr);
	return hr;
}


TOOLDLL_API HRESULT __stdcall SVDestroyInputValueDefinitionStructures(
	InputValueDefinitionStruct* paStructs)
{
	FDB(0, "===== SVDestroyInputValueDefinitionStructures - Enter\n");
	HRESULT hr = S_OK;

	SVTRY;
	{
		for (int i = 0; i < NUM_INPUT_VALUES; i++)
		{
			::SysFreeString(paStructs[i].bstrDisplayName);
			::SysFreeString(paStructs[i].bstrGroup);
			::SysFreeString(paStructs[i].bstrHelpText);
			//::SysFreeString(paStructs[i].bstrGroup);
			hr = ::VariantClear(&paStructs[i].vDefaultValue);
			if (hr != S_OK)
			{
				FDB(1, "VariantClear did not work\n");
				return S_FALSE;
			}
		}
		delete[] paStructs;
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);
	FDB(0, "===== SVDestroyInputValueDefinitionStructures - Exit2 - hr=0x%X\n", hr);
	return hr;
}

TOOLDLL_API HRESULT __stdcall SVGetResultValueDefinitions(
	long* plArraySize, ResultValueDefinitionStruct** ppResultValues)
{
	HRESULT hr = S_OK;
	SVTRY;
	{
		FDB(0, "===== SVGetResultValueDefinitions - Enter - NumResults %d\n", NUM_RESULT_VALUES);
		*plArraySize = NUM_RESULT_VALUES;
		*ppResultValues = new ResultValueDefinitionStruct[NUM_RESULT_VALUES];

		if (ppResultValues == NULL)
		{
			return S_FALSE;
		}

		std::array<ResultValueDefinitionStructEx, NUM_RESULT_VALUES> ResultsEx;
		CDllTool::getResultValueDefinition(ResultsEx);
		hr = CheckResultValueDefinitions(ResultsEx);
		if (hr != S_OK)
		{
			return hr;
		}

		for (int index = 0; index < NUM_RESULT_VALUES; index++)
		{
			(*ppResultValues)[index].bstrDisplayName = ResultsEx[index].Name.copy();
			(*ppResultValues)[index].lVT = ResultsEx[index].vt;
		}
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);

	FDB(0, "===== SVGetResultValueDefinitions - Exit - hr=0x%X\n", hr);
	return hr;
}

TOOLDLL_API HRESULT __stdcall SVDestroyResultValueDefinitionStructures(
	ResultValueDefinitionStruct* paStructs)
{
	FDB(0, "===== SVDestroyResultValueDefinitionStructures - Enter\n");

	HRESULT hr = S_OK;
	for (int i = 0; i < NUM_RESULT_VALUES; i++)
	{
		::SysFreeString(paStructs[i].bstrDisplayName);
	}
	delete[] paStructs;

	FDB(0, "===== SVDestroyResultValueDefinitionStructures - Exit - hr=0x%X\n", hr);
	return hr;
}



TOOLDLL_API HRESULT __stdcall SVGetNumberOfInputImages(long* plNumberOfInputImages)
{
	FDB(0, "===== SVGetNumberOfInputImages - NUM_INPUT_IMAGES=%d\n", NUM_INPUT_IMAGES);
	if (NUM_INPUT_IMAGES > 4)
	{
		*plNumberOfInputImages = 0;
		return S_FALSE;
	}
	*plNumberOfInputImages = NUM_INPUT_IMAGES;
	return S_OK;
}

#if defined( MIL9 ) || defined( MIL10 ) 
// Only implement One of the set of InputImages / ResultImages
TOOLDLL_API HRESULT __stdcall SVSetMILInputImages(GUID guidTool, long lArraySize, long* paMILhandles)
{
	HRESULT hr = S_FALSE;

	FDB(0, "===== SVSetMILInputImages - Enter\n");
	SVTRY;
	{
		if (lArraySize == NUM_INPUT_IMAGES)
		{
			if (0 == NUM_INPUT_IMAGES)
			{
				FDB(0, "===== SVSetMILInputImages - Exit - hr=0x%X\n", S_OK);
				return S_OK;
			}

			CDllTool* pTool = NULL;
			hr = LookupTool(guidTool, pTool);
			if (hr == S_OK)	// found tool
			{
				hr = pTool->setMILInputImages(paMILhandles);
			}
		}
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);
	FDB(0, "===== SVSetMILInputImages - Exit - hr=0x%X\n", hr);
	return hr;
}

// Only implement One of the set of InputImages / ResultImages
TOOLDLL_API HRESULT __stdcall SVSetMILResultImages(GUID guidTool, long lArraySize, long* paMILhandles)
{
	FDB(0, "===== SVSetMILResultImages - Enter\n");
	HRESULT hr = S_FALSE;
	SVTRY;
	{
		if (lArraySize == NUM_RESULT_IMAGES)
		{
			if (0 == NUM_RESULT_IMAGES)
			{
				FDB(0, "===== SVSetMILResultImages - Exit - hr=0x%X\n", S_OK);
				return S_OK;
			}

			CDllTool* pTool = NULL;
			hr = LookupTool(guidTool, pTool);
			if (hr == S_OK)	// found tool
			{
				hr = pTool->setMILResultImages(paMILhandles);
			}
		}
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);
	FDB(0, "===== SVSetMILResultImages - Exit - hr=0x%X\n", hr);
	return hr;
}
#endif

#if defined(NOMIL) 

TOOLDLL_API HRESULT __stdcall SVSetHBITMAPInputImages(GUID guidTool, HBITMAP* paHandles)
{
	FDB(0, "===== SVSetHBITMAPInputImages - Enter\n");
	HRESULT hr = S_FALSE;
	SVTRY;
	{
		if (NUM_INPUT_IMAGES != 0)
		{
			CDllTool* pTool = NULL;
			hr = LookupTool(guidTool, pTool);
			if (hr == S_OK)	// found tool
			{
				using HBitmapArray = std::array<HBITMAP, NUM_INPUT_IMAGES>;
				hr = pTool->setHBITMAPInputImages(*(HBitmapArray*)paHandles);
			}
		}
		else
		{
			hr = S_OK;
		}
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);
	FDB(0, "===== SVSetHBITMAPInputImages - Exit - hr=0x%X\n", hr);
	return hr;
}


// Only implement One of the set.. ResultImages
TOOLDLL_API HRESULT __stdcall SVGetHBITMAPResultImages(GUID guidTool, long lArraySize, HBITMAP* paHandles)
{
	FDB(0, "===== SVGetHBITMAPResultImages - Enter - lArraySize=%d = NUM_RESULT_IMAGES=%d\n", lArraySize, NUM_RESULT_IMAGES);
	HRESULT hr = S_FALSE;
	SVTRY;
	{
		if (NUM_RESULT_IMAGES == 0)
		{
			// do nothing
			return S_OK;
		}


		if (lArraySize == NUM_RESULT_IMAGES)
		{
			CDllTool* pTool = NULL;
			hr = LookupTool(guidTool, pTool);
			if (hr == S_OK)	// found tool
			{
				using HBitmapArray = std::array<HBITMAP, NUM_RESULT_IMAGES>;
				pTool->getHBITMAPResultImages(*(HBitmapArray*)paHandles);
			}
		}
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);
	FDB(0, "===== SVGetHBITMAPResultImages - Exit - hr=0x%X\n", hr);
	return hr;
}
#endif // NOMIL

TOOLDLL_API HRESULT __stdcall SVGetResultImageDefinitions(GUID guidTool,
	long* plArraySize, ImageDefinitionStruct** ppImageDefinition)
{

	FDB(0, "===== SVGetResultImageDefinitions - Enter\n");
	HRESULT hr = S_OK;
	SVTRY;
	{

		if (NUM_RESULT_IMAGES == 0)
		{
			*plArraySize = NUM_RESULT_IMAGES;
			return S_OK;
		}
		else if (NUM_RESULT_IMAGES > 4)
		{
			*plArraySize = 0;
			return S_FALSE;
		}

		CDllTool* pTool = NULL;

		HRESULT hrLookup = LookupTool(guidTool, pTool);
		if (hrLookup != S_OK)
		{
			hr = S_FALSE;
		}
		else // found tool
		{
			*plArraySize = NUM_RESULT_IMAGES;
			*ppImageDefinition = new ImageDefinitionStruct[NUM_RESULT_IMAGES];
			using ImageDefinitionArray = std::array<ImageDefinitionStruct, NUM_RESULT_IMAGES>;

			ImageDefinitionStructEx* pIMageDefinitionEx = new ImageDefinitionStructEx[NUM_RESULT_IMAGES];
			using ImageDefinitionArrayEx = std::array<ImageDefinitionStructEx, NUM_RESULT_IMAGES>;
			pTool->getResultImageDefs(*(ImageDefinitionArrayEx*)(pIMageDefinitionEx));
			for (int i = 0; i < NUM_RESULT_IMAGES; i++)
			{
				std::wstring displayname = pIMageDefinitionEx[i].bstrDisplayName;
				(*ppImageDefinition)[i].bstrDisplayName = ::SysAllocString(displayname.c_str());
				(*ppImageDefinition)[i].lHeight = pIMageDefinitionEx[i].lHeight;
				(*ppImageDefinition)[i].lWidth = pIMageDefinitionEx[i].lWidth;
				(*ppImageDefinition)[i].eImageFormat = pIMageDefinitionEx[i].eImageFormat;
			}

			delete[] pIMageDefinitionEx;
			// check for color image type
			for (int i = 0; i < NUM_RESULT_IMAGES; i++)
			{
				FDB(1, "Image %d width: %d, height: %d \n", i, (*ppImageDefinition)[i].lWidth, (*ppImageDefinition)[i].lHeight);
				switch ((*ppImageDefinition)[i].eImageFormat)
				{
				case SVImageFormatMono8:
					break;
				case SVImageFormatBGR888X:
					if (g_svo_interface_version == 1)
					{
						FDBERROR("ImageFormat SVImageFormatBGR888X not supported!\n");
						FDB(0, "===== SVGetResultImageDefinitions - Exit - hr=0x%X ERRORCODE_IMAGEFORMAT_INVALID\n", ERRORCODE_IMAGEFORMAT_INVALID);
						return ERRORCODE_IMAGEFORMAT_INVALID;
					}
					FDBERROR("ImageFormat SVImageFormatBGR888X not supported!\n");
					FDB(0, "===== SVGetResultImageDefinitions - Exit - hr=0x%X\n", ERRORCODE_IMAGEFORMAT_INVALID);
					return ERRORCODE_IMAGEFORMAT_INVALID;
					break;
				case SVImageFormatUnknown:
				default:
					FDBERROR("Unknown image format\n");
					FDB(0, "===== SVGetResultImageDefinitions - Exit - hr=0x%X ERRORCODE_IMAGEFORMAT_INVALID\n", ERRORCODE_IMAGEFORMAT_INVALID);
					return ERRORCODE_IMAGEFORMAT_INVALID;
					break;
				}
			}
		}
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);
	FDB(0, "===== SVGetResultImageDefinitions - Exit - hr=0x%X\n", hr);
	return hr;
}

TOOLDLL_API HRESULT __stdcall SVDestroyImageDefinitionStructure(ImageDefinitionStruct* paStructs)
{

	FDB(0, "===== SVDestroyImageDefinitionStructure - Enter\n");

	HRESULT hr = S_OK;
	for (int i = 0; i < NUM_RESULT_IMAGES; i++)
	{
		::SysFreeString(paStructs[i].bstrDisplayName);
	}
	delete[] paStructs;

	FDB(0, "===== SVDestroyImageDefinitionStructure - Exit - hr=0x%X\n", hr);
	return hr;
}


