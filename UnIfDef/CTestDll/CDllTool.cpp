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

#endif 
	FDB(1, "CDllTool::getInputValuesDefinition - Exit\n");
}

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

#endif
}


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
HRESULT CDllTool::initRun(const std::array<ImageDefinitionStructEx, NUM_INPUT_IMAGES>  &rImageDefinitions, const std::array<VARIANT, NUM_INPUT_VALUES> &rInputValues)

{
	HRESULT hr = S_OK;

	// Clean old Data
	cleanData();

	// Resize and Set Inputs
	m_aInputImageIds.resize(NUM_INPUT_IMAGES);

	m_aInputValues.resize(NUM_INPUT_VALUES);
	setInputValues(rInputValues);

	// Resize  ResultsImage
	m_aResultImageDefs.resize(NUM_RESULT_IMAGES);
	m_aResultImageIds.resize(NUM_RESULT_IMAGES);
#ifdef NOMIL
	m_aResultImageInfo.resize(NUM_RESULT_IMAGES);
#endif

	m_aResultValues.resize(NUM_RESULT_VALUES);



	//TODO: init normal ResultValues and delete the sample variables provided here afterwards.
#ifdef EXAMPLECODE
	m_aResultValues[ResultValue_LONG].vt = VT_I4;
	m_aResultValues[ResultValue_LONG].lVal = 2;

	m_aResultValues[ResultValue_BOOL].vt = VT_BOOL;
	m_aResultValues[ResultValue_BOOL].boolVal = false;

	m_aResultValues[ResultValue_DOUBLE].vt = VT_R8;
	m_aResultValues[ResultValue_DOUBLE].dblVal = 1.5;

	m_aResultValues[ResultValue_BSTR] = L"Result Value";

#endif


	// TODO: init ResultValues Arrays:
#ifdef EXAMPLECODE
#endif

	//TODO: set definition for result images. In this sample the first input image has the same definition as result image
#ifdef EXAMPLECODE
	m_aResultImageDefs[ResultImage_Gray] = rImageDefinitions[InputImage_Gray];
	std::wstring strName = L"Result Image " + std::to_wstring(ResultImage_Gray);
	m_aResultImageDefs[ResultImage_Gray].bstrDisplayName = strName.c_str();


#ifdef NOMIL
	m_aResultImageInfo[ResultImage_Gray] = std::make_unique<CMyBitMapInfo>(m_aResultImageDefs[ResultImage_Gray]);
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

HRESULT CDllTool::setInputValues(const std::array<VARIANT, NUM_INPUT_VALUES> &rInputValues)
{
	FDB(1, "CDllTool::setInputValues - Enter\n");
	HRESULT hr = S_OK;

	//TODO: Delete InputValue_RGBVAL_ARRAY Example
#ifdef EXAMPLECODE
#endif

	for (int i = 0; i < NUM_INPUT_VALUES; i++)
	{
		m_aInputValues[i] = &rInputValues[i];
	}
	FDB(1, "CDllTool::setInputValues - EXIT\n");
	return S_OK;
}

#if defined MIL9 || defined MIL10
HRESULT CDllTool::setMILInputImages(long* paMILhandles)
{
	for (int i = 0; i < NUM_INPUT_IMAGES; i++)
	{
		m_aInputImageIds[i] = static_cast<MIL_ID> (paMILhandles[i]);
	}
	return S_OK;
}

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

#pragma endregion set function

#pragma region public get function

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

HRESULT CDllTool::getResultValues(std::array<VARIANT, NUM_RESULT_VALUES> &rResultValues) const
{
	try
	{
		for (int i = 0; i < NUM_RESULT_VALUES; i++)
		{
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



#ifdef NOMIL
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
#pragma endregion get function
#pragma endregion public function

#pragma region private function


void CDllTool::cleanData()
{
	FDB(1, "CDllTool::cleanData\n");
	for (size_t i = 0; i < m_aInputValues.size(); i++)
	{
		::VariantClear(&m_aInputValues[i]);
	}
	m_aInputValues.clear();

	FDB(1, "CDllTool::cleanData ResultValues\n");
	for (size_t i = 0; i < m_aResultValues.size(); i++)
	{
		::VariantClear(&m_aResultValues[i]);
	}
	m_aResultValues.clear();

	m_aInputImageIds.clear();

	m_aResultImageIds.clear();
	m_aResultImageDefs.clear();
#ifdef NOMIL
	m_aResultImageInfo.clear();
#endif

	FDB(1, "CDllTool::cleanData exit\n");
	//TODO: add your code to clean data if necessary.
}

#if defined(NOMIL)  
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

#ifdef EXAMPLECODE
HRESULT CDllTool::runEXAMPLE()
{
	FDB(1, "runEXAMPLE - Begin\n");

	copyDataEXAMPLE();


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
#endif

#ifdef EXAMPLECODE
void CDllTool::copyImageEXAMPLE()
{
	// Copy Input to Output images
#ifdef NOMIL
	copyHBitMap(m_aInputImageIds[InputImage_Gray], m_aResultImageInfo[ResultImage_Gray]->getBitmapHandle());
#else
	MbufCopy(m_aInputImageIds[InputImage_Gray], m_aResultImageIds[ResultImage_Gray]);
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
#endif
