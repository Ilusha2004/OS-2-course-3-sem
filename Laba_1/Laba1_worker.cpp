//Ковунов Илья Александрович 7 группа

#include <iostream>
#include <windows.h>
#include <ctime>
#include <process.h>

using namespace std;

volatile int AmountZero;

struct Array {
	int size;
	int* array;
};

/*
N5
Найти количество нулевых значений. После каждого действия элементов «спать» 12 миллисекунд. Завершить
свою работу.
*/

UINT WINAPI worker(void* array) {

	Array* temp_arr = (Array*)array;

	for (int i = 0; i < temp_arr->size; i++) {
		if (temp_arr->array[i] == 0) {
			AmountZero++;
		}
		Sleep(12);
	}
	return 0;

}

/*
1. Создать массив целых чисел, размерность и элементы которого вводятся с консоли( или сгенерировать
случайно).
2. Ввести время для остановки и запуска потока worker.
3. Создать поток worker, передать в поток данные: размер маcсива, масив и т.д.
4. Приостановить поток worker(SuspendThread), затем через некоторое время снова запустить поток.
5. Уметь создавать поток командой _beginthreadex
6. Дождаться завершения потока worker.
7. Вывести на консоль результат работы потока worker
8. Завершить работу.
*/

int main(int *argc, char** argv) {

	srand(time(NULL));

	HANDLE Worker;
	UINT IDWorker;

	int Time_Start;
	int Time_Stop;
	Array *Arr = new Array();

	cout << "Enter amount elements: ";
	cin >> Arr->size;
	int* array = new int[Arr->size];
	cout << "Randomise or manual [r/m]: ";

	char symbol;
	cin >> symbol;

	switch (symbol) {
		case 'r': for (int i = 0; i < Arr->size; i++) { array[i] = rand() % 10; cout << array[i] << " "; } break;
		case 'm': for (int i = 0; i < Arr->size; i++) cin >> array[i]; break;
		default: break;
	}

	Arr->array = array;

	cout << "\nInput stop time: "; cin >> Time_Stop;
	cout << "Input start time: "; cin >> Time_Start;
	cout << endl;

	Worker = (HANDLE)_beginthreadex(NULL, 0, worker, Arr, 0, &IDWorker);

	if (Worker == NULL) {
		return GetLastError();
	}

	SuspendThread(Worker);

	cout << "Worker stop working" << endl;

	Sleep(Time_Stop);
	ResumeThread(Worker);

	cout << "Worker start work again" << endl;

	WaitForSingleObject(Worker, INFINITE);

	cout << "Amount Zero in array: " << AmountZero << endl;

	TerminateThread(Worker, 0);

	CloseHandle(Worker);

}
