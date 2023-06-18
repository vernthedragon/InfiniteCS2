#pragma once
#include <cstdint>
#include <string>
#include <map>
#include <fstream>
#include <vector>
enum ConfigType : signed char {
	VartypeBool = 0,
	VartypeInt = 1,
	VartypeUInt = 2,
	VartypeColor = 3,
	VartypeString = 4
};
enum BindParentType : signed char {
	BindtypeUnBinded = 0,
	BindtypeSwitch = 1,
	BindtypeSlider = 2,
	BindtypeSelect = 3,
	BindtypeMultiselect = 4
};
enum BindType : int {
	BindOff = 0,
	BindHold = 1,
	BindToggle = 2,
	BindRelease = 3
};

typedef bool SwitchBind;

struct SliderBind {
	int NewValue;
	int OldValue;
};

typedef SliderBind SelectBind;
struct MultiSelectBind {
	unsigned int NewValue;
	unsigned int OldValue;
};

struct Bind_t {
	void* Data;
	int VKey;
	BindParentType ParentType;
	int Type;
	std::string Bind;
};
struct ConfigVariable {
	ConfigType Type;
	void* Var;
	Bind_t Bind; //in the future we can add supprt for multibinding
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
	bool Load();
	bool Save();
	UserConfigData Data;
};
class CConfig {
public:
	void ResetValues();
	struct Movement_t {
		bool Bunnyhop = false;
		int BunnyhopStrafeType = 0;
	};

	Movement_t Movement;

	struct MenuSettings_t {
		int AnimationSpeed = 100.f;
	};
	MenuSettings_t Menu;
	bool MenuOpen = false;
	int MenuScale = 2;
	bool AutoSave = false;
	bool DisableComplexAnimations = false;
	BaseConfig Base;
};

class CConfigSystem {
public:
	bool Reload();
	bool RemoveConfig(const std::string& Bind);
	bool SaveToConfig(const std::string& Bind);
	bool LoadToConfig(const std::string& Bind);
	void RebindConfig(const std::string& OldBind, const std::string& NewBind);
	bool PreLoadConfigFile(std::ifstream& stream, UserConfigData& DataOut);
	bool CreateConfig(const std::string& Name);
	ConfigVariable* BindBindWithVar(const std::string& Bind, BindParentType type) {
		ConfigVariable* variable = Variables[Bind];

		if (variable->Bind.ParentType != BindtypeUnBinded) {
			return variable; //dont rebind
		}

		variable->Bind.ParentType = type;
		variable->Bind.Type = BindType::BindOff;
		variable->Bind.VKey = -1;
		variable->Bind.Data = nullptr;
		variable->Bind.Bind = Bind;
		if (type == BindtypeSlider || type == BindtypeSelect) {
			variable->Bind.Data = new SliderBind{};
		}
		else if (type == BindtypeMultiselect) {
			variable->Bind.Data = new MultiSelectBind{};
		}
		BindedVariables.push_back(variable);
		return variable;
	}
	void AddVar(const std::string& Bind, bool* Var) {

		if (Bind == "Binded Key")
			return;
		if (Bind == "Bind Type")
			return;
		if (Bind == "Bind Off Value")
			return;
		if (Bind == "Bind On Value")
			return;

		ConfigVariable* variable = new ConfigVariable{};
		variable->Var = reinterpret_cast<void*>(Var);
		variable->Type = VartypeBool;
		variable->Bind.ParentType = BindtypeUnBinded;
		Variables[Bind] = variable;
	}
	void AddVar(const std::string& Bind, int* Var) {
		if (Bind == "Binded Key")
			return;
		if (Bind == "Bind Type")
			return;
		if (Bind == "Bind Off Value")
			return;
		if (Bind == "Bind On Value")
			return;
		ConfigVariable* variable = new ConfigVariable{};
		variable->Var = reinterpret_cast<void*>(Var);
		variable->Type = VartypeInt;
		variable->Bind.ParentType = BindtypeUnBinded;
		Variables[Bind] = variable;
	}
	void AddVar(const std::string& Bind, unsigned int* Var) {
		if (Bind == "Binded Key")
			return;
		if (Bind == "Bind Type")
			return;
		if (Bind == "Bind Off Value")
			return;
		if (Bind == "Bind On Value")
			return;
		ConfigVariable* variable = new ConfigVariable{};
		variable->Var = reinterpret_cast<void*>(Var);
		variable->Type = VartypeUInt;
		variable->Bind.ParentType = BindtypeUnBinded;
		Variables[Bind] = variable;
	}
	void AddVar(const std::string& Bind, class Color* Var) {
		if (Bind == "Binded Key")
			return;
		if (Bind == "Bind Type")
			return;
		if (Bind == "Bind Off Value")
			return;
		if (Bind == "Bind On Value")
			return;
		ConfigVariable* variable = new ConfigVariable{};
		variable->Var = reinterpret_cast<void*>(Var);
		variable->Type = VartypeColor;
		variable->Bind.ParentType = BindtypeUnBinded;
		Variables[Bind] = variable;
	}
	void AddVar(const std::string& Bind, std::string* Var) {
		if (Bind == "Binded Key")
			return;
		if (Bind == "Bind Type")
			return;
		if (Bind == "Bind Off Value")
			return;
		if (Bind == "Bind On Value")
			return;
		ConfigVariable* variable = new ConfigVariable{};
		variable->Var = reinterpret_cast<void*>(Var);
		variable->Type = VartypeString;
		variable->Bind.ParentType = BindtypeUnBinded;
		Variables[Bind] = variable;
	}
	bool RemoveVar(const std::string& Bind) {
		if (auto Var = Variables.find(Bind); Var != Variables.end()) {
			if (Var->second->Bind.Data) {
				if (Var->second->Bind.ParentType == BindtypeMultiselect) {
					delete (MultiSelectBind*)Var->second->Bind.Data;
				}
				else
				{
					delete (SliderBind*)Var->second->Bind.Data;
				}
			}
			delete Var->second;
			Variables.erase(Var);
			return true;
		}
		return false;
	}
	bool VarExists(const std::string& Bind) {
		return Variables.find(Bind) != Variables.end();
	}
	std::string Loaded;
	std::map< std::string, BaseConfig > Configs;
	std::map< std::string, ConfigVariable* > Variables;
	std::vector < ConfigVariable* > BindedVariables;
};

extern CConfigSystem* ConfigSystem;
extern CConfig* Config;

