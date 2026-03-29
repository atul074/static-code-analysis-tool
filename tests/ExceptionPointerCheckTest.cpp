
void test() {

    int x = 10;
    int* ptr = &x;

    throw x;      // ✅ OK
    throw ptr;    // ❌
    throw new int(5); // ❌
    throw &x;     // ❌
}