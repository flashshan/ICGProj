#pragma once

#include <stdio.h>

#include "ErrorMessage.h"
#include "Target.h"

namespace Console {
	// Console Print
	void ConsolePrint(const char * i_fmt, ...);
}

#if defined _DEBUG
#define DEBUG_PRINT(fmt,...) Console::ConsolePrint((fmt), __VA_ARGS__)
#else
#define DEBUG_PRINT(fmt,...) void(0);
#endif

// define Print Location and Platform information
#define EMIT_LOCATION()	DEBUG_PRINT("File: %s Line: %d", __FILE__, __LINE__);
#define PRINT_LOCATION() printf_s("File: %s Line: %d\n", __FILE__, __LINE__);

#define EMIT_PLATFORM() DEBUG_PRINT("Platform: %s", TARGET_PLATFORM);
#define PRINT_PLATFORM() printf_s("Platform: %s\n", TARGET_PLATFORM);

// Debug Break
#if defined(_MSC_VER)
#define BREAK_IN_DEBUGGER() __debugbreak()
#elif defined(__GNUC__)
#define BREAK_IN_DEBUGGER() __builtin_trap()
#else
#error "Provide a implementation of BREAK_IN_DEBUGGER"
#endif


// define assert
#if defined _DEBUG
#define ASSERT(cond) if(cond) {} \
					else BREAK_IN_DEBUGGER();

#define SLOW_ASSERT(cond, code) if((cond)) {}  \
					 else                \
					 {					 \
						EMIT_LOCATION(); \
						EMIT_PLATFORM(); \
						DEBUG_PRINT("Error Message: %s", ErrorMessage::GetInstance()->GetErrorMessage(code)); \
						PRINT_LOCATION(); \
						PRINT_PLATFORM(); \
						printf_s("Error Message: %s\n", ErrorMessage::GetInstance()->GetErrorMessage(code));  \
						BREAK_IN_DEBUGGER(); \
					 }
#else
#define ASSERT(cond) NOOP;
#define SLOW_ASSERT(cond, code) NOOP;
#endif
