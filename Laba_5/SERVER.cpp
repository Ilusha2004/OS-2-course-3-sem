#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;

int main(int argc, char** argv) {
	srand(time(0));

	int size;
	cout << "SERVER: Enter array size: ";
	cin >> size;
	__int8* arr = new __int8[size];

	for (int i = 0; i < size; ++i) {
		arr[i] = rand() % 256;
		if (arr[i] < 0) {
			arr[i] *= -1;
		}
	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hWritePipe, hReadPipe, hEvent;
	SECURITY_ATTRIBUTES sa;

	hEvent = CreateEvent(nullptr, FALSE, FALSE, L"Event");
	if (!hEvent) {
		cout << "SERVER: Event was not created";
		return 0;
	}

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = nullptr;
	sa.bInheritHandle = TRUE;

	if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
		cout << "SERVER: Pipe was not created";
		return 0;
	}

	ZeroMemory(&si, sizeof(STARTUPINFO));
	wchar_t* wstr = new wchar_t[80];

	wsprintf(wstr, L"\"SORT.exe\" %d %d %d", (int)hWritePipe, (int)hReadPipe, (int)size);

	if (!CreateProcess(nullptr, wstr, nullptr, nullptr, TRUE, CREATE_NEW_CONSOLE, FALSE, FALSE, &si, &pi)) {
		cout << "SERVER: SORT creation failed" << endl;
		return 0;
	}

	__int8* info = new __int8[size + 1];
	info[0] = (__int8)size;
	for (size_t i = 1; i < size + 1; ++i) {
		info[i] = arr[i - 1];
	}

	DWORD dwBytesWritten;
	if (!WriteFile(hWritePipe, info, sizeof(__int8) * (size + 1), &dwBytesWritten, nullptr)) {
		cout << "SERVER: info was not written" << endl;
		return 0;
	}

	WaitForSingleObject(hEvent, INFINITE);

	cout << "SERVER:" << endl;
	cout << "size: " << (int)info[0] << "\n";
	cout << "Random numbers: ";

	for (size_t i = 1; i < info[0]; ++i) {
		cout << (int)info[i] << " ";
	}

	cout << endl << "Sorted array: ";

	for (size_t i = 1; i < size; i++) {
		__int8* val = new __int8[1];

		DWORD dwBytesRead;
		if (!ReadFile(hReadPipe, val, sizeof(__int8), &dwBytesRead, nullptr)) {
			cout << "SERVER: arr was not read" << endl;
			return 0;
		}
		Sleep(100);
		cout << (int)val[0] << " ";
	}

	cout << endl;

	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(hEvent);

	while (_getch() != '0') {}
}