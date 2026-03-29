#include "MemberFunctionPointerComparisonCheck.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/Expr.h"

using namespace clang;
using namespace clang::ast_matchers;

void MemberFunctionPointerComparisonCheck::registerMatchers(MatchFinder &finder) {

    finder.addMatcher(
        binaryOperator(
            anyOf(
                hasOperatorName("=="),
                hasOperatorName("!=")
            )
        ).bind("cmp"),
        this
    );
}

void MemberFunctionPointerComparisonCheck::run(const MatchFinder::MatchResult &result) {

    const auto *binOp =
        result.Nodes.getNodeAs<BinaryOperator>("cmp");

    if (!binOp)
        return;

    const Expr *lhs = binOp->getLHS()->IgnoreParenImpCasts();
    const Expr *rhs = binOp->getRHS()->IgnoreParenImpCasts();

    QualType lhsType = lhs->getType();
    QualType rhsType = rhs->getType();

    bool lhsIsMemberFuncPtr = lhsType->isMemberFunctionPointerType();
    bool rhsIsMemberFuncPtr = rhsType->isMemberFunctionPointerType();

    if (!lhsIsMemberFuncPtr && !rhsIsMemberFuncPtr)
        return;

    // Check if compared with nullptr
    bool lhsIsNull = isa<CXXNullPtrLiteralExpr>(lhs);
    bool rhsIsNull = isa<CXXNullPtrLiteralExpr>(rhs);

    if (lhsIsNull || rhsIsNull)
        return;

    // 🚨 Violation
    MyDiagnostic::report(
        *result.SourceManager,
        binOp->getOperatorLoc(),
        name(),
        "Pointer-to-member function should only be compared with nullptr"
    );
}