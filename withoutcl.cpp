
#include <iostream>
#include <vector>
#include <chrono>  // include the chrono library for timing

using namespace std;

void addVectors(vector<int>& a, vector<int>& b, vector<int>& c) {
    int n = a.size();
    for(int i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

int main() {
    // Create two input vectors
    int n = 1000000;
    vector<int> a(n);
    vector<int> b(n);
    vector<int> c(n);
    for(int i = 0; i < n; i++) {
        a[i] = i;
        b[i] = i * 2;
    }

    // Measure the execution time
    auto start_time = chrono::steady_clock::now();  // get the start time
    addVectors(a, b, c);
    auto end_time = chrono::steady_clock::now();    // get the end time
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);  // calculate the duration in microseconds

    // Print the result and execution time
    for(int i = 0; i < n; i++) {
        cout << c[i] << " ";
    }
    cout << endl;
    cout << "Execution time: " << duration.count() << " microseconds" << endl;

    return 0;
}
