// We will create a 1D space where we will learn how to use structs and pointers.
// This exercise also serves as a way to get more in depth on how spaces in math work, and later one make 2D and 3D
// spaces to use physics!

#include <stdio.h>

struct Object1D
{
    float pos;
    float velocity;
    char icon;
};

int main(void)
{
    printf("Hello, World!\n");
    return 0;
}