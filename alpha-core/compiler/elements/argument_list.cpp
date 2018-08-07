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

#include <vm/instruction_block.h>
#include "type.h"
#include "program.h"
#include "identifier.h"
#include "argument_list.h"

namespace basecode::compiler {

    argument_list::argument_list(block* parent_scope) : element(
                                                            parent_scope,
                                                            element_type_t::argument_list) {
    }

    bool argument_list::on_emit(
            common::result& r,
            emit_context_t& context) {
        auto instruction_block = context.assembler->current_block();
        for (auto it = _elements.rbegin(); it != _elements.rend(); ++it) {
            element* arg = *it;
            switch (arg->element_type()) {
                case element_type_t::proc_call:
                case element_type_t::expression:
                case element_type_t::float_literal:
                case element_type_t::string_literal:
                case element_type_t::unary_operator:
                case element_type_t::binary_operator:
                case element_type_t::boolean_literal:
                case element_type_t::integer_literal:
                case element_type_t::identifier_reference: {
                    auto push_size = vm::op_sizes::qword;
                    auto arg_reg = register_for(r, context, arg);
                    if (arg_reg.var != nullptr) {
                        push_size = vm::op_size_for_byte_size(
                            arg_reg.var->type
                                       ->size_in_bytes());
                        arg_reg.clean_up = true;
                    }
                    context.assembler->push_target_register(
                        arg_reg.size(),
                        arg_reg.reg.i);
                    arg->emit(r, context);
                    context.assembler->pop_target_register();
                    instruction_block->push(push_size, arg_reg.reg.i);
                    break;
                }
                default:
                    break;
            }
        }
        return true;
    }

    void argument_list::add(element* item) {
        _elements.emplace_back(item);
    }

    void argument_list::remove(common::id_t id) {
        auto item = find(id);
        if (item == nullptr)
            return;
        std::remove(
            _elements.begin(),
            _elements.end(),
            item);
    }

    element* argument_list::find(common::id_t id) {
        auto it = std::find_if(
            _elements.begin(),
            _elements.end(),
            [&id](auto item) { return item->id() == id; });
        if (it == _elements.end())
            return nullptr;
        return *it;
    }

    const element_list_t& argument_list::elements() const {
        return _elements;
    }

    void argument_list::on_owned_elements(element_list_t& list) {
        for (auto element : _elements)
            list.emplace_back(element);
    }

};