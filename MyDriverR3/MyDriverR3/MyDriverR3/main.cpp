//
// Created By Takahashi En for Do Some Operations With Driver
//

#include <Windows.h>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

int main() {

	HANDLE handle = NULL;
	handle = CreateFile(
		L"\\\\.\\MyDeviceName",
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		&handle
	);

	if (handle == INVALID_HANDLE_VALUE) {
		cout << "Open Device File Error!\n" << endl;
		system("pause");
		return 0;
	}
	else {
		cout << "Open Device File Success!\n" << endl;
		system("pause");
	}
	
	CloseHandle(handle);
	system("pause");
	return 0;

}