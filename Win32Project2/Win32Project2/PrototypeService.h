#pragma once

#include "ErrorFunctions.h"
struct Contact         // элемент списка подключений       
{
	enum TE {               // состояние  сервера подключения  
		EMPTY,              // пустой элемент списка подключений 
		ACCEPT,             // подключен (accept), но не обслуживается
		CONTACT             // передан обслуживающему серверу  
	}    type;     // тип элемента списка подключений 
	enum ST {               // состояние обслуживающего сервера  
		WORK,               // идет обмен данными с клиентом
		ABORT,              // обслуживающий сервер завершился не нормально 
		TIMEOUT,            // обслуживающий сервер завершился по времени 
		FINISH              // обслуживающий сервер завершился  нормально 
	}      sthread; // состояние  обслуживающего сервера (потока)

	SOCKET      s;         // сокет для обмена данными с клиентом
	SOCKADDR_IN prms;      // параметры  сокета 
	int         lprms;     // длина prms 
	HANDLE      hthread;   // handle потока (или процесса) 
	HANDLE      htimer;    // handle таймера

	char msg[50];           // сообщение 
	char srvname[15];       //  наименование обслуживающего сервера 

	Contact(TE t = EMPTY, const char* namesrv = "") // конструктор 
	{
		memset(&prms, 0, sizeof(SOCKADDR_IN));
		lprms = sizeof(SOCKADDR_IN);
		type = t;
		strcpy(srvname, namesrv);
		msg[0] = 0;
	};

	void SetST(ST sth, const char* m = "")
	{
		sthread = sth;
		strcpy(msg, m);
	}
};

DWORD WINAPI EchoServer(LPVOID pPrm)
{
	DWORD rc = 0;
	Contact *contact = (Contact*)(pPrm);

	cout << "dll v2.0 \ntransmission{\n";
	int lobuf, libuf;
	contact->sthread = contact->WORK;

	int	whenINeedToStop = 1;
	while (true) {

		if ((libuf = recv(contact->s, contact->msg, sizeof(contact->msg), NULL)) == SOCKET_ERROR)
			throw  SetErrorMsgText("recv:", WSAGetLastError());//ожидение сообщения*

		cout << "recv:" << contact->msg << endl;

		if ((lobuf = send(contact->s, contact->msg, strlen(contact->msg) + 1, NULL)) == SOCKET_ERROR)
			throw  SetErrorMsgText("send:", WSAGetLastError());

		cout << "send:" << contact->msg << endl;
		if (atoi(contact->msg) == 0) break;

	}
	cout << "end of transmission}\n";

	contact->sthread = contact->FINISH;
	contact->type = contact->EMPTY;
	rc = contact->sthread;
	ExitThread(rc);
}

DWORD WINAPI TimeServer(LPVOID pPrm)
{
	DWORD rc = 0;
	printf("entry to TimeServer function\n");

	ExitThread(rc);
}

DWORD WINAPI ServiceServer01(LPVOID pPrm)
{
	DWORD rc = 0;


	ExitThread(rc);
}