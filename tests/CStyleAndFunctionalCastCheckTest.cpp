struct Base {
    virtual ~Base() = default;
};

struct Derived : Base {};

void bad() {
    int x = 10;

    float f1 = (float)x;          // ❌ C-style
    int y = int(3.14);            // ❌ functional cast

    Derived d;
    Base* b = (Base*)&d;          // ❌ C-style pointer cast
}

void good() {
    int x = 10;

    float f = static_cast<float>(x);  // ✅ OK

    Derived d;
    Base* b = static_cast<Base*>(&d); // ✅ OK
}

int main() {
    bad();
    good();
}