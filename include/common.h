/*
 * @Author: zhubin
 * @Date: 2023-01-05 15:42:36
 * @LastEditTime: 2023-01-30 09:13:35
 * @FilePath: \libnlp\include\common.h
 * @Description:
 *
 * Copyright (c) 2023 by zhubin, All Rights Reserved.
 */

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

#include <stdint.h>
typedef int8_t nlp_int8_t;
typedef int16_t nlp_int16_t;
typedef int32_t nlp_int32_t;
typedef uint8_t nlp_uint8_t;
typedef ptrdiff_t nlp_ssize_t;
typedef size_t nlp_size_t;


#endif
