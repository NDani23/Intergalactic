#pragma once

#include <sstream>
#include <iostream>
#include <string>

class CMyApp;

class Persistence
{
private:
	CMyApp* m_app;

public:
	Persistence();
	Persistence(CMyApp*);

	void Save();
	bool Load();
};
