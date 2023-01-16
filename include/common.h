
#ifndef COMMON_H_
#define COMMON_H_

#if defined(_WIN32) && !defined(__CYGWIN__)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#endif

#if defined(_WIN32) && defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

#ifdef LIBNLP_STATIC
#define LIBNLP_DLLEXPORT
#else
#ifdef _WIN32
#ifdef LIBNLP_EXPORTS
#define LIBNLP_DLLEXPORT __declspec(dllexport)
#else
#define LIBNLP_DLLEXPORT __declspec(dllimport)
#endif
#elif __GNUC__ >= 4
#define LIBNLP_DLLEXPORT __attribute__((visibility("default")))
#else
#define LIBNLP_DLLEXPORT
#endif
#endif


#endif
