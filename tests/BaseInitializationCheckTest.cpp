class A { public: A(int) {} };
class B { public: B(int) {} };

class C : public A, public B {
public:
    C() : A(1) {} // ❌ B not initialized
};