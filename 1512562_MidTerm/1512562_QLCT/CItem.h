#pragma once
#include <string>
using namespace std;
class CItem
{
public:
	WCHAR mType[20];
	unsigned long long mMoney;
	std::wstring mDescription;
	CItem();
	~CItem();
};

