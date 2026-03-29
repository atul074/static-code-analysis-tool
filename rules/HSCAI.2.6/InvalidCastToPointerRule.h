#pragma once

#include "../../framework/Rule.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang;
using namespace clang::ast_matchers;

class InvalidCastToPointerRule : public Rule {
public:
    void registerMatchers(MatchFinder &finder) override;
    void run(const MatchFinder::MatchResult &result) override;

    std::string name() const override { return "HSCAI.2.6"; }
};