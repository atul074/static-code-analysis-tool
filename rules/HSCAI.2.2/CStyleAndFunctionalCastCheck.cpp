#include "CStyleAndFunctionalCastCheck.h"

void CStyleAndFunctionalCastCheck::registerMatchers(MatchFinder &finder) {

    // C-style cast: (Type)expr
    finder.addMatcher(
        cStyleCastExpr().bind("cStyleCast"),
        this
    );

    // Functional cast: Type(expr)
    finder.addMatcher(
        cxxFunctionalCastExpr().bind("functionalCast"),
        this
    );
}

void CStyleAndFunctionalCastCheck::run(const MatchFinder::MatchResult &result) {

    SourceLocation loc;

    if (const auto *e =
        result.Nodes.getNodeAs<CStyleCastExpr>("cStyleCast")) {

        loc = e->getExprLoc();

        MyDiagnostic::report(
            *result.SourceManager,
            loc,
            name(),
            "C-style cast is not allowed; use static_cast, dynamic_cast, or reinterpret_cast"
        );
    }

    if (const auto *e =
        result.Nodes.getNodeAs<CXXFunctionalCastExpr>("functionalCast")) {

        loc = e->getExprLoc();

        MyDiagnostic::report(
            *result.SourceManager,
            loc,
            name(),
            "Functional-style cast is not allowed; use C++ named casts instead"
        );
    }
}