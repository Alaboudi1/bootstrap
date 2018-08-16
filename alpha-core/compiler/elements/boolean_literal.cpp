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

#include <compiler/session.h>
#include <vm/instruction_block.h>
#include "program.h"
#include "boolean_literal.h"

namespace basecode::compiler {

    boolean_literal::boolean_literal(
            compiler::module* module,
            block* parent_scope,
            bool value) : element(module, parent_scope, element_type_t::boolean_literal),
                          _value(value) {
    }

    bool boolean_literal::value() const {
        return _value;
    }

    bool boolean_literal::on_is_constant() const {
        return true;
    }

    bool boolean_literal::on_as_bool(bool& value) const {
        value = _value;
        return true;
    }

    bool boolean_literal::on_emit(compiler::session& session) {
        auto& assembler = session.assembler();
        auto instruction_block = assembler.current_block();
        auto target_reg = assembler.current_target_register();
        instruction_block->move_constant_to_reg(
            *target_reg,
            static_cast<uint64_t>(_value ? 1 : 0));
        return true;
    }

    compiler::type* boolean_literal::on_infer_type(const compiler::session& session) {
        return session.program().find_type({.name = "bool"});
    }

};