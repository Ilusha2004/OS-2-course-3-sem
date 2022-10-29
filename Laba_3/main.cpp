#include <windows.h>
#include <iostream>
#include <algorithm>
#include <vector>
 
using namespace std;
 
CRITICAL_SECTION cs;
HANDLE hOutEvent, hMultEvent;
int X = 0;
int lenght = 0;
int* res;

struct Information {
    int size;
    int* arr;
    int X;
};
 
DWORD WINAPI work(void* obj) {
    Information* info = (Information*)obj;
    int* f_arr = info->arr;

    cout << "Thread work is started" << endl;
    int time;

    cout << "Enter time to sleep: ";
    cin >> time;
 
    int* new_arr = new int[info->size];
    int t = 0;
    
    for(int i = 0; i < info->size; i++){
        if(f_arr[i] % 3 == 0){ new_arr[t] = f_arr[i]; t++; }
        Sleep(time);
    }

    for(int i = 0; i < info->size; i++){
        if(f_arr[i] % 3 != 0){ new_arr[t] = f_arr[i]; t++; }
        Sleep(time);
    }

    res = new_arr;

    cout  << "------------------------------------------------------------------------------------------------------" << endl 
    << "Thread work is finished" << endl
    << "------------------------------------------------------------------------------------------------------" << endl;
    SetEvent(hOutEvent);
    return 0;
}
 
DWORD WINAPI CountElement(void *obj) {
    EnterCriticalSection(&cs);
    WaitForSingleObject(hMultEvent, INFINITE);
    cout << "------------------------------------------------------------------------------------------------------" << endl;
    cout << "Thread CountElement is started" << endl 
    << "------------------------------------------------------------------------------------------------------";
 
    Information* inf = (Information*)obj;
   
    for (int i = 0; i < inf->size; i++) { if (inf->X == res[i]) lenght++; }

    LeaveCriticalSection(&cs);
    return 0;
}
 
int main() {

    int size;
    cout << "Enter size of array: ";
    cin >> size;
    cout << "Enter elements of array: ";

    int* arr = new int[size];
    for (int i = 0; i < size; ++i) { cin >> arr[i]; }
 
    cout << "------------------------------------------------------------------------------------------------------" << endl << 
    "Input data: " << endl << "Size: " << size << endl << "Elements of array: ";

    for (int i = 0; i < size; ++i) { cout << arr[i] << " "; }
    cout << endl << "------------------------------------------------------------------------------------------------------" << endl;
 
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
 
    cout << "Enter X: ";
    cin >> X;
    info->X = X;
    cout << "------------------------------------------------------------------------------------------------------" << endl;
 
    dwCountWork = ResumeThread(hThreadWork);
    WaitForSingleObject(hOutEvent, INFINITE);
 
    cout << "Elements of array: ";
    for (int i = 0; i < info->size; ++i) { cout << res[i] << " "; }
    cout << endl;
 
    SetEvent(hMultEvent);
    EnterCriticalSection(&cs);
 
    cout << endl << "Result of CountElement: " << lenght << endl;
 
    DeleteCriticalSection(&cs);
    WaitForSingleObject(hThreadWork, INFINITE);
    WaitForSingleObject(hThreadMult, INFINITE);
    CloseHandle(hThreadWork);
    CloseHandle(hThreadMult);

    return 0;
}