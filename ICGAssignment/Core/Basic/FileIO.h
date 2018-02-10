#pragma once

#include "ASSERT.h"

inline uint8_t * LoadFile(const char *i_pFilename, size_t &o_sizeFile);






// implement inline
inline uint8_t * LoadFile(const char *i_pFilename, size_t &o_sizeFile)
{
	ASSERT(i_pFilename != nullptr);

	FILE *pFile = nullptr;

	errno_t fopenError = fopen_s(&pFile, i_pFilename, "rb");
	if (fopenError != 0)
		return nullptr;

	ASSERT(pFile != nullptr);

	int FileIOError = fseek(pFile, 0, SEEK_END);
	ASSERT(FileIOError == 0);

	long FileSize = ftell(pFile);
	ASSERT(FileSize >= 0);

	FileIOError = fseek(pFile, 0, SEEK_SET);
	ASSERT(FileIOError == 0);

	uint8_t * pBuffer = new uint8_t[FileSize];
	ASSERT(pBuffer);

	size_t FileRead = fread(pBuffer, 1, FileSize, pFile);
	ASSERT(FileRead == FileSize);

	fclose(pFile);

	o_sizeFile = FileSize;

	return pBuffer;
}

