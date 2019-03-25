#include<iostream>
#include<cstdlib>
#include<cmath>
#include "numbers.dat"


//find index
int search(int const * Data, int value, int left, int right) {
        int middle = (left + right) / 2;

        if (left == right)
                return left;

        if (Data[middle] == value)
                return middle;

        if (Data[middle] > value)
                return search(Data, value, left, middle - 1);
        else
                return search(Data, value, middle + 1, right);
}

size_t post_search(int const * Data, size_t idx, int value, const char*  what) {

        if (what == "left"){
            while (Data[idx - 1] == value)
                --idx;
            return idx;
        }
        if(what == "right"){
            while (Data[idx + 1] == value)
                ++idx;
            return idx;
        }
        else{
            return 0;
        }
}

bool prime (int n){

    if (n == 1) 
        return false;

    int end = sqrt(n);
    for (int i = 2; i <= end; i++){
        if (n%i == 0)
            return false;
        
    }

    return true;
}

int counter(int left, int right, const int*  Data, size_t size){
    int count = 0;
    int left_index = post_search(Data, search(Data, left, 0, size - 1), left, "left");
    int right_index = post_search(Data, search(Data, right, 0, size - 1), right, "right");

    for (int i = left_index; i < right_index+1; i++){
        if(prime(Data[i]))
            count ++;
    }

    return count;
}

int main(int argc, char* argv[])
{
    if (!(argc % 2) || (argc == 1)) 
        return -1;

    for (int i = 1; i < argc; i += 2) {
        int left = std::atoi(argv[i]);
        int right = std::atoi(argv[i + 1]);
        std::cout << counter(left, right, Data, Size) << std::endl;

    }

    return 0;
}
