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

#include <compiler/compiler_types.h>
#include "element.h"

namespace basecode::compiler {

    class identifier : public element {
    public:
        identifier(
            compiler::module* module,
            block* parent_scope,
            compiler::symbol_element* name,
            compiler::initializer* initializer);

        compiler::field* field();

        bool inferred_type() const;

        bool is_initialized() const;

        void inferred_type(bool value);

        void field(compiler::field* value);

        compiler::type_reference* type_ref();

        compiler::initializer* initializer();

        std::string label_name() const override;

        compiler::symbol_element* symbol() const;

        void type_ref(compiler::type_reference* t);

        void initializer(compiler::initializer* value);

    protected:
        bool on_fold(
            compiler::session& session,
            fold_result_t& result) override;

        bool on_infer_type(
            compiler::session& session,
            infer_type_result_t& result) override;

        compiler::element* on_clone(
            compiler::session& session,
            compiler::block* new_scope) override;

        bool on_is_constant() const override;

        bool on_as_bool(bool& value) const override;

        bool on_as_float(double& value) const override;

        bool on_as_string(std::string& value) const override;

        void on_owned_elements(element_list_t& list) override;

        bool on_as_rune(common::rune_t& value) const override;

        bool on_as_integer(integer_result_t& result) const override;

        bool on_as_identifier(compiler::identifier*& value) const override;

    private:
        bool _inferred_type = false;
        compiler::field* _field = nullptr;
        compiler::symbol_element* _symbol;
        compiler::initializer* _initializer;
        compiler::type_reference* _type_ref = nullptr;
    };

}