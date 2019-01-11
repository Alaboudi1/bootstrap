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

#include "compiler_types.h"

namespace basecode::compiler {

    using intern_string_map_t = std::unordered_map<std::string, common::id_t>;

    class string_intern_map {
    public:
        string_intern_map() = default;

        void reset();

        bool id(
            compiler::string_literal* literal,
            common::id_t& intern_id);

        intern_string_map_t::const_iterator end() const;

        intern_string_map_t::const_iterator begin() const;

        std::string base_label_for_id(common::id_t id) const;

        std::string data_label_for_id(common::id_t id) const;

        common::id_t intern(compiler::string_literal* literal);

        std::string base_label(compiler::string_literal* literal) const;

        std::string data_label(compiler::string_literal* literal) const;

    private:
        intern_string_map_t _interned_strings {};
        std::unordered_map<common::id_t, common::id_t> _element_to_intern_ids {};
    };

};

