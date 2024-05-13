#pragma once
#include <string>

struct ExtensionRegister
{
	const unsigned char* aob;
	const char* mask;
};

extern ExtensionRegister PLAYER_TARGET;
extern ExtensionRegister CREATE_BULLET;
extern ExtensionRegister CHARACTER_LIST;
extern ExtensionRegister ROOT_MOTION_REDUCTION_FACTOR_ACCESS;
