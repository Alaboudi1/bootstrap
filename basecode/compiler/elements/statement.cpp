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

#include "label.h"
#include "statement.h"

namespace basecode::compiler {

    statement::statement(
            compiler::module* module,
            block* parent_scope,
            element* expr) : element(module, parent_scope, element_type_t::statement),
                             _expression(expr) {
    }

    element* statement::expression() {
        return _expression;
    }

    label_list_t& statement::labels() {
        return _labels;
    }

    bool statement::on_emit(compiler::session& session) {
        for (auto label : _labels)
            label->emit(session);

        if (_expression == nullptr)
            return true;

        return _expression->emit(session);
    }

    void statement::on_owned_elements(element_list_t& list) {
        if (_expression != nullptr)
            list.emplace_back(_expression);

        for (auto element : _labels)
            list.emplace_back(element);
    }

};