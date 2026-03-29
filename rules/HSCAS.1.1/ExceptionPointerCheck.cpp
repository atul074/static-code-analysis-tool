#include "ExceptionPointerCheck.h"

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/ExprCXX.h"

using namespace clang;
using namespace clang::ast_matchers;

void ExceptionPointerCheck::registerMatchers(MatchFinder &finder) {

    finder.addMatcher(
        cxxThrowExpr().bind("throwExpr"),
        this
    );
}

void ExceptionPointerCheck::run(const MatchFinder::MatchResult &result) {

    const auto *throwExpr =
        result.Nodes.getNodeAs<CXXThrowExpr>("throwExpr");

    if (!throwExpr)
        return;

    const Expr *subExpr = throwExpr->getSubExpr();
if (!subExpr) return;

subExpr = subExpr->IgnoreImpCasts();

if (subExpr->getType()->isPointerType()) {
    MyDiagnostic::report(
        *result.SourceManager,
        subExpr->getExprLoc(),
        name(),
        "Throwing pointer type is unsafe; throw exception objects by value"
    );
}
}