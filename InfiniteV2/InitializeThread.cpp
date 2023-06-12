#include "InitializeThread.h"
#include "Client.h"
#include "Interfaces.h"
unsigned long __stdcall InitializeThread(void*) {

	Client->Initialize();
	
	return 0;
}