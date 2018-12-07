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

#pragma once

#include "element.h"

namespace basecode::compiler {

    class assignment  : public element {
    public:
        assignment(
            compiler::module* module,
            compiler::block* parent_scope);

        element_list_t& expressions();

    protected:
        bool on_emit(
            compiler::session& session,
            compiler::emit_context_t& context,
            compiler::emit_result_t& result) override;

        void on_owned_elements(element_list_t& list) override;

    private:
        element_list_t _expressions {};
    };

};

