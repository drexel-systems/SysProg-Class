
#include <stdio.h>
#include <stdlib.h> //where we pickup malloc()/free()

void fn();

// note all variable start with "d_" so we can easily isolate them
// in the readelf command with grep, no other reason
// int d_glob_not_init;
// int d_glob_zero_init = 0;
// int d_glb_init = 100;

// static int d_stat_not_init;
// static int d_stat_zero_init = 0;
// static int d_stat_init = 200;

// const int d_const_init = 300;
// const static int d_const_stat_init = 400;

void fn() {
    int *i = malloc(sizeof(int));
}

int main()
{
    fn();
    return 0;
}