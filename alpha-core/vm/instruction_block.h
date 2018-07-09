// ----------------------------------------------------------------------------
//
// Basecode Bootstrap Compiler
// Copyright (C) 2018 Jeff Panici
// All rights reserved.
//
// This software source file is licensed under the terms of MIT license.
// For details, please read the LICENSE.md file.
//
// ----------------------------------------------------------------------------

#pragma once

#include <map>
#include <stack>
#include <string>
#include <vector>
#include "terp.h"
#include "label.h"

namespace basecode::vm {

    enum class target_register_type_t {
        integer,
        floating_point
    };

    struct target_register_t {
        target_register_type_t type;
        union {
            i_registers_t i;
            f_registers_t f;
        } reg;
    };

    enum class instruction_block_type_t {
        implicit,
        procedure
    };

    struct instruction_comments_t {
        std::vector<std::string> lines {};
    };

    class instruction_block {
    public:
        instruction_block(
            instruction_block* parent,
            instruction_block_type_t type);

        virtual ~instruction_block();

        void rts();

        void dup();

        void nop();

        void exit();

        void disassemble();

        void clear_labels();

        void clear_blocks();

        void pop_target_register();

        void comment(const std::string& value);

        target_register_t* current_target_register();

        void push_target_register(i_registers_t reg);

        void push_target_register(f_registers_t reg);

        // not variations
        void not_u8(
            i_registers_t dest_reg,
            i_registers_t src_reg);

        void not_u16(
            i_registers_t dest_reg,
            i_registers_t src_reg);

        void not_u32(
            i_registers_t dest_reg,
            i_registers_t src_reg);

        void not_u64(
            i_registers_t dest_reg,
            i_registers_t src_reg);

        // neg variations
        void neg_u8(
            i_registers_t dest_reg,
            i_registers_t src_reg);

        void neg_u16(
            i_registers_t dest_reg,
            i_registers_t src_reg);

        void neg_u32(
            i_registers_t dest_reg,
            i_registers_t src_reg);

        void neg_u64(
            i_registers_t dest_reg,
            i_registers_t src_reg);

        // load variations
        void load_to_ireg_u8(
            i_registers_t dest_reg,
            i_registers_t address_reg,
            int64_t offset = 0);

        void load_to_ireg_u16(
            i_registers_t dest_reg,
            i_registers_t address_reg,
            int64_t offset = 0);

        void load_to_ireg_u32(
            i_registers_t dest_reg,
            i_registers_t address_reg,
            int64_t offset = 0);

        void load_to_ireg_u64(
            i_registers_t dest_reg,
            i_registers_t address_reg,
            int64_t offset = 0);

        // store variations
        void store_from_ireg_u8(
            i_registers_t src_reg,
            i_registers_t address_reg,
            int64_t offset = 0);

        void store_from_ireg_u16(
            i_registers_t src_reg,
            i_registers_t address_reg,
            int64_t offset = 0);

        void store_from_ireg_u32(
            i_registers_t src_reg,
            i_registers_t address_reg,
            int64_t offset = 0);

        void store_from_ireg_u64(
            i_registers_t src_reg,
            i_registers_t address_reg,
            int64_t offset = 0);

        // move variations
        void move_f32_to_freg(
            f_registers_t dest_reg,
            float immediate);

        void move_f64_to_freg(
            f_registers_t dest_reg,
            double immediate);

        void move_u8_to_ireg(
            i_registers_t dest_reg,
            uint8_t immediate);

        void move_u16_to_ireg(
            i_registers_t dest_reg,
            uint16_t immediate);

        void move_u32_to_ireg(
            i_registers_t dest_reg,
            uint32_t immediate);

        void move_u64_to_ireg(
            i_registers_t dest_reg,
            uint64_t immediate);

        void move_ireg_to_ireg(
            i_registers_t dest_reg,
            i_registers_t src_reg);

        void move_label_to_ireg(
            i_registers_t dest_reg,
            const std::string& label_name);

        instruction_block* parent();

        // branches
        void beq(const std::string& label_name);

        // cmp variations
        void cmp_u64(
            i_registers_t lhs_reg,
            i_registers_t rhs_reg);

        // inc variations
        void inc_u8(i_registers_t reg);

        void inc_u16(i_registers_t reg);

        void inc_u32(i_registers_t reg);

        void inc_u64(i_registers_t reg);

        // dec variations
        void dec_u8(i_registers_t reg);

        void dec_u16(i_registers_t reg);

        void dec_u32(i_registers_t reg);

        void dec_u64(i_registers_t reg);

        // mul variations
        void mul_ireg_by_ireg_u8(
            i_registers_t dest_reg,
            i_registers_t multiplicand_reg,
            i_registers_t multiplier_reg);

        void mul_ireg_by_ireg_u16(
            i_registers_t dest_reg,
            i_registers_t multiplicand_reg,
            i_registers_t multiplier_reg);

        void mul_ireg_by_ireg_u32(
            i_registers_t dest_reg,
            i_registers_t multiplicand_reg,
            i_registers_t multiplier_reg);

        void mul_ireg_by_ireg_u64(
            i_registers_t dest_reg,
            i_registers_t multiplicand_reg,
            i_registers_t multiplier_reg);

        // add variations
        void add_ireg_by_ireg_u8(
            i_registers_t dest_reg,
            i_registers_t augend_reg,
            i_registers_t addened_reg);

        void add_ireg_by_ireg_u16(
            i_registers_t dest_reg,
            i_registers_t augend_reg,
            i_registers_t addened_reg);

        void add_ireg_by_ireg_u32(
            i_registers_t dest_reg,
            i_registers_t augend_reg,
            i_registers_t addened_reg);

        void add_ireg_by_ireg_u64(
            i_registers_t dest_reg,
            i_registers_t augend_reg,
            i_registers_t addened_reg);

        // sub variations
        void sub_ireg_by_ireg_u8(
            i_registers_t dest_reg,
            i_registers_t minuend_reg,
            i_registers_t subtrahend_reg);

        void sub_ireg_by_ireg_u16(
            i_registers_t dest_reg,
            i_registers_t minuend_reg,
            i_registers_t subtrahend_reg);

        void sub_ireg_by_ireg_u32(
            i_registers_t dest_reg,
            i_registers_t minuend_reg,
            i_registers_t subtrahend_reg);

        void sub_ireg_by_ireg_u64(
            i_registers_t dest_reg,
            i_registers_t minuend_reg,
            i_registers_t subtrahend_reg);

        // div variations
        void div_ireg_by_ireg_u8(
            i_registers_t dest_reg,
            i_registers_t dividend_reg,
            i_registers_t divisor_reg);

        void div_ireg_by_ireg_u16(
            i_registers_t dest_reg,
            i_registers_t dividend_reg,
            i_registers_t divisor_reg);

        void div_ireg_by_ireg_u32(
            i_registers_t dest_reg,
            i_registers_t dividend_reg,
            i_registers_t divisor_reg);

        void div_ireg_by_ireg_u64(
            i_registers_t dest_reg,
            i_registers_t dividend_reg,
            i_registers_t divisor_reg);

        // mod variations
        void mod_ireg_by_ireg_u8(
            i_registers_t dest_reg,
            i_registers_t dividend_reg,
            i_registers_t divisor_reg);

        void mod_ireg_by_ireg_u16(
            i_registers_t dest_reg,
            i_registers_t dividend_reg,
            i_registers_t divisor_reg);

        void mod_ireg_by_ireg_u32(
            i_registers_t dest_reg,
            i_registers_t dividend_reg,
            i_registers_t divisor_reg);

        void mod_ireg_by_ireg_u64(
            i_registers_t dest_reg,
            i_registers_t dividend_reg,
            i_registers_t divisor_reg);

        // swap variations
        void swap_ireg_with_ireg_u8(
            i_registers_t dest_reg,
            i_registers_t src_reg);

        void swap_ireg_with_ireg_u16(
            i_registers_t dest_reg,
            i_registers_t src_reg);

        void swap_ireg_with_ireg_u32(
            i_registers_t dest_reg,
            i_registers_t src_reg);

        void swap_ireg_with_ireg_u64(
            i_registers_t dest_reg,
            i_registers_t src_reg);

        // test mask for zero and branch
        void test_mask_branch_if_zero_u8(
            i_registers_t value_reg,
            i_registers_t mask_reg,
            i_registers_t address_reg);

        void test_mask_branch_if_zero_u16(
            i_registers_t value_reg,
            i_registers_t mask_reg,
            i_registers_t address_reg);

        void test_mask_branch_if_zero_u32(
            i_registers_t value_reg,
            i_registers_t mask_reg,
            i_registers_t address_reg);

        void test_mask_branch_if_zero_u64(
            i_registers_t value_reg,
            i_registers_t mask_reg,
            i_registers_t address_reg);

        // test mask for non-zero and branch
        void test_mask_branch_if_not_zero_u8(
            i_registers_t value_reg,
            i_registers_t mask_reg,
            i_registers_t address_reg);

        void test_mask_branch_if_not_zero_u16(
            i_registers_t value_reg,
            i_registers_t mask_reg,
            i_registers_t address_reg);

        void test_mask_branch_if_not_zero_u32(
            i_registers_t value_reg,
            i_registers_t mask_reg,
            i_registers_t address_reg);

        void test_mask_branch_if_not_zero_u64(
            i_registers_t value_reg,
            i_registers_t mask_reg,
            i_registers_t address_reg);

        // interrupts and traps
        void swi(uint8_t index);

        void trap(uint8_t index);

        void clear_instructions();

        // push variations
        void push_f32(float value);

        void push_f64(double value);

        void push_u8(uint8_t value);

        void push_u16(uint16_t value);

        void push_u32(uint32_t value);

        void push_u64(uint64_t value);

        void push_u8(i_registers_t reg);

        void push_u16(i_registers_t reg);

        void push_f32(f_registers_t reg);

        void push_u32(i_registers_t reg);

        void push_f64(f_registers_t reg);

        void push_u64(i_registers_t reg);

        // register allocators
        i_registers_t allocate_ireg();

        f_registers_t allocate_freg();

        // pop variations
        void pop_u8(i_registers_t reg);

        void pop_f32(f_registers_t reg);

        void pop_f64(f_registers_t reg);

        void pop_u16(i_registers_t reg);

        void pop_u32(i_registers_t reg);

        void pop_u64(i_registers_t reg);

        // register allocators
        void free_ireg(i_registers_t reg);

        void free_freg(f_registers_t reg);

        bool reserve_ireg(i_registers_t reg);

        bool reserve_freg(f_registers_t reg);

        void jump_indirect(i_registers_t reg);

        instruction_block_type_t type() const;

        void call(const std::string& proc_name);

        void add_block(instruction_block* block);

        void remove_block(instruction_block* block);

        void call_foreign(const std::string& proc_name);

        vm::label* make_label(const std::string& name);

        void jump_direct(const std::string& label_name);

    private:
        void make_cmp_instruction(
            op_sizes size,
            i_registers_t lhs_reg,
            i_registers_t rhs_reg);

        void make_not_instruction(
            op_sizes size,
            i_registers_t dest_reg,
            i_registers_t src_reg);

        void make_neg_instruction(
            op_sizes size,
            i_registers_t dest_reg,
            i_registers_t src_reg);

        void make_inc_instruction(
            op_sizes size,
            i_registers_t reg);

        void make_dec_instruction(
            op_sizes size,
            i_registers_t reg);

        void make_load_instruction(
            op_sizes size,
            i_registers_t dest_reg,
            i_registers_t address_reg,
            int64_t offset);

        void make_store_instruction(
            op_sizes size,
            i_registers_t src_reg,
            i_registers_t address_reg,
            int64_t offset);

        void make_swap_instruction(
            op_sizes size,
            i_registers_t dest_reg,
            i_registers_t src_reg);

        void make_pop_instruction(
            op_sizes size,
            i_registers_t dest_reg);

        void make_pop_instruction(
            op_sizes size,
            f_registers_t dest_reg);

        void make_move_instruction(
            op_sizes size,
            i_registers_t dest_reg,
            uint64_t value);

        void make_move_instruction(
            op_sizes size,
            f_registers_t dest_reg,
            double value);

        void make_move_instruction(
            op_sizes size,
            i_registers_t dest_reg,
            i_registers_t src_reg);

        void make_add_instruction(
            op_sizes size,
            i_registers_t dest_reg,
            i_registers_t augend_reg,
            i_registers_t addend_reg);

        void make_sub_instruction(
            op_sizes size,
            i_registers_t dest_reg,
            i_registers_t minuend_reg,
            i_registers_t subtrahend_reg);

        void disassemble(instruction_block* block);

        void make_float_constant_push_instruction(
            op_sizes size,
            double value);

        void make_integer_constant_push_instruction(
            op_sizes size,
            uint64_t value);

        label_ref_t* find_unresolved_label_up(common::id_t id);

        vm::label* find_label_up(const std::string& label_name);

        void make_push_instruction(op_sizes size, i_registers_t reg);

        void make_push_instruction(op_sizes size, f_registers_t reg);

        label_ref_t* make_unresolved_label_ref(const std::string& label_name);

    private:
        instruction_block_type_t _type;
        instruction_block* _parent = nullptr;
        std::vector<instruction_block*> _blocks {};
        std::vector<instruction_t> _instructions {};
        std::map<std::string, vm::label*> _labels {};
        std::set<f_registers_t> _used_float_registers {};
        std::stack<target_register_t> _target_registers {};
        std::set<i_registers_t> _used_integer_registers {};
        std::map<size_t, instruction_comments_t> _comments {};
        std::map<std::string, size_t> _label_to_instruction_map {};
        std::unordered_map<common::id_t, label_ref_t> _unresolved_labels {};
        std::unordered_map<std::string, common::id_t> _label_to_unresolved_ids {};
    };

};
