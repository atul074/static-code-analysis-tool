const int x = 10;
void test() {
    int *p = (int*)&x; // should trigger
}