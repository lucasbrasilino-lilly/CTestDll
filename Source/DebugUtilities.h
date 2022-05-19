#pragma once

static int debugmode = 0;

/// starts the debuging file and sets the debugmode variable
/// \param mode 0:set debugmode variable to 0 and disable debugging functions, other values do enable debuging.
void start_fdb(int mode);

#if _DEBUG
/// log a message to the log file
/// \param msg the message to print
int cdecl fdb(unsigned int nest, char *msg, ...);

#define FDB(nest, str, ...) fdb(nest, str, __VA_ARGS__)
#define FDBERROR(str, ...) fdb(0, "ERROR " str, __VA_ARGS__)
#else
#  define FDB(nest, str, ...)
#  define FDBERROR(str, ...)
#	ifndef NDEBUG
#		define NDEBUG
#	endif
#endif

#define SV_TRY_CATCH 1
#if SV_TRY_CATCH
#define SVTRY    try {
#else
#define SVTRY    
#endif 

#if  SV_TRY_CATCH
#if _DEBUG
#define SVCATCHALL(hr,errornr )   }\
catch(std::exception& ex)\
{\
(hr) = (errornr);\
FDB(1,"Exception: %s", ex.what());\
}\
catch (...)\
{\
	(hr) = (errornr);\
	FDB(1,"unknown Exception:");\
}
#else 
#define SVCATCHALL(hr,errornr )  }\
catch (...)\
{\
	(hr) = (errornr);\
	FDB(1,"unknown Exception:");\
}
#endif //_DEBUG
#else
#define SVCATCHALL(hr,errornr ) 
#endif 