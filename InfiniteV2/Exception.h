#pragma once
#include <exception>

#include <string>

#include "BuildConfig.h"
#ifdef INFINITE_SHOW_ERRORS

class IException : std::exception {
public:
	IException(const char* szException, unsigned long iErrorCode);
	~IException();

	const char* Throw() const throw() { return this->m_szException; }

private:
	const char* m_szException;
	unsigned long m_iErrorCode;
};

class IExit {
public:
	IExit(const char* szText);
};

#else
class Exception {


public:

	Exception(const std::string& str) {
		
	}
	Exception(std::string&& str) {
	
	}
	~Exception() {
	
	}


};

#endif

