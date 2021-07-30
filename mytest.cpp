#include <iostream>
#include <assert.h>
#include "MyFloat.h"

void test(float m, float n, bool op) {
    MyFloat mf1(m), mf2(n);
    float f1(m), f2(n);
    if (op == 0) {
        if ((mf1 + mf2) == (f1 + f2))
            cout << "passed: " << m << " + " << n << " = " << (mf1 + mf2) << endl;
        else 
            cout << "failed: " << m << " + " << n << " = " << (mf1 + mf2) << endl;
    }
    else {
        if (mf1 - mf2 == f1 - f2)
            cout << "passed: " << m << " - " << n << " = " << (mf1 - mf2) << endl;
        else 
            cout << "failed: " << m << " - " << n << " = " << (mf1 - mf2) << endl;
    }
}

int main(int argc, char** argv) {
    test(1, 1, 0);
    test(2, 2, 0);
    test(10, 10, 0);
    test(20, 10, 0);
    test(1.5, 1.5, 0);
    test(2.25, 3.5, 0);
    test(4.75, 1.25, 0);
    test(0.5, 0.5, 0);
    test(100, 50, 0);
    cout << endl;
    test(1, 1, 1);
    test(2, 2, 1);
    test(10, 10, 1);
    test(20, 10, 1);
    test(1.5, 1.5, 1);
    test(2.25, 3.5, 1);
    test(4.75, 1.25, 1);
    test(0.5, 0.5, 1);
    test(100, 50, 1);
    cout << endl;
    test(1, -1, 0);
    test(1, -1, 1);
    test(12464317.75, -12464317.75,0);
    test(12464317.75, -12464317.75,1);
}