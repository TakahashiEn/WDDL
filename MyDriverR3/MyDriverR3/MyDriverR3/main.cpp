//
// Created By Takahashi En for Do Some Operations With Driver
//

#include <Windows.h>
#include <iostream>
#include <winioctl.h>

using std::cout;
using std::cin;
using std::endl;

// 自定义I/O 控制的识别码的宏
#define IOCTL_MUL			CTL_CODE(FILE_DEVICE_UNKNOWN, \
									0x8888, METHOD_BUFFERED, FILE_ANY_ACCESS)

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

		cout << "IO Control" << endl;
		system("pause");

		DWORD32 a = 0x00000001;
		DWORD32 b = 0x00000000;
		DWORD ioRet = 0;

		DeviceIoControl(
			handle,
			IOCTL_MUL,
			&a,
			4,
			&b,
			4,
			&ioRet,
			NULL
		);

		cout << "--" << a << "--" << b << "--" << ioRet << endl;

	}
	
	system("pause");
	CloseHandle(handle);
	return 0;

}