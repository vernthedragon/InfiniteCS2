#pragma once
#include <cstdint>
#include <string>
#include <map>
#include <fstream>

enum ConfigType : signed char {
	VartypeBool = 0,
	VartypeInt = 1,
	VartypeUInt = 2,
	VartypeColor = 3,
	VartypeString = 4
};

struct ConfigVariable {
	ConfigType Type;
	void* Var;
};
struct UserConfigData {
	std::string LastModified;
	std::string Author;
	std::time_t UNIXStamp;
	//add the creator ID + user ID (required for cloud later)
};
class BaseConfig {
public:
	std::string Bind;
	bool Delete();
	bool Rename(const std::string& NewBindedName);
	bool Load();
	bool Save();
	UserConfigData Data;
};
class CConfig {
public:
	void ResetValues();
	bool MenuOpen = false;
	int MenuScale = 2;
	BaseConfig Base;
};

class CConfigSystem {
public:
	bool Reload();
	bool RemoveConfig(const std::string& Bind);
	bool SaveToConfig(const std::string& Bind);
	bool LoadToConfig(const std::string& Bind);
	bool PreLoadConfigFile(std::ifstream& stream, UserConfigData& DataOut);
	bool CreateConfig(const std::string& Name);
	void AddVar(const std::string& Bind, bool* Var) {
		ConfigVariable* variable = new ConfigVariable{};
		variable->Var = reinterpret_cast<void*>(Var);
		variable->Type = VartypeBool;
		Variables[Bind] = variable;
	}
	void AddVar(const std::string& Bind, int* Var) {
		ConfigVariable* variable = new ConfigVariable{};
		variable->Var = reinterpret_cast<void*>(Var);
		variable->Type = VartypeInt;
		Variables[Bind] = variable;
	}
	void AddVar(const std::string& Bind, unsigned int* Var) {
		ConfigVariable* variable = new ConfigVariable{};
		variable->Var = reinterpret_cast<void*>(Var);
		variable->Type = VartypeUInt;
		Variables[Bind] = variable;
	}
	void AddVar(const std::string& Bind, class Color* Var) {
		ConfigVariable* variable = new ConfigVariable{};
		variable->Var = reinterpret_cast<void*>(Var);
		variable->Type = VartypeColor;
		Variables[Bind] = variable;
	}
	void AddVar(const std::string& Bind, std::string* Var) {
		ConfigVariable* variable = new ConfigVariable{};
		variable->Var = reinterpret_cast<void*>(Var);
		variable->Type = VartypeString;
		Variables[Bind] = variable;
	}
	bool RemoveVar(const std::string& Bind) {
		if (auto Var = Variables.find(Bind); Var != Variables.end()) {
			Variables.erase(Bind);
			return true;
		}
		return false;
	}
	std::map< std::string, BaseConfig > Configs;
	std::map< std::string, ConfigVariable* > Variables;
};

extern CConfigSystem* ConfigSystem;
extern CConfig* Config;

