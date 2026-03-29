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

    // Ignore constructors/destructors
    if (isa<CXXConstructorDecl>(method) || isa<CXXDestructorDecl>(method))
        return;

    // -----------------------------
    // Case 1: Method overrides base class function
    // -----------------------------
    if (method->size_overridden_methods() > 0) {

        // Should use override keyword
        if (!method->hasAttr<OverrideAttr>()) {
            MyDiagnostic::report(
                *result.SourceManager,
                method->getLocation(),
                name(),
                "Overriding method should use 'override' specifier"
            );
        }
    }

    // -----------------------------
    // Case 2: Method marked override but not actually overriding
    // -----------------------------
    if (method->hasAttr<OverrideAttr>() &&
        method->size_overridden_methods() == 0) {

        MyDiagnostic::report(
            *result.SourceManager,
            method->getLocation(),
            name(),
            "'override' used but method does not override any base class method"
        );
    }

    // -----------------------------
    // Case 3: Virtual function without virtual keyword (base class)
    // -----------------------------
    if (method->isVirtual() &&
        method->size_overridden_methods() == 0) {

        // First declaration should explicitly use virtual
        if (!method->isVirtualAsWritten()) {
            MyDiagnostic::report(
                *result.SourceManager,
                method->getLocation(),
                name(),
                "Base class virtual method should explicitly use 'virtual'"
            );
        }
    }

    // -----------------------------
    // Case 4: final used without virtual/override context
    // -----------------------------
    if (method->hasAttr<FinalAttr>()) {

        if (!method->isVirtual()) {
            MyDiagnostic::report(
                *result.SourceManager,
                method->getLocation(),
                name(),
                "'final' specifier used on non-virtual method"
            );
        }
    }
}