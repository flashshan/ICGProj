#pragma once

#include <map>

#include "BasicTypes.h"

enum class ErrorType : int32 {
	// Pointer issue
	ENullPointer = 1001,

	// Memory issue
	EHeapOverflow = 1101,
	EStackOverflow = 1102,

	// Value issue
	ENonPostiveValue = 2001,
	ENonZeroValue = 2002,
};


// singleton class
class ErrorMessage {

#pragma region Singleton
public:
	static FORCEINLINE ErrorMessage *CreateInstance();
	static FORCEINLINE ErrorMessage *GetInstance();
	static FORCEINLINE void DestroyInstance();
private:
	static ErrorMessage *globalInstance_;
#pragma endregion

public:
	inline ~ErrorMessage();

	FORCEINLINE const char *GetErrorMessage(ErrorType i_errorCode) const;

private:
	FORCEINLINE ErrorMessage();
	FORCEINLINE ErrorMessage(const ErrorMessage &i_other) {}
	FORCEINLINE ErrorMessage& operator=(const ErrorMessage &i_other) {}


	void initialMessage();

private:
	std::map<int32, char *> messages_;
};




// implement forceinline


#pragma region Implement Singleton
FORCEINLINE ErrorMessage *ErrorMessage::CreateInstance()
{
	ErrorMessage::globalInstance_ = new ErrorMessage();
	return ErrorMessage::globalInstance_;
}

FORCEINLINE ErrorMessage* ErrorMessage::GetInstance()
{
	if (ErrorMessage::globalInstance_ == nullptr)
		ErrorMessage::globalInstance_ = new ErrorMessage();
	return ErrorMessage::globalInstance_;
}

FORCEINLINE void ErrorMessage::DestroyInstance()
{
	//ASSERT(ErrorMessage::globalInstance_ != nullptr);
	delete ErrorMessage::globalInstance_;
	ErrorMessage::globalInstance_ = nullptr;
}
#pragma endregion


FORCEINLINE ErrorMessage::ErrorMessage()
{
	initialMessage();
}

FORCEINLINE const char* ErrorMessage::GetErrorMessage(ErrorType i_errorCode) const
{
	auto it = messages_.find(static_cast<int32>(i_errorCode));

	if (it == messages_.end())
	{
		return "Wrong ErrorCode";
	}
	else
	{
		return it->second;
	}
}

inline ErrorMessage::~ErrorMessage()
{
	messages_.clear();
}