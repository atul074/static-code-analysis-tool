#include "Analyzer.h"

#include "../rules/HSCAA.1.2/ReturnValueUsedCheck.h"
#include "../rules/HSCAA.2.1/UnusedVariableCheck.h"
#include "../rules/HSCAA.2.2/UnusedParameterCheck.h"
#include "../rules/HSCAA.2.3/UnusedTypeCheck.h"
#include "../rules/HSCAA.2.4/UnusedFunctionCheck.h"
#include "../rules/HSCAG.4.2/NameHidingCheck.h"
#include "../rules/HSCAG.8.3/DanglingPointerAssignmentCheck.h"
#include "../rules/HSCAG.8.4/RefQualifierCheck.h"
#include "../rules/HSCAP.1.1/DynamicTypeUsageCheck.h"
#include "../rules/HSCAP.1.2/BaseInitializationCheck.h" 
#include "../rules/HSCAP.1.3/ExplicitConstructorCheck.h"
#include "../rules/HSCAN.3.1/VirtualSpecifierCheck.h"
#include "../rules/HSCAN.3.2/VirtualFunctionDefaultArgsCheck.h"
#include "../rules/HSCAS.1.1/ExceptionPointerCheck.h"
//#include "../rules/HSCAS.4.1/ExceptionNoexceptCheck.h"
#include "../rules/HSCAI.2.1/VirtualBaseCastCheck.h"
#include "../rules/HSCAI.2.2/CStyleAndFunctionalCastCheck.h"
#include "../rules/HSCAI.2.3/ConstCastAwayQualifierRule.h"
#include "../rules/HSCAI.2.5/ReinterpretCastRule.h"
#include "../rules/HSCAI.2.6/InvalidCastToPointerRule.h"
#include "../rules/HSCAI.18.1/OverlappingCopyRule.h"
#include "../rules/HSCAJ.4.1/IfElseMustEndWithElseRule.h"
#include "../rules/HSCAV.6.2/NoRawNewDeleteRule.h"
#include "../rules/HSCAV.6.3/NoAdvancedMemoryRule.h"
#include "../rules/HSCBC.6.3/UseAfterMoveRule.h"
#include "../rules/HSCAP.1.4/MemberInitializationCheck.h"
#include "../rules/HSCAP.0.2/SelfAssignmentCheck.h"
#include "../rules/HSCAN.1.2/VirtualInheritanceMismatchCheck.h"
#include "../rules/HSCAN.3.4/MemberFunctionPointerComparisonCheck.h"
#include "../rules/HSCAR.8.1/FunctionTemplateSpecializationCheck.h"
#include "../rules/HSCAS.4.1/NoexceptCheck.h"
Analyzer::Analyzer(Context& ctx) : context(ctx) {}

void Analyzer::registerRules() {

    auto shouldEnable = [&](const std::string& ruleName) {

    // 1. Disabled rules → highest priority
    if (context.disabledRules.count(ruleName))
        return false;

    // 2. Explicitly enabled
    if (context.enabledRules.count(ruleName))
        return true;

    // 3. Enable all (*) mode
    if (context.enableAll)
        return true;

    // 4. Default → do NOT run
    return false;
};

    auto add = [&](auto rule) {
        if (shouldEnable(rule->name())) {
            rules.push_back(std::move(rule));
        }
    };

    add(std::make_unique<ReturnValueUsedCheck>());
    add(std::make_unique<UnusedVariableCheck>());
    add(std::make_unique<UnusedParameterCheck>());
    add(std::make_unique<UnusedTypeCheck>());
    add(std::make_unique<UnusedFunctionCheck>());
    add(std::make_unique<NameHidingCheck>());
    add(std::make_unique<DanglingPointerAssignmentCheck>());
    add(std::make_unique<RefQualifierCheck>());
    add(std::make_unique<DynamicTypeUsageCheck>());
    add(std::make_unique<BaseInitializationCheck>());
    add(std::make_unique<ExplicitConstructorCheck>());
    add(std::make_unique<VirtualSpecifierCheck>());
    add(std::make_unique<VirtualFunctionDefaultArgsCheck>());
    add(std::make_unique<ExceptionPointerCheck>());
    //add(std::make_unique<ExceptionNoexceptCheck>());
    add(std::make_unique<VirtualBaseCastCheck>());
    add(std::make_unique<CStyleAndFunctionalCastCheck>());
    add(std::make_unique<ConstCastAwayQualifierRule>());
    add(std::make_unique<ReinterpretCastRule>());
    add(std::make_unique<InvalidCastToPointerRule>());
    add(std::make_unique<OverlappingCopyRule>());
    add(std::make_unique<IfElseMustEndWithElseRule>());
    add(std::make_unique<NoRawNewDeleteRule>());
    add(std::make_unique<NoAdvancedMemoryRule>());
    add(std::make_unique<UseAfterMoveRule>());
    add(std::make_unique<MemberInitializationCheck>());
    add(std::make_unique<SelfAssignmentCheck>());
    add(std::make_unique<VirtualInheritanceMismatchCheck>());
    add(std::make_unique<MemberFunctionPointerComparisonCheck>());
    add(std::make_unique<FunctionTemplateSpecializationCheck>());
    add(std::make_unique<NoexceptCheck>());
}

void Analyzer::setupMatchers() {
    for (auto &rule : rules) {
        rule->registerMatchers(finder);
    }
}

clang::ast_matchers::MatchFinder& Analyzer::getFinder() {
    return finder;
}