#pragma once

#ifdef DX11MATH_EXPORTS
#define DX11MATH_API __declspec(dllexport)
#else
#define DX11MATH_API __declspec(dllimport)
#endif

//#undef AFX_DATA
//#define AFX_DATA AFX_EXT_DATA
//
//#undef AFX_DATA
//#define AFX_DATA


#define VECTOR_CALL __vectorcall