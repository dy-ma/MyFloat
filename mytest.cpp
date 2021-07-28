#include <iostream>
#include <assert.h>
#include "MyFloat.h"

int main(int argc, char** argv) {
    float f1 = strtof(argv[1],NULL);
    float f2 = strtof(argv[2],NULL);
    MyFloat mf1(f1);
    MyFloat mf2(f2);
    if (mf1 < f2) cout << "true" << endl;
    else cout << "false" << endl;
}