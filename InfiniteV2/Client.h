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
#include <thread>
#include <chrono>
#include "Config.h"
#include "Exception.h"
struct InfUserData {
	std::string Username;
};
struct CLocalPlayerCache {
	Vec3 Velocity;
	float VelocityLength;
	float VelocityLength2D;
	bool Alive;
};
class CClient {
public:
	void Initialize();
	bool SetupHooks();
	bool Hook(LPVOID Target, LPVOID Detour, LPVOID* OutOriginal, const char* Name);
	void Close();
	void Log(const char* message...);
	void UpdateKeyStates();
	bool KeyPressed( int VKEY);
	bool KeyToggled( int VKEY);
	std::uint8_t* FindPattern(HMODULE Module, const char* Signature);
	std::uint8_t* ResolveRIP(std::uint8_t* address, std::uint32_t rva_offset, std::uint32_t rip_offset);
	void UpdateLocal();
	std::string InfFolder;
	InfUserData UserData;
	class UserCmd* cmd;
	class IPlayer* local;
	class IController* controller;
	class ConVar* cl_sidespeed;
	CLocalPlayerCache cache;
	int ScrollAmmount = 0;
	bool KeysPressed[256];
	bool KeyStates[256];
	bool OldKeyStates[256];
};

extern CClient* Client;