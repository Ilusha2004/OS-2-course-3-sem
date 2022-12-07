#include <iostream>
#include <windows.h>
#include <vector>
#include <algorithm>
#include <conio.h>

using namespace std;

int main(int argc, char** argv) {
	HANDLE hWritePipe = (HANDLE)atoi(argv[1]);
	HANDLE hReadPipe = (HANDLE)atoi(argv[2]);

	int size = atoi(argv[3]);

	cout << size << endl;

	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Event");
	if (!hEvent) {
		cout << "SORT: event was not read";
	}

	__int8* info = new __int8[size + 1];

	DWORD dwBytesRead;
	if (!ReadFile(hReadPipe, info, sizeof(__int8) * (size + 1), &dwBytesRead, NULL)) {
		cout << "SORT: info was not read";
	}

	cout << "SORT: array: ";

	for (size_t i = 1; i <= info[0]; i++) {
		cout << (int)info[i] << " ";
	}

	cout << endl;

	__int8* arr = new __int8[info[0]];

	cout << "SORT: sorting: " << endl;

	vector<__int8> temp_arr;
	for (size_t i = 1; i < size + 1; i++) {
		temp_arr.push_back(info[i - 1]);
	}

	sort(temp_arr.begin(), temp_arr.end(), [&temp_arr](__int8 a, __int8 b)
		{ return a < b; }
	);

	__int8* res = new __int8[temp_arr.size()];
	for (int i = 0; i < temp_arr.size(); ++i) {
		res[i] = temp_arr[i];
	}

	cout << "SORT: transfering data";
	for (size_t i = 2; i < temp_arr.size(); i++) {
		DWORD dwBytesWritten;
		__int8* val = new __int8[1];
		val[0] = res[i];
		if (!WriteFile(hWritePipe, val, sizeof(__int8), &dwBytesWritten, NULL)) {
			cout << "SORT: res was not written";
		}
	}

	SetEvent(hEvent);
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(hEvent);

	while (_getch() != '0') {}
}