#include "stdafx.h"
#include "DebugUtilities.h"
#include "TableAndArrayHelper.h"

// get the Array elements or table rows or 0, 
// this function is used to check result agains the maximum size value.
long TaAHelper::getMaxElements(const _variant_t* pVariant)
{
	if (pVariant && (pVariant->vt & VT_ARRAY) != 0)
	{
		if (pVariant->parray)
		{
			return pVariant->parray->rgsabound[0].cElements;
		}
	}
	return 0;
}

long TaAHelper::getTableColCount(_variant_t* pVariantTable)
{
	SAFEARRAY *psa = pVariantTable->parray;
	if (psa->cDims != 2)
	{
		FDBERROR("getTableColCount Table needs 2 dimensions!");
		return 0;
	}
	return psa->rgsabound[1].cElements;
}

long TaAHelper::getTableColCount_CCOM(_variant_t* pVariantTable)
{
	long cols = 0;

	if (pVariantTable->vt == (VT_ARRAY | VT_R8))
	{
		CComSafeArray<double> saInput;
		saInput.Attach(pVariantTable->parray);
		int dim = saInput.GetDimensions();
		assert(dim == 2);
		cols = saInput.GetCount();
		saInput.Detach();

	}
	else if (pVariantTable->vt == (VT_ARRAY | VT_I4))
	{
		CComSafeArray<LONG> saInput;
		saInput.Attach(pVariantTable->parray);
		int dim = saInput.GetDimensions();
		assert(dim == 2);
		cols = saInput.GetCount();
		saInput.Detach();
	}
	else if (pVariantTable->vt == (VT_ARRAY | VT_BSTR))
	{
		CComSafeArray<BSTR> saInput;
		saInput.Attach(pVariantTable->parray);
		int dim = saInput.GetDimensions();
		assert(dim == 2);
		cols = saInput.GetCount();
		saInput.Detach();
	}
	return cols;
}

long TaAHelper::getTableRowCount(_variant_t* pVariantTable)
{
	SAFEARRAY *psa = pVariantTable->parray;
	if (psa->cDims != 2)
	{
		FDBERROR("getTableRowCount Table needs 2 dimensions!");
		return 0;
	}
	return psa->rgsabound[0].cElements;
}

long TaAHelper::getArrayElementCount(_variant_t* pVariantArray)
{
	SAFEARRAY *psa = pVariantArray->parray;
	if (psa->cDims != 1)
	{
		FDBERROR("getArrayElementCount array needs 1 dimensions!");
		return 0;
	}
	return psa->rgsabound[0].cElements;
}

long TaAHelper::getTableRowCount_CCOM(_variant_t* pVariantTable)
{
	long rows = 0;
	if (pVariantTable->vt == (VT_ARRAY | VT_R8))
	{
		CComSafeArray<double> saInput;
		saInput.Attach(pVariantTable->parray);
		UINT dim = saInput.GetDimensions();
		assert(dim == 2);
		rows = saInput.GetCount(1);
		pVariantTable->parray = saInput.Detach();
	}
	else if (pVariantTable->vt == (VT_ARRAY | VT_I4))
	{
		CComSafeArray<long> saInput;
		saInput.Attach(pVariantTable->parray);
		UINT dim = saInput.GetDimensions();
		assert(dim == 2);
		rows = saInput.GetCount(1);
		pVariantTable->parray = saInput.Detach();
	}
	else if (pVariantTable->vt == (VT_ARRAY | VT_BSTR))
	{
		CComSafeArray<BSTR> saInput;
		saInput.Attach(pVariantTable->parray);
		UINT dim = saInput.GetDimensions();
		assert(dim == 2);
		rows = saInput.GetCount(1);
		pVariantTable->parray = saInput.Detach();
	}
	return rows;

}

// This function will resize the output array to the correct size
void TaAHelper::CopyTableColR8ToArrayR8_CCOM(_variant_t* pVariantTable, const LONG col, _variant_t* pVariantArray)
{
	FDB(1, "CopyTableColToArray \n");
	assert(pVariantTable->vt == (VT_ARRAY | VT_R8));
	assert(pVariantArray->vt == (VT_ARRAY | VT_R8));

	LONG numTableRows = getTableRowCount(pVariantTable);
	LONG numTableCols = getTableColCount(pVariantTable);
	LONG numArrayElements = getArrayElementCount(pVariantArray);

	/// resize output array if it does not already has the correct size
	if (numTableRows != numArrayElements)
	{
		TaAHelper::clearArrayOrTable(pVariantArray);
		TaAHelper::emptyArray_R8(pVariantArray, numTableRows);
	}

	CComSafeArray<double> saInput;
	saInput.Attach(pVariantTable->parray);
	UINT dimTable = saInput.GetDimensions();
	assert(dimTable == 2);

	LONG Index[2];
	double val{ 0 };

	// copy selected row to output array
	Index[0] = col < numTableCols ? col : numTableCols;

	CComSafeArray<double> saOutput;
	saOutput.Attach(pVariantArray->parray);
	for (LONG y = 0; y < numTableRows; y++)
	{
		Index[1] = y;
		saInput.MultiDimGetAt(Index, val);
		saOutput[y] = val;
	}

	pVariantArray->parray = saOutput.Detach();
	pVariantArray->parray = saInput.Detach();
}

void TaAHelper::CopyTableColR8ToArrayR8(_variant_t* pVariantTable, const unsigned int col, _variant_t* pVariantArray)
{
	FDB(1, "CopyTableColR8ToArrayR8 col %d - Enter\n", col);
	assert(pVariantTable->vt == (VT_ARRAY | VT_R8));
	assert(pVariantArray->vt == (VT_ARRAY | VT_R8));


	long numTableRows = getTableRowCount(pVariantTable);
	long numTableCols = getTableColCount(pVariantTable);
	long numArrayElements = getArrayElementCount(pVariantArray);
	FDB(1, " Now Rows %d, Num cols %d,  num array elements %d\n", numTableRows, numTableCols, numArrayElements);


	/// resize output array if it does not already has the correct size
	if (numTableRows != numArrayElements)
	{
		FDB(1, " Resize output array to %d\n", numTableRows);
		TaAHelper::clearArrayOrTable(pVariantArray);
		TaAHelper::emptyArray_R8(pVariantArray, numTableRows);
	}

	// Access data and copy
	SAFEARRAY *psaIn = pVariantTable->parray;
	SAFEARRAY *psaOut = pVariantArray->parray;

	double *pDataIn;
	SafeArrayLock(psaOut);
	SafeArrayAccessData(psaIn, (void**)&pDataIn);

	double * pDataOut = static_cast<double*>(psaOut->pvData);

	for (int row = 0; row < numTableRows; row++)
	{
		pDataOut[row] = pDataIn[col];
		pDataIn += numTableCols;
	}

	SafeArrayUnaccessData(psaIn);
	SafeArrayUnlock(psaOut);
	FDB(1, "CopyTableColToArray - Exit\n");
}

void TaAHelper::CopyArrayToArray(_variant_t* pVariantArrayInput, _variant_t* pVariantArrayOutput)
{
	FDB(1, "CopyArrayToArray \n");
	if (pVariantArrayOutput->vt == VT_EMPTY)
	{
		FDB(1, "Type of dest is VT_EMPTY \n");
		TaAHelper::emptyArray(pVariantArrayOutput, pVariantArrayInput->vt, 2);
	}

	if (pVariantArrayInput->vt == (VT_ARRAY | VT_I4) && pVariantArrayOutput->vt == (VT_ARRAY | VT_I4))
	{
		FDB(1, " CopyArrayToArray VT_I4 \n");
		CopyArrayI4ToArrayI4_CCOM(pVariantArrayInput, pVariantArrayOutput);
	}
	else if (pVariantArrayInput->vt == (VT_ARRAY | VT_R8) && pVariantArrayOutput->vt == (VT_ARRAY | VT_R8))
	{
		FDB(1, "CopyArrayToArray VT_R8 \n");
		CopyArrayR8ToArrayR8(pVariantArrayInput, pVariantArrayOutput);
	}
	else
	{
		FDB(1, "CopyArrayToArray No type found \n");
	}
}

void TaAHelper::CopyArrayI4ToArrayI4_CCOM(_variant_t* pVariantArrayInput, _variant_t* pVariantArrayOutput)
{
	assert(pVariantArrayInput->vt == (VT_ARRAY | VT_I4));
	assert(pVariantArrayOutput->vt == (VT_ARRAY | VT_I4));

	CComSafeArray<LONG> saInput_LONG_array;
	saInput_LONG_array.Attach(pVariantArrayInput->parray);

	LONG inputarrayLen = saInput_LONG_array.GetCount();
	LONG inputarrayLowerBound = saInput_LONG_array.GetLowerBound();

	//copy input to output 
	CComSafeArray<LONG> saResult_LONG_array(inputarrayLen);
	for (LONG i = 0; i < inputarrayLen; i++)
	{
		saResult_LONG_array[i] = saInput_LONG_array[i];
	}

	if (inputarrayLen)
	{
		pVariantArrayOutput->vt = VT_ARRAY | VT_I4;
		pVariantArrayOutput->parray = saResult_LONG_array.Detach();
	}
	SafeArrayDestroy(pVariantArrayInput->parray);
	pVariantArrayInput->parray = nullptr;
	pVariantArrayInput->parray = saInput_LONG_array.Detach();
}

void TaAHelper::CopyArrayR8ToArrayR8(_variant_t* pVariantArrayInput, _variant_t* pVariantArrayOutput)
{
	FDB(1, "CopyArrayR8ToArrayR8\n");

	assert(pVariantArrayInput->vt == (VT_ARRAY | VT_R8));
	assert(pVariantArrayOutput->vt == (VT_ARRAY | VT_R8));

	// CComSafeArray<double> saInput_array((pVariantArrayInput->parray));
	CComSafeArray<double> saInput_array;
	saInput_array.Attach(pVariantArrayInput->parray);
	LONG inputarrayLen = saInput_array.GetCount();
	LONG inputarrayLowerBound = saInput_array.GetLowerBound();

	//copy input to output 
	FDB(1, "CopyArrayR8ToArrayR8 : Copy \n");
	CComSafeArray<double> saResult_array(inputarrayLen);
	for (LONG i = 0; i < inputarrayLen; i++)
	{
		saResult_array[i] = saInput_array[i];
	}
	pVariantArrayInput->parray = saInput_array.Detach();

	pVariantArrayOutput->vt = VT_ARRAY | VT_R8;
	SafeArrayDestroy(pVariantArrayInput->parray);
	pVariantArrayInput->parray = nullptr;
	pVariantArrayOutput->parray = saResult_array.Detach();
	FDB(1, "CopyArrayR8ToArrayR8 - Exit\n");
}

void TaAHelper::clearArrayOrTable(_variant_t* pVariant)
{
	FDB(1, "TaAHelper::clearArrayOrTable -  Enter \n");
	if (pVariant)
	{
		pVariant->Clear();
	}
	FDB(1, "TaAHelper::clearArrayOrTable -  Exit \n");
}

void TaAHelper::emptyArray(_variant_t* pVariant, VARTYPE type, int rows)
{
	FDB(1, "TaAHelper::emptyArray type:");
	if ((type & VT_I4) == VT_I4)
	{
		FDB(0, "VT_I4\n");
		emptyArray_I4(pVariant, rows);
	}
	else if ((type & VT_R8) == VT_R8)
	{
		FDB(0, "VT_R8\n");
		emptyArray_R8(pVariant, rows);
	}
	else if ((type  & VT_BSTR) == VT_BSTR)
	{
		FDB(0, "VT_BSTR\n");
		emptyArray_BSTR(pVariant, rows);
	}
	else
	{
		FDBERROR("TaAHelper::emptyArray used with wrong type!\n");
	}

}

void TaAHelper::emptyArray_R8(_variant_t* pVariant, const LONG elements)
{
	FDB(1, "TaAHelper::emptyArray_R8 with %d elements\n", elements);
	CComSafeArray<double> isa(elements);
	for (int i = 0; i < elements; i++)
	{
		isa[i] = 0.0;
	}
	clearArrayOrTable(pVariant);
	pVariant->vt = VT_ARRAY | VT_R8;
	pVariant->parray = isa.Detach();
}

void TaAHelper::emptyArray_I4(_variant_t* pVariant, const LONG elements)
{
	FDB(1, "TaAHelper::emptyArray_I4 with %d elements\n", elements);
	CComSafeArray<LONG> isa(elements);
	for (int i = 0; i < elements; i++)
	{
		isa[i] = 0;
	}
	clearArrayOrTable(pVariant);
	pVariant->vt = VT_ARRAY | VT_I4;
	pVariant->parray = isa.Detach();
}

void TaAHelper::emptyArray_BSTR(_variant_t* pVariant, const LONG elements)
{
	FDB(1, "TaAHelper::emptyArray_BSTR with %d elements\n", elements);

	_bstr_t Columname = { "" };
	CComSafeArray<BSTR> saStr(elements);
	for (int i = 0; i < elements; i++)
	{
		saStr.SetAt(i, Columname);
	}
	clearArrayOrTable(pVariant);
	pVariant->vt = VT_ARRAY | VT_BSTR;
	pVariant->parray = saStr.Detach();


}

void TaAHelper::emptyTable_R8(_variant_t* pVariant, const ULONG cols, const ULONG rows)
{
	clearArrayOrTable(pVariant);
	FDB(1, "TaAHelper::emptyTable_R8 with %d cols and %d rows\n", cols, rows);
	CComSafeArrayBound bound[2] = { cols,rows };
	CComSafeArray<double> tdimsa(bound, 2);
	LONG aIndex[2];
	for (ULONG x = 0; x < cols; x++)
	{
		aIndex[0] = x;
		for (ULONG y = 0; y < rows; y++)
		{
			aIndex[1] = y;
			HRESULT hr = tdimsa.MultiDimSetAt(aIndex, 0.0);
			assert(hr == S_OK);
		}
	}
	pVariant->vt = VT_ARRAY | VT_R8;
	pVariant->parray = tdimsa.Detach();
}

void TaAHelper::emptyTable_I4(_variant_t* pVariant, const ULONG cols, const ULONG rows)
{
	FDB(1, "TaAHelper::emptyTable_I4 with %d cols and %d rows\n", cols, rows);
	CComSafeArrayBound bound[2] = { cols,rows };
	CComSafeArray<long> tdimsa(bound, 2);
	LONG aIndex[2];
	for (ULONG x = 0; x < cols; x++)
	{
		aIndex[0] = x;
		for (ULONG y = 0; y < rows; y++)
		{
			aIndex[1] = y;
			HRESULT hr = tdimsa.MultiDimSetAt(aIndex, 0);
			assert(hr == S_OK);
		}
	}
	clearArrayOrTable(pVariant);
	pVariant->vt = VT_ARRAY | VT_I4;
	pVariant->parray = tdimsa.Detach();
}

void TaAHelper::CopyTableToTable(_variant_t* pVariantTableInput, _variant_t* pVariantTableOutput, ULONG columnCount)
{
	FDB(1, "CopyTableToTable\n");
	if (pVariantTableInput->vt == (VT_ARRAY | VT_R8))
	{
		FDB(2, "Input is of correct type\n");
		CComSafeArray<double> saInput;
		saInput.Attach(pVariantTableInput->parray);
		int dim = saInput.GetDimensions();
		assert(dim == 2);
		unsigned long NX = saInput.GetCount();
		unsigned long NY = saInput.GetCount(1);
		FDB(2, "Input NX %d, NY%d \n", NX, NY);

		CComSafeArrayBound bound[2] = { { columnCount,0 },{ NY,0 } };
		CComSafeArray<double> tdimsa(bound, 2);
		LONG aIndex[2];
		for (unsigned long x = 0; x < min(columnCount, NX); x++)
		{
			for (unsigned long y = 0; y < NY; y++)
			{
				aIndex[0] = x;
				aIndex[1] = y;
				double val{ 0 };

				if (x < NX &&  y < NY)
				{
					HRESULT hrIn = saInput.MultiDimGetAt(aIndex, val);
					if (hrIn != S_OK)
					{
						FDBERROR("MultiDimGetAt failed");
					}
				}

				HRESULT hr = tdimsa.MultiDimSetAt(aIndex, val);
				assert(hr == S_OK);
			}
		}

		clearArrayOrTable(pVariantTableOutput);
		pVariantTableOutput->vt = VT_ARRAY | VT_R8;
		pVariantTableOutput->parray = tdimsa.Detach();

		saInput.Detach();
	}
	else
	{
		FDBERROR("Input Table of wrong type!\n");
	}

}

void TaAHelper::DEBUGPrintTableOrArray(_variant_t* pVariant)
{
#if _DEBUG
	FDB(1, "TaAHelper::DEBUGPrintTableOrArray\n");
	if (pVariant->vt == (VT_ARRAY | VT_BSTR))
	{
		FDB(2, "Type: BSTR\n");
		CComSafeArray<BSTR> saInput;
		saInput.Attach(pVariant->parray);
		int dim = saInput.GetDimensions();
		if (dim == 1)
		{
			FDB(2, "Array Names\n");
			for (ULONG y = 0; y < saInput.GetCount(); y++)
			{
				CComBSTR  name = saInput.GetAt(y);
				_bstr_t bstrname = name.m_str;
				FDB(1, "bstrname: %s\n", LPCSTR(bstrname));
			}
		}
		else
		{
			FDB(2, "2dim Table not supported\n");
		}
		pVariant->parray = saInput.Detach();
	}
	else if (pVariant->vt == (VT_ARRAY | VT_R8))
	{
		FDB(2, "Type: VT_R8\n");
		CComSafeArray<double> saInput;
		saInput.Attach(pVariant->parray);
		int dim = saInput.GetDimensions();
		if (dim == 1)
		{
			FDB(2, "Array Values\n");
			for (ULONG y = 0; y < saInput.GetCount(); y++)
			{
				FDB(1, "Value: %f\n", saInput.GetAt(y));
			}
		}
		else if (dim == 2)
		{
			FDB(2, "Table Values\n");
			int cols = saInput.GetCount();
			int rows = saInput.GetCount(1);
			long Index[2];
			double val{ 0 };

			std::stringstream sts;
			for (int y = 0; y < rows; y++)
			{
				sts = std::stringstream();
				for (int x = 0; x < cols; x++)
				{
					Index[0] = x;
					Index[1] = y;
					saInput.MultiDimGetAt(Index, val);
					sts << "(" << x << "," << y << "): " << val << " ";
				}
				sts << std::endl;
				FDB(2, "%s", sts.str().c_str());
			}
		}
		pVariant->parray = saInput.Detach();
	}
	else if (pVariant->vt == (VT_ARRAY | VT_I4))
	{
		FDB(2, "Type: VT_I4\n");
		CComSafeArray<long> saInput;
		saInput.Attach(pVariant->parray);
		int dim = saInput.GetDimensions();
		if (dim == 1)
		{
			FDB(2, "Array Values\n");
			for (ULONG y = 0; y < saInput.GetCount(); y++)
			{
				FDB(2, "Value: %d\n", saInput.GetAt(y));
			}
		}
		else if (dim == 2)
		{
			FDB(2, "Table Values\n");
			int cols = saInput.GetCount();
			int rows = saInput.GetCount(1);
			long Index[2];
			long val{ 0 };

			std::stringstream sts;
			for (int y = 0; y < rows; y++)
			{
				sts = std::stringstream();
				for (int x = 0; x < cols; x++)
				{
					Index[0] = x;
					Index[1] = y;
					saInput.MultiDimGetAt(Index, val);
					sts << "(" << x << "," << y << "): " << val << " ";
				}
				sts << std::endl;
				FDB(2, "%s", sts.str().c_str());
			}
		}
		pVariant->parray = saInput.Detach();
	}
	else
	{
		FDB(2, "Type: UNKOWN\n");
	}
#endif
}