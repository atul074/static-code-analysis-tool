#include "ConfigLoader.h"
#include "Context.h"
#include <fstream>

Context loadChecks(const std::string& path) {

    Context ctx;
    std::ifstream file(path);

    if (!file.is_open()) return ctx;

    std::string line;

    while (std::getline(file, line)) {

        if (line.empty() || line[0] == '#') continue;

        if (line == "*") {
            ctx.enableAll = true;
        }
        else if (line[0] == '-') {
            ctx.disabledRules.insert(line.substr(1));
        }
        else {
            ctx.enabledRules.insert(line);
        }
    }

    return ctx;
}