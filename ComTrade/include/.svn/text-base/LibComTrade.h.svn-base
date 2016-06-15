#ifndef _LIBCOMTRADE_H_
#define _LIBCOMTRADE_H_

#ifdef LIBCOMTRADE_EXPORTS
#define COMTRADE_API __declspec(dllexport)
#else
#define COMTRADE_API __declspec(dllimport)
#endif

#ifndef WIN32
#undef  COMTRADE_API
#define COMTRADE_API
#endif

#ifdef WIN32
#pragma   warning(disable   :   4251)
#endif  

#endif