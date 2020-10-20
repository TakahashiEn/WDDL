#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <winsvc.h>
#include <string>
#include <wchar.h>


using std::wstring;
using std::string;
using std::cin;
using std::cout;
using std::endl;



wstring string2wstring(string str)
{
	wstring result;
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	TCHAR* buffer = new TCHAR[len + 1];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';
	result.append(buffer);
	delete[] buffer;
	return result;
}

string wstring2string(wstring wstr)
{
	string result;
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	result.append(buffer);
	delete[] buffer;
	return result;
}

int main() {
	printf("Hello DL\n");

	SC_HANDLE sh = OpenSCManager(
		NULL,
		NULL,
		SC_MANAGER_ALL_ACCESS
	);

	if (!sh) {
		//printf("====== OPEN SC Manager Failed! ======\n");
		//printf("====== Please run as Admin ======\n");
		// CloseServiceHandle(sh);

		return 0;
	}
	//printf("====== OPEN SC Manager Success! ======\n");

	//printf("Driver Name: ");

	string name = "pci";

	//cin >> name;

	//cout << name << endl;

	wstring driverName;

	driverName.assign(string2wstring(name));

	//cout << "UnLoading " << wstring2string(driverName) << endl;

	SC_HANDLE shDriver = OpenService(
		sh,
		driverName.c_str(),
		SERVICE_ALL_ACCESS
	);

	// Unload Driver
	if (!DeleteService(shDriver)) {
		DWORD ErrorCode = GetLastError();
		switch (ErrorCode)
		{
		case ERROR_SERVICE_DOES_NOT_EXIST:
		case ERROR_SERVICE_NOT_FOUND:
			cout << "Driver Not Found !!!" << endl;
			getchar();
			break;
		default:
			cout << "Unload Driver Error" << endl;
			cout << "Error Code: " << ErrorCode << endl;
			getchar();
			break;
		}
	}
	else {
		cout << "Unlaod Driver Success !!!" << endl;
	}

	CloseServiceHandle(shDriver);
	CloseServiceHandle(sh);

	return 0;
}