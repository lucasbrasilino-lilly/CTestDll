#include "stdafx.h"

#ifdef _DEBUG

#include <stdio.h>
#include <stdarg.h>
#include "defines.h"
#include "DebugUtilities.h"

void start_fdb(int mode)
{
	debugmode = mode;

	if (debugmode == 0) return;

	_unlink(DEBUGFILE);
	FILE* mydbhFile = NULL;
	fopen_s(&mydbhFile, DEBUGFILE, "w+t");
	fprintf(mydbhFile, "Debug log started with mode %d\n", mode);
	fclose(mydbhFile);
}

int cdecl fdb(unsigned int nesting, char *msg, ...)
{
	if (debugmode == 0) return 0;

	va_list marker;

	FILE* mydbhFile = NULL;
	fopen_s(&mydbhFile, DEBUGFILE, "a+t");
	for (unsigned int i = 0; i < nesting; ++i)
	{
		fprintf(mydbhFile, "    ");
	}
	va_start(marker, msg);
	vfprintf(mydbhFile, msg, marker);
	va_end(marker);
	fflush(mydbhFile);
	fclose(mydbhFile);

	return 0;
}

#endif