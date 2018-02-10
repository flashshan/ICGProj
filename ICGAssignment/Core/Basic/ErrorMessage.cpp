#include "Core\Basic\ErrorMessage.h"

ErrorMessage* ErrorMessage::globalInstance_ = nullptr;

void ErrorMessage::initialMessage()
{
	messages_.insert(std::pair<int32, char*>(static_cast<int32>(ErrorType::ENullPointer), "Pointer point to NULL"));
	
	messages_.insert(std::pair<int32, char*>(static_cast<int32>(ErrorType::EHeapOverflow), "Heap Over Flow"));
	messages_.insert(std::pair<int32, char*>(static_cast<int32>(ErrorType::EStackOverflow), "Stack Over Flow"));

	messages_.insert(std::pair<int32, char*>(static_cast<int32>(ErrorType::ENonPostiveValue), "Parameter Value below zero"));
	messages_.insert(std::pair<int32, char*>(static_cast<int32>(ErrorType::ENonZeroValue), "Parameter value is not zero"));

}
