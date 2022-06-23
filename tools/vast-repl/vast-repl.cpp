// Copyright (c) 2022-present, Trail of Bits, Inc.

#include "vast/Util/Warnings.hpp"

VAST_RELAX_WARNINGS
#include "mlir/IR/Dialect.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/InitAllDialects.h"
#include "mlir/InitAllPasses.h"
#include "mlir/Parser.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Pass/PassManager.h"
#include "mlir/Support/FileUtilities.h"
#include "mlir/Support/MlirOptMain.h"
#include "vast/repl/linenoise.hpp"
VAST_UNRELAX_WARNINGS

#include "vast/Dialect/Dialects.hpp"
#include "vast/Dialect/HighLevel/Passes.hpp"
#include "vast/Util/Common.hpp"
#include "vast/repl/cli.hpp"
#include "vast/repl/command.hpp"

using logical_result = mlir::LogicalResult;

using args_t = std::vector< vast::repl::string_ref >;

args_t load_args(int argc, char **argv) {
    args_t args;

    for (int i = 1; i < argc; i++) {
        args.push_back(argv[i]);
    }

    return args;
}

namespace vast::repl
{
    struct prompt {
        explicit prompt(MContext &ctx)
            : ctx(ctx) {}

        logical_result init(std::span< string_ref > args) {
            if (args.size() == 1) {
                auto params = parse_params< command::load::command_params >(args);
                return cli.exec(make_command< command::load >(params));
            } else {
                throw std::runtime_error("unsupported arguments");
            }
        }

        logical_result run() {
            const auto path = ".vast-repl.history";

            linenoise::SetHistoryMaxLen(1000);
            linenoise::LoadHistory(path);

            llvm::outs() << "Welcome to 'vast-repl', an interactive MLIR modifier. Type 'help' to "
                            "get started.\n";

            while (!cli.exit()) {
                std::string cmd;
                if (auto quit = linenoise::Readline("> ", cmd)) {
                    break;
                }

                if (failed(cli.exec(cmd))) {
                    return mlir::failure();
                }

                linenoise::AddHistory(cmd.c_str());
                linenoise::SaveHistory(path);
            }

            return mlir::success();
        }


        cli_t cli;
        MContext &ctx;
    };

} // namespace vast::repl

int main(int argc, char **argv) {
    mlir::DialectRegistry registry;
    vast::registerAllDialects(registry);
    mlir::registerAllDialects(registry);

    args_t args = load_args(argc, argv);

    vast::MContext ctx(registry);
    ctx.loadAllAvailableDialects();

    auto prompt = vast::repl::prompt(ctx);

    if (!args.empty()) {
        if (auto res = failed(prompt.init(args))) {
            std::exit(res);
        }
    }

    std::exit(failed(prompt.run()));
}