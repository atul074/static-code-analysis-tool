class Base {
public:
    virtual void foo(int x = 10) {}
};

class Derived1 : public Base {
public:
    void foo(int x = 20) {}  // ❌ different default
};

class Derived2 : public Base {
public:
    void foo(int x = 10) {}  // ✅ OK
};

class Derived3 : public Base {
public:
    void foo(int x) {}  // ❌ missing default
};

class Derived4 : public Base {
public:
    void foo(int x = 10 + 0) {}  // ⚠️ text differs (will be flagged)
};