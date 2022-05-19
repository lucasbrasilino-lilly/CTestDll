//*****************************************************************************
/// \copyright (c) 2020,2020 by Seidenader Maschinenbau GmbH. 
/// All Rights Reserved
/// \file CDllTool.cpp
/// \brief (see header file of the same name)
//*****************************************************************************

#include "stdafx.h"
#include "CDllTool.h"
#include "DebugUtilities.h"
#include "TableAndArrayHelper.h"


// Implement CDllTool Class
CDllTool::CDllTool()
{
	FDB(1, "CDllTool()\n");
	::ZeroMemory(&m_GUID, sizeof(GUID));
	//TODO: add initializion of member parameters if necessary.
}

CDllTool::~CDllTool()
{
	FDB(1, "~CDllTool()\n");
	cleanData();
}

#pragma region public static function

#ifdef USE_INPUT_IMAGES
#if !defined(SVO_INTERFACE_001_ONLY)
void CDllTool::getInputImageInformation(std::array<InputImageInformationStruct, NUM_INPUT_IMAGES> &rInputImageInfoDef)
{
	//TODO: fill the required InputImageInformationStructs here, similar to the example provided. Delete the example code afterwards.
	for (int i = 0; i < NUM_INPUT_IMAGES; i++)
	{
		rInputImageInfoDef[i].AllowedImageTypes = AllowedInputImageTypes[i];
		std::wstring strName = L"Input Image " + std::to_wstring(i);
		rInputImageInfoDef[i].DisplayName = strName.c_str();
		rInputImageInfoDef[i].HelpText = "HelpText";
	}
}
#endif
#endif

#ifdef USE_INPUT_VALUES
void CDllTool::getInputValuesDefinition(std::array<InputValueDefinitionStructEx, NUM_INPUT_VALUES>  &rInputDef)
{
	FDB(1, "CDllTool::getInputValuesDefinition - Enter\n");
	//TODO: add the definition for all inputs, like the comment sample. Delete the comment sample afterwards.
#ifdef EXAMPLECODE
	rInputDef[InputValue_LONG].vt = VT_I4;
	rInputDef[InputValue_LONG].Name = "Value 1 Long";
	rInputDef[InputValue_LONG].HelpText = "Insert Long Value";
	rInputDef[InputValue_LONG].Group = "Common";
	rInputDef[InputValue_LONG].vDefaultValue.vt = VT_I4;
	rInputDef[InputValue_LONG].vDefaultValue.lVal = 4;

	rInputDef[InputValue_BOOL].vt = VT_BOOL;
	rInputDef[InputValue_BOOL].Name = "Value 1 Bool";
	rInputDef[InputValue_BOOL].HelpText = "Insert Bool Value";
	rInputDef[InputValue_BOOL].Group = "Common";
	rInputDef[InputValue_BOOL].vDefaultValue.vt = VT_BOOL;
	rInputDef[InputValue_BOOL].vDefaultValue.boolVal = VARIANT_TRUE;

	rInputDef[InputValue_DOUBLE].vt = VT_R8;
	rInputDef[InputValue_DOUBLE].Name = "Value 2 Double";
	rInputDef[InputValue_DOUBLE].HelpText = "Insert Double Value";
	rInputDef[InputValue_DOUBLE].Group = "Common";
	rInputDef[InputValue_DOUBLE].vDefaultValue.vt = VT_R8;
	rInputDef[InputValue_DOUBLE].vDefaultValue.dblVal = 2.5;

	rInputDef[InputValue_BSTR].vt = VT_BSTR;
	rInputDef[InputValue_BSTR].Name = "String Value";
	rInputDef[InputValue_BSTR].HelpText = "Insert String";
	rInputDef[InputValue_BSTR].Group = "Common";
	rInputDef[InputValue_BSTR].vDefaultValue = "Markus"; // more readable and correct because  variant_t::operator = exist for various differnt types _

#if !defined(SVO_INTERFACE_001_ONLY)
	rInputDef[InputValue_DOUBLE_ARRAY].vt = VT_ARRAY | VT_R8;
	rInputDef[InputValue_DOUBLE_ARRAY].Name = "Double Vector";
	rInputDef[InputValue_DOUBLE_ARRAY].HelpText = "Insert Double Vector";
	rInputDef[InputValue_DOUBLE_ARRAY].Group = "Array";
	TaAHelper::emptyArray_R8(&rInputDef[InputValue_DOUBLE_ARRAY].vDefaultValue, 15);

	rInputDef[InputValue_LONG_ARRAY].vt = VT_ARRAY | VT_I4;
	rInputDef[InputValue_LONG_ARRAY].Name = "Long Vector";
	rInputDef[InputValue_LONG_ARRAY].HelpText = "LONG Vektor eingeben";
	rInputDef[InputValue_LONG_ARRAY].Group = "Array";
	TaAHelper::emptyArray_I4(&rInputDef[InputValue_LONG_ARRAY].vDefaultValue, 12);

	rInputDef[InputValue_TABLE_ARRAY].vt = VT_ARRAY | VT_R8;
	rInputDef[InputValue_TABLE_ARRAY].Name = "Wert6";
	rInputDef[InputValue_TABLE_ARRAY].HelpText = "Table Object eingeben";
	rInputDef[InputValue_TABLE_ARRAY].Group = "Table";
	TaAHelper::emptyTable_R8(&rInputDef[InputValue_TABLE_ARRAY].vDefaultValue, 10, 2);

	rInputDef[InputValue_TABLE_NAMES].vt = VT_ARRAY | VT_BSTR;
	rInputDef[InputValue_TABLE_NAMES].Name = "ColumnNames";
	rInputDef[InputValue_TABLE_NAMES].HelpText = "BSTR Array für Namen";
	rInputDef[InputValue_TABLE_NAMES].Group = "Table";
	TaAHelper::emptyArray_BSTR(&rInputDef[InputValue_TABLE_NAMES].vDefaultValue, 10);

	rInputDef[InputValue_LONG_TABLE_SELECT].vt = VT_I4;
	rInputDef[InputValue_LONG_TABLE_SELECT].Name = "tableCol";
	rInputDef[InputValue_LONG_TABLE_SELECT].HelpText = "Long selected Column for table";
	rInputDef[InputValue_LONG_TABLE_SELECT].Group = "TABLE SELECT";
	rInputDef[InputValue_LONG_TABLE_SELECT].vDefaultValue.vt = VT_I4;
	rInputDef[InputValue_LONG_TABLE_SELECT].vDefaultValue.lVal = 0;

	rInputDef[InputValue_RGBVAL_ARRAY].vt = VT_ARRAY | VT_I4;
	rInputDef[InputValue_RGBVAL_ARRAY].Name = "RGB Value";
	rInputDef[InputValue_RGBVAL_ARRAY].HelpText = "3 Long Werte eingeben";
	rInputDef[InputValue_RGBVAL_ARRAY].Group = "Array2";
	TaAHelper::emptyArray_I4(&rInputDef[InputValue_RGBVAL_ARRAY].vDefaultValue, 3);
#endif 
#endif 
	FDB(1, "CDllTool::getInputValuesDefinition - Exit\n");
}
#endif

#ifdef USE_RESULT_VALUES
void CDllTool::getResultValueDefinition(std::array<ResultValueDefinitionStructEx, NUM_RESULT_VALUES>  &rResultDef)
{
	//TODO: Add the definition for all results, like the comment sample. Delete the comment sample afterwards.
#ifdef EXAMPLECODE
	rResultDef[ResultValue_LONG].vt = VT_I4;
	rResultDef[ResultValue_LONG].Name = "cpyFromIn1";
	rResultDef[ResultValue_LONG].HelpText = "copy from input1";
	rResultDef[ResultValue_LONG].Group = "Copy";

	rResultDef[ResultValue_BOOL].vt = VT_BOOL;
	rResultDef[ResultValue_BOOL].Name = "cpyFromIn2";
	rResultDef[ResultValue_BOOL].HelpText = "copy from input2";
	rResultDef[ResultValue_BOOL].Group = "Copy";

	rResultDef[ResultValue_DOUBLE].vt = VT_R8;
	rResultDef[ResultValue_DOUBLE].Name = "cpyFromIn3";
	rResultDef[ResultValue_DOUBLE].HelpText = "copy from input3";
	rResultDef[ResultValue_DOUBLE].Group = "Copy";

	rResultDef[ResultValue_BSTR].vt = VT_BSTR;
	rResultDef[ResultValue_BSTR].Name = "cpyFromIn4";
	rResultDef[ResultValue_BSTR].HelpText = "copy from input4";
	rResultDef[ResultValue_BSTR].Group = "Copy";

#if !defined(SVO_INTERFACE_001_ONLY)
	rResultDef[ResultValue_DOUBLE_ARRAY].vt = VT_ARRAY | VT_R8;
	rResultDef[ResultValue_DOUBLE_ARRAY].Name = "cpyFromIn5";
	rResultDef[ResultValue_DOUBLE_ARRAY].HelpText = "copy from input5";
	rResultDef[ResultValue_DOUBLE_ARRAY].Group = "Copy";

	rResultDef[ResultValue_LONG_ARRAY].vt = VT_ARRAY | VT_I4;
	rResultDef[ResultValue_LONG_ARRAY].Name = "cpyFromIn6";
	rResultDef[ResultValue_LONG_ARRAY].HelpText = "copy from input6";
	rResultDef[ResultValue_LONG_ARRAY].Group = "Copy";

	rResultDef[ResultValue_LONG_ROWCOUNT].vt = VT_I4;
	rResultDef[ResultValue_LONG_ROWCOUNT].Name = "RowCountIn_6_7";
	rResultDef[ResultValue_LONG_ROWCOUNT].HelpText = " Row Count input 6,7";
	rResultDef[ResultValue_LONG_ROWCOUNT].Group = "SpecCopy";

	rResultDef[RESULTVALUE_BSTR_ROWNAME].vt = VT_BSTR;
	rResultDef[RESULTVALUE_BSTR_ROWNAME].Name = "ColInput_6_7";
	rResultDef[RESULTVALUE_BSTR_ROWNAME].HelpText = "Columname input 6,7";
	rResultDef[RESULTVALUE_BSTR_ROWNAME].Group = "SpecCopy";

	rResultDef[RESULTVALUE_DOUBLE_ARRAY_ROW].vt = VT_ARRAY | VT_R8;
	rResultDef[RESULTVALUE_DOUBLE_ARRAY_ROW].Name = "Array_6_7";
	rResultDef[RESULTVALUE_DOUBLE_ARRAY_ROW].HelpText = "ARRAY input 6,7 ";
	rResultDef[RESULTVALUE_DOUBLE_ARRAY_ROW].Group = "SpecCopy";
#endif 
#endif
}
#endif

#ifdef USE_RESULT_TABLES
#if !defined(SVO_INTERFACE_001_ONLY)
void CDllTool::getResultTableDefinition(std::array<ResultTableDefinitionStructEx, NUM_RESULT_TABLES> &rResultTableDef)
{
	//TODO: Add the definition for all results, like the comment sample. Delete the comment sample afterwards.
#ifdef EXAMPLECODE
	_bstr_t ColumnNames[ColumnCountA] = { "eins", "zwei", "drei", "vier","fuenf","sechs" };
	CComSafeArray<BSTR> saStrColumnNames(ColumnCountA);
	for (int i = 0; i < ColumnCountA; i++)
	{
		saStrColumnNames.SetAt(i, ColumnNames[i]);
	}

	rResultTableDef[FirstResultTable].vt = VT_R8;
	rResultTableDef[FirstResultTable].Name = "First_table";
	rResultTableDef[FirstResultTable].HelpText = "Erstes Table Objekt";
	rResultTableDef[FirstResultTable].ColoumnCount = ColumnCountA;
	rResultTableDef[FirstResultTable].ColumnNames.vt = VT_ARRAY | VT_BSTR;
	rResultTableDef[FirstResultTable].ColumnNames.parray = saStrColumnNames.Detach();
	rResultTableDef[FirstResultTable].RowCount = RowCountA;
	rResultTableDef[FirstResultTable].Group = "TableGroup";

	_bstr_t ColumnNames2[3] = { "Red", "Green", "Blue" };
	CComSafeArray<BSTR> saStrColumnNames2(ColumnCount_RGB);
	for (int i = 0; i < ColumnCount_RGB; i++)
	{
		saStrColumnNames2.SetAt(i, ColumnNames2[i]);
	}

	rResultTableDef[ResultTable_ColImageRGB].vt = VT_R8;
	rResultTableDef[ResultTable_ColImageRGB].Name = "RGB_Col";
	rResultTableDef[ResultTable_ColImageRGB].HelpText = "2nd Table Objekt";
	rResultTableDef[ResultTable_ColImageRGB].ColoumnCount = ColumnCount_RGB;
	rResultTableDef[ResultTable_ColImageRGB].ColumnNames.vt = VT_ARRAY | VT_BSTR;
	rResultTableDef[ResultTable_ColImageRGB].ColumnNames.parray = saStrColumnNames2.Detach();
	rResultTableDef[ResultTable_ColImageRGB].RowCount = RowCount_RGB;
	rResultTableDef[ResultTable_ColImageRGB].Group = "TableGroup";
#endif
}
#endif
#endif

#ifdef USE_INPUT_VALUES
HRESULT CDllTool::validateInputValue(long lParameterNumber, const	VARIANT vParameterValue)
{
	HRESULT hr = S_OK;

	//TODO: ADD check if input values valid. Delete the sample variables provided here afterwards. 
	switch (lParameterNumber)
	{
#ifdef EXAMPLECODE
	case InputValue_LONG:
		if (vParameterValue.vt != VT_I4 || vParameterValue.lVal < c_I1_minValue || vParameterValue.lVal > c_I1_maxValue)
		{
			hr = ERRORCODE_INPUTVALUE_LONG_INVALID;
		}
		break;
	case InputValue_BOOL:
		if (vParameterValue.vt != VT_BOOL)
		{
			hr = ERRORCODE_INPUTVALUE_BOOL_INVALID;
		}
		break;
	case InputValue_DOUBLE:
		if (vParameterValue.vt != VT_R8)
		{
			hr = ERRORCODE_INPUTVALUE_TYPE_INVALID;
		}
		break;
	case InputValue_BSTR:
		if (vParameterValue.vt != VT_BSTR)
		{
			hr = ERRORCODE_INPUTVALUE_TYPE_INVALID;
		}
		break;
#if !defined(SVO_INTERFACE_001_ONLY)
	case InputValue_DOUBLE_ARRAY:
	{
		if (vParameterValue.vt != (VT_R8 | VT_ARRAY))
		{
			hr = ERRORCODE_INPUTVALUE_DOUBLE_ARRAY_WRONGTYPE;
		}
		break;
	}
	case  InputValue_LONG_ARRAY:
	{
		if (vParameterValue.vt != (VT_ARRAY | VT_I4))
		{
			hr = ERRORCODE_INPUTVALUE_INT_ARRAY_WRONGTYPE;
		}
		break;
	}
	case InputValue_RGBVAL_ARRAY:
		if (vParameterValue.vt != (VT_ARRAY | VT_I4))
		{
			hr = ERRORCODE_INPUTVALUE_INT_ARRAY_WRONGTYPE;
		}
		else if (vParameterValue.parray->rgsabound[0].cElements > 3)
		{
			hr = ERRORCODE_INPUTVALUE_ARRAY_TO_BIG;
		}
		else if (vParameterValue.parray->rgsabound[0].cElements < 3)
		{
			hr = ERRORCODE_INPUTVALUE_ARRAY_TO_SMALL;
		}
		break;
#endif
#endif
	default:
		hr = S_OK;
		break;
	}


	if (hr != S_OK)
	{
		FDB(1, "validateInputValue num %d NOT OK \n", lParameterNumber);
	}
	else
	{
		FDB(1, "validateInputValue num %d OK\n", lParameterNumber);
	}

	return hr;
}
#endif

_bstr_t CDllTool::getErrorMessageString(HRESULT errorcode)
{
	FDB(1, "getErrorMessageString - Enter errorcode=0x%X\n");
	const int string_length = 100;
	wchar_t tmp_string[string_length];

	switch (errorcode)
	{
		// ---- Common Errorcodes Start
	case ERRORCODE_IMAGEFORMAT_INVALID:
		swprintf(tmp_string, string_length, L"Image format invalid!");
		break;
	case ERRORCODE_INPUTVALUE_DEFAULTTYPE_INVALID:
		swprintf(tmp_string, string_length, L"Input Value default type is invalid");
		break;
	case ERRORCODE_INITIALIZATION_INCOMPLETE:
		swprintf(tmp_string, string_length, L"Initialization Incomplete");
		break;
	case ERRORCODE_INPUTVALUE_TYPE_INVALID:
		swprintf(tmp_string, string_length, L"Type of Input Value is invalid");
		break;
	case ERRORCODE_RESULTVALUE_TYPE_INVALID:
		swprintf(tmp_string, string_length, L"Type of result type is invalid");
		break;
	case ERRORCODE_INPUTVALUE_ARRAY_TO_BIG:
		swprintf(tmp_string, string_length, L"The input array size is too big");
		break;
	case ERRORCODE_INPUTVALUE_ARRAY_TO_SMALL:
		swprintf(tmp_string, string_length, L"The input array size is too small");
		break;
		// ---- Common Errorcodes END

		//TODO: ADD case for your error codes. Delete comment sample afterwards. 
#ifdef EXAMPLECODE
	case ERRORCODE_INPUTVALUE_LONG_INVALID:
		swprintf(tmp_string, string_length, L"Invalid size of input value Long : Must between %d and %d"
			, c_I1_minValue, c_I1_maxValue);
		break;
	case ERRORCODE_INPUTVALUE_BOOL_INVALID:
		swprintf(tmp_string, string_length, L"Invalid type of input value bool");
		break;
	case ERRORCODE_INPUTVALUE_DOUBLE_ARRAY_WRONGTYPE:
		swprintf(tmp_string, string_length, L"Invalid Type not VT_ARRAY | VT_R8");
		break;
	case ERRORCODE_INPUTVALUE_DOUBLE_ARRAY_TOOSMALL:
		swprintf(tmp_string, string_length, L" VT_ARRAY | VT_DOUBLE  Array need at least 5 elements");
		break;
	case ERRORCODE_INPUTVALUE_INT_ARRAY_WRONGTYPE:
		swprintf(tmp_string, string_length, L"Invalid Type not VT_ARRAY | VT_I4");
		break;
#endif
	default:
		// if the errorcode is unknown, an empty string should be returned. 
		// In SVGetErrorMessageString() this implies a E_NOTIMP HRESULT send to SVObserver
		swprintf(tmp_string, string_length, L"");
		break;
	}

	FDB(1, "getErrorMessageString:  %S\n", tmp_string);
	return tmp_string;
}

#pragma endregion public static function

#pragma region public function
#ifdef USE_INPUT_IMAGES
#ifdef USE_INPUT_VALUES
HRESULT CDllTool::initRun(const std::array<ImageDefinitionStructEx, NUM_INPUT_IMAGES>  &rImageDefinitions, const std::array<VARIANT, NUM_INPUT_VALUES> &rInputValues)
#else
HRESULT CDllTool::initRun(const std::array<ImageDefinitionStructEx, NUM_INPUT_IMAGES>  &rImageDefinitions)
#endif

#else
#ifdef USE_INPUT_VALUES
HRESULT CDllTool::initRun(const std::array<VARIANT, NUM_INPUT_VALUES> &rInputValues)
#else
HRESULT CDllTool::initRun()
#endif
#endif
{
	HRESULT hr = S_OK;

	// Clean old Data
	cleanData();

	// Resize and Set Inputs
#ifdef USE_INPUT_IMAGES
	m_aInputImageIds.resize(NUM_INPUT_IMAGES);
#endif

#ifdef USE_INPUT_VALUES
	m_aInputValues.resize(NUM_INPUT_VALUES);
	setInputValues(rInputValues);
#endif

#ifdef USE_RESULT_IMAGES
	// Resize  ResultsImage
	m_aResultImageDefs.resize(NUM_RESULT_IMAGES);
	m_aResultImageIds.resize(NUM_RESULT_IMAGES);
#ifdef NOMIL
	m_aResultImageInfo.resize(NUM_RESULT_IMAGES);
#endif
#endif

#ifdef USE_RESULT_VALUES
	m_aResultValues.resize(NUM_RESULT_VALUES);
#endif
#ifdef USE_RESULT_ARRAYS
#if !defined(SVO_INTERFACE_001_ONLY)
	m_aMaxResultArraySizes.resize(NUM_RESULT_VALUES, 0); // resize with default value
#endif
#endif


#ifdef USE_RESULT_TABLES
#if !defined(SVO_INTERFACE_001_ONLY)
	m_ResultTables.resize(NUM_RESULT_TABLES);
	m_aMaxResultTablesRowSizes.resize(NUM_RESULT_TABLES, 0);
#endif
#endif

	//TODO: init normal ResultValues and delete the sample variables provided here afterwards.
#ifdef EXAMPLECODE
	m_aResultValues[ResultValue_LONG].vt = VT_I4;
	m_aResultValues[ResultValue_LONG].lVal = 2;

	m_aResultValues[ResultValue_BOOL].vt = VT_BOOL;
	m_aResultValues[ResultValue_BOOL].boolVal = false;

	m_aResultValues[ResultValue_DOUBLE].vt = VT_R8;
	m_aResultValues[ResultValue_DOUBLE].dblVal = 1.5;

	m_aResultValues[ResultValue_BSTR] = L"Result Value";

#if !defined(SVO_INTERFACE_001_ONLY)
	m_aResultValues[ResultValue_LONG_ROWCOUNT].vt = VT_I4;
	m_aResultValues[ResultValue_LONG_ROWCOUNT].lVal = 0;

	m_aResultValues[RESULTVALUE_BSTR_ROWNAME] = L"n.a.";
#endif
#endif


	// TODO: init ResultValues Arrays:
#ifdef EXAMPLECODE
#if !defined(SVO_INTERFACE_001_ONLY)
	TaAHelper::emptyArray_R8(&m_aResultValues[ResultValue_DOUBLE_ARRAY], 2);
	m_aMaxResultArraySizes[ResultValue_DOUBLE_ARRAY] = 250;

	TaAHelper::emptyArray_I4(&m_aResultValues[ResultValue_LONG_ARRAY], 2);
	m_aMaxResultArraySizes[ResultValue_LONG_ARRAY] = 250;

	TaAHelper::emptyArray_R8(&m_aResultValues[RESULTVALUE_DOUBLE_ARRAY_ROW], 50);
	m_aMaxResultArraySizes[RESULTVALUE_DOUBLE_ARRAY_ROW] = 250;

	//TODO: init Result Tables:
	m_aMaxResultTablesRowSizes[FirstResultTable] = 10;
	//Define the table Tool Input create 6 * 9 double Array 
	TaAHelper::emptyTable_R8(&m_ResultTables[FirstResultTable], ColumnCountA, RowCountA);

	FDB(1, "Init ResultTable_ColImageRGB, Rows: %d\n", rImageDefinitions[InputImage_Color].lHeight);
	m_aMaxResultTablesRowSizes[ResultTable_ColImageRGB] = rImageDefinitions[InputImage_Color].lHeight;
	TaAHelper::emptyTable_R8(&m_ResultTables[ResultTable_ColImageRGB], ColumnCount_RGB, rImageDefinitions[InputImage_Color].lHeight); // 
#endif
#endif

	//TODO: set definition for result images. In this sample the first input image has the same definition as result image
#ifdef EXAMPLECODE
	m_aResultImageDefs[ResultImage_Gray] = rImageDefinitions[InputImage_Gray];
	std::wstring strName = L"Result Image " + std::to_wstring(ResultImage_Gray);
	m_aResultImageDefs[ResultImage_Gray].bstrDisplayName = strName.c_str();

#if !defined(SVO_INTERFACE_001_ONLY)
	m_aResultImageDefs[ResultImage_Color] = rImageDefinitions[InputImage_Color];
	m_aResultImageDefs[ResultImage_Color].eImageFormat = SVImageFormatBGR888X;
	std::wstring strName2 = L"Result Image " + std::to_wstring(InputImage_Color);
	m_aResultImageDefs[ResultImage_Color].bstrDisplayName = strName2.c_str();
#endif

#ifdef NOMIL
	m_aResultImageInfo[ResultImage_Gray] = std::make_unique<CMyBitMapInfo>(m_aResultImageDefs[ResultImage_Gray]);
#if !defined(SVO_INTERFACE_001_ONLY)
	m_aResultImageInfo[ResultImage_Color] = std::make_unique<CMyBitMapInfo>(m_aResultImageDefs[ResultImage_Color]);
#endif
#endif
#endif

	//TODO: do stuff to init and load: Everything that is not needed to do in run.

	return hr;
}

HRESULT CDllTool::run()
{
	HRESULT hr = S_OK;

	//TODO: add your code and delete the sample variables provided here afterwards
#ifdef EXAMPLECODE
	hr = runEXAMPLE();
#endif

	return hr;
}

#pragma region public set function
void CDllTool::setGUID(const GUID guid)
{
	m_GUID = guid;
}

#ifdef USE_INPUT_VALUES
HRESULT CDllTool::setInputValues(const std::array<VARIANT, NUM_INPUT_VALUES> &rInputValues)
{
	FDB(1, "CDllTool::setInputValues - Enter\n");
	HRESULT hr = S_OK;

	//TODO: Delete InputValue_RGBVAL_ARRAY Example
#ifdef EXAMPLECODE
#if !defined(SVO_INTERFACE_001_ONLY)
	if (rInputValues[InputValue_RGBVAL_ARRAY].vt == (VT_ARRAY | VT_I4))
	{
		FDB(1, "InputValue_RGBVAL_ARRAY\n");
		SAFEARRAY *psa = rInputValues[InputValue_RGBVAL_ARRAY].parray;
		int elements = psa->rgsabound[0].cElements;
		FDB(1, "InputValue_RGBVAL_ARRAY Size %d\n", elements);

		LONG *pData;
		SafeArrayAccessData(rInputValues[InputValue_RGBVAL_ARRAY].parray, (void**)&pData);

		for (int i = 0; i < elements; i++)
		{
			FDB(1, "InputValue_RGBVAL_ARRAY Element %d is %d\n", i, pData[i]);
		}
		SafeArrayUnaccessData(rInputValues[InputValue_RGBVAL_ARRAY].parray);
	}
#endif
#endif

	for (int i = 0; i < NUM_INPUT_VALUES; i++)
	{
		m_aInputValues[i] = &rInputValues[i];
	}
	FDB(1, "CDllTool::setInputValues - EXIT\n");
	return S_OK;
}
#endif

#if defined MIL9 || defined MIL10
#ifdef USE_INPUT_IMAGES
HRESULT CDllTool::setMILInputImages(long* paMILhandles)
{
	for (int i = 0; i < NUM_INPUT_IMAGES; i++)
	{
		m_aInputImageIds[i] = static_cast<MIL_ID> (paMILhandles[i]);
	}
	return S_OK;
}
#endif

#ifdef USE_RESULT_IMAGES
HRESULT CDllTool::setMILResultImages(long* paMILhandles)
{
	FDB(1, "CDllTool::setMILResultImages\n");
	for (int i = 0; i < NUM_RESULT_IMAGES; i++)
	{
		m_aResultImageIds[i] = static_cast<MIL_ID> (paMILhandles[i]);
	}
	return S_OK;
}
#endif
#endif

#ifdef USE_INPUT_IMAGES
#if defined(NOMIL)
HRESULT CDllTool::setHBITMAPInputImages(const std::array<HBITMAP, NUM_INPUT_IMAGES> &rHandles)
{
	for (int i = 0; i < NUM_INPUT_IMAGES; i++)
	{
		m_aInputImageIds[i] = rHandles[i];
	}
	return S_OK;
}
#endif
#endif

#pragma endregion set function

#pragma region public get function

#ifdef USE_RESULT_IMAGES
HRESULT CDllTool::getResultImageDefs(std::array<ImageDefinitionStructEx, NUM_RESULT_IMAGES> &rImageDefinitions) const
{
	HRESULT hr = S_OK;
	if (m_aResultImageDefs.size() == NUM_RESULT_IMAGES)
	{
		for (int i = 0; i < NUM_RESULT_IMAGES; i++)
		{
			rImageDefinitions[i].eImageFormat = m_aResultImageDefs[i].eImageFormat;
			rImageDefinitions[i].lWidth = m_aResultImageDefs[i].lWidth;
			rImageDefinitions[i].lHeight = m_aResultImageDefs[i].lHeight;
			rImageDefinitions[i].bstrDisplayName = m_aResultImageDefs[i].bstrDisplayName;
		}
	}
	else
	{
		hr = S_FALSE;
	}
	return hr;
}
#endif

#ifdef USE_RESULT_VALUES
HRESULT CDllTool::getResultValues(std::array<VARIANT, NUM_RESULT_VALUES> &rResultValues) const
{
	try
	{
		for (int i = 0; i < NUM_RESULT_VALUES; i++)
		{
#ifdef USE_RESULT_ARRAYS
#if !defined(SVO_INTERFACE_001_ONLY)
			long maxElements = TaAHelper::getMaxElements(&m_aResultValues[i]);
			if (maxElements != 0)
			{
				if (maxElements > m_aMaxResultArraySizes[i])
				{
					FDBERROR("getResultValues Array larger than specified MAX size (%d)\n", i);
					return E_FAIL;
				}
			}
#endif
#endif
			HRESULT hrCopy = ::VariantCopy(&rResultValues[i], &m_aResultValues[i]);
			if (hrCopy != S_OK)
			{
				FDBERROR("VariantCopy failed\n");
				return E_FAIL;
			}
		}
	}
	catch (...)
	{
		FDBERROR("getResultValues Failed\n");
		return E_FAIL;
	}

	return S_OK;
}
#endif
#ifdef USE_RESULT_TABLES
#if !defined(SVO_INTERFACE_001_ONLY)
HRESULT CDllTool::getResultTables(std::array<VARIANT, NUM_RESULT_TABLES> &rResultTables) const
{
	HRESULT hr = S_OK;
	for (int i = 0; i < NUM_RESULT_TABLES; i++)
	{
		hr = ::VariantCopy(&rResultTables[i], &m_ResultTables[i]);
		if (hr != S_OK)
		{
			return hr;
		}
	}
	return hr;
}
#endif
#endif

#ifdef USE_RESULT_TABLES
#if !defined(SVO_INTERFACE_001_ONLY)
HRESULT CDllTool::getResultTablesMaxRowSizes(std::array<int, NUM_RESULT_TABLES> &rRowSizes) const
{
	for (int i = 0; i < NUM_RESULT_TABLES; i++)
	{
		rRowSizes[i] = m_aMaxResultTablesRowSizes[i];
	}
	return S_OK;

}
#endif
#endif

#ifdef USE_RESULT_VALUES
#ifdef USE_RESULT_ARRAYS
#if !defined(SVO_INTERFACE_001_ONLY)
HRESULT  CDllTool::getResultValuesMaxArraySizes(std::array<int, NUM_RESULT_VALUES> &rArraySizes) const
{
	for (int i = 0; i < NUM_RESULT_VALUES; i++)
	{
		rArraySizes[i] = m_aMaxResultArraySizes[i];
	}
	return S_OK;
}
#endif
#endif
#endif

#ifdef NOMIL
#ifdef USE_RESULT_IMAGES
HRESULT CDllTool::getHBITMAPResultImages(std::array<HBITMAP, NUM_RESULT_IMAGES> &rHandles) const
{
	for (int i = 0; i < NUM_RESULT_IMAGES; i++)
	{
		if (nullptr != m_aResultImageInfo[i])
		{
			rHandles[i] = m_aResultImageInfo[i]->getBitmapHandle();
		}
		else
		{
			return E_FAIL;
		}
	}
	return S_OK;
}
#endif
#endif
#pragma endregion get function
#pragma endregion public function

#pragma region private function


void CDllTool::cleanData()
{
	FDB(1, "CDllTool::cleanData\n");
#ifdef USE_INPUT_VALUES
	for (size_t i = 0; i < m_aInputValues.size(); i++)
	{
		::VariantClear(&m_aInputValues[i]);
	}
	m_aInputValues.clear();
#endif

#ifdef USE_RESULT_VALUES
	FDB(1, "CDllTool::cleanData ResultValues\n");
	for (size_t i = 0; i < m_aResultValues.size(); i++)
	{
		::VariantClear(&m_aResultValues[i]);
	}
	m_aResultValues.clear();
#ifdef USE_RESULT_ARRAYS
#if !defined(SVO_INTERFACE_001_ONLY)
	m_aMaxResultArraySizes.clear();
#endif
#endif
#endif
#ifdef USE_RESULT_TABLES
#if !defined(SVO_INTERFACE_001_ONLY)
	FDB(1, "CDllTool::cleanData Tables\n");
	for (size_t i = 0; i < m_ResultTables.size(); i++)
	{
		::VariantClear(&m_ResultTables[i]);
	}
	m_aMaxResultTablesRowSizes.clear();
#endif
#endif

#ifdef USE_INPUT_IMAGES
	m_aInputImageIds.clear();
#endif

#ifdef USE_RESULT_IMAGES
	m_aResultImageIds.clear();
	m_aResultImageDefs.clear();
#ifdef NOMIL
	m_aResultImageInfo.clear();
#endif
#endif

	FDB(1, "CDllTool::cleanData exit\n");
	//TODO: add your code to clean data if necessary.
}

#if defined(NOMIL)  
#if defined(USE_INPUT_IMAGES) || defined(USE_RESULT_IMAGES)
HRESULT CDllTool::copyHBitMap(HBITMAP l_hBitMapSource, HBITMAP l_hBitMapDestination)
{
	FDB(1, "copyHBitMap\n");
	DIBSECTION dibSource;
	DIBSECTION dibDestination;

	//set result image1			
	if (::GetObject(l_hBitMapSource, sizeof(dibSource), &dibSource) != 0 &&
		::GetObject(l_hBitMapDestination, sizeof(dibDestination), &dibDestination) != 0)
	{
		memcpy(dibDestination.dsBm.bmBits, dibSource.dsBm.bmBits, dibDestination.dsBmih.biSizeImage);
		FDB(1, "copyHBitMap Passed\n");
		return S_OK;
	}
	else
	{
		FDBERROR("copyHBitMap Failed\n");
		return S_FALSE;
	}
}
#endif
#endif

#ifdef EXAMPLECODE
HRESULT CDllTool::runEXAMPLE()
{
	FDB(1, "runEXAMPLE - Begin\n");

	copyDataEXAMPLE();

#if !defined(SVO_INTERFACE_001_ONLY)
	// Debug Print RGB Value
	FDB(1, "DEBUGPrintTableOrArray RGB Value\n");
	TaAHelper::DEBUGPrintTableOrArray(&m_aInputValues[InputValue_RGBVAL_ARRAY]);

	manipulateArraysAndTablesEXAMPLE();
#endif

	copyImageEXAMPLE();

	FDB(1, "runEXAMPLE - End\n");
	return S_OK;
}
#endif

#ifdef EXAMPLECODE
void CDllTool::copyDataEXAMPLE()
{
	// Copy input long to result long
	long inputLong = m_aInputValues[InputValue_LONG];
	m_aResultValues[ResultValue_LONG].lVal = inputLong;

	// Copy input bool to result bool
	VARIANT_BOOL inputbool = m_aInputValues[InputValue_BOOL];
	m_aResultValues[ResultValue_BOOL].boolVal = inputbool;

	// Copy input double to result double
	double inputdouble = m_aInputValues[InputValue_DOUBLE];
	m_aResultValues[ResultValue_DOUBLE].dblVal = inputdouble;

	// Copy input BSTR to result BSTR
	_bstr_t inputstring = m_aInputValues[InputValue_BSTR];
	std::wstring output = L"Input: ";
	output += (wchar_t*)inputstring;
	m_aResultValues[ResultValue_BSTR] = output.c_str();
}
#endif

#ifdef EXAMPLECODE
#if !defined(SVO_INTERFACE_001_ONLY)
void CDllTool::manipulateArraysAndTablesEXAMPLE()
{
	// copy input double array to result double array
	TaAHelper::clearArrayOrTable(&m_aResultValues[ResultValue_DOUBLE_ARRAY]);
	TaAHelper::CopyArrayToArray(&m_aInputValues[InputValue_DOUBLE_ARRAY], &m_aResultValues[ResultValue_DOUBLE_ARRAY]);

	// copy input LONG array to result LONG array
	TaAHelper::clearArrayOrTable(&m_aResultValues[ResultValue_LONG_ARRAY]);
	TaAHelper::CopyArrayToArray(&m_aInputValues[InputValue_LONG_ARRAY], &m_aResultValues[ResultValue_LONG_ARRAY]);

	// Print a Table in Debug Mode
	TaAHelper::DEBUGPrintTableOrArray(&m_aInputValues[InputValue_TABLE_ARRAY]);

	// access input table, copy selected column to output array
	long select = m_aInputValues[InputValue_LONG_TABLE_SELECT].lVal;
	if (select > TaAHelper::getTableColCount(&m_aInputValues[InputValue_TABLE_ARRAY]))
	{
		select = TaAHelper::getTableColCount(&m_aInputValues[InputValue_TABLE_ARRAY]) - 1;
	}

	FDB(1, "InputTable rows: %d \n", TaAHelper::getTableRowCount(&m_aInputValues[InputValue_TABLE_ARRAY]));
	FDB(1, "OutputTable Elements: %d \n", TaAHelper::getArrayElementCount(&m_aResultValues[RESULTVALUE_DOUBLE_ARRAY_ROW]));
	if (TaAHelper::getArrayElementCount(&m_aResultValues[RESULTVALUE_DOUBLE_ARRAY_ROW]) !=
		TaAHelper::getTableRowCount(&m_aInputValues[InputValue_TABLE_ARRAY]))
	{
		FDB(1, "Reset Table for CopyTableColR8ToArrayR8\n");
		TaAHelper::clearArrayOrTable(&m_aResultValues[RESULTVALUE_DOUBLE_ARRAY_ROW]);
		TaAHelper::emptyArray_R8(&m_aResultValues[RESULTVALUE_DOUBLE_ARRAY_ROW], TaAHelper::getTableRowCount(&m_aInputValues[InputValue_TABLE_ARRAY]));
	}
	TaAHelper::CopyTableColR8ToArrayR8(&m_aInputValues[InputValue_TABLE_ARRAY], select, &m_aResultValues[RESULTVALUE_DOUBLE_ARRAY_ROW]);

	// Get the row count of a table
	long rowcount = TaAHelper::getTableRowCount(&m_aInputValues[InputValue_TABLE_ARRAY]);
	m_aResultValues[ResultValue_LONG_ROWCOUNT].lVal = rowcount;

	// Get the BSTR Column name of an input Table
	TaAHelper::DEBUGPrintTableOrArray(&m_aInputValues[InputValue_TABLE_NAMES]);
	assert(VT_ARRAY | VT_BSTR);
	CComSafeArray<BSTR> saInput_table_names((m_aInputValues[InputValue_TABLE_NAMES].parray));
	int dim = saInput_table_names.GetDimensions();
	assert(dim == 1);
	CComBSTR  name = saInput_table_names.GetAt(select);
	_bstr_t bstrname = name.m_str;
	FDB(1, "bstrname: %s\n", LPCSTR(bstrname));
	m_aResultValues[RESULTVALUE_BSTR_ROWNAME] = bstrname;

	// Copy the first x columns to the Result Table
	TaAHelper::clearArrayOrTable(&m_ResultTables[FirstResultTable]);
	TaAHelper::CopyTableToTable(&m_aInputValues[InputValue_TABLE_ARRAY], &m_ResultTables[FirstResultTable], ColumnCountA);
}
#endif
#endif

#ifdef EXAMPLECODE
void CDllTool::copyImageEXAMPLE()
{
	// Copy Input to Output images
#ifdef NOMIL
	copyHBitMap(m_aInputImageIds[InputImage_Gray], m_aResultImageInfo[ResultImage_Gray]->getBitmapHandle());
#if !defined(SVO_INTERFACE_001_ONLY)
	copyHBitMap(m_aInputImageIds[InputImage_Color], m_aResultImageInfo[ResultImage_Color]->getBitmapHandle());
	FDB(1, "Copy HBITMAP RGB Image 1st Column to Table\n");
	copyRGBImageColToTable(m_aInputImageIds[InputImage_Color], &m_ResultTables[ResultTable_ColImageRGB]);
#endif
#else
	MbufCopy(m_aInputImageIds[InputImage_Gray], m_aResultImageIds[ResultImage_Gray]);
#if !defined(SVO_INTERFACE_001_ONLY)
	MbufCopy(m_aInputImageIds[InputImage_Color], m_aResultImageIds[ResultImage_Color]);


	FDB(1, "Copy MIL RGB Image 1st Colum to Table\n");
	copyRGBImageColToTable(m_aInputImageIds[InputImage_Color], &m_ResultTables[ResultTable_ColImageRGB]);
#endif
#endif // NOMIL
}
#endif
#pragma endregion private function

#ifdef EXAMPLECODE
#if defined(MIL9) || defined(MIL10)
void CDllTool::DEBUG_MILImageInfo(MIL_ID milImage)
{
#ifdef DEBUG
	FDB(1, "DEBUG_MILImageInfo ENTER\n");

	FDB(2, "M_SIZE_X: %d\n", MbufInquire(milImage, M_SIZE_X, M_NULL));
	FDB(2, "M_SIZE_Y: %d\n", MbufInquire(milImage, M_SIZE_Y, M_NULL));
	FDB(2, "M_SIZE_BAND: %d\n", MbufInquire(milImage, M_SIZE_BAND, M_NULL));
	FDB(2, "M_ALLOCATION_OVERSCAN_SIZE: %d\n", MbufInquire(milImage, M_ALLOCATION_OVERSCAN_SIZE, M_NULL));

	MIL_INT dataFormat = MbufInquire(milImage, M_DATA_FORMAT, M_NULL);
	if ((M_PACKED & dataFormat) == M_PACKED)
	{
		FDB(2, "M_DATA_FORMAT: M_PACKED\n");

		switch (dataFormat & M_INTERNAL_FORMAT)
		{
		case M_MONO1:        FDB(2, "Data Format: M_MONO1 \n");         break;
		case M_MONO8:        FDB(2, "Data Format: M_MONO8 \n");         break;
		case M_MONO16:       FDB(2, "Data Format: M_MONO16 \n");        break;
		case M_MONO32:       FDB(2, "Data Format: M_MONO32 \n");        break;
		case M_MONO64:       FDB(2, "Data Format: M_MONO64 \n");        break;
		case M_RGB15:        FDB(2, "Data Format: M_RGB15 \n");        break;
		case M_RGB16:        FDB(2, "Data Format: M_RGB16 \n");        break;
		case M_RGB24:        FDB(2, "Data Format: M_RGB24 \n");        break;
		case M_RGBX32:       FDB(2, "Data Format: M_RGBX32 \n");        break;
		case M_XRGB32:       FDB(2, "Data Format: M_XRGB32 \n");        break;
		case M_RGB48:        FDB(2, "Data Format: M_RGB48 \n");        break;
		case M_RGB96:        FDB(2, "Data Format: M_RGB96 \n");        break;
		case M_RGB192:       FDB(2, "Data Format: M_RGB192 \n");        break;
		case M_RGB3:         FDB(2, "Data Format: M_RGB3 \n");        break;
		case M_BGR15:        FDB(2, "Data Format: M_BGR15 \n");        break;
		case M_BGR16:        FDB(2, "Data Format: M_BGR16 \n");        break;
		case M_BGR24:        FDB(2, "Data Format: M_BGR24 \n");        break;
		case M_BGRX32:       FDB(2, "Data Format: M_BGRX32 \n");        break;
		case M_XBGR32:       FDB(2, "Data Format: M_XBGR32 \n");        break;
		case M_BGR30:        FDB(2, "Data Format: M_BGR30 \n");        break;
		case M_YUV9:         FDB(2, "Data Format: M_YUV9 \n");        break;
		case M_YUV12:        FDB(2, "Data Format: M_YUV12 \n");        break;
		case M_YUV16:        FDB(2, "Data Format: M_YUV16 \n");        break;
		case M_YUV24:        FDB(2, "Data Format: M_YUV24 \n");        break;
		case M_YUV16_YUYV:   FDB(2, "Data Format: M_YUV16_YUYV \n");        break;
		case M_YUV16_UYVY:   FDB(2, "Data Format: M_YUV16_UYVY \n");        break;
		case M_YUV12_1394:   FDB(2, "Data Format: M_YUV12_1394 \n");        break;
		default:             FDB(2, "Data Format: M_INVALID \n");		 break;
		}

	}
	if ((M_PLANAR & dataFormat) == M_PLANAR)
	{
		FDB(2, "M_DATA_FORMAT: M_PLANAR\n");
	}

	FDB(1, "Other way \n");
	MIL_BUFFER_INFO pBuf;
	MfuncInquire(milImage, M_BUFFER_INFO, &pBuf);

	MIL_INT ImgSizeBand = MfuncBufSizeBand(pBuf);
	MIL_INT ImgType = MfuncBufType(pBuf);
	MIL_INT ImgSign = ImgType & (M_SIGNED | M_UNSIGNED | M_FLOAT);
	MIL_INT ImgSizeBit = MfuncBufSizeBit(pBuf);
	MIL_INT64 ImgFormat = MfuncBufFormat(pBuf);

	if ((ImgFormat & M_PACKED) == M_PACKED)
	{
		FDB(2, "M_BUFFER_INFO: M_PACKED\n");
	}

	FDB(1, "DEBUG_MILImageInfo EXIT\n");
#endif // DEBUG
}
#endif
#endif // EXAMPLECODE

#ifdef EXAMPLECODE
#if !defined(SVO_INTERFACE_001_ONLY)
#ifdef NOMIL
void CDllTool::copyRGBImageColToTable(HBITMAP image, _variant_t* pVariantTable)
{
	FDB(1, "copyRGBImageColToTable\n");
	DIBSECTION dibSource;
	//set result image1			
	if (::GetObject(image, sizeof(dibSource), &dibSource) != 0)
	{
		BYTE *source = (BYTE*)dibSource.dsBm.bmBits;
		BYTE *lineptr;

		LONG pitchByte = dibSource.dsBm.bmWidthBytes;
		LONG imageHeight = dibSource.dsBm.bmHeight;

		/*
		// Debug print first 3 columns and row
		for (LONG y = 0; y < 3; y++)
		{
			lineptr = &(source)[pitchByte *  y];
			for (int x = 0; x < 3; x++)
			{
				FDB(1, "P(%03d,%03d): RGB (%03d, %03d, %03d) ", x, y, lineptr[2], lineptr[1], lineptr[0]);
				lineptr += 4;
			}
			FDB(0, "\n");
		}
		*/

		FDB(2, "Clear table\n");
		TaAHelper::emptyTable_R8(pVariantTable, ColumnCount_RGB, static_cast<unsigned long> (imageHeight));

		assert(pVariantTable->vt == (VT_ARRAY | VT_R8));

		// This makes a copy of the SAFEARRAY
		//CComSafeArray<double> saTable(pVariantTable->parray);
		CComSafeArray<double> saTable;
		saTable.Attach(pVariantTable->parray);
		int dim = saTable.GetDimensions();
		assert(dim == 2);
		unsigned long NX = saTable.GetCount();
		unsigned long NY = saTable.GetCount(1);
		assert(NX == 3);
		assert(NY == imageHeight);


		LONG aIndexR[2];
		aIndexR[0] = 0;
		LONG aIndexG[2];
		aIndexG[0] = 1;
		LONG aIndexB[2];
		aIndexB[0] = 2;

		for (LONG i = 0; i < imageHeight; i++)
		{
			lineptr = &((BYTE*)source)[pitchByte *  i];
			aIndexR[1] = i;
			aIndexG[1] = i;
			aIndexB[1] = i;
			saTable.MultiDimSetAt(aIndexR, static_cast<double> (lineptr[2]));
			saTable.MultiDimSetAt(aIndexG, static_cast<double> (lineptr[1]));
			saTable.MultiDimSetAt(aIndexB, static_cast<double> (lineptr[0]));
		}
		pVariantTable->parray = saTable.Detach();
	}
}

#else
// Function to show access to RGB pixel Data and to show table access
void CDllTool::copyRGBImageColToTable(MIL_ID milImage, _variant_t* pVariantTable)
{
	DEBUG_MILImageInfo(milImage);

	MIL_INT source = MbufInquire(milImage, M_HOST_ADDRESS, M_NULL);
	MIL_INT pitchByte = MbufInquire(milImage, M_PITCH_BYTE, M_NULL);
	MIL_INT imageHeight = MbufInquire(milImage, M_SIZE_Y, M_NULL);
	MIL_INT imageWidth = MbufInquire(milImage, M_SIZE_X, M_NULL);
	MIL_INT sizeBand = MbufInquire(milImage, M_SIZE_BAND, M_NULL);

	BYTE *lineptr;
	for (int y = 0; y < 3; y++)
	{
		lineptr = &((BYTE*)source)[pitchByte *  y];
		for (int x = 0; x < 3; x++)
		{
			FDB(1, "P(%03d,%03d): BGRX (%03d, %03d, %03d) ", x, y, lineptr[2], lineptr[1], lineptr[0]);
			lineptr += 4;
		}
		FDB(0, "\n");
	}

	TaAHelper::clearArrayOrTable(pVariantTable);
	TaAHelper::emptyTable_R8(pVariantTable, 3, static_cast<unsigned int> (imageHeight));

	assert(pVariantTable->vt == (VT_ARRAY | VT_R8));

	// This makes a copy of the SAFEARRAY
	//CComSafeArray<double> saTable(pVariantTable->parray);
	CComSafeArray<double> saTable;
	saTable.Attach(pVariantTable->parray);
	int dim = saTable.GetDimensions();
	assert(dim == 2);
	unsigned long NX = saTable.GetCount();
	unsigned long NY = saTable.GetCount(1);
	assert(NX == 3);
	assert(NY == imageHeight);


	LONG aIndexR[2];
	aIndexR[0] = 0;
	LONG aIndexG[2];
	aIndexG[0] = 1;
	LONG aIndexB[2];
	aIndexB[0] = 2;

	for (LONG i = 0; i < imageHeight; i++)
	{
		lineptr = &((BYTE*)source)[pitchByte *  i];
		aIndexR[1] = i;
		aIndexG[1] = i;
		aIndexB[1] = i;
		saTable.MultiDimSetAt(aIndexR, static_cast<double> (lineptr[2]));
		saTable.MultiDimSetAt(aIndexG, static_cast<double> (lineptr[1]));
		saTable.MultiDimSetAt(aIndexB, static_cast<double> (lineptr[0]));
	}
	pVariantTable->parray = saTable.Detach();
	//TaAHelper::DEBUGPrintTableOrArray(pVariantTable);

	// Direct SafeArrayaccess
	/*
	FDB(1, "---- Direct Access \n");
	SAFEARRAY * psa = pVariantTable->parray;
	SAFEARRAYBOUND aDim[1];
	FDB(1, "Dimensions:  %d\n", psa->cDims);
	FDB(1, "Elements:  %d\n", psa->cbElements);
	FDB(1, "sizeof Double %d\n", sizeof(double) );
	FDB(1, "Locks:  %d\n", psa->cLocks);

	FDB(1, "Rgsa Elements 0:  %d\n", psa->rgsabound[0].cElements);
	FDB(1, "Rgsa Bounds 0:  %d\n", psa->rgsabound[0].lLbound);
	FDB(1, "Rgsa Elements 1:  %d\n", psa->rgsabound[1].cElements);
	FDB(1, "Rgsa Bounds 1:  %d\n", psa->rgsabound[1].lLbound);


	if (SUCCEEDED( SafeArrayLock(pVariantTable->parray) ) )
	{
		double (*pData)[3] = static_cast<double(*)[3]>(pVariantTable->parray->pvData);
		for (LONG i = 0; i < imageHeight; i++)
		{
			pData[i][0] = 2.5;
			pData[i][2] = 3.5;
		}
	}

	if (SUCCEEDED( SafeArrayUnlock(pVariantTable->parray) ))
	{

	}
	TaAHelper::DEBUGPrintTableOrArray(pVariantTable);
	*/
}
#endif
#endif
#endif