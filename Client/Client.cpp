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
	switch (code) // проверка кода возврата 
	{
	case WSAEINTR: msgText = "WSAEINTR"; break;                      //Работа функции прервана
	case WSAEACCES: msgText = "WSAEACCES"; break;                    //Разрешение отвергнуто
	case WSAEFAULT: msgText = "WSAEFAULT"; break;                    //Ошибочный адрес
	case WSAEINVAL: msgText = "WSAEINVAL"; break;                    //Ошибка в аргументе
	case WSAEMFILE: msgText = "WSAEMFILE"; break;                    //Слишком много файлов открыто
	case WSAEWOULDBLOCK: msgText = "WSAEWOULDBLOCK"; break;          //Ресурс временно недоступен
	case WSAEINPROGRESS: msgText = "WSAEINPROGRESS"; break;          //Операция в процессе развития
	case WSAEALREADY: msgText = "WSAEALREADY"; break;                //Операция уже выполняется
	case WSAENOTSOCK: msgText = "WSAENOTSOCK"; break;                //Сокет задан неправильно
	case WSAEDESTADDRREQ: msgText = "WSAEDESTADDRREQ"; break;        //Требуется адрес расположения
	case WSAEMSGSIZE: msgText = "WSAEMSGSIZE"; break;                //Сообщение слишком длинное
	case WSAEPROTOTYPE: msgText = "WSAEPROTOTYPE"; break;            //Неправильный тип протокола для сокета
	case WSAENOPROTOOPT: msgText = "WSAENOPROTOOPT"; break;          //Ошибка в опции протокола
	case WSAEPROTONOSUPPORT: msgText = "WSAEPROTONOSUPPORT"; break;  //Протокол не поддерживается
	case WSAESOCKTNOSUPPORT: msgText = "WSAESOCKTNOSUPPORT"; break;  //Тип сокета не поддерживается
	case WSAEOPNOTSUPP: msgText = "WSAEOPNOTSUPP"; break;            //Операция не поддерживается
	case WSAEPFNOSUPPORT: msgText = "WSAEPFNOSUPPORT"; break;        //Тип протоколов не поддерживается
	case WSAEAFNOSUPPORT: msgText = "WSAEAFNOSUPPORT"; break;        //Тип адресов не поддерживается протоколом
	case WSAEADDRINUSE: msgText = "WSAEADDRINUSE"; break;            //Адрес уже используется
	case WSAEADDRNOTAVAIL: msgText = "WSAEADDRNOTAVAIL"; break;      //Запрошенный адрес не может быть использован
	case WSAENETDOWN: msgText = "WSAENETDOWN"; break;                //Сеть отключена
	case WSAENETUNREACH: msgText = "WSAENETUNREACH"; break;          //Сеть не достижима
	case WSAENETRESET: msgText = "WSAENETRESET"; break;              //Сеть разорвала соединение
	case WSAECONNABORTED: msgText = "WSAECONNABORTED"; break;        //Программный отказ связи
	case WSAECONNRESET: msgText = "WSAECONNRESET"; break;            //Связь восстановлена
	case WSAENOBUFS: msgText = "WSAENOBUFS"; break;                  //Не хватает памяти для буферов
	case WSAEISCONN: msgText = "WSAEISCONN"; break;                  //Сокет уже подключен
	case WSAENOTCONN: msgText = "WSAENOTCONN"; break;                //Сокет не подключен
	case WSAESHUTDOWN: msgText = "WSAESHUTDOWN"; break;              //Нельзя выполнить send: сокет завершил работу
	case WSAETIMEDOUT: msgText = "WSAETIMEDOUT"; break;              //Закончился отведенный интервал времени
	case WSAECONNREFUSED: msgText = "WSAECONNREFUSED"; break;        //Соединение отклонено
	case WSAEHOSTDOWN: msgText = "WSAEHOSTDOWN"; break;              //Хост в неработоспособном состоянии
	case WSAEHOSTUNREACH: msgText = "WSAEHOSTUNREACH"; break;        //Нет маршрута для хоста
	case WSAEPROCLIM: msgText = "WSAEPROCLIM"; break;                //Слишком много процессов
	case WSASYSNOTREADY: msgText = "WSASYSNOTREADY"; break;          //Сеть не доступна
	case WSAVERNOTSUPPORTED: msgText = "WSAVERNOTSUPPORTED"; break;  //Данная версия недоступна
	case WSANOTINITIALISED: msgText = "WSANOTINITIALISED"; break;    //Не выполнена инициализация WS2_32.DLL
	case WSAEDISCON: msgText = "WSAEDISCON"; break;                  //Выполняется отключение
	case WSATYPE_NOT_FOUND: msgText = "WSATYPE_NOT_FOUND"; break;    //Класс не найден
	case WSAHOST_NOT_FOUND: msgText = "WSAHOST_NOT_FOUND"; break;    //Хост не найден
	case WSATRY_AGAIN: msgText = "WSATRY_AGAIN"; break;              //Неавторизированный хост не найден
	case WSANO_RECOVERY: msgText = "WSANO_RECOVERY"; break;          //Неопределенная ошибка
	case WSANO_DATA: msgText = "WSANO_DATA"; break;                  //Нет записи запрошенного типа
	case WSA_INVALID_HANDLE: msgText = "WSA_INVALID_HANDLE"; break;  //Указанный дескриптор события с ошибкой
	case WSA_INVALID_PARAMETER: msgText = "WSA_INVALID_PARAMETER"; break; //Один или более параметров с ошибкой
	case WSA_IO_INCOMPLETE: msgText = "WSA_IO_INCOMPLETE"; break;         //Объект ввода-вывода не в сигнальном состоянии
	case WSA_IO_PENDING: msgText = "WSA_IO_PENDING"; break;               //Операция завершится позже
	case WSA_NOT_ENOUGH_MEMORY: msgText = "WSA_NOT_ENOUGH_MEMORY"; break; //Не достаточно памяти
	case WSA_OPERATION_ABORTED: msgText = "WSA_OPERATION_ABORTED"; break; //Операция отвергнута
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
	char* call,			//[in] позывной сервера 
	SOCKADDR_IN* from,	//[out] указатель на SOCKADDR_IN
	int* flen,			//[out] указатель на размер from 
	SOCKET * cC,		//сокет
	SOCKADDR_IN * all
	)
{
	char ibuf[50],		//буфер ввода 
		obuf[50];		//буфер вывода
	int libuf = 0,		//количество принятых байт
		lobuf = 0;		//количество отправленных байт

	int optval = 1;
	DWORD val = 5000;


	if (setsockopt(*cC, SOL_SOCKET, SO_BROADCAST, (const char*)&val, sizeof(DWORD) == SOCKET_ERROR))
		throw GetErrorMsgText(WSAGetLastError());

	if ((lobuf = sendto(*cC, call, strlen(call) + 1, NULL,
		(sockaddr*)all, sizeof(*all))) == SOCKET_ERROR)
		throw SetErrorMsgText("Sendto:", WSAGetLastError());
	cout << "ОТправлена: " << call << endl;


	if ((libuf = recvfrom(*cC, ibuf, sizeof(ibuf), NULL, (sockaddr*)from, flen)) == SOCKET_ERROR)
		if (WSAGetLastError() == WSAETIMEDOUT) return false;
		else throw SetErrorMsgText("Recv:", WSAGetLastError());
	cout << "Получено: " << ibuf << endl;
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

		char Call[50]; //запрос клиента
		char Name[50] = "desktop"; //имя сервера
		char Calls[50] = "vi"; //позывной

		SOCKADDR_IN Server = { 0 };
		Server.sin_family = AF_INET;
		Server.sin_port = htons(port);

		SOCKADDR_IN Server_IN; //параметры сокета сервера
		int Flen = sizeof(Server);
		int chs = 0;
		cout << "1) По имени сервера" << endl << "2) По позывному" << endl << "\t";
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
			cout << "Позывной: ";
			cin >> Calls;
			SOCKET cC;
			if ((cC = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
				throw SetErrorMsgText("Socket:", WSAGetLastError());

			struct timeval timeout;
			timeout.tv_sec = 3000;
			timeout.tv_usec = 0;
			if (setsockopt(cC, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) == SOCKET_ERROR)
				throw SetErrorMsgText("Opt:", WSAGetLastError());

			SOCKADDR_IN all; //параметры сокета sS
			all.sin_family = AF_INET; //используется IP-адресация 
			all.sin_port = htons(bport); //порт для широковещания
			all.sin_addr.s_addr = INADDR_BROADCAST; //всем
			SOCKADDR_IN clnt; //параметры сокета клиента
			memset(&clnt, 0, sizeof(clnt)); //обнулить память
			int lc = sizeof(clnt); //размер SOCKADDR_IN

			bool bsr = GetServer(Calls, &clnt, &lc, &cC, &all);
			if (bsr == false) throw "Server not found;";
			else
			{
				Server_IN.sin_addr.s_addr = clnt.sin_addr.s_addr;
				if (closesocket(cC) == SOCKET_ERROR)
					throw SetErrorMsgText("Closesocket:", WSAGetLastError());
				cout << "Порт сервера: ";
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
			cout << "Введите Rand для продолжения. Введите любое другое сочетание символов, чтобы завершить работы клиента. Введите repeat чтобы завершить передачу данных." << endl;
			break;
		case 2:
			strcpy(Call, "Time");
			cout << "Введите Time для продолжения. Введите любое другое сочетание символов, чтобы завершить работы клиента. Введите repeat чтобы завершить передачу данных" << endl;
			break;
		case 3:
			strcpy(Call, "Echo");
			cout << "Введите Echo для отправления. Введите любое другое сочетание символов, чтобы завершить работы клиента. Введите repeat чтобы завершить передачу данных" << endl;
			break;
		}
	repeat:
		// Отправляем запрос серверу
		if ((lobuf = send(ClientSocket, Call, sizeof(Call), NULL)) == SOCKET_ERROR)
			throw SetErrorMsgText("Send:", WSAGetLastError());
		cout << "Отправлена: " << Call << endl;
		char rCall[50];
		try {
			if ((lobuf = recv(ClientSocket, rCall, sizeof(rCall), NULL)) == SOCKET_ERROR)
				throw SetErrorMsgText("Recv:", WSAGetLastError());
			cout << "Получено: " << rCall << endl;
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
						printf("Сервер остановил соединение: %s\n", obuf);
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
						cout << "ОТправлена: " << iib << endl;

						rcv = false;
					}
					// Принимаем сообщение

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
							printf("Сервер остановил соединение: %s\n", obuf);
							break;
						}
						else
							// Выводим полученное сообщение
						{

							if (strstr("timeout", obuf) != 0)
							{
								cout << "Time out. Disconnect" << endl;
								fin = true;
								cout << "fin= " << fin << endl;
							}
							if (strstr("timeout", obuf) == 0)
							{
								//cout << "Получено: " << obuf << endl;
								printf("Полученное сообщение:[%s]\n", obuf);
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

		// Закрываем сокет
		if (closesocket(ClientSocket) == SOCKET_ERROR)
			throw SetErrorMsgText("Closesocket:", WSAGetLastError());
		endfunction:
		// Очищаем ресурсы
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