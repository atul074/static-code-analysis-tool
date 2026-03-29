struct A {
    int x;
};

template<typename T>
T&& move(T& t) { return (T&&)t; }

void bad() {
    A a;
    A b = move(a);   // ❌ move
    int y = a.x;     // ❌ use after move
}

void good() {
    A a;
    A b = move(a);
    A c;
    c = b;           // ✅ ok
}

void also_good() {
    A a;
    int x = a.x;     // ✅ used before move
    A b = move(a);
}