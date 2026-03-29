#include "FunctionTemplateSpecializationCheck.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/Decl.h"

using namespace clang;
using namespace clang::ast_matchers;

void FunctionTemplateSpecializationCheck::registerMatchers(MatchFinder &finder) {

    finder.addMatcher(
        functionDecl().bind("func"),
        this
    );
}

void FunctionTemplateSpecializationCheck::run(const MatchFinder::MatchResult &result) {

    const auto *func =
        result.Nodes.getNodeAs<FunctionDecl>("func");

    if (!func)
        return;

    // Check if explicit specialization
    if (func->getTemplateSpecializationKind() ==
        TSK_ExplicitSpecialization) {

        MyDiagnostic::report(
            *result.SourceManager,
            func->getLocation(),
            name(),
            "Function template explicit specialization is not allowed"
        );
    }
}