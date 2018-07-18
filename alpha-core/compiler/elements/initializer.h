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

    class initializer : public element {
    public:
        initializer(
            block* parent_scope,
            element* expr);

        element* expression();

        void expression(element* value);

        compiler::procedure_type* procedure_type();

    protected:
        bool on_emit(
            common::result& r,
            emit_context_t& context) override;

        bool on_as_bool(bool& value) const override;

        bool on_as_float(double& value) const override;

        bool on_as_integer(uint64_t& value) const override;

        bool on_as_string(std::string& value) const override;

    private:
        element* _expr = nullptr;
    };

};

