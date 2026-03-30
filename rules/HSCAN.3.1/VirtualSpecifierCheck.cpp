#include "VirtualSpecifierCheck.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/DeclCXX.h"

using namespace clang;
using namespace clang::ast_matchers;

void VirtualSpecifierCheck::registerMatchers(MatchFinder &finder) {

    finder.addMatcher(
        cxxMethodDecl(
            isUserProvided(),
            unless(isImplicit())
        ).bind("method"),
        this
    );
}

void VirtualSpecifierCheck::run(const MatchFinder::MatchResult &result) {

    const auto *method =
        result.Nodes.getNodeAs<CXXMethodDecl>("method");

    if (!method)
        return;

    const auto *SM = result.SourceManager;

    // Ignore system headers
    if (SM->isInSystemHeader(method->getLocation()))
        return;

    // Ignore constructors/destructors
    if (isa<CXXConstructorDecl>(method) || isa<CXXDestructorDecl>(method))
        return;

    // -----------------------------
    // Case 1: Missing override
    // -----------------------------
    if (method->size_overridden_methods() > 0) {

        if (!method->hasAttr<OverrideAttr>()) {
            MyDiagnostic::report(
                *SM,
                method->getLocation(),
                name(),
                "Overriding method should use 'override' specifier"
            );
        }
    }

    // -----------------------------
    // Case 2: override but no base method
    // -----------------------------
    if (method->hasAttr<OverrideAttr>() &&
        method->size_overridden_methods() == 0) {

        MyDiagnostic::report(
            *SM,
            method->getLocation(),
            name(),
            "'override' used but method does not override any base class method"
        );
    }

    // -----------------------------
    // Case 3: Base virtual function missing 'virtual'
    // -----------------------------
    if (method->isVirtual() &&
        method->size_overridden_methods() == 0 &&
        method->getParent()->getNumBases() == 0) {

        if (!method->isVirtualAsWritten()) {
            MyDiagnostic::report(
                *SM,
                method->getLocation(),
                name(),
                "Base class virtual method should explicitly use 'virtual'"
            );
        }
    }

    // -----------------------------
    // Case 4: final on non-virtual
    // -----------------------------
    if (method->hasAttr<FinalAttr>() && !method->isVirtual()) {

        MyDiagnostic::report(
            *SM,
            method->getLocation(),
            name(),
            "'final' specifier used on non-virtual method"
        );
    }
}