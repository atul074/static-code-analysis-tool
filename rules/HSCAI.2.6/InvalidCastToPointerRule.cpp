#include "InvalidCastToPointerRule.h"
#include "../../core/Diagnostic.h"

void InvalidCastToPointerRule::registerMatchers(MatchFinder &finder) {
    finder.addMatcher(
        explicitCastExpr().bind("cast"),
        this
    );
}

void InvalidCastToPointerRule::run(const MatchFinder::MatchResult &result) {
    const auto *castExpr = result.Nodes.getNodeAs<ExplicitCastExpr>("cast");
    if (!castExpr) return;

    QualType srcType = castExpr->getSubExpr()->getType();
    QualType destType = castExpr->getType();

    // Destination must be a pointer
    if (!destType->isPointerType()) return;

    // Ignore casts inside macros
    const auto &SM = *result.SourceManager;
    if (SM.isMacroBodyExpansion(castExpr->getExprLoc())) return;

    // Remove references
    if (srcType->isReferenceType()) {
        srcType = srcType.getNonReferenceType();
    }

    // Case 1: integral type
    if (srcType->isIntegralType(*result.Context)) {
        MyDiagnostic::report(
            SM,
            castExpr->getExprLoc(),
            name(),
            "Casting integral type to pointer type is not allowed"
        );
        return;
    }

    // Case 2: enum type
    if (srcType->isEnumeralType()) {
        MyDiagnostic::report(
            SM,
            castExpr->getExprLoc(),
            name(),
            "Casting enum type to pointer type is not allowed"
        );
        return;
    }

    // Case 3: void* → typed pointer (allowed in C, but banned here)
    if (srcType->isPointerType()) {
        QualType pointee = srcType->getPointeeType();
        if (pointee->isVoidType()) {
            MyDiagnostic::report(
                SM,
                castExpr->getExprLoc(),
                name(),
                "Casting void* to object pointer type is not allowed"
            );
            return;
        }
    }
}