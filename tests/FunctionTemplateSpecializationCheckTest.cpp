template<typename T>
void foo(T x) {}

template<>
void foo<int>(int x) { } // ❌