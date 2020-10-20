// Takahashi En

#include <ntddk.h>

VOID doNothing(
	HANDLE ppid,
	HANDLE mypid,
	BOOLEAN bcreate
) {

	DbgPrint("Process Notifying !!!\n");

}

VOID DriverUnload(
	PDRIVER_OBJECT pd
) {
	DbgPrint("Unload\n");
	PsSetCreateProcessNotifyRoutine(doNothing, TRUE);
}


NTSTATUS DriverEntry(
	PDRIVER_OBJECT driver,
	PUNICODE_STRING regPath
) {
	driver->DriverUnload = DriverUnload;

	DbgPrint("---%wZ---\n", regPath);

	PsSetCreateProcessNotifyRoutine(doNothing, FALSE);

	return 0;
}
