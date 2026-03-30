#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

#include "core/Analyzer.h"
#include "core/Context.h"
#include "core/ConfigLoader.h"

using namespace clang::tooling;

// Command-line category
static llvm::cl::OptionCategory MyToolCategory("jargon-master options");

// Optional: custom checks file path
static llvm::cl::opt<std::string> ChecksFile(
    "checks",
    llvm::cl::desc("Path to .jargon-checks file"),
    llvm::cl::init(".jargon-checks"),
    llvm::cl::cat(MyToolCategory)
);

int main(int argc, const char **argv) {

    // -----------------------------
    // Parse command-line arguments
    // -----------------------------
    auto ExpectedParser =
        CommonOptionsParser::create(argc, argv, MyToolCategory);

    if (!ExpectedParser) {
        llvm::errs() << "Error creating parser\n";
        return 1;
    }

    CommonOptionsParser &OptionsParser = ExpectedParser.get();

    // -----------------------------
    // Create Clang tool
    // -----------------------------
    ClangTool Tool(
        OptionsParser.getCompilations(),
        OptionsParser.getSourcePathList()
    );

    // -----------------------------
    // Load rule configuration
    // -----------------------------
    Context ctx = loadChecks(ChecksFile);

    // DEBUG (optional)
    /*
    llvm::errs() << "EnableAll: " << ctx.enableAll << "\n";

    for (auto &r : ctx.enabledRules)
        llvm::errs() << "Enabled: " << r << "\n";

    for (auto &r : ctx.disabledRules)
        llvm::errs() << "Disabled: " << r << "\n";
    */

    // -----------------------------
    // Setup analyzer
    // -----------------------------
    Analyzer analyzer(ctx);
    analyzer.registerRules();
    analyzer.setupMatchers();

    // -----------------------------
    // Run tool
    // -----------------------------
    return Tool.run(
        newFrontendActionFactory(&analyzer.getFinder()).get()
    );
}