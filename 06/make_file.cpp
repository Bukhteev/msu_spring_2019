#include <time.h>
#include <stdlib.h>
#include <stdio.h>
 
int main() {
    srand((unsigned)time(NULL));
    FILE* f = fopen("in2.bin", "w");
    int n = 200000000 + rand()%25;
    for (int i = 0; i < n; i++) {
        int a = 0 + rand()%10;
        fwrite(&a, sizeof(int), 1, f);
    // printf("\n");
    //     system("pause");
}
 
}