#pragma once

#include "../../framework/Rule.h"
#include "../../core/Diagnostic.h"

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/ExprCXX.h"

using namespace clang;
using namespace clang::ast_matchers;

class VirtualBaseCastCheck : public Rule {
public:
    std::string name() const override {
        return "HSCAI.2.1";
    }

    void registerMatchers(MatchFinder &finder) override;
    void run(const MatchFinder::MatchResult &result) override;

private:
    bool isVirtualBaseToDerived(const CXXRecordDecl *base,
                               const CXXRecordDecl *derived);
};