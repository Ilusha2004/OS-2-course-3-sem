#include <iostream>
#include <string>
#include <windows.h>

//Сортировка выбором. Полученный массив вывести. Тип элементов - символы.

using namespace std;

int main(int arc, char** argv) {
	srand(time(NULL));

	int size;

	cout << "Enter size of array: ";
	cin >> size;

	char* array = new char[size];

	cout << "Randomise or manual [r/m]: ";

	char symbol;
	cin >> symbol;

	switch (symbol) {
	case 'r': for (int i = 0; i < size; i++) { array[i] = char(rand() % 128); cout << array[i] << " "; } break;
	case 'm': 
		cout << "Enter " << size << " elements of array: ";
		for (int i = 0; i < size; i++) { cin >> array[i]; break; }

	default: break;
	}

	cout << endl;

	wstring cmd_info = L"\"Child.exe\" ";
	cmd_info += to_wstring(size);
	cmd_info += L" ";
	for (int i = 0; i < size; i++) {
		cmd_info += to_wstring(array[i]);
		if (i != size - 1) {
			cmd_info += L" ";
		}
	}

	wchar_t* stemp = new wchar_t[cmd_info.length()];
	wcscpy_s(stemp, cmd_info.length() + 1, cmd_info.c_str());

	STARTUPINFO si;
	PROCESS_INFORMATION piApp;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USECOUNTCHARS | STARTF_USEPOSITION | STARTF_USEFILLATTRIBUTE;
	si.dwFillAttribute = FOREGROUND_GREEN;
	si.dwY = 1000;
	si.dwYCountChars = 90000;

	// Установить высоту (Y) смещения от верхнего левого угла экрана.

	if (!CreateProcess(nullptr, stemp, nullptr, nullptr, FALSE, CREATE_NEW_CONSOLE, nullptr, nullptr, &si, &piApp)) {
		cout << "New process is not working" << endl;
		return 0;
	}

	cout << "Process is working" << endl;

	WaitForSingleObject(piApp.hProcess, INFINITE);
	CloseHandle(piApp.hThread);
	CloseHandle(piApp.hProcess);

	cout << "Process was closed" << endl;

	return 0;
}