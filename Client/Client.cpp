// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;

bool SystemMessage(char *ch)
{
	bool result = false;
	char Timeout[50] = "Close: timeout;", Close[50] = "Close: finish;", Abort[50] = "Close: Abort;";
	if (strcmp(ch, Timeout) == NULL) result = true;
	else if (strcmp(ch, Abort) == NULL) result = true;
	else if (strcmp(ch, Close) == NULL) result = true;
	return result;
}

string GetErrorMsgText(int code)
{
	string msgText;
	switch (code) // �������� ���� �������� 
	{
	case WSAEINTR: msgText = "WSAEINTR"; break;                      //������ ������� ��������
	case WSAEACCES: msgText = "WSAEACCES"; break;                    //���������� ����������
	case WSAEFAULT: msgText = "WSAEFAULT"; break;                    //��������� �����
	case WSAEINVAL: msgText = "WSAEINVAL"; break;                    //������ � ���������
	case WSAEMFILE: msgText = "WSAEMFILE"; break;                    //������� ����� ������ �������
	case WSAEWOULDBLOCK: msgText = "WSAEWOULDBLOCK"; break;          //������ �������� ����������
	case WSAEINPROGRESS: msgText = "WSAEINPROGRESS"; break;          //�������� � �������� ��������
	case WSAEALREADY: msgText = "WSAEALREADY"; break;                //�������� ��� �����������
	case WSAENOTSOCK: msgText = "WSAENOTSOCK"; break;                //����� ����� �����������
	case WSAEDESTADDRREQ: msgText = "WSAEDESTADDRREQ"; break;        //��������� ����� ������������
	case WSAEMSGSIZE: msgText = "WSAEMSGSIZE"; break;                //��������� ������� �������
	case WSAEPROTOTYPE: msgText = "WSAEPROTOTYPE"; break;            //������������ ��� ��������� ��� ������
	case WSAENOPROTOOPT: msgText = "WSAENOPROTOOPT"; break;          //������ � ����� ���������
	case WSAEPROTONOSUPPORT: msgText = "WSAEPROTONOSUPPORT"; break;  //�������� �� ��������������
	case WSAESOCKTNOSUPPORT: msgText = "WSAESOCKTNOSUPPORT"; break;  //��� ������ �� ��������������
	case WSAEOPNOTSUPP: msgText = "WSAEOPNOTSUPP"; break;            //�������� �� ��������������
	case WSAEPFNOSUPPORT: msgText = "WSAEPFNOSUPPORT"; break;        //��� ���������� �� ��������������
	case WSAEAFNOSUPPORT: msgText = "WSAEAFNOSUPPORT"; break;        //��� ������� �� �������������� ����������
	case WSAEADDRINUSE: msgText = "WSAEADDRINUSE"; break;            //����� ��� ������������
	case WSAEADDRNOTAVAIL: msgText = "WSAEADDRNOTAVAIL"; break;      //����������� ����� �� ����� ���� �����������
	case WSAENETDOWN: msgText = "WSAENETDOWN"; break;                //���� ���������
	case WSAENETUNREACH: msgText = "WSAENETUNREACH"; break;          //���� �� ���������
	case WSAENETRESET: msgText = "WSAENETRESET"; break;              //���� ��������� ����������
	case WSAECONNABORTED: msgText = "WSAECONNABORTED"; break;        //����������� ����� �����
	case WSAECONNRESET: msgText = "WSAECONNRESET"; break;            //����� �������������
	case WSAENOBUFS: msgText = "WSAENOBUFS"; break;                  //�� ������� ������ ��� �������
	case WSAEISCONN: msgText = "WSAEISCONN"; break;                  //����� ��� ���������
	case WSAENOTCONN: msgText = "WSAENOTCONN"; break;                //����� �� ���������
	case WSAESHUTDOWN: msgText = "WSAESHUTDOWN"; break;              //������ ��������� send: ����� �������� ������
	case WSAETIMEDOUT: msgText = "WSAETIMEDOUT"; break;              //���������� ���������� �������� �������
	case WSAECONNREFUSED: msgText = "WSAECONNREFUSED"; break;        //���������� ���������
	case WSAEHOSTDOWN: msgText = "WSAEHOSTDOWN"; break;              //���� � ����������������� ���������
	case WSAEHOSTUNREACH: msgText = "WSAEHOSTUNREACH"; break;        //��� �������� ��� �����
	case WSAEPROCLIM: msgText = "WSAEPROCLIM"; break;                //������� ����� ���������
	case WSASYSNOTREADY: msgText = "WSASYSNOTREADY"; break;          //���� �� ��������
	case WSAVERNOTSUPPORTED: msgText = "WSAVERNOTSUPPORTED"; break;  //������ ������ ����������
	case WSANOTINITIALISED: msgText = "WSANOTINITIALISED"; break;    //�� ��������� ������������� WS2_32.DLL
	case WSAEDISCON: msgText = "WSAEDISCON"; break;                  //����������� ����������
	case WSATYPE_NOT_FOUND: msgText = "WSATYPE_NOT_FOUND"; break;    //����� �� ������
	case WSAHOST_NOT_FOUND: msgText = "WSAHOST_NOT_FOUND"; break;    //���� �� ������
	case WSATRY_AGAIN: msgText = "WSATRY_AGAIN"; break;              //������������������ ���� �� ������
	case WSANO_RECOVERY: msgText = "WSANO_RECOVERY"; break;          //�������������� ������
	case WSANO_DATA: msgText = "WSANO_DATA"; break;                  //��� ������ ������������ ����
	case WSA_INVALID_HANDLE: msgText = "WSA_INVALID_HANDLE"; break;  //��������� ���������� ������� � �������
	case WSA_INVALID_PARAMETER: msgText = "WSA_INVALID_PARAMETER"; break; //���� ��� ����� ���������� � �������
	case WSA_IO_INCOMPLETE: msgText = "WSA_IO_INCOMPLETE"; break;         //������ �����-������ �� � ���������� ���������
	case WSA_IO_PENDING: msgText = "WSA_IO_PENDING"; break;               //�������� ���������� �����
	case WSA_NOT_ENOUGH_MEMORY: msgText = "WSA_NOT_ENOUGH_MEMORY"; break; //�� ���������� ������
	case WSA_OPERATION_ABORTED: msgText = "WSA_OPERATION_ABORTED"; break; //�������� ����������
	case WSASYSCALLFAILURE: msgText = "WSASYSCALLFAILURE"; break;
	default: msgText = "***ERROR***"; break;
	};
	return msgText;
}

string SetErrorMsgText(string msgText, int code)
{
	return msgText + GetErrorMsgText(code);
}

bool GetServer(
	char* call,			//[in] �������� ������� 
	SOCKADDR_IN* from,	//[out] ��������� �� SOCKADDR_IN
	int* flen,			//[out] ��������� �� ������ from 
	SOCKET * cC,		//�����
	SOCKADDR_IN * all
	)
{
	char ibuf[50],		//����� ����� 
		obuf[50];		//����� ������
	int libuf = 0,		//���������� �������� ����
		lobuf = 0;		//���������� ������������ ����

	int optval = 1;
	DWORD val = 5000;


	if (setsockopt(*cC, SOL_SOCKET, SO_BROADCAST, (const char*)&val, sizeof(DWORD) == SOCKET_ERROR))
		throw GetErrorMsgText(WSAGetLastError());

	if ((lobuf = sendto(*cC, call, strlen(call) + 1, NULL,
		(sockaddr*)all, sizeof(*all))) == SOCKET_ERROR)
		throw SetErrorMsgText("Sendto:", WSAGetLastError());
	cout << "����������: " << call << endl;


	if ((libuf = recvfrom(*cC, ibuf, sizeof(ibuf), NULL, (sockaddr*)from, flen)) == SOCKET_ERROR)
		if (WSAGetLastError() == WSAETIMEDOUT) return false;
		else throw SetErrorMsgText("Recv:", WSAGetLastError());
	cout << "��������: " << ibuf << endl;
	if (strcmp(call, ibuf) == 0) 
	{
		return true;
	}
	else 
	{
		return false;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Russian");
	SetConsoleTitle("Client");
	int port = 0;
	SOCKET ClientSocket;
	WSADATA wsaData;
	try
	{
		
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw SetErrorMsgText("Startup:", WSAGetLastError());
	hiBeloded:
		int ch = 0;
		bool fin = false;
		int cCall = 0;
		char strcall[10];


		int max = 100, lobuf = 1;
		char obuf[50] = "";
		char ibuf[50] = "";
		int bport = 2000;

		char Call[50]; //������ �������
		char Name[50] = "desktop"; //��� �������
		char Calls[50] = "vi"; //��������

		SOCKADDR_IN Server = { 0 };
		Server.sin_family = AF_INET;
		Server.sin_port = htons(port);

		SOCKADDR_IN Server_IN; //��������� ������ �������
		int Flen = sizeof(Server);
		int chs = 0;
		cout << "1) �� ����� �������" << endl << "2) �� ���������" << endl << "\t";
		cin >> chs;
		if (chs == 1)
		{
			cout << "Enter: ";
			cin >> Name;
			hostent* s = gethostbyname(Name);
			if (s == NULL) throw "Server not found;";
			cout << "Enter port of server: ";
			cin >> port;
			Server_IN.sin_addr = *(struct in_addr *)s->h_addr_list[0];
		}
		else if (chs == 2)
		{
			cout << "��������: ";
			cin >> Calls;
			SOCKET cC;
			if ((cC = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
				throw SetErrorMsgText("Socket:", WSAGetLastError());

			struct timeval timeout;
			timeout.tv_sec = 3000;
			timeout.tv_usec = 0;
			if (setsockopt(cC, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) == SOCKET_ERROR)
				throw SetErrorMsgText("Opt:", WSAGetLastError());

			SOCKADDR_IN all; //��������� ������ sS
			all.sin_family = AF_INET; //������������ IP-��������� 
			all.sin_port = htons(bport); //���� ��� �������������
			all.sin_addr.s_addr = INADDR_BROADCAST; //����
			SOCKADDR_IN clnt; //��������� ������ �������
			memset(&clnt, 0, sizeof(clnt)); //�������� ������
			int lc = sizeof(clnt); //������ SOCKADDR_IN

			bool bsr = GetServer(Calls, &clnt, &lc, &cC, &all);
			if (bsr == false) throw "Server not found;";
			else
			{
				Server_IN.sin_addr.s_addr = clnt.sin_addr.s_addr;
				if (closesocket(cC) == SOCKET_ERROR)
					throw SetErrorMsgText("Closesocket:", WSAGetLastError());
				cout << "���� �������: ";
				cin >> port;
			}

		}
		else throw "Wrong code";

		Server_IN.sin_family = AF_INET;
		Server_IN.sin_port = htons(port);

	
		if ((ClientSocket = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
			throw SetErrorMsgText("Socket:", WSAGetLastError());
		if ((connect(ClientSocket, (sockaddr*)&Server_IN, sizeof(Server_IN))) == SOCKET_ERROR)
			throw "Fail connection;";
		else {
			cout << "You are connected" << endl;
		}

		while (!fin)
		{
			cout << "Service: " << endl << "1) Rand" << endl << "2) Time" << endl << "3) Echo" << endl<<"4)Exit"<<endl << "\t ";
			try {
				cin >> strcall;
				if (strcmp(strcall,"1") == 0 || (strcmp(strcall, "2") == 0 || strcmp(strcall, "3") == 0))
				{
					fin = true;
					break;
				}
				if (strcmp(strcall, "4") == 0)
				{
					if (closesocket(ClientSocket) == SOCKET_ERROR)
						throw SetErrorMsgText("Closesocket:", WSAGetLastError());
					return 0;
				}
				else
				{
					cout << "Fail service" << endl;
				}
			}
			catch(...)
			{ }
		}
		cCall = stoi(strcall);
		switch (cCall)
		{
		case 1:
			strcpy(Call, "Rand");
			cout << "������� Rand ��� �����������. ������� ����� ������ ��������� ��������, ����� ��������� ������ �������. ������� repeat ����� ��������� �������� ������." << endl;
			break;
		case 2:
			strcpy(Call, "Time");
			cout << "������� Time ��� �����������. ������� ����� ������ ��������� ��������, ����� ��������� ������ �������. ������� repeat ����� ��������� �������� ������" << endl;
			break;
		case 3:
			strcpy(Call, "Echo");
			cout << "������� Echo ��� �����������. ������� ����� ������ ��������� ��������, ����� ��������� ������ �������. ������� repeat ����� ��������� �������� ������" << endl;
			break;
		}
	repeat:
		// ���������� ������ �������
		if ((lobuf = send(ClientSocket, Call, sizeof(Call), NULL)) == SOCKET_ERROR)
			throw SetErrorMsgText("Send:", WSAGetLastError());
		cout << "����������: " << Call << endl;
		char rCall[50];
		try {
			if ((lobuf = recv(ClientSocket, rCall, sizeof(rCall), NULL)) == SOCKET_ERROR)
				throw SetErrorMsgText("Recv:", WSAGetLastError());
			cout << "��������: " << rCall << endl;
		}
		catch(...)
		{ }
		if (strcmp(Call, rCall) != 0)
			throw rCall;
		else
		{
			bool Check = true;
			fin = false;

			u_long nonblk = 1;
			if (SOCKET_ERROR == ioctlsocket(ClientSocket, FIONBIO, &nonblk))
				throw SetErrorMsgText("Ioctlsocket:", WSAGetLastError());
			clock_t StartTime = clock();
			bool rcv = true;
			char iib[50];
			try {
				while (!fin)
				{
					if (SystemMessage(obuf))
					{
						printf("������ ��������� ����������: %s\n", obuf);
						break;
					}
					time_t startTime = clock();

					if (rcv)
					{

						std::cout << "\t";
						std::cin >> iib;

						time_t finishTime = clock();
						//if ((finishTime - startTime) / CLOCKS_PER_SEC < 6) {
						if ((lobuf = send(ClientSocket, iib, sizeof(iib), NULL)) == SOCKET_ERROR)
							throw SetErrorMsgText("Send:", WSAGetLastError());
						cout << "����������: " << iib << endl;

						rcv = false;
					}
					// ��������� ���������

					if ((recv(ClientSocket, obuf, sizeof(obuf), NULL)) == SOCKET_ERROR)
					{
						try {
							switch (WSAGetLastError())
							{
							case WSAEWOULDBLOCK: Sleep(100); break;
							default: throw SetErrorMsgText("Recv:", WSAGetLastError());
							}
							//Start ECho


						}
						//End ECho

						catch (...)
						{
							cout << "Time out. Disconnect" << endl;
							fin = true;
							cout << "fin= " << fin << endl;
						}
					}
					else
					{
						if (SystemMessage(obuf))
						{
							printf("������ ��������� ����������: %s\n", obuf);
							break;
						}
						else
							// ������� ���������� ���������
						{

							if (strstr("timeout", obuf) != 0)
							{
								cout << "Time out. Disconnect" << endl;
								fin = true;
								cout << "fin= " << fin << endl;
							}
							if (strstr("timeout", obuf) == 0)
							{
								//cout << "��������: " << obuf << endl;
								printf("���������� ���������:[%s]\n", obuf);
								rcv = true;
							}

						}


					}
					if (strcmp("repeat", iib) == 0)
					{
						fin = false;
						goto repeat;
					}
				}
			}
			catch (...)
			{
				cout << "Error" << endl;
			}
			clock_t FinishTime = clock();
			printf("Time: %lf sec.\n", (double)(FinishTime - StartTime) / CLOCKS_PER_SEC);

		}

		// ��������� �����
		if (closesocket(ClientSocket) == SOCKET_ERROR)
			throw SetErrorMsgText("Closesocket:", WSAGetLastError());
		endfunction:
		// ������� �������
		if (WSACleanup() == SOCKET_ERROR)
			throw SetErrorMsgText("Cleanup:", WSAGetLastError());
	}
	catch (char* errorMsgText)
	{
		cout << errorMsgText << endl;
	}

	system("pause");
	return 0;
}