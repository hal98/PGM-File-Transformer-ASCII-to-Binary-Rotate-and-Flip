/* Classify a triangle according to the integer lengths of its sides. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>

// Integer constants representing types of triangle.
enum { Equilateral, Isosceles, Right, Scalene, Flat, Impossible, Illegal};

// Check whether a character is a digit.
bool digit(char c) {
    // if(c>= '0' && c<='9') return true;
    // else return false;
    return isdigit(c);
}
// Convert a side lenght string into an integer.

long convertsa(const char sa[]) {
    return atoi(sa);
}
long convertsb(const char sb[]) {
    return atoi(sb);
}
long convertsc(const char sc[]) {
    return atoi(sc);
}

// Check whether a triangle string is properly formatted and in range.
bool valid(const char sa[], const char sb[], const char sc[]) {
    
    long la=strlen(sa);
    long lb=strlen(sb);
    long lc=strlen(sc);
    if(strlen(sa)>=2 && convertsa(sa)<9) return false;
    if(strlen(sb)>=2 && convertsb(sb)<9) return false;
    if(strlen(sc)>=2 && convertsc(sc)<9) return false;
    if(strlen(sa)>=3 && convertsa(sa)<100) return false;
    if(strlen(sb)>=3 && convertsb(sb)<100) return false;
    if(strlen(sc)>=3 && convertsc(sc)<100) return false;
    for( int i=0; i<la; i++) {
        if (digit(sa[i]) == false) return false;
    }
    for( int i=0; i<lb; i++) {
        if (digit(sb[i]) == false) return false;
    }
    for( int i=0; i<lc; i++) {
        if (digit(sc[i]) == false) return false;
    }
    return true;
}

// Classify a triangle, given side lengths as strings:
int triangle(const char sa[], const char sb[], const char sc[]) {
    if (valid(sa, sb, sc) == true) {
        long a=convertsa(sa);
        long b=convertsb(sb);
        long c=convertsc(sc);
        if (a <= 0|| b <= 0|| c <= 0) return Illegal;
        else if ((a + b) < c || (a+c) < b || (b+c) < a ) return Impossible;
        else if ( (a+b) == c || (a+c) == b || (b+c) == a) return Flat;
        else if (a==b && b==c) return Equilateral;
        else if (a==b||b==c||c==a) return Isosceles;
        else if (a*a == b*b + c*c || b*b == a*a + c*c || c*c == a*a + b*b) return Right;
        else if (a != b || b != c || a != c) return Scalene;
        else return 0;}
    else return Illegal;
 }



// -----------------------------------------------------------------------------
// User interface and testing.

void print(int type) {
    switch (type) {
        case Equilateral: printf("Equilateral"); break;
        case Isosceles: printf("Isosceles"); break;
        case Right: printf("Right"); break;
        case Scalene: printf("Scalene"); break;
        case Flat: printf("Flat"); break;
        case Impossible: printf("Impossible"); break;
        case Illegal: printf("Impossible"); break;
    }
    printf("\n");
}

// Check that you haven't changed the triangle type constants.  (If you do, it
// spoils automatic marking, when your program is linked with a test program.)
void checkConstants() {
    assert(Equilateral==0 && Isosceles==1 && Right==2 && Scalene==3);
    assert(Flat==4 && Impossible==5 && Illegal==6);
}

// Tests 1 to 2: check equilateral
void testEquilateral() {
    assert(triangle("8", "8", "8") == Equilateral);
    assert(triangle("1073", "1073", "1073") == Equilateral);
}

// Tests 3 to 5: check isosceles
void testIsosceles() {
    assert(triangle("25", "25", "27") == Isosceles);
    assert(triangle("25", "27", "25") == Isosceles);
    assert(triangle("27", "25", "25") == Isosceles);
}

// Tests 6 to 14: check right angled
void testRight() {
    assert(triangle("3", "4", "5") == Right);
    assert(triangle("3", "5", "4") == Right);
    assert(triangle("5", "3", "4") == Right);
    assert(triangle("5", "12", "13") == Right);
    assert(triangle("5", "13", "12") == Right);
    assert(triangle("12", "5", "13") == Right);
    assert(triangle("12", "13", "5") == Right);
    assert(triangle("13", "5", "12") == Right);
    assert(triangle("13", "12", "5") == Right);
}

// Tests 15 to 20: check scalene
void testScalene() {
    assert(triangle("12", "14", "15") == Scalene);
    assert(triangle("12", "15", "14") == Scalene);
    assert(triangle("14", "12", "15") == Scalene);
    assert(triangle("14", "15", "12") == Scalene);
    assert(triangle("15", "12", "14") == Scalene);
    assert(triangle("15", "14", "12") == Scalene);
}

// Tests 21 to 25: check flat
void testFlat() {
    assert(triangle("7", "9", "16") == Flat);
    assert(triangle("7", "16", "9") == Flat);
    assert(triangle("9", "16", "7") == Flat);
    assert(triangle("16", "7", "9") == Flat);
    assert(triangle("16", "9", "7") == Flat);
}

// Tests 26 to 31: check impossible
void testImpossible() {
    assert(triangle("2", "13", "3") == Impossible);
    assert(triangle("3", "2", "13") == Impossible);
    assert(triangle("3", "13", "2") == Impossible);
    assert(triangle("13", "2", "3") == Impossible);
    assert(triangle("13", "3", "2") == Impossible);
}

// Tests 32 to 42: check illegal
void testIllegal() {
    assert(triangle("0", "0", "0") == Illegal);
    assert(triangle("0", "10", "12") == Illegal);
    assert(triangle("10", "12", "0") == Illegal);
    assert(triangle("-1", "-1", "-1") == Illegal);
    assert(triangle("-1", "10", "12") == Illegal);
    assert(triangle("10", "-1", "12") == Illegal);
    assert(triangle("10", "12", "-1") == Illegal);
    assert(triangle("x", "y", "z") == Illegal);
    assert(triangle("3", "4y", "5") == Illegal);
    assert(triangle("10", "12", "13.4") == Illegal);
    assert(triangle("03", "4", "5") == Illegal);
}

// Tests 43 to 47: check the upper limits on lengths
void testLimits() {
    assert(triangle("2147483647","2147483647","2147483647") == Equilateral);
    assert(triangle("2147483648","2147483647","2147483647") == Illegal);
    assert(triangle("2147483647","2147483648","2147483647") == Illegal);
    assert(triangle("2147483647","2147483647","2147483648") == Illegal);
    assert(triangle("2147483649","2147483649","2147483649") == Illegal);
}

// Tests 48 to 50: check for correct handling of overflow
void testOverflow() {
    assert(triangle("1100000000","1705032704","1805032704") == Scalene);
    assert(triangle("2000000001","2000000002","2000000003") == Scalene);
    assert(triangle("150000002","666666671","683333338") == Scalene);
}

// Run tests on the triangle function.
void test() {
    checkConstants();
    testEquilateral();
    testIsosceles();
    testRight();
    testScalene();
    testFlat();
    testImpossible();
    testIllegal();
    testLimits();
    testOverflow();
    printf("All tests passed\n");
}

// Run the program or, if there are no arguments, test it.
int main(int n, char **argv) {
    if (n == 1) {
        test();
    }
    else if (n == 4) {
        int result = triangle(argv[1], argv[2], argv[3]);
        print(result);
    }
    else {
        fprintf(stderr, "Use e.g.: ./triangle 3 4 5\n");
        exit(1);
    }
    return 0;
}
