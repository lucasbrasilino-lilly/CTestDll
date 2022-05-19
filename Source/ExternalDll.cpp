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

#ifdef USE_INPUT_IMAGES
static_assert(NUM_INPUT_IMAGES <= 4, "There must not be more then 4 input images!");
#endif
#ifdef USE_RESULT_IMAGES
static_assert(NUM_RESULT_IMAGES <= 4, "There must not be more then 4 result images!");
#endif

#ifdef USE_INPUT_VALUES
static_assert(NUM_INPUT_VALUES <= 50, "There must not be more then 50 input values!");
#endif

#ifdef USE_RESULT_VALUES
static_assert(NUM_RESULT_VALUES <= 50, "There must not be more then 50 result values!");
#endif

#ifdef USE_RESULT_TABLES
static_assert(NUM_RESULT_TABLES <= 4, "There must not be more then 4 result tables!");
#endif

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
#ifdef USE_INPUT_IMAGES
	SVTRY;
	{
		FDB(2, "CheckInputImageDefinitions - Enter\n");

		for (int i = 0; i < NUM_INPUT_IMAGES; i++)
		{
			FDB(2, "CheckInputImageDefinitions - Image %d\n", i);
#if defined(SVO_INTERFACE_001_ONLY)
			if (pInputImageDefinitions[i].eImageFormat != SVImageFormatMono8)
			{
				return ERRORCODE_IMAGEFORMAT_INVALID;
			}
#else
			switch (pInputImageDefinitions[i].eImageFormat)
			{
			case SVImageFormatMono8:
				FDB(2, "CheckInputImageDefinitions - Case Mono8\n");
				if (AllowedInputImageTypes[i] != AllowedImageType::c_GrayOrColor)
				{
					if (AllowedInputImageTypes[i] != AllowedImageType::c_Gray)
					{
						return ERRORCODE_IMAGEFORMAT_INVALID;
					}
				}
				break;
			case SVImageFormatBGR888X:
				FDB(2, "CheckInputImageDefinitions - Case SVImageFormatBGR888X\n");
				if (AllowedInputImageTypes[i] != AllowedImageType::c_GrayOrColor)
				{
					if (AllowedInputImageTypes[i] != AllowedImageType::c_Color)
					{
						return ERRORCODE_IMAGEFORMAT_INVALID;
					}
				}
				break;
			case SVImageFormatUnknown:
			default:
				FDBERROR("UNKNOWN IMAGE FORMAT!");
				return ERRORCODE_IMAGEFORMAT_INVALID;
			}
#endif
		}
		FDB(2, "CheckInputImageDefinitions - Exit OK\n");
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);

#endif
	return hr;
}

#ifdef USE_INPUT_VALUES
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
#ifdef SVO_INTERFACE_001_ONLY
		check_interface_1_compatibility = true;
#endif
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
#endif

#ifdef USE_RESULT_VALUES
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
#endif



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

#if defined(SVO_INTERFACE_820_OR_HIGHER)
		if (g_svo_interface_version < 820)
		{
			strcat_s(strTemp, " This Dll needs SVO Interface 820 or higher");
			hr = S_FALSE;
		}
		else
		{
			strcat_s(strTemp, " Interface OK");
		}
#endif

#if defined(SVO_INTERFACE_1000_OR_HIGHER)
		if (g_svo_interface_version == 1)
		{
			strcat_s(strTemp, " This Dll needs SVO Interface 1000 or higher");
			hr = S_FALSE;
		}
		if (g_svo_interface_version == 820)
		{
			strcat_s(strTemp, " This Dll needs SVO Interface 1000 or higher");
			hr = S_FALSE;
		}
		else
		{
			strcat_s(strTemp, " Interface OK");
		}
#endif



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

#ifdef USE_INPUT_IMAGES
		if (lImageArraySize != NUM_INPUT_IMAGES)
		{
			FDBERROR("SVInitializeRun Failed Input sizes\n");
			return S_FALSE;
		}
#else
		if (lImageArraySize != 0)
		{
			FDBERROR("SVInitializeRun Failed Input sizes\n");
			return S_FALSE;
		}
#endif

#ifdef USE_INPUT_VALUES
		if (lValueArraySize != NUM_INPUT_VALUES)
		{
			FDBERROR("SVInitializeRun Failed Input sizes\n");
			return S_FALSE;
		}
#else
		if (lValueArraySize != 0)
		{
			FDBERROR("SVInitializeRun Failed Input sizes\n");
			return S_FALSE;
		}
#endif

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

#ifdef USE_INPUT_VALUES
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
#endif

#ifdef USE_INPUT_IMAGES
		using ImageDefinitionArray = const std::array<ImageDefinitionStruct, NUM_INPUT_IMAGES>;
		using ImageDefinitionArrayEx = const std::array<ImageDefinitionStructEx, NUM_INPUT_IMAGES>;

		ImageDefinitionStructEx* pIMageDefinitionEx = new ImageDefinitionStructEx[NUM_INPUT_IMAGES];

		for (int i = 0; i < NUM_INPUT_IMAGES; i++)
		{
			pIMageDefinitionEx[i] = pInputImageDefinitions[i];
		}
#ifdef USE_INPUT_VALUES
		hr = pDllTool->initRun(*(ImageDefinitionArrayEx*)pIMageDefinitionEx, *(VariantArray*)pavInputValues);
#else
		hr = pDllTool->initRun(*(ImageDefinitionArrayEx*)pIMageDefinitionEx);
#endif
		delete[] pIMageDefinitionEx;
#else
#ifdef USE_INPUT_VALUES
		hr = pDllTool->initRun(*(VariantArray*)pavInputValues);
#else
		hr = pDllTool->initRun();
#endif
#endif

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
#ifdef USE_INPUT_VALUES
#if !defined(SVO_INTERFACE_001_ONLY) 
TOOLDLL_API HRESULT __stdcall SVGetInputValueDefinitionsEx(long* plArraySize,
	InputValueDefinitionStructEx** ppaStructs)
{
	HRESULT hr = S_OK;
	SVTRY;
	{

		FDB(0, "===== SVGetInputValueDefinitionsEx - Enter - NumInputValues %d\n", NUM_INPUT_VALUES);
		typedef std::basic_string<WCHAR> wstring;

		if (NUM_INPUT_VALUES > 0)
			*ppaStructs = new InputValueDefinitionStructEx[NUM_INPUT_VALUES];
		else
			*ppaStructs = NULL;

		//### check alloc failed
		if (ppaStructs == NULL)
		{
			FDBERROR("SVGetInputValueDefinitionsEx - allocation InputValueDefinitionStruct failed\n");
			return S_FALSE;
		}

		for (long i = 0; i < NUM_INPUT_VALUES; i++)
		{
			::VariantInit(&((*ppaStructs)[i].vDefaultValue));
		}

		*plArraySize = NUM_INPUT_VALUES;
		using ValueDefinitionArray = std::array<InputValueDefinitionStructEx, NUM_INPUT_VALUES>;

		CDllTool::getInputValuesDefinition(*(ValueDefinitionArray*)(*ppaStructs));
		hr = CheckInputValueDefinitions(*(ValueDefinitionArray*)(*ppaStructs));
		if (hr != S_OK)
		{
			return hr;
		}
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);

	FDB(0, "===== SVGetInputValueDefinitionsEx - Exit - hr=0x%X\n", hr);

	return hr;
}
#endif
#endif

#ifdef USE_INPUT_VALUES
#if !defined(SVO_INTERFACE_001_ONLY)
// cppcheck-suppress unusedFunction
TOOLDLL_API HRESULT __stdcall SVDestroyInputValueDefinitionStructuresEx(
	InputValueDefinitionStructEx* paStructs)
{
	FDB(0, "===== SVDestroyInputValueDefinitionStructuresEx - Enter\n");

	HRESULT hr = S_OK;
	SVTRY;
	{
		for (int i = 0; i < NUM_INPUT_VALUES; i++)
		{

			::VariantClear(&paStructs[i].vDefaultValue);
		}
		delete[] paStructs;
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);

	FDB(0, "===== SVDestroyInputValueDefinitionStructuresEx - Exit - hr=0x%X\n", hr);
	return hr;
}
#endif
#endif


// cppcheck-suppress unusedFunction SVDestroyInputImageInformation
#if !defined(SVO_INTERFACE_001_ONLY)
TOOLDLL_API HRESULT __stdcall SVDestroyInputImageInformation(InputImageInformationStruct* pInputImageInformation)
{

	HRESULT hr{ S_OK };
	SVTRY
		FDB(0, "===== SVDestroyInputImageInformation\n");
	delete[] pInputImageInformation;
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);
	return hr;
}
#endif


// cppcheck-suppress unusedFunction
#if !defined(SVO_INTERFACE_001_ONLY)
TOOLDLL_API HRESULT __stdcall SVDestroyResultValueDefinitionStructuresEx(ResultValueDefinitionStructEx* paStructs)
{
	FDB(0, "===== SVDestroyResultValueDefinitionStructuresEx\n");
	delete[] paStructs;
	return S_OK;
}
#endif

// cppcheck-suppress unusedFunction
TOOLDLL_API HRESULT __stdcall SVSetInputValues(GUID guidTool, long lArraySize,
	VARIANT* paInputValues)
{
	HRESULT hr = S_FALSE;
	SVTRY;
	{
#ifdef USE_INPUT_VALUES
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
#else
		if (lArraySize == 0)
		{
			hr = S_OK;
		}
		else
		{
			FDB(1, "SVSetInputValues - Number of parameters are different!\n");
		}
#endif
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
#ifdef USE_RESULT_VALUES
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
#else
		FDB(0, "===== SVGetResultValues - Enter - lArraySize=%d = NUM_RESULT_VALUES=%d\n", lArraySize, 0);
		return S_OK;
#endif
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
#ifdef USE_INPUT_VALUES
		hr = CDllTool::validateInputValue(lParameterNumber, vParameterValue);
#else
		hr = S_OK;
#endif
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);
	FDB(0, "===== SVValidateValueParameter - Exit - hr=0x%X\n", hr);
	return hr;
}


#ifdef USE_RESULT_VALUES
#if !defined(SVO_INTERFACE_001_ONLY) 
// cppcheck-suppress unusedFunction
TOOLDLL_API HRESULT __stdcall SVGetResultValueDefinitionsEx(long* plArraySize, ResultValueDefinitionStructEx** ppaResultValues)
{
	FDB(0, "===== SVGetResultValueDefinitionsEx - Enter\n");
	HRESULT hr = S_OK;
	SVTRY;
	{
		*plArraySize = NUM_RESULT_VALUES;
		*ppaResultValues = new ResultValueDefinitionStructEx[NUM_RESULT_VALUES];
		if (nullptr == *ppaResultValues)
		{
			FDB(0, "===== SVGetResultValueDefinitionsEx - Exit - hr=0x%X\n", S_FALSE);
			return S_FALSE;
		}
		using ValueDefinitionArray = std::array<ResultValueDefinitionStructEx, NUM_RESULT_VALUES>;
		CDllTool::getResultValueDefinition(*(ValueDefinitionArray*)(*ppaResultValues));
		if (hr == S_OK)
		{
			hr = CheckResultValueDefinitions(*(ValueDefinitionArray*)(*ppaResultValues));
			FDB(0, "===== SVGetResultValueDefinitionsEx - Exit - hr=0x%X\n", hr);
			return hr;
		}
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);
	FDB(0, "===== SVGetResultValueDefinitionsEx - Exit - hr=0x%X\n", hr);
	return hr;
}
#endif
#endif

#ifdef USE_RESULT_TABLES
#if !defined(SVO_INTERFACE_001_ONLY)
// cppcheck-suppress unusedFunction
TOOLDLL_API HRESULT __stdcall GetResultTableDefinitionsEx(long* pArraySize, ResultTableDefinitionStructEx** ppaResultTableDefs)
{
	FDB(0, "===== GetResultTableDefinitionsEx - Enter\n");
	HRESULT hr = S_OK;
	SVTRY;
	{
		if (NUM_RESULT_TABLES > 0)
		{
			*ppaResultTableDefs = new ResultTableDefinitionStructEx[NUM_RESULT_TABLES];
		}
		else // e.g. NUM_RESULT_TABLES == 0
		{
			*ppaResultTableDefs = NULL;
			FDB(0, "===== GetResultTableDefinitionsEx - Exit - hr=0x%X\n", S_OK);
			return S_OK;
		}

		if (*ppaResultTableDefs == NULL)
		{
			FDB(0, "===== GetResultTableDefinitionsEx - Exit - hr=0x%X\n", S_FALSE);
			return S_FALSE;
		}
		*pArraySize = NUM_RESULT_TABLES;
		using ResultTableDefinitionArray = std::array<ResultTableDefinitionStructEx, NUM_RESULT_TABLES>;
		CDllTool::getResultTableDefinition(*(ResultTableDefinitionArray*)(*ppaResultTableDefs));

		FDB(0, "===== GetResultTableDefinitionsEx - Exit - hr=0x%X\n", hr);
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);
	return hr;
}
#endif
#endif

#ifdef USE_RESULT_TABLES
#if !defined(SVO_INTERFACE_001_ONLY) 
TOOLDLL_API HRESULT __stdcall DestroyResultTableDefinitionStructuresEx(ResultTableDefinitionStructEx* ppaResultTableDefs)
{
	FDB(0, "===== DestroyResultTableDefinitionStructuresEx\n");
	delete[] ppaResultTableDefs;
	return S_OK;
}
#endif
#endif

#ifdef USE_RESULT_TABLES
#if !defined(SVO_INTERFACE_001_ONLY)
TOOLDLL_API HRESULT __stdcall GetResultTables(GUID guidTool, long lArraySize, VARIANT* paResultTables)
{
	FDB(0, "===== GetResultTables - Enter\n");
	HRESULT hr = S_FALSE;
	SVTRY;
	{
		if (NUM_RESULT_TABLES == 0)
		{
			hr = S_OK;
		}
		else if (lArraySize == NUM_RESULT_TABLES)
		{
			CDllTool* pTool = NULL;
			hr = LookupTool(guidTool, pTool);

			if (hr == S_OK)
			{	// found tool
				using VariantArray = std::array<VARIANT, NUM_RESULT_TABLES>;
				hr = pTool->getResultTables(*(VariantArray*)(paResultTables));
			}
		}
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);
	FDB(0, "===== GetResultTables - Exit - hr=0x%X\n", hr);
	return hr;
}
#endif
#endif

// cppcheck-suppress unusedFunction
#ifdef USE_RESULT_TABLES
#if !defined(SVO_INTERFACE_001_ONLY) 
TOOLDLL_API HRESULT __stdcall GetResultTablesMaxRowSize(GUID guidTool, long lArraySize, int pRowSizes[])
{
	FDB(0, "===== GetResultTablesMaxRowSize - Enter\n");
	HRESULT hr = S_FALSE;
	SVTRY;
	{
		if (lArraySize == NUM_RESULT_TABLES)
		{
			CDllTool* pTool = NULL;
			hr = LookupTool(guidTool, pTool);

			if (hr == S_OK)
			{
				hr = pTool->getResultTablesMaxRowSizes(*(std::array<int, NUM_RESULT_TABLES>*)pRowSizes);
			}
		}
		else
		{
			FDB(1, "lArraySize does not match NUM_RESULT_TABLES\n");
			hr = S_FALSE;
		}
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);
	FDB(0, "===== GetResultTablesMaxRowSize - Exit - hr=0x%X\n", hr);
	return hr;
}
#endif
#endif

#ifdef USE_RESULT_ARRAYS
#if !defined(SVO_INTERFACE_001_ONLY) 
// cppcheck-suppress unusedFunction
TOOLDLL_API HRESULT __stdcall GetResultValuesMaxArraySize(GUID guidTool, long size, int arraysize[])
{
	FDB(0, "===== GetResultValuesMaxArraySize - Enter \n");
	FDB(1, "Size: %d\n", size);
	HRESULT hr = S_FALSE;
	SVTRY;
	{
#ifdef USE_RESULT_VALUES
		if (size == NUM_RESULT_VALUES)
		{
			CDllTool* pTool = NULL;
			hr = LookupTool(guidTool, pTool);

			if (hr == S_OK)
			{
				hr = pTool->getResultValuesMaxArraySizes(*(std::array<int, NUM_RESULT_VALUES>*) arraysize);
			}
		}
		else
		{
			FDB(1, "The provided size does not match NUM_RESULT_VALUES\n");
		}
#else
		if (size == 0)
		{
			hr = S_OK;
		}
#endif
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);
	FDB(0, "===== GetResultValuesMaxArraySize - Exit - hr=0x%X\n", hr);
	return hr;
}
#endif
#endif


TOOLDLL_API HRESULT __stdcall SVGetInputValueDefinitions(long* plArraySize,
	InputValueDefinitionStruct** ppInputValueDefinition)
{
	HRESULT hr = S_OK;
	SVTRY;
	{
#ifdef USE_INPUT_VALUES
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
#else
		FDB(0, "===== SVGetInputValueDefinitions - Enter - NumInputValues %d\n", 0);
		*ppInputValueDefinition = NULL;
		*plArraySize = 0;
#endif
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
#ifdef USE_INPUT_VALUES
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
#endif
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
#ifdef USE_RESULT_VALUES
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
#else
		FDB(0, "===== SVGetResultValueDefinitions - Enter - NumResults %d\n", 0);
		*plArraySize = 0;
		*ppResultValues = nullptr;
#endif
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
#ifdef USE_RESULT_VALUES
	for (int i = 0; i < NUM_RESULT_VALUES; i++)
	{
		::SysFreeString(paStructs[i].bstrDisplayName);
	}
	delete[] paStructs;
#endif

	FDB(0, "===== SVDestroyResultValueDefinitionStructures - Exit - hr=0x%X\n", hr);
	return hr;
}

#if !defined(SVO_INTERFACE_001_ONLY)
TOOLDLL_API HRESULT __stdcall SVGetInputImageInformation(long* plArraySize,
	InputImageInformationStruct** ppInputImageInformation)
{
	HRESULT hr = S_OK;
	SVTRY;
	{
#ifdef USE_INPUT_IMAGES
		FDB(0, "===== SVGetInputImageInformation - Enter - NumInputImages %d, %d\n", NUM_INPUT_IMAGES, *plArraySize);


		typedef std::basic_string<WCHAR> wstring;

		*plArraySize = NUM_INPUT_IMAGES;
		if (NUM_INPUT_IMAGES > 0)
		{
			*ppInputImageInformation = new InputImageInformationStruct[NUM_INPUT_IMAGES];
		}
		else
		{
			*ppInputImageInformation = nullptr;
			return S_OK; // no images is ok
		}

		//### check alloc failed
		if (ppInputImageInformation == nullptr)
		{
			FDB(1, "SVGetInputImageInformation - allocation InputValueDefinitionStruct failed\n");
			return S_FALSE;
		}

		using ImageInformationArray = std::array<InputImageInformationStruct, NUM_INPUT_IMAGES>;
		CDllTool::getInputImageInformation(*(ImageInformationArray*)(*ppInputImageInformation));

		for (int i = 0; i < NUM_INPUT_IMAGES; ++i)
		{
			switch ((*ppInputImageInformation)[i].AllowedImageTypes)
			{
			case AllowedImageType::c_Gray:
			case AllowedImageType::c_GrayOrColor:
			case AllowedImageType::c_Color:
				break;
			default:
				FDBERROR("UNDEFINED Image Format!\n");
				hr = S_FALSE;
				break;
			}
		}
#else
		FDB(0, "===== SVGetInputImageInformation - Enter - NumInputImages %d, %d\n", 0, *plArraySize);
		*plArraySize = 0;
		*ppInputImageInformation = nullptr;
		FDB(0, "===== SVGetInputImageInformation - Exit - hr=0x%X\n", S_OK);
		return S_OK; // no images is ok
#endif
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);
	FDB(0, "===== SVGetInputImageInformation - Exit - hr=0x%X\n", hr);
	return hr;
}
#endif
#ifdef USE_RESULT_TABLES
#if !defined(SVO_INTERFACE_001_ONLY)
TOOLDLL_API HRESULT __stdcall GetResultTableDefinitions(long* pArraySize, ResultTableDefinitionStruct** ppaResultTableDefs)
{
	FDB(0, "===== GetResultTableDefinitions - Enter (NUM_RESULT_TABLES = %d)\n", NUM_RESULT_TABLES);
	HRESULT hr = S_OK;
	SVTRY;
	{
		if (NUM_RESULT_TABLES > 0)
		{
			*ppaResultTableDefs = new ResultTableDefinitionStruct[NUM_RESULT_TABLES];
		}
		else
		{
			*ppaResultTableDefs = NULL;
		}

		if (ppaResultTableDefs == NULL)
		{
			FDB(0, "===== GetResultTableDefinitions - Exit - hr=0x%X\n", S_FALSE);
			return S_FALSE;
		}

		*pArraySize = NUM_RESULT_TABLES;
		std::array<ResultTableDefinitionStructEx, NUM_RESULT_TABLES> ResultsEx;
		CDllTool::getResultTableDefinition(ResultsEx);

		for (int index = 0; index < NUM_RESULT_TABLES; index++)
		{
			(*ppaResultTableDefs)[index].bstrDisplayName = ResultsEx[index].HelpText.copy();
			(*ppaResultTableDefs)[index].lVT = ResultsEx[index].vt;
			(*ppaResultTableDefs)[index].ColoumnCount = ResultsEx[index].ColoumnCount;
			(*ppaResultTableDefs)[index].RowCount = ResultsEx[index].RowCount;
			(*ppaResultTableDefs)[index].ColumnNames.Attach(ResultsEx[index].ColumnNames.Detach());
			//smart array of bstr with names
		}
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);
	FDB(0, "===== GetResultTableDefinitions - Exit - hr=0x%X\n", hr);
	return hr;
}
#endif
#endif
#ifdef USE_RESULT_TABLES
#if !defined(SVO_INTERFACE_001_ONLY)
TOOLDLL_API HRESULT __stdcall DestroyResultTableDefinitionStructures(ResultTableDefinitionStruct* ppaResultTableDefs)
{
	FDB(0, "===== DestroyResultTableDefinitionStructures\n");
	delete[] ppaResultTableDefs;
	return S_OK;
}
#endif
#endif


TOOLDLL_API HRESULT __stdcall SVGetNumberOfInputImages(long* plNumberOfInputImages)
{
#ifdef USE_INPUT_IMAGES
	FDB(0, "===== SVGetNumberOfInputImages - NUM_INPUT_IMAGES=%d\n", NUM_INPUT_IMAGES);
	if (NUM_INPUT_IMAGES > 4)
	{
		*plNumberOfInputImages = 0;
		return S_FALSE;
	}
	*plNumberOfInputImages = NUM_INPUT_IMAGES;
#else
	*plNumberOfInputImages = 0;
#endif
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
#ifdef USE_INPUT_IMAGES
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
#else
		FDB(0, "===== SVSetMILInputImages - Exit - hr=0x%X\n", S_OK);
		return S_OK;
#endif
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
#ifdef USE_RESULT_IMAGES
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
#else
		FDB(0, "===== SVSetMILResultImages - Exit - hr=0x%X\n", S_OK);
		return S_OK;
#endif
	}
	SVCATCHALL(hr, ERRORCODE_USER_EXCEPTION);
	FDB(0, "===== SVSetMILResultImages - Exit - hr=0x%X\n", hr);
	return hr;
}
#endif

#if defined(NOMIL) 

TOOLDLL_API HRESULT __stdcall SVSetHBITMAPInputImages(GUID guidTool, HBITMAP* paHandles)
{
#if defined(USE_INPUT_IMAGES)
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
#else
	FDB(0, "===== SVSetHBITMAPInputImages\n");
	return S_OK;
#endif
}


// Only implement One of the set.. ResultImages
TOOLDLL_API HRESULT __stdcall SVGetHBITMAPResultImages(GUID guidTool, long lArraySize, HBITMAP* paHandles)
{
#ifdef USE_RESULT_IMAGES
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
#else
	FDB(0, "===== SVGetHBITMAPResultImages - lArraySize=%d = NUM_RESULT_IMAGES=%d\n", lArraySize, 0);
	return S_OK;
#endif
}
#endif // NOMIL

TOOLDLL_API HRESULT __stdcall SVGetResultImageDefinitions(GUID guidTool,
	long* plArraySize, ImageDefinitionStruct** ppImageDefinition)
{

	FDB(0, "===== SVGetResultImageDefinitions - Enter\n");
#ifdef USE_RESULT_IMAGES
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
#ifdef SVO_INTERFACE_001_ONLY
					FDBERROR("ImageFormat SVImageFormatBGR888X not supported!\n");
					FDB(0, "===== SVGetResultImageDefinitions - Exit - hr=0x%X\n", ERRORCODE_IMAGEFORMAT_INVALID);
					return ERRORCODE_IMAGEFORMAT_INVALID;
#endif
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
#else
	*plArraySize = 0;
	return S_OK;
#endif
}

TOOLDLL_API HRESULT __stdcall SVDestroyImageDefinitionStructure(ImageDefinitionStruct* paStructs)
{

	FDB(0, "===== SVDestroyImageDefinitionStructure - Enter\n");

	HRESULT hr = S_OK;
#ifdef USE_RESULT_IMAGES
	for (int i = 0; i < NUM_RESULT_IMAGES; i++)
	{
		::SysFreeString(paStructs[i].bstrDisplayName);
	}
#endif
	delete[] paStructs;

	FDB(0, "===== SVDestroyImageDefinitionStructure - Exit - hr=0x%X\n", hr);
	return hr;
}


