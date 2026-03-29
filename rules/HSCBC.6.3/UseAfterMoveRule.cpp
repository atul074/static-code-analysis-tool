#include "UseAfterMoveRule.h"
#include "../../core/Diagnostic.h"

#include <unordered_map>

using namespace clang;
using namespace clang::ast_matchers;

std::string UseAfterMoveRule::name() const {
    return "HSCBC.6.3";
}

// store move location per variable
static std::unordered_map<const VarDecl*, SourceLocation> movedVars;

void UseAfterMoveRule::registerMatchers(MatchFinder &finder) {

    // match move(a)
    finder.addMatcher(
        callExpr(
            callee(functionDecl(matchesName("::(std::)?move"))),
            hasArgument(0, declRefExpr(to(varDecl().bind("movedVar"))))
        ).bind("moveCall"),
        this
    );

    // match usage of variable
    finder.addMatcher(
        declRefExpr(
            to(varDecl().bind("usedVar"))
        ).bind("use"),
        this
    );

    // reset per function
    finder.addMatcher(
        functionDecl(isDefinition()).bind("func"),
        this
    );
}

void UseAfterMoveRule::run(const MatchFinder::MatchResult &result) {

    if (result.Nodes.getNodeAs<FunctionDecl>("func")) {
        movedVars.clear();
        return;
    }

    if (const auto *moveCall = result.Nodes.getNodeAs<CallExpr>("moveCall")) {
        if (const auto *var = result.Nodes.getNodeAs<VarDecl>("movedVar")) {

            if (var->isImplicit()) return;

            movedVars[var] = moveCall->getExprLoc();
        }
        return;
    }

    if (const auto *use = result.Nodes.getNodeAs<DeclRefExpr>("use")) {
        if (const auto *var = result.Nodes.getNodeAs<VarDecl>("usedVar")) {

            if (var->isImplicit()) return;

            auto it = movedVars.find(var);
            if (it == movedVars.end()) return;

            if (result.SourceManager->isBeforeInTranslationUnit(
                    it->second,
                    use->getExprLoc())) {

                MyDiagnostic::report(
                    *result.SourceManager,
                    use->getExprLoc(),
                    name(),
                    "object used after being moved (potentially moved-from state)"
                );
            }
        }
    }
}