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
#include "label.h"
#include "break_element.h"

namespace basecode::compiler {

    break_element::break_element(
            compiler::module* module,
            compiler::block* parent_scope,
            compiler::label* label) : element(module, parent_scope, element_type_t::break_e),
                                      _label(label) {
    }

    bool break_element::on_emit(
            compiler::session& session,
            compiler::emit_context_t& context,
            compiler::emit_result_t& result) {
        auto& assembler = session.assembler();
        auto block = assembler.current_block();

        vm::label_ref_t* label_ref = nullptr;

        std::string label_name;
        if (_label != nullptr) {
            label_name = _label->name();
            label_ref = assembler.make_label_ref(_label->name());
        } else {
            auto control_flow = assembler.current_control_flow();
            if (control_flow == nullptr
                ||  control_flow->exit_label == nullptr) {
                session.error(
                    this,
                    "P081",
                    "no valid exit label on stack.",
                    location());
                return false;
            }
            label_ref = control_flow->exit_label;
            label_name = label_ref->name;
        }

        block->comment(fmt::format("break to label: {}", label_name), 4);
        block->jump_direct(label_ref);

        return true;
    }

    compiler::label* break_element::label() {
        return _label;
    }

    void break_element::on_owned_elements(element_list_t& list) {
        if (_label != nullptr)
            list.emplace_back(_label);
    }

};