#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int a[32];

unsigned long crash_array(int n){
    int i;
    unsigned long sum = 0;

    for (i=0; i<n; i++){
        sum = sum + a[i];
    }

    return sum;

}

unsigned long sum(int n){
    int i;
    unsigned long sum = 0;

    for (i=0; i < n; i++){
        sum = sum + i;
    }
    return sum;
}

void foo(const char * num){
    printf(num);

}

int main(void) { 

    unsigned long s;
    unsigned long s1; 

    s = sum(100);
    printf("Hello World, the sum:%ld\n", s);

    s1 = crash_array(32);
    printf("crash array sum:%ld\n",s1);
    
    return 0; 
}
