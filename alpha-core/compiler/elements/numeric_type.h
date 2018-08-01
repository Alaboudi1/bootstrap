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

#include "type.h"
#include "program.h"

namespace basecode::compiler {

    struct numeric_type_properties_t {
        std::string name;
        int64_t min;
        uint64_t max;
        size_t size_in_bytes;
    };

    using numeric_type_map_t = std::unordered_map<std::string, numeric_type_properties_t*>;

    class numeric_type : public compiler::type {
    public:
        static std::string narrow_to_value(uint64_t value);

        static void make_types(
            common::result& r,
            compiler::block* parent_scope,
            compiler::program* program);

        numeric_type(
            block* parent_scope,
            compiler::symbol_element* symbol,
            int64_t min,
            uint64_t max);

        int64_t min() const;

        uint64_t max() const;

    protected:
        static inline std::vector<numeric_type_properties_t> s_type_properties = {
            {"bool", 0,         1,           1},
            {"u8",   0,         UINT8_MAX,   1},
            {"u16",  0,         UINT16_MAX,  2},
            {"u32",  0,         UINT32_MAX,  4},
            {"u64",  0,         UINT64_MAX,  8},
            {"s8",   INT8_MIN,  INT8_MAX,    1},
            {"s16",  INT16_MIN, INT16_MAX,   2},
            {"s32",  INT32_MIN, INT32_MAX,   4},
            {"s64",  INT64_MIN, INT64_MAX,   8},
            {"f32",  0,         UINT32_MAX,  4},
            {"f64",  0,         UINT64_MAX,  8},
        };

        static inline numeric_type_map_t s_types_map = {
            {"bool", {&s_type_properties[0]}},
            {"u8",   {&s_type_properties[1]}},
            {"u16",  {&s_type_properties[2]}},
            {"u32",  {&s_type_properties[3]}},
            {"u64",  {&s_type_properties[4]}},
            {"s8",   {&s_type_properties[5]}},
            {"s16",  {&s_type_properties[6]}},
            {"s32",  {&s_type_properties[7]}},
            {"s64",  {&s_type_properties[8]}},
            {"f32",  {&s_type_properties[9]}},
            {"f64",  {&s_type_properties[10]}},
        };

        bool on_initialize(
            common::result& r,
            compiler::program* program) override;

    private:
        int64_t _min;
        uint64_t _max;
    };

};

