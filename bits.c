#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

// Convert an int into a binary ./string of 32 bits.
void binary(int n, char s[33]) {
  char c;
  s[32] = '\0';
  for(int i=31; i>=0; i--){
    if (n%2 == 0) c = '0';
    else c = '1';
    s[i] = c;
    n = n >> 1;
  }
}

// Convert an int into a hex string of 8 hex digits.
void hex(int n, char s[9]) {
  s[8] = '\0';
  for(int i=7; i>= 0; i--){
   int temp = n & 0xF;
   if (temp < 10)
    s[i] = temp + '0';
   else
    s[i] = temp + 'A' - 10;
  n = n >> 4;
  }
}

// Construct a colour from its components.
int colour(int r, int g, int b, int a) {
    int x = 0;
    x = x | a;
    x = x | (b << 8);
    x = x | (g << 16);
    x = x | (r << 24);
    return x;
}

// Unpack a colour into its components.
void components(int c, int rgba[4]) {
  rgba[0] = ((c&0xFF000000)>>24);
  rgba[1] = ((c&0x00FF0000)>>16);
  rgba[2] = ((c&0x0000FF00)>>8);
  rgba[3] = (c&0x000000FF);
}

// Form a 3D point from three signed 10-bit integers.
int point(int x, int y, int z) {
  return ((x & 0x3FF) << 20) | ((y & 0x3FF) << 10) | (z & 0x3FF);
}

// Separate a position into three signed 10-bit coordinates.
void coordinates(int p, int xyz[3]) {
  xyz[0] = (p >> 20) & 0x3FF;
  xyz[1] = (p >> 10) & 0x3FF;
  xyz[2] = p & 0x3FF;
  for (int i=0; i<3; i++) {
    if ((xyz[i] & 0x200) != 0) xyz[i] = 0xFFFFFC00 | xyz[i];}
}

// ----------------------------------------------------------------------------
// Testing and user interface.

// Do a single test of the binary function with given input and output.
// Initialise s with incorrect values to give predictable results.
bool checkBinary(int in, char out[33]) {
    char s[33];
    for (int i=0; i<33; i++) s[i] = 'x';
    binary(in, s);
    return strcmp(s, out) == 0;
}

// Tests 1 to 13
void testBinary() {
    assert(checkBinary(0, "00000000000000000000000000000000"));
    assert(checkBinary(1, "00000000000000000000000000000001"));
    assert(checkBinary(2, "00000000000000000000000000000010"));
    assert(checkBinary(3, "00000000000000000000000000000011"));
    assert(checkBinary(4, "00000000000000000000000000000100"));
    assert(checkBinary(100, "00000000000000000000000001100100"));
    assert(checkBinary(2147483647, "01111111111111111111111111111111"));
    assert(checkBinary(-1, "11111111111111111111111111111111"));
    assert(checkBinary(-2, "11111111111111111111111111111110"));
    assert(checkBinary(-3, "11111111111111111111111111111101"));
    assert(checkBinary(-4, "11111111111111111111111111111100"));
    assert(checkBinary(-100, "11111111111111111111111110011100"));
    assert(checkBinary(-2147483648, "10000000000000000000000000000000"));
}

// Do a single test of the hex function with given input and output.
bool checkHex(int in, char out[9]) {
    char s[9];
    for (int i=0; i<9; i++) s[i] = 'x';
    hex(in, s);
    return strcmp(s, out) == 0;
}

// Tests 14 to 28
void testHex() {
    assert(checkHex(0, "00000000"));
    assert(checkHex(1, "00000001"));
    assert(checkHex(2, "00000002"));
    assert(checkHex(10, "0000000A"));
    assert(checkHex(15, "0000000F"));
    assert(checkHex(16, "00000010"));
    assert(checkHex(100, "00000064"));
    assert(checkHex(2147483647, "7FFFFFFF"));
    assert(checkHex(-1, "FFFFFFFF"));
    assert(checkHex(-2, "FFFFFFFE"));
    assert(checkHex(-3, "FFFFFFFD"));
    assert(checkHex(-4, "FFFFFFFC"));
    assert(checkHex(-16, "FFFFFFF0"));
    assert(checkHex(-100, "FFFFFF9C"));
    assert(checkHex(-2147483648, "80000000"));
}

// Tests 29 to 33
void testColour() {
    assert(colour(255,0,0,0) == 0xFF000000);
    assert(colour(0,255,0,0) == 0x00FF0000);
    assert(colour(0,0,255,0) == 0x0000FF00);
    assert(colour(0,0,0,255) == 0x000000FF);
    assert(colour(1,2,3,4) == 0x01020304);
}

// Do a single test of the components function with given input and output.
bool checkComponents(int n, int r, int g, int b, int a) {
    int rgba[4];
    for (int i=0; i<4; i++) rgba[i] = -1;
    components(n, rgba);
    return rgba[0] == r && rgba[1] == g && rgba[2] == b && rgba[3] == a;
}

// Tests 34 to 38
void testComponents() {
    assert(checkComponents(0xFF000000, 255, 0, 0, 0));
    assert(checkComponents(0x00FF0000, 0, 255, 0, 0));
    assert(checkComponents(0x0000FF00, 0, 0, 255, 0));
    assert(checkComponents(0x000000FF, 0, 0, 0, 255));
    assert(checkComponents(0x01020304, 1, 2, 3, 4));
}

// Tests 39 to 44
void testPoint() {
    assert(point(0,0,0) == 0);
    assert(point(1,3,7) == 0x00100C07);
    assert(point(1,3,-7) == 0x00100FF9);
    assert(point(1,-3,7) == 0x001FF407);
    assert(point(-1,3,7) == 0x3FF00C07);
    assert(point(-1,-3,-7) == 0x3FFFF7F9);
}

// Do a single test of the coordinates function with given input and output.
bool checkCoordinates(int p, int x, int y, int z) {
    int xyz[3];
    for (int i=0; i<3; i++) xyz[i] = 1000000000;
    coordinates(p, xyz);
    return xyz[0] == x && xyz[1] == y && xyz[2] == z;
}

// Tests 45 to 50
void testCoordinates() {
    assert(checkCoordinates(0, 0, 0, 0));
    assert(checkCoordinates(0x00100C07, 1, 3, 7));
    assert(checkCoordinates(0x00100FF9, 1, 3, -7));
    assert(checkCoordinates(0x001FF407, 1, -3, 7));
    assert(checkCoordinates(0x3FF00C07, -1, 3, 7));
    assert(checkCoordinates(0x3FFFF7F9, -1, -3, -7));
}

void test() {
    testBinary();
    testHex();
    testColour();
    testComponents();
    testPoint();
    testCoordinates();
    printf("All tests pass");
}

// Print a number in binary.
void printBinary(int n) {
    char s[33];
    binary(n, s);
    printf("%s\n", s);
}

// Print a number in hex.
void printHex(int n) {
    char s[9];
    hex(n, s);
    printf("%s\n", s);
}

// Print a number in hex or binary, or run tests.
int main(int n, char *args[n]) {
    if (n == 1) test();
    else if (n == 2) printHex(atoi(args[1]));
    else if (n == 3 && strcmp(args[1],"-b")==0) printBinary(atoi(args[2]));
    else printf("Use:   ./bits   or   ./bits n   or   ./bits -b n");
    return 0;
}
