﻿// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include "Windows.h"
#include "RandServer.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    return TRUE;
}

extern "C" __declspec(dllexport) HANDLE Rand(LPVOID param) {
    return CreateThread(NULL, NULL, RandServer, param, NULL, NULL);
}
