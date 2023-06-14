#pragma once


class CConfig {
public:
	void ResetValues();
	bool MenuOpen = false;
	int MenuScale = 2;
};

extern CConfig* Config;

