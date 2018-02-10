#include "Core\Basic\Assert.h"

#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

namespace Console {

	void ConsolePrint(const char * i_fmt, ...)
	{
		const size_t lenTemp = 480;
		char strTemp[lenTemp] = "DEBUG: ";

		strcat_s(strTemp, i_fmt);
		strcat_s(strTemp, "\n");

		const size_t lenOutput = lenTemp + 32;

		char strOutput[lenOutput];

		va_list args;

		va_start(args, i_fmt);

		vsprintf_s(strOutput, lenOutput, strTemp, args);

		va_end(args);

		OutputDebugStringA(strOutput);
	}
}