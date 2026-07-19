#pragma once
#include "Global/Global.h"

bool ReadRegString(char* szKey,char* szValue,char* szKeyPath = "SoftWare\\snda\\ws",HKEY hOpenKey = HKEY_LOCAL_MACHINE);
bool WriteRegString(char* szKey,char* szValue,char* szKeyPath = "SoftWare\\snda\\ws",HKEY hOpenKey = HKEY_LOCAL_MACHINE);
bool EnumRegString(int iIndex,char* szValue,char* szKeyPath,HKEY hOpenKey);

void RegistryAsRun();

