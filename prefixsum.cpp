#include "iostream"
#include "vector"
#include "ctime"
#include "omp.h"
#include "math.h"


using namespace std;
vector<int> calcPrefixSum ( vector<int> input, int num_threads){
    int n = input.size();
    int s = input[n-1];
    for (int i=0;i<(int)((float)log(n)/(float)log(2));i++){
        omp_set_num_threads(num_threads);
        #pragma omp parallel
        {
            int id = omp_get_thread_num();
            for (int j=id;j<=(n-1)/(int)pow(2,i+1);j+=num_threads){
                input[j*(int)pow(2,i+1)+(int)pow(2,i+1)-1]= input[j*(int)pow(2,i+1)+(int)pow(2,i)-1]+input[j*(int)pow(2,i+1)+(int)pow(2,i+1)-1];
            }
        }
    }

    input[n-1]=0;
    for (int i=(int)((float)log(n)/(float)log(2))-1;i>=0;i--){
        omp_set_num_threads(num_threads);
        #pragma omp parallel
        {
            int id = omp_get_thread_num();
            for (int j=id;j<=((n-1)/pow(2,i+1));j+=num_threads){
                int x = input[j*(int)pow(2,i+1)+(int)pow(2,i)-1];
                input[j*(int)pow(2,i+1)+(int)pow(2,i)-1] = input[j*(int)pow(2,i+1)+(int)pow(2,i+1)-1];
                input[j*(int)pow(2,i+1)+(int)pow(2,i+1)-1] = input[j*(int)pow(2,i+1)+(int)pow(2,i+1)-1]+x;
            }
        }
    }

    vector<int> finput;
    for (int i=0;i<n-1;i++){
        finput.push_back(input[i+1]);
    }
    finput.push_back(input[n-1]+s);

    return finput;
}



