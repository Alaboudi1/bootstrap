// ----------------------------------------------------------------------------
//
// Basecode Bootstrap Compiler
// Copyright (C) 2018 Jeff Panici
// All rights reserved.
//
// This software source file is licensed under the terms of MIT license.
// For details, please read the LICENSE file.
//
// ----------------------------------------------------------------------------

#include <fstream>
#include <fmt/ostream.h>
#include <parser/lexer.h>
#include <parser/ast_formatter.h>
#include <compiler/elements/program.h>
#include "bytecode_emitter.h"
#include "code_dom_formatter.h"

namespace basecode::compiler {

    bytecode_emitter::bytecode_emitter(
        const bytecode_emitter_options_t& options): _terp(options.heap_size, options.stack_size),
                                                    _options(options) {
    }

    bytecode_emitter::~bytecode_emitter() {
    }

    bool bytecode_emitter::compile(
            common::result& r,
            vm::assembly_listing& listing,
        std::istream& input) {
        compile_stream(r, listing, input);
        return !r.is_failed();
    }

    bool bytecode_emitter::compile_files(
            common::result& r,
            vm::assembly_listing& listing,
            const std::vector<std::filesystem::path>& source_files) {
        for (const auto& source_file : source_files) {
            listing.add_source_file(source_file.string());

            fmt::print("compile: {} ... ", source_file.string());
            if (!std::filesystem::exists(source_file)) {
                fmt::print("FAILED, file does not exist.\n");
                continue;
            }

            std::ifstream input_stream(source_file);
            if (input_stream.is_open()) {
                if (!compile(r, listing, input_stream))
                    fmt::print("FAILED.\n");
                else
                    fmt::print("PASSED.\n");
                input_stream.close();
            }
        }

        return !r.is_failed();
    }

    bool bytecode_emitter::compile_stream(
            common::result& r,
            vm::assembly_listing& listing,
            std::istream& input) {
        syntax::parser alpha_parser(input);
        auto module_node = alpha_parser.parse(r);
        if (module_node != nullptr && !r.is_failed()) {
            if (_options.verbose) {
                alpha_parser.write_ast_graph(
                    _options.ast_graph_file_name,
                    module_node);
            }

            compiler::program program_element(&_terp);
            if (program_element.compile(r, listing, module_node)) {
                if (_options.verbose) {
                    write_code_dom_graph(
                        _options.code_dom_graph_file_name,
                        &program_element);
                }
            }
        }

        return !r.is_failed();
    }

    void bytecode_emitter::write_code_dom_graph(
            const std::filesystem::path& path,
            const compiler::program* program) {
        auto close_required = false;
        FILE* code_dom_output_file = stdout;
        if (!path.empty()) {
            code_dom_output_file = fopen(
                path.c_str(),
                "wt");
            close_required = true;
        }

        code_dom_formatter formatter(
            program,
            code_dom_output_file);
        formatter.format(fmt::format("Code DOM Graph: {}", path.string()));

        if (close_required)
            fclose(code_dom_output_file);
    }

    bool bytecode_emitter::initialize(common::result& r) {
        return _terp.initialize(r);
    }

}