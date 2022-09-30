#include "stdafx.h"
#include "Winsock2.h"
#include "ErrorFunctions.h"
#include <string>
#include <list>
#include <time.h>
#include <iostream>

#define AS_SQ 10
using namespace std;

SOCKET sS;
int serverPort;
char dllName[50];
char namedPipeName[10];


volatile long connectionCount = 0;
volatile long sayNoCount = 0;
volatile long successConnections = 0;
volatile long currentActiveConnections = 0;


HANDLE hAcceptServer, hConsolePipe, hGarbageCleaner, hDispatchServer, hResponseServer;
HANDLE hClientConnectedEvent = CreateEvent(NULL,
	FALSE, //�������������� �����
	FALSE,
	L"ClientConnected");;

DWORD WINAPI AcceptServer(LPVOID pPrm);  // ��������� ������� 
DWORD WINAPI ConsolePipe(LPVOID pPrm);
DWORD WINAPI GarbageCleaner(LPVOID pPrm);
DWORD WINAPI DispatchServer(LPVOID pPrm);
DWORD WINAPI ResponseServer(LPVOID pPrm);

CRITICAL_SECTION scListContact;


enum TalkersCommand {
	START, STOP, EXIT, STATISTICS, WAIT, SHUTDOWN, GETCOMMAND
};
volatile TalkersCommand  previousCommand = GETCOMMAND;      // ���������� ������� ������� 


struct Contact         // ������� ������ �����������       
{
	enum TE {               // ���������  ������� �����������  
		EMPTY,              // ������ ������� ������ ����������� 
		ACCEPT,             // ��������� (accept), �� �� �������������
		CONTACT             // ������� �������������� �������  
	}    type;     // ��� �������� ������ ����������� 
	enum ST {               // ��������� �������������� �������  
		WORK,               // ���� ����� ������� � ��������
		ABORT,              // ������������� ������ ���������� �� ��������� 
		TIMEOUT,            // ������������� ������ ���������� �� ������� 
		FINISH              // ������������� ������ ����������  ��������� 
	}      sthread; // ���������  �������������� ������� (������)

	SOCKET      s;         // ����� ��� ������ ������� � ��������
	SOCKADDR_IN prms;      // ���������  ������ 
	int         lprms;     // ����� prms 
	HANDLE      hthread;   // handle ������ (��� ��������) 
	HANDLE      htimer;    // handle �������
	HANDLE		serverHThtead;// handle �������������� ������� ������� � ����������� ����� ���������

	char msg[50];           // ��������� 
	char srvname[15];       //  ������������ �������������� ������� 

	Contact(TE t = EMPTY, const char* namesrv = "") // ����������� 
	{
		ZeroMemory(&prms, sizeof(SOCKADDR_IN));
		lprms = sizeof(SOCKADDR_IN);
		type = t;
		strcpy(srvname, namesrv);
		msg[0] = 0x00;
	};

	void SetST(ST sth, const char* m = "")
	{
		sthread = sth;
		strcpy(msg, m);
	}
};
typedef list<Contact> ListContact;

ListContact contacts;

bool  GetRequestFromClient(char*name, short port, SOCKADDR_IN* from, int* flen);

bool AcceptCycle(int sq)
{
	bool rc = false;
	Contact c(Contact::ACCEPT, "EchoServer");
	while (sq-- > 0 && !rc)
	{
		if ((c.s = accept(sS, (sockaddr*)&c.prms, &c.lprms)) == INVALID_SOCKET)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
				throw  SetErrorMsgText("accept:", WSAGetLastError());
		}
		else
		{
			rc = true;               // �����������
			EnterCriticalSection(&scListContact);
			contacts.push_front(c);
			LeaveCriticalSection(&scListContact);
			puts("������ �����������");
			InterlockedIncrement(&connectionCount);
		}
	}
	return rc;
};

void openSocket() {
	SOCKADDR_IN serv;                     // ���������  ������ sS
	sockaddr_in clnt;
	u_long nonblk = 1;
	
	if ((sS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
		throw  SetErrorMsgText("socket:", WSAGetLastError());

	int lclnt = sizeof(clnt);
	serv.sin_family = AF_INET;           // ������������ IP-���������  
	serv.sin_port = htons(serverPort);          // ���� 2000
	serv.sin_addr.s_addr = INADDR_ANY; //inet_addr("192.168.0.111"); ;   // ����� ����������� IP-����� 

	if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
		throw  SetErrorMsgText("bind:", WSAGetLastError());
	if (listen(sS, SOMAXCONN) == SOCKET_ERROR)
		throw  SetErrorMsgText("listen:", WSAGetLastError());
	if (ioctlsocket(sS, FIONBIO, &nonblk) == SOCKET_ERROR)
		throw SetErrorMsgText("ioctlsocket:", WSAGetLastError());
}

void closeSocket() {
	if (closesocket(sS) == SOCKET_ERROR)
		throw  SetErrorMsgText("closesocket:", WSAGetLastError());
}

void CommandsCycle(TalkersCommand& cmd)      // ���� ��������� ������
{
	int  sq = 0;
	while (cmd != EXIT)     // ���� ��������� ������ ������� � �����������
	{
		switch (cmd)
		{
		case START: cmd = GETCOMMAND; // ����������� ����������� ��������
			if (previousCommand != START) {
				sq = AS_SQ;
				puts("������� Start");
				openSocket();
				previousCommand = START;
			}
			else puts("��� �������\n");
			break;
		case STOP:  cmd = GETCOMMAND; // ���������� ����������� ��������   
			if (previousCommand != STOP) {
				sq = 0;
				puts("������� Stop \n");
				closeSocket();
				previousCommand = STOP;
			}
			else puts("��� ����������\n");

			break;
		case WAIT:  cmd = GETCOMMAND; // ������� ���������������� ����������� �������� �� ��� ���, ���� �� ���������� ��������� ������, ������������ � �������.   
			sq = 0;
			puts("������ � ������ ��������\n");
			closeSocket();
			while (contacts.size() != 0); 
			cmd = START;
			previousCommand = WAIT;
			puts("����� ������");

			break;
		case SHUTDOWN:  // ������� ����������� ������������������ ������: wait, exit.    
			sq = 0;
			puts("WAIT �����������");
			closeSocket();
			while (contacts.size() != 0); // �������� ���������� 
			puts("EXIT �����������");
			cmd = EXIT;
			break;
		case GETCOMMAND:  cmd = GETCOMMAND; // ��������� �������, ������� �� ������������� ��� ����� � ������� ����������, � ��������������� �������� ��� ��������, ��� ������ ����� ������� � ����������, ��������� ������� ����������.

			break;
		};
		if (cmd != STOP) {
			if (AcceptCycle(sq))   
			{
				cmd = GETCOMMAND;
				SetEvent(hClientConnectedEvent);
			}
			else SleepEx(0, TRUE);    // ��������� ����������� ��������� 
		}
	};
};

DWORD WINAPI AcceptServer(LPVOID pPrm)    // �������� 
{
	puts("������ acceptServer");
	DWORD rc = 0;    // ��� �������� 
	WSADATA wsaData;
	try
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw  SetErrorMsgText("Startup:", WSAGetLastError());
		CommandsCycle(*((TalkersCommand*)pPrm));
	}
	catch (string errorMsgText)
	{
		printf("\n%s", errorMsgText.c_str());
	}
	puts("���������� acceptServer");
	ExitThread(*(DWORD*)pPrm);  // ���������� ������ ������
}

TalkersCommand set_param(char* param) {
	if (!strcmp(param, "start")) return START;
	if (!strcmp(param, "stop")) return STOP;
	if (!strcmp(param, "exit")) return EXIT;
	if (!strcmp(param, "wait")) return WAIT;
	if (!strcmp(param, "shutdown")) return SHUTDOWN;
	if (!strcmp(param, "statistics")) return STATISTICS;
	if (!strcmp(param, "getcommand")) return GETCOMMAND;
}
DWORD WINAPI ConsolePipe(LPVOID pPrm)    // �������� 
{
	DWORD rc = 0;
	char rbuf[100];
	DWORD dwRead, dwWrite;
	HANDLE hPipe;
	try
	{
		char namedPipeConnectionString[50];
		sprintf(namedPipeConnectionString, "\\\\.\\pipe\\%s", namedPipeName);
		if ((hPipe = CreateNamedPipeA(namedPipeConnectionString,
			PIPE_ACCESS_DUPLEX,
			PIPE_TYPE_MESSAGE | PIPE_WAIT,
			1, NULL, NULL,
			INFINITE, NULL)) == INVALID_HANDLE_VALUE)
			throw SetPipeError("create:", GetLastError());
		if (!ConnectNamedPipe(hPipe, NULL))
			throw SetPipeError("connect:", GetLastError());
		TalkersCommand& param = *((TalkersCommand*)pPrm);
		while (param != EXIT) {
			puts("����������� �� named pipe ...");
			ConnectNamedPipe(hPipe, NULL);
			while (ReadFile(hPipe, rbuf, sizeof(rbuf), &dwRead, NULL))
			{
				printf("��������� �� �������:  %s\n", rbuf);
				param = set_param(rbuf);
				if (param == STATISTICS)
				{
					char sendStastistics[200];
					sprintf(sendStastistics, "\n----STATISTICS----\n"\
						"����� ���������� �����������:    %d\n" \
						"����� ���������� �������:        %d\n" \
						"����������� �������:             %d\n" \
						"���������� �������� �����������: %d\n" \
						"-------------------------------------", connectionCount, sayNoCount, successConnections, contacts.size());
					WriteFile(hPipe, sendStastistics, sizeof(sendStastistics), &dwWrite, NULL);
				}
				if (param != STATISTICS)
					WriteFile(hPipe, rbuf, strlen(rbuf) + 1, &dwWrite, NULL);
			}
			puts("--------------����� ������-----------------");
			DisconnectNamedPipe(hPipe);
		}
	}
	catch (string ErrorPipeText)
	{
		printf("\n%s", ErrorPipeText.c_str());
		return -1;
	}

	CloseHandle(hPipe);
	ExitThread(rc);  // ���������� ������ ������
}


DWORD WINAPI GarbageCleaner(LPVOID pPrm)    // �������� 
{
	DWORD rc = 0;    // ��� �������� 	
	while (*((TalkersCommand*)pPrm) != EXIT) {
		int listSize = 0;
		int howMuchClean = 0;
		if (contacts.size() != 0) {
			for (auto i = contacts.begin(); i != contacts.end();) {
				if (i->type == i->EMPTY) {
					EnterCriticalSection(&scListContact);
					if (i->sthread == i->FINISH)
						InterlockedIncrement(&successConnections);
					if (i->sthread == i->ABORT || i->sthread == i->TIMEOUT)
						InterlockedIncrement(&sayNoCount);
					i = contacts.erase(i);
					howMuchClean++;
					listSize = contacts.size();
					LeaveCriticalSection(&scListContact);
				}
				else ++i;
			}
		}
	}
	puts("���������� garbageCleaner");
	ExitThread(rc);  // ���������� ������ ������
}

HANDLE(*ts)(char*, LPVOID);
HMODULE st;

void CALLBACK ASWTimer(LPVOID Prm, DWORD, DWORD) {
	Contact *contact = (Contact*)(Prm);
	printf("ASWTimer ������� %p\n", contact->hthread);
	//Beep(500, 100); // ������!
	TerminateThread(contact->serverHThtead, NULL);
	send(contact->s, "TimeOUT", strlen("TimeOUT") + 1, NULL);
	EnterCriticalSection(&scListContact);
	CancelWaitableTimer(contact->htimer);
	contact->type = contact->EMPTY;
	contact->sthread = contact->TIMEOUT;
	LeaveCriticalSection(&scListContact);
}



DWORD WINAPI DispatchServer(LPVOID pPrm)
{
	DWORD rc = 0;    // ��� �������� 	
	TalkersCommand& command = *(TalkersCommand*)pPrm;
	while (command != EXIT)     // ���� ��������� ������ ������� � �����������		
	{
		if (command != STOP) {
			WaitForSingleObject(hClientConnectedEvent, 5000);// ���� ���� ������������ �������
															 // �.�. � acceptCycle ������� �������� � ���������� ���������
			ResetEvent(hClientConnectedEvent);
			EnterCriticalSection(&scListContact);
			for (auto i = contacts.begin(); i != contacts.end(); i++) {
				if (i->type == i->ACCEPT) {
					u_long nonblk = 0;
					if (ioctlsocket(i->s, FIONBIO, &nonblk) == SOCKET_ERROR)
						throw SetErrorMsgText("ioctlsocket:", WSAGetLastError());

					char serviceType[5];
					clock_t start = clock();
					recv(i->s, serviceType, sizeof(serviceType), NULL);
					strcpy(i->msg, serviceType);

					clock_t delta = clock() - start;
					if (delta > 5000) {
						puts("��������");
						i->sthread = i->TIMEOUT;
						if ((send(i->s, "TimeOUT", strlen("TimeOUT") + 1, NULL)) == SOCKET_ERROR)
							throw  SetErrorMsgText("send:", WSAGetLastError());
						if (closesocket(i->s) == SOCKET_ERROR)
							throw  SetErrorMsgText("closesocket:", WSAGetLastError());
						i->type = i->EMPTY;
					}
					else if (delta <= 5000) {
						if (strcmp(i->msg, "Echo") && strcmp(i->msg, "Time") && strcmp(i->msg, "0001")) {
							if ((send(i->s, "ErrorInquiry", strlen("ErrorInquiry") + 1, NULL)) == SOCKET_ERROR)
								throw  SetErrorMsgText("send:", WSAGetLastError());
							i->sthread = i->ABORT;
							i->type = i->EMPTY;
							if (closesocket(i->s) == SOCKET_ERROR)
								throw  SetErrorMsgText("closesocket:", WSAGetLastError());
						}
						else {
							i->type = i->CONTACT;
							i->hthread = hAcceptServer;
							i->serverHThtead = ts(serviceType, (LPVOID)&(*i));
							i->htimer = CreateWaitableTimer(0, FALSE, 0);
							LARGE_INTEGER Li;
							int seconds = 10;
							Li.QuadPart = -(10000000 * seconds);
							SetWaitableTimer(i->htimer, &Li, 0, ASWTimer, (LPVOID)&(*i), FALSE);
							SleepEx(0, TRUE);
						}
					}
				}
			}
			LeaveCriticalSection(&scListContact);
		}
	}
	puts("���������� dispatchServer");
	ExitThread(rc);  // ���������� ������ ������
}

SOCKET sSUDP;

bool PutAnswerToClient(char * name, sockaddr* to, int * lto) {

	char msg[] = "������ �������������� � �������\n";
	if ((sendto(sSUDP, msg, sizeof(msg) + 1, NULL, to, *lto)) == SOCKET_ERROR)
		throw  SetErrorMsgText("sendto:", WSAGetLastError());
	return false;
}

bool  GetRequestFromClient(char* name, short port, SOCKADDR_IN* from, int* flen)
{
	SOCKADDR_IN clnt;               // ���������  ������ �������
	int lc = sizeof(clnt);
	ZeroMemory(&clnt, lc);   // �������� ������
	char ibuf[500];                  //����� ����� 
	int  lb = 0;                    //���������� �������� ����
	int optval = 1;
	int TimeOut = 1000;
	setsockopt(sSUDP, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int));
	setsockopt(sSUDP, SOL_SOCKET, SO_RCVTIMEO, (char*)&TimeOut, sizeof(TimeOut));
	while (true) {
		if ((lb = recvfrom(sSUDP, ibuf, sizeof(ibuf), NULL, (sockaddr*)&clnt, &lc)) == SOCKET_ERROR) return false;
		if (!strcmp(name, ibuf)) {
			*from = clnt;
			*flen = lc;
			return true;
		}
		puts("\n������������ ���");
	}
	return false;
}



DWORD WINAPI ResponseServer(LPVOID pPrm)
{
	DWORD rc = 0;
	WSADATA wsaData;
	SOCKADDR_IN serv;                     // ���������  ������ sS
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		throw  SetErrorMsgText("Startup:", WSAGetLastError());
	if ((sSUDP = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
		throw  SetErrorMsgText("socket:", WSAGetLastError());
	serv.sin_family = AF_INET;           // ������������ IP-���������  
	serv.sin_port = htons(serverPort);          // ���� 2000
	serv.sin_addr.s_addr = INADDR_ANY;//inet_addr("192.168.43.78");   // ����� ����������� IP-����� 

	if (bind(sSUDP, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
		throw  SetErrorMsgText("bind:", WSAGetLastError());


	SOCKADDR_IN someServer;
	int serverSize = sizeof(someServer);

	SOCKADDR_IN from;               // ���������  ������ �������
	int lc = sizeof(from);
	ZeroMemory(&from, lc);
	int numberOfClients = 0;
	while (*(TalkersCommand*)pPrm != EXIT) 	
	{
		try
		{
			if (GetRequestFromClient("Hello", serverPort, &from, &lc))
			{
				printf("\n������ �����������\naddress: %s port %d", inet_ntoa(from.sin_addr), htons(from.sin_port));
				PutAnswerToClient("Hello", (sockaddr*)&from, &lc);
			}
		}
		catch (string errorMsgText)
		{
			printf("\n%s", errorMsgText.c_str());
		}
	}
	if (closesocket(sSUDP) == SOCKET_ERROR)
		throw  SetErrorMsgText("closesocket:", WSAGetLastError());
	if (WSACleanup() == SOCKET_ERROR)
		throw  SetErrorMsgText("Cleanup:", WSAGetLastError());
	ExitThread(rc);
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "ru");
	if (argc == 2) {
		serverPort = atoi(argv[1]);
	}
	else if (argc == 3) {
		serverPort = atoi(argv[1]);
		strcpy(dllName, argv[2]);
	}
	else if (argc == 4) {
		serverPort = atoi(argv[1]);
		strcpy(dllName, argv[2]);
		strcpy(namedPipeName, argv[3]);
	}
	else {
		serverPort = 2000;
		strcpy(dllName, "Win32Project1.dll");
		strcpy(namedPipeName, "pipe");
	}
	
	volatile TalkersCommand  cmd = START;      // ������� ������� 

	InitializeCriticalSection(&scListContact);
	hAcceptServer = CreateThread(NULL, NULL, AcceptServer,
		(LPVOID)&cmd, NULL, NULL),
		hDispatchServer = CreateThread(NULL, NULL, DispatchServer,
		(LPVOID)&cmd, NULL, NULL),
		hGarbageCleaner = CreateThread(NULL, NULL, GarbageCleaner,
		(LPVOID)&cmd, NULL, NULL),
		hConsolePipe = CreateThread(NULL, NULL, ConsolePipe,
		(LPVOID)&cmd, NULL, NULL);
	hResponseServer = CreateThread(NULL, NULL, ResponseServer,
		(LPVOID)&cmd, NULL, NULL);

	SetThreadPriority(hGarbageCleaner, THREAD_PRIORITY_LOWEST);

	printf("����: %d\n", serverPort);

	st = LoadLibraryA(dllName);
	ts = (HANDLE(*)(char*, LPVOID))GetProcAddress(st, "SSS");

	WaitForSingleObject(hAcceptServer, INFINITE);
	CloseHandle(hAcceptServer);
	WaitForSingleObject(hDispatchServer, INFINITE);
	CloseHandle(hDispatchServer);
	WaitForSingleObject(hGarbageCleaner, INFINITE);
	CloseHandle(hGarbageCleaner);
	WaitForSingleObject(hConsolePipe, INFINITE);
	CloseHandle(hConsolePipe);
	WaitForSingleObject(hResponseServer, INFINITE);
	CloseHandle(hResponseServer);
	FreeLibrary(st);
	return 0;
};

