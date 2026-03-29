#pragma once

#include "../../framework/Rule.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

class UseAfterMoveRule : public Rule {
public:
    std::string name() const override;

    void registerMatchers(clang::ast_matchers::MatchFinder &finder) override;
    void run(const clang::ast_matchers::MatchFinder::MatchResult &result) override;
};