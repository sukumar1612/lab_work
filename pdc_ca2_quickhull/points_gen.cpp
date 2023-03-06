#include<bits/stdc++.h>
#include <fstream>
#include <ctime>
#include <cstdlib>

using namespace std;
#define IntegerPair pair<int, int>

int main(){
    ofstream outfile1("num1.txt", ios::out | ios::binary);
    ofstream outfile2("num2.txt", ios::out | ios::binary);

    int minVal = -23000;
    int maxVal = 23000;
    int n = 100000000;

    vector<int> a1(n);
    vector<int> a2(n);

    srand(time(0));
    int numSamplesPerDimension = ceil(sqrt(n));
    double stepSize = (double)(maxVal - minVal) / numSamplesPerDimension;
    for (int i = 0; i < numSamplesPerDimension; i++) {
        for (int j = 0; j < numSamplesPerDimension; j++) {
            a1[i] = (double)i * stepSize + (rand() % (int)stepSize) + minVal;
            a2[i] = (double)j * stepSize + (rand() % (int)stepSize) + minVal;
        }
    }
    ostream_iterator<int> oi1(outfile1);
    copy(a1.begin(), a1.end(), oi1);

    ostream_iterator<int> oi2(outfile2);
    copy(a2.begin(), a2.end(), oi2);

    outfile1.close();
    outfile2.close();

    return 0;
}