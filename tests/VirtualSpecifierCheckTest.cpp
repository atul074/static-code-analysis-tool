// ================= VALID CASES =================

class Base {
public:
    virtual void foo();        // OK
};

class Derived : public Base {
public:
    void foo() override;       // OK
};

class DerivedFinal : public Base {
public:
    void foo() override final; // OK
};

class ProperFinalBase {
public:
    virtual void foo() final;  // OK
};

// ================= INVALID CASES =================

// Case 1: Missing override
class MissingOverride : public Base {
public:
    void foo();   // EXPECT: missing override
};

// Case 2: override but no base function
class WrongOverride {
public:
    void foo() override;   // EXPECT: invalid override
};

// Case 3: final on non-virtual
class FinalWithoutVirtual {
public:
    void foo() final;   // EXPECT: final on non-virtual
};

// Case 4: multi-level missing override
class Mid : public Base {
public:
    void foo() override;   // OK
};

class Leaf : public Mid {
public:
    void foo();   // EXPECT: missing override
};

// Case 5: final but not virtual
class FinalWrong {
public:
    void foo() final;   // EXPECT: invalid
};