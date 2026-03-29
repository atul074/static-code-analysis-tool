#include "VirtualBaseCastCheck.h"

void VirtualBaseCastCheck::registerMatchers(MatchFinder &finder) {

    finder.addMatcher(cxxStaticCastExpr().bind("staticCast"), this);
    finder.addMatcher(cStyleCastExpr().bind("cStyleCast"), this);
    finder.addMatcher(cxxReinterpretCastExpr().bind("reinterpretCast"), this);
}

bool VirtualBaseCastCheck::isVirtualBaseToDerived(
    const CXXRecordDecl *base,
    const CXXRecordDecl *derived) {

    if (!base || !derived) return false;

    for (const auto &baseSpec : derived->bases()) {
        const Type *type = baseSpec.getType().getTypePtrOrNull();
        if (!type) continue;

        const CXXRecordDecl *parent = type->getAsCXXRecordDecl();
        if (!parent) continue;

        // Direct virtual inheritance
        if (baseSpec.isVirtual() && parent == base)
            return true;

        // Recursive check
        if (isVirtualBaseToDerived(base, parent))
            return true;
    }

    return false;
}

void VirtualBaseCastCheck::run(const MatchFinder::MatchResult &result) {

    const Expr *subExpr = nullptr;
    QualType toType;
    SourceLocation loc;

    if (const auto *e = result.Nodes.getNodeAs<CXXStaticCastExpr>("staticCast")) {
        subExpr = e->getSubExpr();
        toType = e->getType();
        loc = e->getExprLoc();
    }
    else if (const auto *e = result.Nodes.getNodeAs<CStyleCastExpr>("cStyleCast")) {
        subExpr = e->getSubExpr();
        toType = e->getType();
        loc = e->getExprLoc();
    }
    else if (const auto *e = result.Nodes.getNodeAs<CXXReinterpretCastExpr>("reinterpretCast")) {
        subExpr = e->getSubExpr();
        toType = e->getType();
        loc = e->getExprLoc();
    }
    else {
        return;
    }

    if (!subExpr)
        return;

    QualType fromType = subExpr->getType();

    // Normalize types
    fromType = fromType.getCanonicalType();
    toType   = toType.getCanonicalType();

    // Only pointer casts
    if (!fromType->isPointerType() || !toType->isPointerType())
        return;

    const CXXRecordDecl *fromDecl =
        fromType->getPointeeType()->getAsCXXRecordDecl();

    const CXXRecordDecl *toDecl =
        toType->getPointeeType()->getAsCXXRecordDecl();

    if (!fromDecl || !toDecl)
        return;

    if (isVirtualBaseToDerived(fromDecl, toDecl)) {
        MyDiagnostic::report(
            *result.SourceManager,
            loc,
            name(),
            "Casting from virtual base to derived using unsafe cast; use dynamic_cast instead"
        );
    }
}