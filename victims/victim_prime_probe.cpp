#include <l1.hpp>
#include <utils.hpp>
#include <cstdio>
int main(void){

    char super_secret[10] = { 'C', 'T', 'F', '{', 'M', 'E', 'M','E', '}', '\n'};

    while(true) {
        for (int i = 0; i < 10; i++) {
            int val = super_secret[i];
            putchar(val);
        }
    }

    return 0;
}
