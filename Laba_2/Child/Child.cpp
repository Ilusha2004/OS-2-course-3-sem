#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
	cout << "Child created." << endl;

	if (argc > 1) {
		int size = atoi(argv[1]);
		char* arr = new char[size];

		for (int i = 2, j = 0; i < argc; ++i) { arr[j] = atoi(argv[i]); ++j; }

		for (int i = 0; i < size; i++) {
			int j = i - 1;
			int key = arr[i];
			while (arr[j] > key && j) {
				arr[j + 1] = arr[j];
				j--;
			}
			arr[j + 1] = key;
		}

		cout << "Array after sorting: ";
		for (int i = 0; i < size; ++i) { cout << arr[i] << " "; }
		cout << endl;

	}
	else { cout << "Child.exe not found" << endl; }

	system("pause");

	return 0;
}