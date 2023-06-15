#include "Config.h"
#include "Client.h"
#include <json.h>
#include <json-forwards.h>
#include <fstream>
#include <time.h>
#define CONFIGFILETYPE ".infinite"

CConfig* Config = new CConfig();
CConfigSystem* ConfigSystem = new CConfigSystem();

typedef void(*LPSEARCHFUNC)(LPCTSTR lpszFileName);
//THIS FUNCTION IS UNSAFE WARNING! IT CAN CAUSE BUFFER OVERFLOW ATTACK (_CRT_SECURE_NO_WARNINGS) IS ENABLED
BOOL SearchFiles(LPCTSTR lpszFileName, LPSEARCHFUNC lpSearchFunc, BOOL bInnerFolders = FALSE)
{
	LPTSTR part;
	char tmp[MAX_PATH];
	char name[MAX_PATH];

	HANDLE hSearch = NULL;
	WIN32_FIND_DATA wfd;
	memset(&wfd, 0, sizeof(WIN32_FIND_DATA));

	if (bInnerFolders)
	{
		if (GetFullPathName(lpszFileName, MAX_PATH, tmp, &part) == 0) return FALSE;
		strcpy(name, part);
		strcpy(part, "*.*");
		wfd.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
		if (!((hSearch = FindFirstFile(tmp, &wfd)) == INVALID_HANDLE_VALUE))
			do
			{
				if (!strncmp(wfd.cFileName, ".", 1) || !strncmp(wfd.cFileName, "..", 2))
					continue;

				if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					char next[MAX_PATH];
					if (GetFullPathName(lpszFileName, MAX_PATH, next, &part) == 0) return FALSE;
					strcpy(part, wfd.cFileName);
					strcat(next, "\\");
					strcat(next, name);

					SearchFiles(next, lpSearchFunc, TRUE);
				}
			} while (FindNextFile(hSearch, &wfd));
			FindClose(hSearch);
	}

	if ((hSearch = FindFirstFile(lpszFileName, &wfd)) == INVALID_HANDLE_VALUE)
		return TRUE;
	do
		if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			char file[MAX_PATH];
			if (GetFullPathName(lpszFileName, MAX_PATH, file, &part) == 0) return FALSE;
			strcpy(part, wfd.cFileName);

			lpSearchFunc(wfd.cFileName);
		}
	while (FindNextFile(hSearch, &wfd));
	FindClose(hSearch);
	return TRUE;
}
void ReadConfigs(LPCTSTR lpszFileName)
{

	std::string File = Client->InfFolder + "\\" + lpszFileName + CONFIGFILETYPE;



	std::ifstream FileReader(File);

	if (!FileReader.good()) {
		FileReader.close();
		return;
	}

	BaseConfig Cfg;
	Cfg.Bind = lpszFileName;
	

	if (!ConfigSystem->PreLoadConfigFile(FileReader, Cfg.Data))
		return;

	ConfigSystem->Configs[Cfg.Bind] = Cfg;
}
bool BaseConfig::Delete() {
	std::string File = Client->InfFolder + "\\" + this->Bind + CONFIGFILETYPE;
	bool res = std::remove(File.c_str());

	if (!ConfigSystem->RemoveConfig(this->Bind))
		return false;

	return res;
}
bool BaseConfig::Rename(const std::string& NewBindedName) {
	std::string OldFile = Client->InfFolder + "\\" + this->Bind + CONFIGFILETYPE;
	std::string NewFile = Client->InfFolder + "\\" + NewBindedName + CONFIGFILETYPE;
	this->Bind = NewBindedName;
	return std::rename(OldFile.c_str(), NewFile.c_str()) > 0;
}
bool BaseConfig::Load() {
	return ConfigSystem->LoadToConfig(this->Bind);
}
bool BaseConfig::Save() {
	return ConfigSystem->SaveToConfig(this->Bind);
}
bool CConfigSystem::RemoveConfig(const std::string& Bind) {
	if (auto Cfg = Configs.find(Bind); Cfg != Configs.end()) {
		Configs.erase(Cfg);
		return true;
	}
	return false;
}
bool CConfigSystem::Reload() {
	Configs.clear();
	std::string Search = Client->InfFolder + "\\*" + CONFIGFILETYPE;
	return SearchFiles(Search.c_str(), ReadConfigs, TRUE);
}
bool CConfigSystem::SaveToConfig(const std::string& Bind) {
	Json::Value Root;
	std::time_t UnixTimeStamp = std::time(nullptr);
	Root["UserConfigData"]["Author"] = Client->UserData.Username;
	Root["UserConfigData"]["UNIXStamp"] = (unsigned int)UnixTimeStamp;
	std::tm ptm;
	localtime_s(&ptm, &UnixTimeStamp);
	char buffer[32];
	// Format: Mo, 15.06 2009 20:20
	std::strftime(buffer, 32, "%a, %d.%m.%Y %H:%M", &ptm);
	Root["UserConfigData"]["LastModified"] = std::string(buffer);


	for (auto& Var : Variables) {
		if (Var.second->Type == VartypeColor) {
			Root["Configuration"][Var.first] = (unsigned int)reinterpret_cast<Col*>(Var.second->Var)->u32();
		}
		else if (Var.second->Type == VartypeBool) {
			Root["Configuration"][Var.first] = *reinterpret_cast<bool*>(Var.second->Var);
		}
		else if (Var.second->Type == VartypeInt) {
			Root["Configuration"][Var.first] = *reinterpret_cast<int*>(Var.second->Var);
		}
		else if (Var.second->Type == VartypeUInt) {
			Root["Configuration"][Var.first] = *reinterpret_cast<unsigned int*>(Var.second->Var);
		}
		else if (Var.second->Type == VartypeString) {
			Root["Configuration"][Var.first] = *reinterpret_cast<std::string*>(Var.second->Var);
		}
	}
	std::string File = Client->InfFolder + "\\" + Bind + CONFIGFILETYPE;
	std::ofstream FileWriter(File);

	if (!FileWriter.good()) {
		FileWriter.close();
		return false;
	}

	FileWriter << Root;
	FileWriter.close();
	Root.clear();
	return true;
}
bool CConfigSystem::CreateConfig(const std::string& Name) {
	CConfig TempConfig;
	memcpy(&TempConfig, Config, sizeof(CConfig));
	Config->ResetValues();
	bool Success = SaveToConfig(Name);
	memcpy(Config, &TempConfig, sizeof(CConfig));
	if (!this->Reload())
		return false;

	return Success;
}
bool CConfigSystem::LoadToConfig(const std::string& Bind) {
	std::string File = Client->InfFolder + "\\" + Bind + CONFIGFILETYPE;



	std::ifstream FileReader(File);

	if (!FileReader.good()) {
		FileReader.close();
		return false;
	}

	
	Json::Value Root;
	FileReader >> Root;

	if (!ConfigSystem->PreLoadConfigFile(FileReader, Config->Base.Data))
		return false;


	Config->Base.Bind = Bind;

	for (auto& Var : Variables) {
		if (Var.second->Type == VartypeColor) {
			if (Root["Configuration"][Var.first].type() != Json::ValueType::uintValue)
			{
				Client->Log("Failed to Load ConfigVar (type missmatch): ");
				Client->Log(Var.first.c_str());
				Client->Log("\n");
				continue;
			}
			reinterpret_cast<Col*>(Var.second->Var)->Setu32(Root["Configuration"][Var.first].asUInt());
		}
		else if (Var.second->Type == VartypeBool) {
			if (Root["Configuration"][Var.first].type() != Json::ValueType::booleanValue)
			{
				Client->Log("Failed to Load ConfigVar (type missmatch): ");
				Client->Log(Var.first.c_str());
				Client->Log("\n");
				continue;
			}
			*reinterpret_cast<bool*>(Var.second->Var) = Root["Configuration"][Var.first].asBool();
		}
		else if (Var.second->Type == VartypeInt) {
			if (Root["Configuration"][Var.first].type() != Json::ValueType::intValue)
			{
				Client->Log("Failed to Load ConfigVar (type missmatch): ");
				Client->Log(Var.first.c_str());
				Client->Log("\n");
				continue;
			}
			*reinterpret_cast<int*>(Var.second->Var) = Root["Configuration"][Var.first].asInt();
		}
		else if (Var.second->Type == VartypeUInt) {
			if (Root["Configuration"][Var.first].type() != Json::ValueType::uintValue)
			{
				Client->Log("Failed to Load ConfigVar (type missmatch): ");
				Client->Log(Var.first.c_str());
				Client->Log("\n");
				continue;
			}
			*reinterpret_cast<unsigned int*>(Var.second->Var) = Root["Configuration"][Var.first].asUInt();
		}
		else if (Var.second->Type == VartypeString) {
			if (Root["Configuration"][Var.first].type() != Json::ValueType::stringValue)
			{
				Client->Log("Failed to Load ConfigVar (type missmatch): ");
				Client->Log(Var.first.c_str());
				Client->Log("\n");
				continue;
			}
			*reinterpret_cast<std::string*>(Var.second->Var) = Root["Configuration"][Var.first].asString();
		}
	}

	Root.clear();
	return true;
}
bool CConfigSystem::PreLoadConfigFile(std::ifstream& stream, UserConfigData& DataOut) {
	Json::Value Root;
	

	stream >> Root;

	if (!Root.isMember("UserConfigData") || !Root.isMember("Configuration")) {
		stream.close();
		Root.clear();
		return false;
	}

	auto& UserConfigData = Root["UserConfigData"];

	DataOut.Author = UserConfigData["Author"].asString();
	DataOut.LastModified = UserConfigData["LastModified"].asString();
	DataOut.UNIXStamp = UserConfigData["UNIXStamp"].asLargestUInt();

	stream.close();
	Root.clear();
	return true;
}

void CConfig::ResetValues() {
	this->MenuOpen = true;
	this->MenuScale = 2;
}