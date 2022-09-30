#include "stdafx.h"
#include "Global.h"
#include "AcceptServer.h"
#include "DispathServer.h"
#include "GarbageCleaner.h"
#include "ConsolePipe.h"
#include "ResponseServer.h"
#include "ServiceWorkTime.h"

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Russian"); 
	SetConsoleTitle("Concurrent Server");
	cout << "������ �������:" << endl;
	cout << "��� �����: DESKTOP-LM80FP0" << endl;
	
	//������������� ����� tcp-�����, ���� ��� �� ���������� ����� ��� �������� ���������� �������� �� ���������
	if (argc > 1)
	{
		int tmp = atoi(argv[1]);
		if (tmp >= 0 && tmp <= 65535)
		{
			port = atoi(argv[1]);
			cout << "���� � �������:   " << port << ";" << endl;

		}
		else
		{
			cout << "������������ ����" << endl;
		}
	}
	else
	{
		cout << "���� �� ���������:   " << port  << endl;
	}

	
	if (argc > 2)
	{
		int tmp = atoi(argv[2]);
		if (tmp >= 0 && tmp <= 65535)
		{
			uport = atoi(argv[2]);
		}
	}


	//������������� ��� ����������� ����������
	if (argc > 3)
	{
		dllname = argv[3];
	}

	//������������� ��� ������������ ������
	if (argc > 4)
	{
		npname = argv[4];
		cout << "��� ������:   " << npname << ";" << endl;
	}
	else
	{
		cout << "��� ������ : " << npname << ";" << endl;
	}

	//������������� �������� �������
	if (argc > 5)
	{
		ucall = argv[5];
		cout << "��������:   " << ucall << ";" << endl;
	}
	else
	{
		cout << "��������:        " << ucall << ";" << endl;
	}
	srand((unsigned)time(NULL));

	volatile TalkersCmd cmd = Start;			//������� ���������� ��������

	InitializeCriticalSection(&scListContact);

	echoServer = LoadLibrary("EchoServer.dll");
	execEcho = (HANDLE(*)(LPVOID))GetProcAddress(echoServer, "Echo");

	randServer = LoadLibrary("RandServer.dll");
	execRand = (HANDLE(*)(LPVOID))GetProcAddress(randServer, "Rand");

	timeServer = LoadLibrary("TimeServer.dll");
	execTime = (HANDLE(*)(LPVOID))GetProcAddress(timeServer, "Time");


	//st1 = LoadLibrary("Servicelibrary.dll"); 
	//ts1 = (HANDLE(*)(char*, LPVOID))GetProcAddress(st1, "SSS"); //����������� �������
	//if (st1 == NULL) cout << "Fail DLL;" << endl;
	//else cout << "Loaded DLL:      " << dllname << ";" << endl << endl;

	//������� ������
	hAcceptServer = CreateThread(NULL, NULL, AcceptServer, (LPVOID)&cmd, NULL, NULL);
	HANDLE hDispathServer = CreateThread(NULL, NULL, DispathServer, (LPVOID)&cmd, NULL, NULL);
	HANDLE hGarbageCleaner = CreateThread(NULL, NULL, GarbageCleaner, (LPVOID)&cmd, NULL, NULL);
	HANDLE hConsolePipe = CreateThread(NULL, NULL, ConsolePipe, (LPVOID)&cmd, NULL, NULL);
	HANDLE hResponseServer = CreateThread(NULL, NULL, ResponseServer, (LPVOID)&cmd, NULL, NULL);
	//HANDLE hServiceWorkTime = CreateThread(NULL, NULL, ServiceWorkTime, (LPVOID)&cmd, NULL, NULL);

	//������������� ����������
	SetThreadPriority(hGarbageCleaner, THREAD_PRIORITY_BELOW_NORMAL);
	SetThreadPriority(hDispathServer, THREAD_PRIORITY_NORMAL);
	SetThreadPriority(hConsolePipe, THREAD_PRIORITY_NORMAL);
	SetThreadPriority(hResponseServer, THREAD_PRIORITY_ABOVE_NORMAL);
	SetThreadPriority(hAcceptServer, THREAD_PRIORITY_HIGHEST);
	//SetThreadPriority(hServiceWorkTime, THREAD_PRIORITY_HIGHEST);


	//���� ���������� �������
	WaitForSingleObject(hAcceptServer, INFINITE);
	WaitForSingleObject(hDispathServer, INFINITE);
	WaitForSingleObject(hGarbageCleaner, INFINITE);
	WaitForSingleObject(hConsolePipe, INFINITE);
	WaitForSingleObject(hResponseServer, INFINITE);
	//WaitForSingleObject(hServiceWorkTime, INFINITE);

	//��������� ����������� (����������� �������)
	CloseHandle(hAcceptServer);
	CloseHandle(hDispathServer);
	CloseHandle(hGarbageCleaner);
	CloseHandle(hConsolePipe);
	CloseHandle(hResponseServer);
	//CloseHandle(hServiceWorkTime);

	DeleteCriticalSection(&scListContact);

	FreeLibrary(echoServer);
	FreeLibrary(timeServer);
	FreeLibrary(randServer);

	system("pause");
	return 0;
};