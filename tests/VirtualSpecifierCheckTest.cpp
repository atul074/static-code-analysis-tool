class Base {
public:
    virtual void foo();   // ✅ OK
    void bar();           // ❌ should be virtual if intended polymorphic
};

class Derived : public Base {
public:
    void foo();           // ❌ missing override
};

class Derived2 : public Base {
public:
    void foo() override;  // ✅ OK
};

class WrongOverride {
public:
    void foo() override;  // ❌ no base method
};

class FinalTest {
public:
    void foo() final;     // ❌ not virtual
};  