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

	CHAR readBuffer[64] = { 0 };

	DWORD readNums = 0;
	DWORD writeNums = 0;

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
		cout << "Read!" << endl;
		system("pause");
		ReadFile(handle, (PVOID)readBuffer, 64, &readNums, NULL);
		cout << "----" << readBuffer << "--" << readNums << "----" << endl;

		cout << "Write" << endl;
		system("pause");
		WriteFile(handle,
			"Hello, I am R3!",
			strlen("Hello, I am R3!"),
			&writeNums,
			NULL
		);
		cout << "----" << "Hello, I am R3!" \
			 << "--" << writeNums << "----" << endl;

	}
	
	system("pause");
	CloseHandle(handle);
	return 0;

}