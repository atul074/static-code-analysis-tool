#include "VirtualFunctionDefaultArgsCheck.h"

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/DeclCXX.h"
#include "clang/Lex/Lexer.h"
#include "llvm/ADT/SmallPtrSet.h"

using namespace clang;
using namespace clang::ast_matchers;

// ---------- Utility: Normalize expression text ----------
static std::string normalize(std::string s) {
    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
    return s;
}

// ---------- Utility: Extract source text ----------
static std::string getExprText(const Expr *E,
                               const SourceManager &SM,
                               const LangOptions &LO) {
    if (!E) return "";

    return Lexer::getSourceText(
        CharSourceRange::getTokenRange(E->getSourceRange()),
        SM,
        LO
    ).str();
}

// ---------- Matcher ----------
void VirtualFunctionDefaultArgsCheck::registerMatchers(MatchFinder &finder) {

    finder.addMatcher(
        cxxMethodDecl(
            isUserProvided(),
            unless(isImplicit()),
            unless(isDeleted())
        ).bind("method"),
        this
    );
}

// ---------- Core Logic ----------
void VirtualFunctionDefaultArgsCheck::run(
    const MatchFinder::MatchResult &result) {
    const auto *method =
        result.Nodes.getNodeAs<CXXMethodDecl>("method");

    if (!method)
        return;

    if (method->isStatic() ||
        isa<CXXConstructorDecl>(method) ||
        isa<CXXDestructorDecl>(method))
        return;

    auto &SM = *result.SourceManager;
    auto &LO = result.Context->getLangOpts();

    llvm::SmallPtrSet<const ParmVarDecl*, 8> Reported;

    // ================================
    // ✅ CASE 1: Proper override (with keyword)
    // ================================
    if (method->size_overridden_methods() > 0) {

        for (const auto *baseMethod : method->overridden_methods()) {

            if (method->getNumParams() != baseMethod->getNumParams())
                continue;

            for (unsigned i = 0; i < method->getNumParams(); ++i) {

                const auto *d = method->getParamDecl(i);
                const auto *b = baseMethod->getParamDecl(i);

                if (Reported.count(d))
                    continue;

                bool dDef = d->hasDefaultArg();
                bool bDef = b->hasDefaultArg();

                if (dDef != bDef) {
                    MyDiagnostic::report(
                        SM, d->getLocation(), name(),
                        "Default argument mismatch with base method"
                    );
                    Reported.insert(d);
                    continue;
                }

                if (dDef && bDef) {

                    std::string ds =
                        Lexer::getSourceText(
                            CharSourceRange::getTokenRange(
                                d->getDefaultArg()->getSourceRange()),
                            SM, LO).str();

                    std::string bs =
                        Lexer::getSourceText(
                            CharSourceRange::getTokenRange(
                                b->getDefaultArg()->getSourceRange()),
                            SM, LO).str();

                    if (ds != bs) {
                        MyDiagnostic::report(
                            SM,
                            d->getDefaultArg()->getExprLoc(),
                            name(),
                            "Default argument value differs from base method"
                        );
                        Reported.insert(d);
                    }
                }
            }
        }

        return;
    }

    // ================================
    // ✅ CASE 2: Missing override keyword (manual detection)
    // ================================
    const auto *parent = method->getParent();
    if (!parent)
        return;

    for (const auto &base : parent->bases()) {

        const auto *baseDecl =
            base.getType()->getAsCXXRecordDecl();

        if (!baseDecl)
            continue;

        for (const auto *baseMethod : baseDecl->methods()) {

            if (method->getName() != baseMethod->getName())
                continue;

            if (method->getNumParams() != baseMethod->getNumParams())
                continue;

            for (unsigned i = 0; i < method->getNumParams(); ++i) {

                const auto *d = method->getParamDecl(i);
                const auto *b = baseMethod->getParamDecl(i);

                if (Reported.count(d))
                    continue;

                bool dDef = d->hasDefaultArg();
                bool bDef = b->hasDefaultArg();

                if (dDef != bDef) {
                    MyDiagnostic::report(
                        SM, d->getLocation(), name(),
                        "Default argument mismatch with base method"
                    );
                    Reported.insert(d);
                    continue;
                }

                if (dDef && bDef) {

                    std::string ds =
                        Lexer::getSourceText(
                            CharSourceRange::getTokenRange(
                                d->getDefaultArg()->getSourceRange()),
                            SM, LO).str();

                    std::string bs =
                        Lexer::getSourceText(
                            CharSourceRange::getTokenRange(
                                b->getDefaultArg()->getSourceRange()),
                            SM, LO).str();

                    if (ds != bs) {
                        MyDiagnostic::report(
                            SM,
                            d->getDefaultArg()->getExprLoc(),
                            name(),
                            "Default argument value differs from base method"
                        );
                        Reported.insert(d);
                    }
                }
            }
        }
    }
}