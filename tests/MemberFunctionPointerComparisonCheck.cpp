class A {
public:
    void foo() {}
    void bar() {}
};

void test() {
    void (A::*ptr)() = &A::foo;

    if (ptr != &A::bar) { } // ❌
}