#pragma once

#include "../../framework/Rule.h"
#include "../../core/Diagnostic.h"

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/ExprCXX.h"

using namespace clang;
using namespace clang::ast_matchers;

class CStyleAndFunctionalCastCheck : public Rule {
public:
    std::string name() const override {
        return "HSCAI.2.2";
    }

    void registerMatchers(MatchFinder &finder) override;
    void run(const MatchFinder::MatchResult &result) override;
};