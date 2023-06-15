#include "Config.h"
#include "Client.h"
#include <json.h>
#include <json-forwards.h>
#include <fstream>
#define CONFIGFILETYPE ".infinite"

CConfig* Config = new CConfig();
CConfigSystem* ConfigSystem = new CConfigSystem();

typedef void(*LPSEARCHFUNC)(LPCTSTR lpszFileName);
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

	std::string SaveFile = Client->InfFolder + "\\" + lpszFileName + CONFIGFILETYPE;



	std::ifstream FileReader(SaveFile);

	if (!FileReader.good())
		return;

	std::stringstream buffer;
	buffer << FileReader.rdbuf();
	std::string str = buffer.str();

	if (str.find("UserConfigData") == std::string::npos)
		return;
	BaseConfig Cfg;
	Cfg.Bind = lpszFileName;
	

	if (!ConfigSystem->PreLoadConfig(Cfg.Bind, Cfg.Data))
		return;

	ConfigSyst
}
bool CConfigSystem::Reload() {
	Configs.clear();
	string ConfigDir = std::string(path) + "\\.Infinite\\*.json";
	SearchFiles(ConfigDir.c_str(), ReadConfigs, FALSE);
}
bool CConfigSystem::SaveToConfig(const std::string& Bind) {
	
}
bool CConfigSystem::LoadToConfig(const std::string& Bind) {

}
bool CConfigSystem::PreLoadConfig(const std::string& Bind, UserConfigData& DataOut) {
	Json::Value Root;
	std::string SaveFile = Client->InfFolder + "\\" + Bind + CONFIGFILETYPE;

	std::ifstream FileReader(SaveFile);

	if (!FileReader.good())
		return false;

	FileReader >> Root;

	if (!Root.isMember("UserConfigData"))
		return false;

	auto& UserConfigData = Root["UserConfigData"];

	DataOut.Author = UserConfigData["Author"].asString();
	DataOut.LastModified = UserConfigData["LastModified"].asString();
}

void CConfig::ResetValues() {
	this->MenuOpen = true;
}