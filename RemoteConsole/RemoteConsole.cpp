#include "stdafx.h"

string GetErrorMsgText(int code)
{
	string msgText;
	switch (code)
	{
	default: msgText = "***ERROR***";
		break;
	};
	return msgText;
}
string SetPipeError(string msgText, int code)
{
	return msgText + GetErrorMsgText(code);
};

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Rus");
	SetConsoleTitle("Remote Console");

	try
	{
		printf("\n�������:\n 1) Start  \t \n 2) Stop  \t \n 3) Exit  \t \n 4) Statistics  \n 5) Wait  \t\n 6) Shutdown  \t \n 7) ������� �������\n\n");
		char ReadBuf[50] = "";
		char WriteBuf[2] = "";
		DWORD nBytesRead;
		DWORD nBytesWrite;

		int Code = 0;

		char serverName[256];
		char PipeName[512];
		bool result;


		cout << "��� ����� �������: ";
		cin >> serverName;
		result = sprintf(PipeName, "\\\\%s\\pipe\\vi", serverName);


		BOOL fSuccess;

		SECURITY_ATTRIBUTES SecurityAttributes;
		SECURITY_DESCRIPTOR SecurityDescriptor;

		fSuccess = InitializeSecurityDescriptor(
			&SecurityDescriptor,
			SECURITY_DESCRIPTOR_REVISION);


		if (!fSuccess) {
			throw new string("InitializeSecurityDescriptor(): ������");
		}

		fSuccess = SetSecurityDescriptorDacl(
			&SecurityDescriptor,
			TRUE,
			NULL,
			FALSE);

		if (!fSuccess) {
			throw new string("SetSecurityDescriptorDacl(): ������");
		}

		SecurityAttributes.nLength = sizeof(SecurityAttributes);
		SecurityAttributes.lpSecurityDescriptor = &SecurityDescriptor;
		SecurityAttributes.bInheritHandle = FALSE;

		HANDLE hNamedPipe = CreateFile(PipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, &SecurityAttributes);

		do
		{
			printf("�������: ");
			scanf("%d", &Code);
			if (Code>0 && Code<11)
			{
				sprintf(WriteBuf, "%d", Code - 1);
				if (!WriteFile(hNamedPipe, WriteBuf, strlen(WriteBuf) + 1, &nBytesWrite, NULL))
					throw "WriteFile: ������ ";
				if (ReadFile(hNamedPipe, ReadBuf, sizeof(ReadBuf), &nBytesRead, NULL))
					cout << ReadBuf << endl;
				else
					throw "ReadFile: ������ ";
			}
			else if (Code>10) printf("�������� �������.\n\n");
		} while (Code != 7 && Code != 3 && Code != 6);
	}
	catch (char* ErrorPipeText)
	{
		printf("%s", ErrorPipeText);
		cout << GetLastError() << endl;
	}
	system("pause");
	return 0;
}
