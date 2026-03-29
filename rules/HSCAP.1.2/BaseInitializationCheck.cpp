#include "BaseInitializationCheck.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/DeclCXX.h"

using namespace clang;
using namespace clang::ast_matchers;

void BaseInitializationCheck::registerMatchers(MatchFinder &finder) {

    finder.addMatcher(
        cxxConstructorDecl(
            isDefinition()
        ).bind("ctor"),
        this
    );
}

void BaseInitializationCheck::run(const MatchFinder::MatchResult &result) {

    const auto *ctor =
        result.Nodes.getNodeAs<CXXConstructorDecl>("ctor");

    if (!ctor)
        return;

    const auto *cls = ctor->getParent();
    if (!cls)
        return;

    // Skip if no base classes
    if (!cls->getNumBases())
        return;

    // Collect initialized bases
    std::set<const CXXRecordDecl*> initializedBases;

    for (const auto *init : ctor->inits()) {
        if (init->isBaseInitializer()) {
            const auto *baseType = init->getBaseClass();

            if (const auto *baseDecl = baseType->getAsCXXRecordDecl()) {
                initializedBases.insert(baseDecl);
            }
        }
    }

    // Check each base class
    for (const auto &base : cls->bases()) {

        const auto *baseDecl =
            base.getType()->getAsCXXRecordDecl();

        if (!baseDecl)
            continue;

        // If not initialized → violation
        if (initializedBases.find(baseDecl) == initializedBases.end()) {

            MyDiagnostic::report(
                *result.SourceManager,
                ctor->getLocation(),
                name(),
                "Constructor does not explicitly initialize base class '" +
                baseDecl->getNameAsString() + "'"
            );
        }
    }
}