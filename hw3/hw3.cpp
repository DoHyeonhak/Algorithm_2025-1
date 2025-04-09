#include <iostream>
#include <iomanip>  // Time을 소수점 형식으로 출력하기 위함
#include <ctime>
using namespace std;


int recursive(int k, int n);
int DP(int k, int n);

int main(){
    clock_t start, stop;
    double duration = 0;
    int K = 0, N = 0;

    cout << "Enter the number N: ";
    cin >> N;
    cout << "Enter the number K: ";
    cin >> K;
    
    cout << "N: "<< N << ", K: "<< K << endl;

    cout << fixed << setprecision(10); // duration이 2e-05처럼 지수형으로 표현되는 것을 방지하기 위함

    // recursive
    start = clock();
    int output_recur = recursive(K, N);
    stop = clock();

    duration = (double)(stop - start) / CLOCKS_PER_SEC;
    
    cout << "\n[Resursive method]" << endl;
    cout << "Output: " << output_recur << endl;
    cout << "Time: " << duration << " seconds" << endl;
    
    // dp
    start = clock();
    int output_dp = DP(K, N);
    stop = clock();
    
    duration = (double)(stop - start) / CLOCKS_PER_SEC;
    
    cout << "\n[DP method]" << endl;
    cout << "Output: " << output_dp << endl;
    cout << "Time: " << duration << " seconds"<< endl;

    return 0;
}

int recursive(int k, int n){
    // Write base case code here
    if(k == 1) return n; // K == 1이면 N
    if(n == 1) return 1; // N == 1이면 1
    if(n == 0) return 0; // N == 0이면 0

    // Try dropping from every height X (1 to N) and take the worst case
    int min_attempts = INT_MAX; // min() 계산을 위해 INT_MAX로 초기화
    for (int x = 1; x <= n; x++) {
        // case1: Object breaks. Recursive call
        int break_case = recursive(k-1, x-1);
        // case2: Object survives. Recursive call
        int survive_case = recursive(k, n-x);
        // worst_case = 1 + max(case1, case2)
        int worst_case = 1 + max(break_case, survive_case);
        
        min_attempts = min(min_attempts, worst_case);
    }

    return min_attempts;
}

int DP(int k, int n){
    // DP Table: dp[i][j] represents minimum attempts for i objects and j meters
    int dp[k + 1][n + 1];
    // base cases
    for(int i = 1; i <= n; i++){  // K == 1이면 N
        dp[1][i] = i;
    }
    for(int i = 1; i <= k; i++){
        dp[i][0] = 0; // N == 0이면 0
        dp[i][1] = 1; // N == 1이면 1
    }
    // DP는 Bottom-UP 방식이라서 아래부터 올라간다. 
    // 그래서 대상 문제에 대하여 하위 문제들부터 풀어나가는 형식으로 처리한다. 
    for (int i = 2; i <= k; i++) {
        for (int j = 2; j <= n; j++) {
            dp[i][j] = INT_MAX; // 쓰레기값 방지를 위한 초기화
            // Try dropping from every height X (1 to j)
            for (int x = 1; x <= j; x++) {
                // case 1: when object breaks
                int b = dp[i-1][x-1];
                // case 2: when object survives
                int s = dp[i][j-x];
            
                int worst = 1 + max(b, s);
                dp[i][j] = min(dp[i][j], worst);
            }
        }
    }

    return dp[k][n];
}
