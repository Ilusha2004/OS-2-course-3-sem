#include <windows.h>
#include <iostream>
 
using namespace std;
 
CRITICAL_SECTION cs;
HANDLE hOutEvent, hMultEvent;
volative int x = 0;
volative int lenght = 0;

int* res;

struct Information {
    int size;
    char* arr;
    int X;
};
 
DWORD WINAPI work(void* obj) {
    Information* info = (Information*)obj;
    char* f_arr = info->arr;
 
    cout << "--- Thread work is started" << endl;
    int time;

    cout << "Enter time to sleep: ";
    cin >> time;
 
    int t = 0;
    int ind = 0;
    res = new int[info->size];
    
    for (int i = 0; i < info->size; ++i) {
        if (isdigit(f_arr[i])) {
            int t = int(f_arr[i]) - 48;
            if (t % 2 == 0) {
                res[ind] = f_arr[i];
                ind++;
            }
        }
        Sleep(time);
    }
 
    for (int i = 0; i < info->size; ++i) {
        if (isdigit(f_arr[i])) {
            int t = int(f_arr[i]) - 48;
            if (t % 2 == 1) {
                res[ind] = f_arr[i];
                ind++;
            }
        }
        Sleep(time);
    }
 
    cout << "  Result array: ";
    for (int i = 0; i < info->size; ++i) { cout << res[i] << " "; }
 
    cout << endl;
    cout << "--- Thread work is finished" << endl << endl;
    SetEvent(hOutEvent);
    return 0;
}
 
DWORD WINAPI CountElement(void *obj) {
    EnterCriticalSection(&cs);
    WaitForSingleObject(hMultEvent, INFINITE);
    cout << "--- Thread MultElement is started" << endl;
 
    Information* inf = (Information*)obj;
   
    for (int i = 0; i < inf->size; i++) { if (inf->X == res[i]) lenght++; }

    LeaveCriticalSection(&cs);
    return 0;
}
 
int main() {

    int size;

    cout << "Enter size of array: " << endl << "  ";

    cin >> size;

    cout << endl << "Enter elements of array: " << endl << "  ";
    
    char* arr = new char[size];

    for (int i = 0; i < size; ++i) { cin >> arr[i]; }
 
    cout << endl << "-----------------------------------" << endl << endl << 
    "Input data: " << endl << "  Size: " << size << endl << "  Elements of array: ";

    for (int i = 0; i < size; ++i) { cout << arr[i] << " "; }
    cout << endl;
 
    Information* info = new Information();
    info->arr = arr;
    info->size = size;
 
    InitializeCriticalSection(&cs);
 
    hOutEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    hMultEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
 
    HANDLE hThreadWork;
    DWORD IDThreadWork;
    DWORD dwCountWork;
    hThreadWork = CreateThread(NULL, 0, work, (void*)info, CREATE_SUSPENDED, &IDThreadWork);
 
    HANDLE hThreadMult;
    DWORD IDThreadMult;
    DWORD dwCountMult;
    hThreadMult = CreateThread(NULL, 0, CountElement, (void*)info, NULL, &IDThreadMult);
 
    cout << endl << "Enter X: ";
    cin >> X;
    info->X = X;
    cout << endl;
 
    dwCountWork = ResumeThread(hThreadWork);
    WaitForSingleObject(hOutEvent, INFINITE);
 
    cout << "Elements of array: ";

    for (int i = 0; i < info->size; ++i) { cout << res[i] << " "; }

    cout << endl;
 
    SetEvent(hMultEvent);
    EnterCriticalSection(&cs);
 
    cout << endl << "Result of MultElement: " << lenght << endl;
 
    DeleteCriticalSection(&cs);
    WaitForSingleObject(hThreadWork, INFINITE);
    WaitForSingleObject(hThreadMult, INFINITE);
    CloseHandle(hThreadWork);
    CloseHandle(hThreadMult);
 
    return 0;
}