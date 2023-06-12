#pragma once
#include "Vectors.h"
#include "Matrix.h"


#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <psapi.h>
#include <time.h>
#include <process.h>
#include <vector>
#include <deque>
#include <map>
#include <ostream>
#include <Shlobj.h>
#include <stdint.h>
#include <string>
#include <string.h>
#include <cmath>
#include <float.h>
#include <codecvt>
#include <sapi.h>
#include <algorithm>
#include <iterator>
#include <d3d9.h>
#include <rpcndr.h>

class CClient {
public:
	void Initialize();
	void Close();
	void Log(const char* message...);
	std::uint8_t* FindPattern(HMODULE Module, const char* Signature);
	std::uint8_t* ResolveRIP(std::uint8_t* address, std::uint32_t rva_offset, std::uint32_t rip_offset);
};

extern CClient* Client;