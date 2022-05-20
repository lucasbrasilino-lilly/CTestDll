//****************************************************************************
/// \copyright COPYRIGHT (c) 2020,2020 by Seidenader Maschinenbau GmbH. 
/// All Rights Reserved 
/// \file CDllTool.h
/// This Class includes the main functionalities of the external tool.
//*****************************************************************************
#pragma once

#include "defines.h"
#include "StructDefinitions.h"


#ifdef NOMIL 
#include "CMyBitMapInfo.h"
#else
#include <mil.h>
#endif

class CDllTool
{
public:
	/// Constructor
	CDllTool();

	/// Destructor
	~CDllTool();

#pragma region public static function


	/// Returns the input value definitions (in the parameter).
	/// \param[in/out] rInputDef 
	static void getInputValuesDefinition(std::array<InputValueDefinitionStructEx, NUM_INPUT_VALUES>  &rInputDef);

	/// Return the result value definition (in the parameter).
	/// \param[in/out] rResultDef 
	static void getResultValueDefinition(std::array<ResultValueDefinitionStructEx, NUM_RESULT_VALUES>  &rResultDef);

	/// Check if the value is valid (Variant Type and range)
	/// \param[in] lParameterNumber the index of the parameter. Should match to enum InputValues
	/// \param[in] vParameterValue	the parameter value
	/// \return HRESULT S_OK if value is valid, otherwise an errorcode from defines.h should be returned
	static HRESULT validateInputValue(long lParameterNumber, const VARIANT vParameterValue);

	/// Returns an error message matching an error code.
	/// \detail This method will be called to inquire for an error message corresponding  to a given errorcode. 
	static _bstr_t CDllTool::getErrorMessageString(HRESULT errorcode);
#pragma endregion public static function

#pragma region public function
public:
	/// initRun initializes the external dLL functionality and prepares it for the run
	/// \detail Also resets all internal  inputs and outputs, allocates what can be allocated before run.
	/// \return HRESULT S_OK in case of success
	/// \param[in] rImageDefinitions	An array with the input image definitions.
	/// \param[in] pInputValues		An array of the current input values.
	HRESULT initRun(const std::array<ImageDefinitionStructEx, NUM_INPUT_IMAGES>  &rImageDefinitions, const std::array<VARIANT, NUM_INPUT_VALUES> &rInputValues);

	/// Runs the external dll main functionality.
	/// \return HRESULT S_OK in case of success
	HRESULT run();

#pragma region public set function
	/// Sets the guid of the tool. The GUID will be used to tell apart the instances of this DLL. 
	/// \param[in] guid GUID-number
	void setGUID(const GUID guid);

	/// Sets the input values for the next run into m_aInputValues;
	/// The values passed into the DLL by this function, 
	/// specifically the BSTRs, may not be manipulated by the DLL. 
	/// This means that the DLL may not delete, destroy, re-allocate, or write to the referenced BSTR. 
	/// SVObserver will create and destroy these BSTRs.
	/// \param[in] rInputValues An array of the current input values
	/// \return HRESULT S_OK in case of success
	HRESULT setInputValues(const std::array<VARIANT, NUM_INPUT_VALUES> &rInputValues);

#if (defined MIL9 || defined MIL10)
	/// Sets the input MIL images for the next run.
	///	\param[in] rMILhandles  an array of the input handles.
	/// \return HRESULT S_OK in case of success
	HRESULT setMILInputImages(long* paMILhandles);
#endif

#if (defined MIL9 || defined MIL10)
	/// Set the MIl handles to the result images.
	///\param[in] rMILhandles the handles of the result images
	/// \return HRESULT S_OK in case of success
	HRESULT setMILResultImages(long* paMILhandles);
#endif

#if defined(NOMIL)
	/// Sets the input HBITMAP images for the next run.
	///	\param[in] rHandles  an array of the input handles.
	/// \return HRESULT S_OK in case of success
	HRESULT setHBITMAPInputImages(const std::array<HBITMAP, NUM_INPUT_IMAGES> &rHandles);
#endif

#pragma endregion public set function

#pragma region public get function
	/// Returns the result image definitions. 
	/// \param[in/out] rImageDefinitions result image definitions
	/// \return HRESULT S_OK in case of success
	HRESULT getResultImageDefs(std::array<ImageDefinitionStructEx, NUM_RESULT_IMAGES> &rImageDefinitions) const;

	/// Returns the result values.
	/// \param[in/out] rResultValues Return the result values of the last run.
	/// \return HRESULT S_OK in case of success
	HRESULT getResultValues(std::array<VARIANT, NUM_RESULT_VALUES> &rResultValues) const;



#ifdef NOMIL
	/// Returns handles to the result images.
	///  
	/// \param [in/out] rHandles returns the handles of the images of the last run
	/// \return HRESULT S_OK in case of success
	HRESULT getHBITMAPResultImages(std::array<HBITMAP, NUM_RESULT_IMAGES> &rHandles) const;
#endif

#pragma endregion public get function 
#pragma endregion public function

#pragma region private function
private:
	/// Clears the data of the class. 
	///  
	/// Clears all vectors and destroys objects in this vectors. 
	// all vectors have size 0 afterwards
	/// Needs to be called by the destructor.
	void cleanData();

#ifdef NOMIL
	/// Copies an HBITMAP. 
	/// \param hBitMapSource Handle to the source image.
	/// \param hBitMapDestination Handle to the destination image.
	/// \return HRESULT S_OK in case of success
	HRESULT copyHBitMap(HBITMAP hBitMapSource, HBITMAP hBitMapDestination);
#endif

#ifdef EXAMPLECODE
	/// Runs an example to show some template capability 
	/// 
	/// \return HRESULT S_OK in case of success
	HRESULT runEXAMPLE();
	/// Example function to illustrate how data is copied  
	void copyDataEXAMPLE();

	/// Example function to illustrate how data is copied  
	void copyImageEXAMPLE();

#ifdef NOMIL
#endif

#if defined(MIL9) || defined(MIL10) 
	void DEBUG_MILImageInfo(MIL_ID milImage);
#endif
#endif


#pragma endregion private function

#pragma region member parameters
private:
	GUID m_GUID; /** GUID to identify this ExternalTool by SVObserver*/

	std::vector<_variant_t> m_aInputValues;
	std::vector<_variant_t> m_aResultValues;


	// Input Images
#ifdef NOMIL
	std::vector<HBITMAP> m_aInputImageIds;
#else
	std::vector<MIL_ID> m_aInputImageIds;
#endif

	// Result Images
#ifdef NOMIL
	std::vector<HBITMAP> m_aResultImageIds;
	std::vector<std::unique_ptr<CMyBitMapInfo>> m_aResultImageInfo;
	std::vector<ImageDefinitionStructEx> m_aResultImageDefs;
#else
	std::vector<MIL_ID> m_aResultImageIds;
	std::vector<ImageDefinitionStructEx> m_aResultImageDefs;
#endif

	//TODO: add your result parameter(s) and remove the sample variables provided here afterwards.
#ifdef EXAMPLECODE
	static const int c_I1_minValue = 0;
	static const int c_I1_maxValue = 1000000;
#endif
#pragma endregion member parameters
};