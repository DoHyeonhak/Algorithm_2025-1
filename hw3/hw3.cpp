#include <iostream>
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
    if(k == 1) return n; // K = 1
    if(n == 1) return 1; // N = 1
    if(n == 0) return 0; // N = 0

    // Try dropping from every height X (1 to N) and take the worst case
    int min_attempts = INT_MAX;
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
    // Fill base cases
    for(int i = 1; i <= k; i++){
        dp[i][0] = 0;
        dp[i][1] = 1;
    }
    for(int i = 1; i <= n; i++){    // k == 1인 케이스. (n==1 or 0)인 경우보다 뒤에 해야지 base init 정확
        dp[1][i] = i;
    }
    
    // Fill the DP table
    for (int i = 2; i <= k; i++) { // Objects
        for (int j = 2; j <= n; j++) { // Meters
        // Try dropping from every height X (1 to j)
            dp[i][j] = INT_MAX; // 초기화 안해주면 쓰레기값 때문에 문제가 생김
            for (int x = 1; x <= j; x++) {
                // case 1: when object breaks, b = dp[.....][.....];
                int b = dp[i-1][x-1];
                // case 2: when object survives, s = dp[.....][.....];
                int s = dp[i][j-x];
            
                int worst = 1 + max(b, s);
                dp[i][j] = min(dp[i][j], worst); // Take minimum over all trials
            } // end of for x
        } // end of for j
    } // end of for i

    return dp[k][n];
}
