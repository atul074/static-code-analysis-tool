enum Color { RED, GREEN };

void valid_cases() noexcept {
    int x = 10;
    int *p1 = &x;

    // pointer → pointer (OK)
    void *vp = static_cast<void*>(p1);

    (void)vp;
}

void invalid_cases() noexcept {
    int x = 10;

    // ❌ integral → pointer
    int *p1 = (int*)x;

    // ❌ enum → pointer
    Color c = RED;
    int *p2 = (int*)c;

    // ❌ void* → typed pointer
    void *vp = &x;
    int *p3 = (int*)vp;

    (void)p1;
    (void)p2;
    (void)p3;
}

int main() {
    valid_cases();
    invalid_cases();
    return 0;
}