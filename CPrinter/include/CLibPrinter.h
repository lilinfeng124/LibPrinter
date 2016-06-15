#ifndef _LIBPRINTER_H_
#define _LIBPRINTER_H_

#ifdef LIBPRINTER_EXPORTS
#define PRINTER_API __declspec(dllexport)
#else
#define PRINTER_API __declspec(dllimport)
#endif

#ifndef WIN32
#undef  PRINTER_API
#define PRINTER_API
#endif

#ifdef WIN32
#pragma   warning(disable   :   4251)
#endif  

#endif