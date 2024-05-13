#include "pch.h"

#include "Logger.h"
#include "common/AOBScanCore.h"
#include "../extensions/ExtensionMasks.h"

void* scan(const ExtensionRegister& extension)
{
	Logger::info("AOB length is %d", strlen(extension.mask));
	Logger::info("About to scan for access code...\n");
	void* invariant = AOBScanAddress(extension.aob, extension.mask);
	return invariant;
}
