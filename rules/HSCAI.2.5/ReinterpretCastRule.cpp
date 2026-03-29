#include "ReinterpretCastRule.h"
#include "../../core/Diagnostic.h"

void ReinterpretCastRule::registerMatchers(MatchFinder &finder) {
    finder.addMatcher(
        cxxReinterpretCastExpr().bind("cast"),
        this
    );
}

void ReinterpretCastRule::run(const MatchFinder::MatchResult &result) {
    const auto *castExpr = result.Nodes.getNodeAs<CXXReinterpretCastExpr>("cast");
    if (!castExpr) return;

    const auto &SM = *result.SourceManager;

    // Avoid reporting inside macros (optional but good practice)
    if (SM.isMacroBodyExpansion(castExpr->getExprLoc())) return;

    MyDiagnostic::report(
        SM,
        castExpr->getExprLoc(),
        name(),
        "reinterpret_cast shall not be used"
    );
}