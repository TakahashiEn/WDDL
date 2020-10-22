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
		0,
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
	
	// Register Major Functions
	pDriver->MajorFunction[IRP_MJ_CREATE] = MyCreate;
	pDriver->MajorFunction[IRP_MJ_CLOSE] = MyClose;
	pDriver->MajorFunction[IRP_MJ_CLEANUP] = MyCleanUp;

	return 0;
}
