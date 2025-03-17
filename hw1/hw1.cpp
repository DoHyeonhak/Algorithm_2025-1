/*
[ Reference ]
 * 강의 slide chapter 25-26, 33-35
 * blog : 음수 무한대 선언 : https://velog.io/@u_jinju/C-INTMIN-INTMAX
*/

#include <iostream>
#include <string>
#include <limits.h>         // for using INT_MIN (a negative infinite number)
#include <cctype>
using namespace std;

#define MAX_HEAP_SIZE 30    // Heap maximum size

int heap_size = 0;
int length = 0;             // 기본 heap을 위한 변수, priority Q에서는 필요하지 않음(heap_size로 다 계산 가능)

typedef struct{             // 프로그램 구동에 필요한 struct 변수 (학생 이름, 점수, 강의)
    string student;         // student name : 최대 20자 입력 받을 수 있음, 과제에서 20자 이하로 항상 입력받는 것으로 가정 명시, char[20]아닌 string으로 선택
    int score;              // score
    string classname;       // class name
}Data;


/* Function about Heap */ 

void Max_Heapify(Data arr[], int i){    // Priority Queue function인 EXTRACT_MAX에서 사용
    int left = 2 * i;
    int right = 2 * i + 1;
    int largest = 0;

    if(left <= heap_size && arr[left].score > arr[i].score){
        largest = left;
    }else{
        largest = i;
    }
    
    if(right <= heap_size && arr[right].score > arr[largest].score){
        largest = right;
    }

    if(largest!=i){
        Data temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;

        Max_Heapify(arr, largest);
    }
}

void Build_Max_Heap(Data arr[]){
    heap_size = length;
    for(int i = length/2; i >= 1; i--){
        Max_Heapify(arr, i);
    }
}

void HeapSort(Data arr[]){
    Build_Max_Heap(arr);
    int temp_size = heap_size;
    for(int i = length; i >= 2; i--){
        Data temp = arr[1];
        arr[1] = arr[i];
        arr[i] = temp;

        heap_size--;
        
        Max_Heapify(arr, 1);
    }
    heap_size = temp_size;
}


/* Function about Priority Queue */ 

Data EXTRACT_MAX(Data arr[]){
    if(heap_size == 0){
        cout << "Cannot delete from an empty queue." << endl;
        return (Data){"", -1, ""};
    }

    Data max = arr[1];
    arr[1] = arr[heap_size];
    heap_size--;

    Max_Heapify(arr, 1);

    return max;
}

Data MAXIMUM(Data arr[]){
    return arr[1];
}

void INCREASE_KEY(Data arr[], int i, Data key){
    if(key.score < arr[i].score){
        cout << "new key is smaller than current key" << endl;
        return;
    }
    arr[i] = key;
    while(i > 1 && arr[i/2].score < arr[i].score){  // 결과적으로 max-heap 유지시킴
        Data temp = arr[i];
        arr[i] = arr[i/2];
        arr[i/2] = temp;
        i = i/2;
    }
}

void INSERT(Data arr[], Data key){
    heap_size++;
    arr[heap_size].score = INT_MIN;   // using limits.h : int type에서 가장 작은 숫자를 위함 (algorithm 강좌 slide에서는 -무한대로 표시되어 있음)
    INCREASE_KEY(arr, heap_size, key);
}

void PRINT_ALL(Data arr[]){
    if(heap_size < 1){
        cout << "Print impossible: Queue is empty." << endl;
        return;
    }
    cout << "Current queue elements:" << endl;
    for(int i = 1; i <= heap_size; i ++){
        cout << i << ". [" << arr[i].student << ", " << arr[i].score << ", " << arr[i].classname << "]" << endl;
    }
}

Data GET_ELEMENT(Data arr[], int idx){
    return arr[idx];
}


int main(){

    Data arr[MAX_HEAP_SIZE+1];  // index는 1부터 시작하고, heap maximum size는 30이므로 30 + 1로 지정
    char cmd = 0;

/*
    (Optional):
    Does your program work correctly when there is a space before or after a
    menu choice, such as " P" or "p "? Which implementation is more reasonable:
    allowing spaces or restricting input to exact characters?
*/
    while(1){
        cout << "*********** MENU ****************\nI : Insert a new element into the queue.\nD : Delete the element with the largest key from the queue.\nR : Retrieve the element with the largest key.\nN : Increase the key of an element in the queue.\nP : Print all elemenets in the queue.\nQ : Quit.\n" << endl;
        cout << "Choose menu: ";
        
        cin >> cmd;
        cin.ignore();

        cmd = toupper(cmd); // 소문자라면 대문자로 전환 : case-insensitive
        
        if(cmd == 'I'){
            if(heap_size == MAX_HEAP_SIZE){
                cout << "Heap is already full\n" << endl;
                continue;
            }

            Data info;  // new element variable
        
            // 학생 이름, 점수, 강의 데이터 읽기
            cout << "Enter the name of the student: ";
            getline(cin, info.student); // 개행문자 단위로 나뉘어짐(공백은 문자열 포함)
            
            while(1){
                string temp;
                bool isInteger = true;

                cout << "Enter the score of the element: ";
                cin >> temp;
                cin.ignore();   // 개행문자 제거
                
                for(int i = 0; i < temp.length(); i++){ // 정수 이외의 다른 것이 포함된 경우 확인
                    if(!isdigit(temp[i])){
                        isInteger = false;
                        break;
                    }
                }

                if(!isInteger){ // 정수가 아닌 다른 문자가 포함된다면
                    cout << "This is a non-numeric format. Please enter numeric characters only." << endl;
                    continue;
                }
                
                info.score = stoi(temp);

                if(info.score < 0 || info.score > 100){ // 점수 범위 확인
                    cout << "Invalid score. Please enter a valid integer between 0 and 100." << endl;
                    continue;
                }

                break;
            }

            cout << "Enter the class name: ";
            getline(cin, info.classname);

            // 요소 추가
            INSERT(arr, info);

            cout << "New element [" << info.student << ", " << info.score << ", " + info.classname << "] has been inserted. " << endl;

        }else if(cmd == 'D'){
            if(heap_size == 0){     // Queue가 비어있는 경우
                cout << "Cannot delete from an empty queue." << endl;
                continue;
            }

            // 추출 및 제거, 출력
            Data key = EXTRACT_MAX(arr);
            cout << "Deleted element: [" << key.student << ", " << key.score << ", " << key.classname << "]" << endl;

        }else if(cmd == 'R'){
            if(heap_size == 0){     // Queue가 비어있는 경우
                cout << "Queue is empty." << endl;
                continue;
            }

            // 추출 및 출력
            Data key =MAXIMUM(arr);
            cout << "Element with the largest key: [" << key.student << ", " << key.score << ", " << key.classname << "]" << endl;

        }else if(cmd == 'N'){
            int idx = 0;
            int score = 0;
            
            cout << "Enter the index of the element: ";
            cin >> idx;
            
            Data info = GET_ELEMENT(arr, idx);

            while(1){
                string temp;
                bool isInteger = true;

                cout << "Enter the new score: ";
                cin >> temp;
                cin.ignore();   // 개행문자 제거
                
                for(int i = 0; i < temp.length(); i++){ // 정수 이외의 다른 것이 포함된 경우 확인
                    if(!isdigit(temp[i])){
                        isInteger = false;
                        break;
                    }
                }

                if(!isInteger){ // 정수가 아닌 다른 문자가 포함된다면
                    cout << "This is a non-numeric format. Please enter numeric characters only." << endl;
                    continue;
                }
                
                score = stoi(temp);

                if(score <= info.score){ // 이전 숫자보다 작다면
                    cout << "New score should be larger than current score. Please enter again." << endl;
                    continue;
                }

                if(score < 0 || score > 100){ // 점수 범위 확인
                    cout << "Invalid score. Please enter a valid integer between 0 and 100." << endl;
                    continue;
                }

                info.score = score;
                break;
            }

            // 수정된 key를 바탕으로 heap 정렬 및 결과 출력
            INCREASE_KEY(arr, idx, info);
            cout << "key updated. [" << info.student << ", " << info.score << ", " << info.classname << "] has been repositioned in the queue." << endl;

        }else if(cmd == 'P'){
            PRINT_ALL(arr); // 모든 요소 출력
        }else if(cmd == 'Q'){
            cout << "Program terminated." << endl;
            break;  
        }else{
            cout << "Enter the command correctly!" << endl;
        }
        cout << endl;
    }

    return 0;
}