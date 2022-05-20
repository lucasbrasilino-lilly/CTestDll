//****************************************************************************
/// \copyright COPYRIGHT (c) 2020,2020 by Seidenader Maschinenbau GmbH. 
/// All Rights Reserved 
/// \file TableAndArrayHelper.h
/// This file does add a namespace for static function to deal with _variant_t 
/// arrays ands tables used for SVObserver External Tool Dlls.
///
/// It is not inteded to be a complete library for dealing with these task but
/// to show and implement (and reference) Good Practice approach for similar tasks
//*****************************************************************************
#pragma once

#include "DebugUtilities.h"

class TaAHelper
{
public:
	/// Copy a column of a table R8 to a array R8 .
	/// 
	/// if the destination array does not have the correct element size, it will be newly created with
	/// elements matching the table row size
	static void CopyTableColR8ToArrayR8(_variant_t* pVariantTable, const unsigned int col, _variant_t* pVariantArray);

	/// if col is to large, the last colum will be used
	static void CopyTableColR8ToArrayR8_CCOM(_variant_t* pVariantTable, const LONG col, _variant_t* pVariantArray);

	// assumes both are of  same type
	// the output array will have  the same size and value as the input
	static void CopyArrayToArray(_variant_t* pVariantArrayInput, _variant_t* pVariantArrayOutput);

	static void CopyArrayI4ToArrayI4_CCOM(_variant_t* pVariantArrayInput, _variant_t* pVariantArrayOutput);
	static void CopyArrayR8ToArrayR8(_variant_t* pVariantArrayInput, _variant_t* pVariantArrayOutput);

	// Copy the amount of columns specified to the output table
	static void CopyTableToTable(_variant_t* pVariantTableInput, _variant_t* pVariantTableOutput, ULONG columnCount);


	/// get the number of elements in a1 dim array;
	static long getArrayElementCount(_variant_t* pVariantArray);

	///  get the row count of a Table (dimensions = 2)
	/// if the dimensions do not match or the VARTYPE is unknown, 0 is returned.
	static long getTableRowCount(_variant_t* pVariantTable);

	static long getTableRowCount_CCOM(_variant_t* pVariantTable);

	/// if the input array does not have 2 dimensions, a value of 0 is returned.
	static long getTableColCount(_variant_t* pVariantTable);

	/// Same as getTableColCount, but using CCom arrays
	static long getTableColCount_CCOM(_variant_t* pVariantTable);

	/// Print a table or array for debug purposes
	static void DEBUGPrintTableOrArray(_variant_t* pVariant);

	static void clearArrayOrTable(_variant_t* pVariant);

	// creates an empty array of the specified type with the specified amount of elements
	// type can be set to VT_I4, VT_R8 or VT_BSTR
	// elements are initialized with 0. 0-0 or ""
	static void emptyArray(_variant_t* pVariant, VARTYPE type, int elements);

	// creates an empty double array with the specified amount of elements
	// all elements are set to 0.0
	static void emptyArray_R8(_variant_t* pVariant, const LONG elements);

	// creates an empty long array with the specified amount of elements
	// all elements are set to 0
	static void emptyArray_I4(_variant_t* pVariant, const LONG elements);

	// creates an empty BSTR array with the specified amount of elements
	// all elements are set to ""
	static void emptyArray_BSTR(_variant_t* pVariant, const LONG elements);


	static void emptyTable_R8(_variant_t* pVariant, const ULONG cols, const ULONG rows);

	// create empty Table initialized with 0
	static void emptyTable_I4(_variant_t* pVariant, const ULONG cols, const ULONG rows);

	static long getMaxElements(const _variant_t* pVariant);

};