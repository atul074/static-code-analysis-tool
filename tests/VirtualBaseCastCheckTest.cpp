struct Base {
    virtual ~Base() = default;
};

struct Mid : virtual Base {};
struct Derived : Mid {};

void bad(Base *b) {
    // Unsafe but compiles
    Derived *d = reinterpret_cast<Derived*>(b); // ❌ should warn
}

void good(Base *b) {
    Derived *d = dynamic_cast<Derived*>(b); // ✅ OK
}

int main() {
    Base b;
    bad(&b);
}