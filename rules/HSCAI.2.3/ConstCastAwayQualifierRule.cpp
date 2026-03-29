#include "ConstCastAwayQualifierRule.h"
#include "../../core/Diagnostic.h"

void ConstCastAwayQualifierRule::registerMatchers(MatchFinder &finder) {
    finder.addMatcher(
    explicitCastExpr(
        hasSourceExpression(expr().bind("srcExpr"))
    ).bind("cast"),
    this
);
}

void ConstCastAwayQualifierRule::run(const MatchFinder::MatchResult &result) {
    const auto *castExpr = result.Nodes.getNodeAs<ExplicitCastExpr>("cast");
    if (!castExpr) return;

    QualType srcType = castExpr->getSubExpr()->getType();
    QualType destType = castExpr->getType();

    // Only check pointer or reference types
    if (!(srcType->isPointerType() || srcType->isReferenceType())) return;
    if (!(destType->isPointerType() || destType->isReferenceType())) return;

    QualType srcPointee = srcType->getPointeeType();
    QualType destPointee = destType->getPointeeType();

    if (srcType->isReferenceType()) {
        srcPointee = srcType.getNonReferenceType();
    }
    if (destType->isReferenceType()) {
        destPointee = destType.getNonReferenceType();
    }

    if (srcPointee.isNull() || destPointee.isNull()) return;

    bool srcConst = srcPointee.isConstQualified();
    bool destConst = destPointee.isConstQualified();

    bool srcVolatile = srcPointee.isVolatileQualified();
    bool destVolatile = destPointee.isVolatileQualified();

    // Detect removal of const or volatile
    if ((srcConst && !destConst) || (srcVolatile && !destVolatile)) {
        auto &SM = *result.SourceManager;

        MyDiagnostic::report(
            SM,
            castExpr->getExprLoc(),
            "HSCAI.2.3",
            "Casting removes const/volatile qualifier from pointee type"
        );
    }
}