#include "Exception.h"
#include <Windows.h>

bool DoneExceptionOnce = false;
bool DoneTerminate = false;
IException::IException(const char* szException, DWORD iErrorCode) {
    this->m_iErrorCode = iErrorCode;
    this->m_szException = szException;
    printf(this->m_szException);
    printf("\n");
    if (!DoneExceptionOnce) {
        MessageBoxA(NULL, this->m_szException, "Infinite.dev", MB_OK | MB_ICONWARNING);
        DoneExceptionOnce = true;
    }
    if (!DoneTerminate) { DoneTerminate = true; TerminateProcess(GetCurrentProcess(), m_iErrorCode); }
}
IException::~IException() { if (!DoneTerminate) { DoneTerminate = true; TerminateProcess(GetCurrentProcess(), m_iErrorCode); } }

