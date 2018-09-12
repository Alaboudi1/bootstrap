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

    class type_reference : public element {
    public:
        type_reference(
            compiler::module* module,
            block* parent_scope,
            const qualified_symbol_t& symbol,
            compiler::type* type);

        bool resolved() const;

        compiler::type* type();

        bool is_unknown_type() const;

        void type(compiler::type* value);

        const qualified_symbol_t& symbol() const;

    protected:
        bool on_infer_type(
            const compiler::session& session,
            infer_type_result_t& result) override;

        bool on_is_constant() const override;

    private:
        qualified_symbol_t _symbol;
        compiler::type* _type = nullptr;
    };

};
