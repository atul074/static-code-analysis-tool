void valid_cases() noexcept {
    int x = 10;
    int *p1 = &x;

    // Allowed casts
    void *vp = static_cast<void*>(p1);
    int *p2 = static_cast<int*>(vp);

    (void)p2;
}

void invalid_cases() noexcept {
    int x = 10;
    int *p1 = &x;

    // ❌ reinterpret_cast usage
    long addr = reinterpret_cast<long>(p1);

    void *vp = reinterpret_cast<void*>(p1);

    int *p2 = reinterpret_cast<int*>(vp);

    (void)addr;
    (void)p2;
}

int main() {
    valid_cases();
    invalid_cases();
    return 0;
}