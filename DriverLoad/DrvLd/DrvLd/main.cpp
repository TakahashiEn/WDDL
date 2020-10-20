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
		printf("====== OPEN SC Manager Failed! ======\n");
		printf("====== Please run as Admin ======\n");
		// CloseServiceHandle(sh);

		return 0;
	}
	printf("====== OPEN SC Manager Success! ======\n");

	printf("File Path: ");
	
	string filePath;
	
	cin >> filePath;

	cout << filePath << " " << filePath.length() <<  endl;

	int pos = filePath.find_last_of("\\");

	wstring driverName;

	driverName.assign(string2wstring((filePath.substr(pos + 1))));

	cout << pos << endl;

	cout << "Loading " << wstring2string(driverName) << endl;

	// Load Driver
	SC_HANDLE shDriver = CreateService(
		sh,
		driverName.c_str(),
		driverName.c_str(),
		SERVICE_ALL_ACCESS,
		SERVICE_KERNEL_DRIVER,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_IGNORE,
		string2wstring(filePath).c_str(),
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	);

	if (!shDriver) {
		if (GetLastError() == ERROR_SERVICE_EXISTS) {
			cout << "Driver Already Exist !!!" << endl;
			if (!shDriver) {
				shDriver = OpenService(
					sh, 
					driverName.c_str(),
					SERVICE_ALL_ACCESS
				);
			}
		}
		else {
			cout << "Loading Driver Error" << endl;
			cout << "Error Code: " << GetLastError() << endl;
		}
	}
	else {
		cout << "Loading Driver Success !!!" << endl;
	}


	// Start Driver
	if (!StartService(shDriver, NULL, NULL)) {
		DWORD ErrorCode = GetLastError();
		switch (ErrorCode) {
		case ERROR_SERVICE_ALREADY_RUNNING:
			cout << "Driver Already Running !!!" << endl;
			break;
		case ERROR_SERVICE_NOT_FOUND:
			cout << "Driver Not Found !!!" << endl;
			break;
		default:
			cout << "Start Driver Error" << endl;
			cout << "Error Code: " << ErrorCode << endl;
			break;
		}
	}
	else {
		cout << "Start Driver Success !!!" << endl;
	}

	// Stop Driver
	SERVICE_STATUS ss = { 0 };
	if (!ControlService(
		shDriver,
		SERVICE_CONTROL_STOP,
		&ss
	)) {
		DWORD ErrorCode = GetLastError();
		switch (ErrorCode) {
		case ERROR_SERVICE_NEVER_STARTED:
		case ERROR_SERVICE_NOT_ACTIVE:
			cout << "Driver Not Running !!!" << endl;
			break;
		case ERROR_SERVICE_NOT_FOUND:
			cout << "Driver Not Found !!!" << endl;
			break;
		default:
			cout << "Stop Driver Error" << endl;
			cout << "Error Code: " << ErrorCode << endl;
			break;
		}
	}
	else {
		cout << "Stop Driver Success !!!" << endl;
	}

	// Unload Driver
	if (!DeleteService(shDriver)) {
		DWORD ErrorCode = GetLastError();
		switch (ErrorCode)
		{
		case ERROR_SERVICE_DOES_NOT_EXIST:
		case ERROR_SERVICE_NOT_FOUND:
			cout << "Driver Not Found !!!" << endl;
			break;
		default:
			cout << "Unload Driver Error" << endl;
			cout << "Error Code: " << ErrorCode << endl;
			break;
		}
	}
	else {
		cout << "Stop Driver Success !!!" << endl;
	}

	CloseServiceHandle(shDriver);
	CloseServiceHandle(sh);

	getchar();
	getchar();
	
	return 0;
}