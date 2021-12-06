#pragma once

#include <string>
#include "GL/glew.h"

void GLClearError();
void LogCall(const char* tag, std::string log);
void ErrorLog(std::string str, const char* file, int line);
bool LogAssert(const char* funciton, const char* file, int line);

#ifndef NDEBUG

#define LOG(tag, str) LogCall(tag, str) 

#define ERROR(str) ErrorLog(str, __FILE__, __LINE__)

#define ASSERT(expr) \
	GLClearError(); \
	do \
	{ \
		if (!(expr)) \
		{ \
			LogAssert(#expr, __FILE__, __LINE__); \
		} \
	} \
	while (0) \
	
#else  // NDEBUG is defined
// These are the release mode definitions for the macros above.  They are all defined in such a way as to be 
// ignored completely by the compiler.
#define ERROR(str) do { (void)sizeof(str); } while(0) 
#define GCC_WARNING(str) do { (void)sizeof(str); } while(0) 
#define GCC_INFO(str) do { (void)sizeof(str); } while(0) 
#define LOG(tag, str) do { (void)sizeof(tag); (void)sizeof(str); } while(0) 
#define ASSERT(expr) do { (void)sizeof(expr); } while(0) 
#endif  // !defined NDEBUG