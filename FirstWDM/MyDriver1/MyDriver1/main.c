//
// Create By Takahashi En
//

/*
When you create a new WDM Project
You need do this:
1. Configure your VS Env:
Debug->Active(x64) :{
	C / C++:{
	General: {
		Warning Level : Level4(/ W4)
			Treat Warrings As Errors : No(/ WX - )
		}
	Code Generation : {
		Spectre Mitigation : Disabled
	}
	}
	Inf2Cat{
		General: {
			Run Inf2Cat : No
		}
	}
}
2. Delete Manufacturer Entry in file XXX.inf
*/

#include <ntddk.h>

#define MY_DEVICE_NAME		L"\\Device\\MyDeviceName"
#define SYMBOL_LINK_NAME	L"\\??\\MyDeviceName"

//VOID doNothing(
//	HANDLE ppid,
//	HANDLE mypid,
//	BOOLEAN bcreate
//) {
//
//	DbgPrint("Process Notifying !!!\n");
//
//}

VOID DriverUnload(
	PDRIVER_OBJECT pDriver
) {
	DbgPrint("Unload\n");

	// do some clean
	if (pDriver->DeviceObject) {
		IoDeleteDevice(pDriver->DeviceObject);
		UNICODE_STRING symbolLinkName = { 0 };
		RtlInitUnicodeString(&symbolLinkName, SYMBOL_LINK_NAME);
		IoDeleteSymbolicLink(&symbolLinkName);
		DbgPrint("Dirver Clean Finish!\n");
	}
}

NTSTATUS MyCreate(
	PDEVICE_OBJECT pDevice,
	PIRP pIrp
) {
	NTSTATUS status = STATUS_SUCCESS;
	DbgPrint("MyCreate Func Running...\n");
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return status;
}

NTSTATUS MyClose(
	PDEVICE_OBJECT pDevice,
	PIRP pIrp
) {
	NTSTATUS status = STATUS_SUCCESS;
	DbgPrint("MyClose Func Running...\n");
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return status;
}

NTSTATUS MyCleanUp(
	PDEVICE_OBJECT pDevice,
	PIRP pIrp
) {
	NTSTATUS status = STATUS_SUCCESS;
	DbgPrint("MyCleanUp Func Running...\n");
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return status;
}

NTSTATUS MyRead(
	PDEVICE_OBJECT pDevice,
	PIRP pIrp
) {
	NTSTATUS status = STATUS_SUCCESS;
	DbgPrint("MyRead Func Running...\n");
	pIrp->IoStatus.Status = status;

	// Set Data that R3 will be read
	PIO_STACK_LOCATION pStack = IoGetCurrentIrpStackLocation(pIrp);

	// Request Info From R3 Prog
	ULONG readSize = pStack->Parameters.Read.Length;


	// SystemBuffer 和 R3 程序读请求时提供的缓冲区指向的是同一块物理地址
	// 内核在处理 R3 程序读请求时会将其提供的虚拟地址指向的物理空间的地址
	// 重映射为一份新的虚拟地址提供给内核的驱动程序使用
	// 故对这块地址的操作就相当于操作 R3 程序的内存空间
	PCHAR readBuffer = pIrp->AssociatedIrp.SystemBuffer;
	RtlCopyMemory(readBuffer, "Hello, This is Kernel Space!", \
		strlen("Hello, This is Kernel Space!"));

	pIrp->IoStatus.Information = strlen("Hello, This is Kernel Space!");
	DbgPrint("Kernel Message Len: %d\n", strlen("Hello, This is Kernel Space!"));
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return status;
}

NTSTATUS MyWrite(
	PDEVICE_OBJECT pDevice,
	PIRP pIrp
) {
	NTSTATUS status = STATUS_SUCCESS;
	DbgPrint("MyWrite Func Running...\n");
	pIrp->IoStatus.Status = status;

	// Set Data that R3 will be read
	PIO_STACK_LOCATION pStack = IoGetCurrentIrpStackLocation(pIrp);

	// Request Info From R3 Prog
	ULONG writeSize = pStack->Parameters.Write.Length;

	PCHAR writeBuffer = pIrp->AssociatedIrp.SystemBuffer;

	// Write to the Device Extension
	RtlZeroMemory(pDevice->DeviceExtension, 128);
	RtlCopyMemory(pDevice->DeviceExtension, writeBuffer, writeSize);

	pIrp->IoStatus.Information = writeSize;

	DbgPrint("--%s--%d--\n", (PCHAR)(pDevice->DeviceExtension), writeSize);

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return status;
}

NTSTATUS DriverEntry(
	PDRIVER_OBJECT pDriver,
	PUNICODE_STRING regPath
) {
	DbgPrint("---%wZ---\n", regPath);
	pDriver->DriverUnload = DriverUnload;

	NTSTATUS status = STATUS_SUCCESS;

	UNICODE_STRING deviceName = { 0 };
	RtlInitUnicodeString(&deviceName, MY_DEVICE_NAME);

	// Create Device
	PDEVICE_OBJECT pDevice = NULL;
	status = IoCreateDevice(
		pDriver,
		128,
		&deviceName,
		FILE_DEVICE_UNKNOWN,
		0,
		TRUE,
		&pDevice
	);
	if (!NT_SUCCESS(status)) {
		DbgPrint("Create Device Failed: %x\n", status);
		return status;
	}

	// Create Symbol link
	UNICODE_STRING symbolLinkName = { 0 };
	RtlInitUnicodeString(&symbolLinkName, SYMBOL_LINK_NAME);

	status = IoCreateSymbolicLink(&symbolLinkName, &deviceName);

	if (!NT_SUCCESS(status)) {
		DbgPrint("Create Symbol Link Faialed: %x\n", status);
		IoDeleteDevice(pDevice);
	}

	// 设置设备的读写方式，不然R3 读写时直接蓝屏
	pDevice->Flags |= DO_BUFFERED_IO;
	
	// Register Major Functions
	pDriver->MajorFunction[IRP_MJ_CREATE] = MyCreate;
	pDriver->MajorFunction[IRP_MJ_CLOSE] = MyClose;
	pDriver->MajorFunction[IRP_MJ_CLEANUP] = MyCleanUp;
	pDriver->MajorFunction[IRP_MJ_READ] = MyRead;
	pDriver->MajorFunction[IRP_MJ_WRITE] = MyWrite;

	return 0;
}
