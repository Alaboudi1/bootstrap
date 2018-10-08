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
#include "elements.h"
#include "ast_evaluator.h"
#include "element_builder.h"

namespace basecode::compiler {

    std::unordered_map<syntax::ast_node_types_t, node_evaluator_callable> ast_evaluator::s_node_evaluators = {
        {syntax::ast_node_types_t::pair,                    std::bind(&ast_evaluator::noop, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::label,                   std::bind(&ast_evaluator::noop, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::module,                  std::bind(&ast_evaluator::module, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::symbol,                  std::bind(&ast_evaluator::symbol, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::type_list,               std::bind(&ast_evaluator::noop, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::proc_call,               std::bind(&ast_evaluator::proc_call, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::statement,               std::bind(&ast_evaluator::statement, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::attribute,               std::bind(&ast_evaluator::attribute, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::directive,               std::bind(&ast_evaluator::directive, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::raw_block,               std::bind(&ast_evaluator::raw_block, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::assignment,              std::bind(&ast_evaluator::assignment, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::expression,              std::bind(&ast_evaluator::expression, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::nil_literal,             std::bind(&ast_evaluator::nil, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::basic_block,             std::bind(&ast_evaluator::basic_block, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::symbol_part,             std::bind(&ast_evaluator::noop, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::line_comment,            std::bind(&ast_evaluator::line_comment, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::block_comment,           std::bind(&ast_evaluator::block_comment, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::argument_list,           std::bind(&ast_evaluator::argument_list, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::if_expression,           std::bind(&ast_evaluator::if_expression, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::parameter_list,          std::bind(&ast_evaluator::noop, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::number_literal,          std::bind(&ast_evaluator::number_literal, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::string_literal,          std::bind(&ast_evaluator::string_literal, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::unary_operator,          std::bind(&ast_evaluator::unary_operator, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::spread_operator,         std::bind(&ast_evaluator::spread_operator, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::cast_expression,         std::bind(&ast_evaluator::cast_expression, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::from_expression,         std::bind(&ast_evaluator::noop, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::proc_expression,         std::bind(&ast_evaluator::proc_expression, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::enum_expression,         std::bind(&ast_evaluator::enum_expression, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::binary_operator,         std::bind(&ast_evaluator::binary_operator, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::boolean_literal,         std::bind(&ast_evaluator::boolean_literal, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::else_expression,         std::bind(&ast_evaluator::else_expression, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::break_statement,         std::bind(&ast_evaluator::break_expression, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::with_expression,         std::bind(&ast_evaluator::with_expression, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::type_identifier,         std::bind(&ast_evaluator::type_identifier, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::while_statement,         std::bind(&ast_evaluator::while_expression, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::defer_expression,        std::bind(&ast_evaluator::defer_expression, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::union_expression,        std::bind(&ast_evaluator::union_expression, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::return_statement,        std::bind(&ast_evaluator::return_statement, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::symbol_reference,        std::bind(&ast_evaluator::noop, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::for_in_statement,        std::bind(&ast_evaluator::for_in_statement, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::tuple_expression,        std::bind(&ast_evaluator::tuple_expression, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::switch_expression,       std::bind(&ast_evaluator::noop, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::import_expression,       std::bind(&ast_evaluator::import_expression, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::struct_expression,       std::bind(&ast_evaluator::struct_expression, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::character_literal,       std::bind(&ast_evaluator::character_literal, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::array_constructor,       std::bind(&ast_evaluator::array_constructor, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::module_expression,       std::bind(&ast_evaluator::module_expression, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::elseif_expression,       std::bind(&ast_evaluator::if_expression, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::subscript_operator,      std::bind(&ast_evaluator::subscript_operator, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::continue_statement,      std::bind(&ast_evaluator::continue_expression, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::constant_assignment,     std::bind(&ast_evaluator::assignment, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::transmute_expression,    std::bind(&ast_evaluator::transmute_expression, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::namespace_expression,    std::bind(&ast_evaluator::namespace_expression, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::return_argument_list,    std::bind(&ast_evaluator::noop, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::array_subscript_list,    std::bind(&ast_evaluator::noop, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::assignment_source_list,  std::bind(&ast_evaluator::noop, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {syntax::ast_node_types_t::assignment_target_list,  std::bind(&ast_evaluator::noop, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
    };

    ///////////////////////////////////////////////////////////////////////////

    void evaluator_context_t::apply_comments(compiler::element* element) const {
        for (auto comment : comments) {
            comment->parent_element(element);
            element->comments().emplace_back(comment);
        }
    }

    void evaluator_context_t::apply_attributes(compiler::element* element) const {
        for (auto attribute : attributes.as_list()) {
            attribute->parent_element(element);
            element->attributes().add(attribute);
        }
    }

    ///////////////////////////////////////////////////////////////////////////

    ast_evaluator::ast_evaluator(compiler::session& session) : _session(session) {
    }

    element* ast_evaluator::evaluate(const syntax::ast_node_t* node) {
        if (node == nullptr)
            return nullptr;

        auto& builder = _session.builder();
        auto& scope_manager = _session.scope_manager();

        evaluator_context_t context;
        context.node = node;
        context.scope = scope_manager.current_scope();

        for (const auto& attribute : node->attributes) {
            context.attributes.add(builder.make_attribute(
                scope_manager.current_scope(),
                attribute->token.value,
                evaluate(attribute->lhs.get())));
        }

        for (const auto& comment : node->comments) {
            switch (comment->type) {
                case syntax::ast_node_types_t::line_comment: {
                    context.comments.emplace_back(builder.make_comment(
                        scope_manager.current_scope(),
                        comment_type_t::line,
                        comment->token.value));
                    break;
                }
                case syntax::ast_node_types_t::block_comment: {
                    context.comments.emplace_back(builder.make_comment(
                        scope_manager.current_scope(),
                        comment_type_t::block,
                        comment->token.value));
                    break;
                }
                default:
                    break;
            }
        }

        auto it = s_node_evaluators.find(node->type);
        if (it != s_node_evaluators.end()) {
            evaluator_result_t result {};
            if (it->second(this, context, result)) {
                context.apply_attributes(result.element);
                context.apply_comments(result.element);
                return result.element;
            }
        } else {
            _session.error(
                "P071",
                fmt::format(
                    "ast node evaluation failed: id = {}, type = {}",
                    node->id,
                    syntax::ast_node_type_name(node->type)),
                node->location);
        }

        return nullptr;
    }

    element* ast_evaluator::evaluate_in_scope(
            const evaluator_context_t& context,
            const syntax::ast_node_t* node,
            compiler::block* scope) {
        auto& scope_manager = _session.scope_manager();

        if (scope != nullptr)
            scope_manager.push_scope(scope);

        auto result = evaluate(node);

        if (scope != nullptr)
            scope_manager.pop_scope();

        return result;
    }

    void ast_evaluator::add_procedure_instance(
            const evaluator_context_t& context,
            compiler::procedure_type* proc_type,
            const syntax::ast_node_t* node) {
        auto& builder = _session.builder();

        if (node->children.empty())
            return;

        for (const auto& child_node : node->children) {
            switch (child_node->type) {
                case syntax::ast_node_types_t::attribute: {
                    auto attribute = builder.make_attribute(
                        proc_type->scope(),
                        child_node->token.value,
                        evaluate(child_node->lhs.get()));
                    attribute->parent_element(proc_type);
                    proc_type->attributes().add(attribute);
                    break;
                }
                case syntax::ast_node_types_t::basic_block: {
                    auto basic_block = dynamic_cast<compiler::block*>(evaluate_in_scope(
                        context,
                        child_node.get(),
                        proc_type->scope()));
                    auto instance = builder.make_procedure_instance(
                        proc_type->scope(),
                        proc_type,
                        basic_block);
                    instance->parent_element(proc_type);
                    proc_type->instances().push_back(instance);
                }
                default:
                    break;
            }
        }
    }

    void ast_evaluator::add_expression_to_scope(
            compiler::block* scope,
            compiler::element* expr) {
        switch (expr->element_type()) {
            case element_type_t::import_e: {
                auto import = dynamic_cast<compiler::import*>(expr);
                scope->imports().emplace_back(import);
                break;
            }
            case element_type_t::attribute: {
                auto attribute = dynamic_cast<compiler::attribute*>(expr);
                scope->attributes().add(attribute);
                break;
            }
            case element_type_t::statement: {
                auto statement = dynamic_cast<compiler::statement*>(expr);
                scope->statements().emplace_back(statement);
                break;
            }
            default:
                break;
        }
    }

    compiler::element* ast_evaluator::resolve_symbol_or_evaluate(
            const evaluator_context_t& context,
            const syntax::ast_node_t* node,
            compiler::block* scope) {
        auto& builder = _session.builder();
        auto& scope_manager = _session.scope_manager();

        compiler::element* element = nullptr;
        if (node != nullptr
        &&  node->type == syntax::ast_node_types_t::symbol) {
            qualified_symbol_t qualified_symbol {};
            builder.make_qualified_symbol(qualified_symbol, node);
            element = builder.make_identifier_reference(
                scope_manager.current_scope(),
                qualified_symbol,
                scope_manager.find_identifier(qualified_symbol, scope));
        } else {
            if (scope != nullptr)
                element = evaluate_in_scope(context, node, scope);
            else
                element = evaluate(node);
        }

        return element;
    }

    compiler::block* ast_evaluator::add_namespaces_to_scope(
            const evaluator_context_t& context,
            const syntax::ast_node_t* node,
            compiler::symbol_element* symbol,
            compiler::block* parent_scope) {
        auto& builder = _session.builder();
        auto& scope_manager = _session.scope_manager();

        // XXX: revisit this
        auto namespace_type = scope_manager.find_type(qualified_symbol_t {
            .name = "namespace"
        });
        auto namespace_type_ref = builder.make_type_reference(
            parent_scope,
            namespace_type->symbol()->qualified_symbol(),
            namespace_type);

        auto namespaces = symbol->namespaces();
        auto scope = parent_scope;
        string_list_t temp_list {};
        std::string namespace_name {};
        for (size_t i = 0; i < namespaces.size(); i++) {
            if (!namespace_name.empty())
                temp_list.push_back(namespace_name);
            namespace_name = namespaces[i];
            auto var = scope->identifiers().find(namespace_name);
            if (var == nullptr) {
                auto new_scope = builder.make_block(scope, element_type_t::block);
                auto ns = builder.make_namespace(scope, new_scope);
                auto ns_identifier = builder.make_identifier(
                    scope,
                    builder.make_symbol(scope, namespace_name, temp_list),
                    builder.make_initializer(scope, ns));
                ns_identifier->type_ref(namespace_type_ref);
                ns_identifier->inferred_type(true);
                ns_identifier->parent_element(scope->parent_element());
                scope->blocks().push_back(new_scope);
                scope->identifiers().add(ns_identifier);
                scope = new_scope;
            } else {
                auto expr = var->initializer()->expression();
                if (expr->element_type() == element_type_t::namespace_e) {
                    auto ns = dynamic_cast<namespace_element*>(expr);
                    scope = dynamic_cast<compiler::block*>(ns->expression());
                } else {
                    _session.error(
                        "P018",
                        "only a namespace is valid within a qualified name.",
                        node->lhs->location);
                    return nullptr;
                }
            }
        }
        return scope;
    }

    void ast_evaluator::add_composite_type_fields(
            const evaluator_context_t& context,
            compiler::composite_type* type,
            const syntax::ast_node_t* block) {
        auto& builder = _session.builder();
        compiler::field* previous_field = nullptr;

        for (const auto& child : block->children) {
            if (child->type != syntax::ast_node_types_t::statement) {
                break;
            }

            auto expr_node = child->rhs;
            switch (expr_node->type) {
                case syntax::ast_node_types_t::assignment:
                case syntax::ast_node_types_t::constant_assignment: {
                    element_list_t list {};
                    auto success = add_assignments_to_scope(
                        context,
                        expr_node.get(),
                        list,
                        type->scope());
                    if (success) {
                        auto new_field = builder.make_field(
                            type,
                            type->scope(),
                            dynamic_cast<compiler::declaration*>(list.front()),
                            previous_field != nullptr ? previous_field->end_offset() : 0);
                        type->fields().add(new_field);
                        previous_field = new_field;
                    }
                    break;
                }
                case syntax::ast_node_types_t::symbol: {
                    auto field_decl = declare_identifier(
                        context,
                        expr_node.get(),
                        type->scope());
                    if (field_decl != nullptr) {
                        auto new_field = builder.make_field(
                            type,
                            type->scope(),
                            field_decl,
                            previous_field != nullptr ? previous_field->end_offset() : 0);
                        type->fields().add(new_field);
                        previous_field = new_field;
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }

    compiler::declaration* ast_evaluator::add_identifier_to_scope(
            const evaluator_context_t& context,
            compiler::symbol_element* symbol,
            compiler::type_reference* type_ref,
            const syntax::ast_node_t* node,
            size_t source_index,
            compiler::block* parent_scope) {
        auto& builder = _session.builder();
        auto& scope_manager = _session.scope_manager();

        auto scope = symbol->is_qualified()
                     ? scope_manager.current_top_level()
                     : parent_scope != nullptr ? parent_scope : scope_manager.current_scope();

        scope = add_namespaces_to_scope(context, node, symbol, scope);

        syntax::ast_node_shared_ptr source_node = nullptr;
        if (node != nullptr) {
            source_node = node->rhs->children[source_index];
        }

        auto init_expr = (compiler::element*) nullptr;
        auto init = (compiler::initializer*) nullptr;
        if (node != nullptr) {
            init_expr = evaluate_in_scope(context, source_node.get(), scope);
            if (init_expr != nullptr) {
                if (init_expr->element_type() == element_type_t::symbol) {
                    auto init_symbol = dynamic_cast<compiler::symbol_element*>(init_expr);
                    init_expr = builder.make_identifier_reference(
                        scope,
                        init_symbol->qualified_symbol(),
                        nullptr);
                }
                if (init_expr->is_constant()) {
                    init = builder.make_initializer(scope, init_expr);
                }
            } else {
                if (_session.result().is_failed())
                    return nullptr;
            }
        }

        // XXX: clean up
        if (!symbol->is_constant()) {
            auto is_module = init_expr != nullptr
                             && init_expr->element_type() == element_type_t::module_reference;
            if (is_module) {
                _session.error(
                    "P029",
                    "constant assignment (::=) is required for module references.",
                    node->location);
                return nullptr;
            }

            auto is_ns = init_expr != nullptr
                         && init_expr->element_type() == element_type_t::namespace_e;
            if (is_ns) {
                _session.error(
                    "P029",
                    "constant assignment (::=) is required for namespaces.",
                    node->location);
                return nullptr;
            }

            auto is_type = init_expr != nullptr && init_expr->is_type();
            auto is_type_directive = init_expr != nullptr
                                     && init_expr->element_type() == element_type_t::directive
                                     && dynamic_cast<compiler::directive*>(init_expr)->name() == "type";
            if (is_type || is_type_directive) {
                _session.error(
                    "P029",
                    "constant assignment (::=) is required for types.",
                    node->location);
                return nullptr;
            }
        }

        auto new_identifier = builder.make_identifier(scope, symbol, init);
        if (init_expr != nullptr) {
            if (init == nullptr)
                init_expr->parent_element(new_identifier);
        }

        if (init_expr != nullptr && type_ref == nullptr) {
            infer_type_result_t infer_type_result {};
            if (!init_expr->infer_type(_session, infer_type_result)) {
                _session.error(
                    "P019",
                    fmt::format("unable to infer type: {}", new_identifier->symbol()->name()),
                    new_identifier->symbol()->location());
                return nullptr;
            }

            if (infer_type_result.reference == nullptr) {
                infer_type_result.reference = builder.make_type_reference(
                    scope,
                    infer_type_result.inferred_type->symbol()->qualified_symbol(),
                    infer_type_result.inferred_type);
            }

            new_identifier->type_ref(infer_type_result.reference);
            new_identifier->inferred_type(infer_type_result.inferred_type != nullptr);
        } else {
            new_identifier->type_ref(type_ref);
            if (type_ref->is_unknown_type()) {
                _session.scope_manager()
                    .identifiers_with_unknown_types()
                    .push_back(new_identifier);
            }
        }

        scope->identifiers().add(new_identifier);

        if (init != nullptr
        &&  init->expression()->element_type() == element_type_t::proc_type) {
            add_procedure_instance(
                context,
                dynamic_cast<procedure_type*>(init->expression()),
                source_node.get());
        }

        compiler::binary_operator* assign_bin_op = nullptr;

        if (init == nullptr
        &&  init_expr == nullptr
        &&  new_identifier->type_ref() == nullptr) {
            _session.error(
                "P019",
                fmt::format("unable to infer type: {}", new_identifier->symbol()->name()),
                new_identifier->symbol()->location());
            return nullptr;
        } else {
            if (init == nullptr && init_expr != nullptr) {
                // XXX: revisit after type-widening in binary/unary operators is fixed
                //if (symbol->is_constant()) {
                //    _session.error(
                //        "P028",
                //        "constant variables require constant expressions.",
                //        symbol->location());
                //    return nullptr;
                //}
                assign_bin_op = builder.make_binary_operator(
                    scope,
                    operator_type_t::assignment,
                    new_identifier,
                    init_expr);
            }
        }

        return builder.make_declaration(scope, new_identifier, assign_bin_op);
    }

    bool ast_evaluator::nil(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        result.element = _session.builder().nil_literal();
        return true;
    }

    bool ast_evaluator::noop(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        return false;
    }

    bool ast_evaluator::symbol(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        result.element = _session.builder().make_symbol_from_node(context.node);
        return true;
    }

    bool ast_evaluator::raw_block(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        result.element = _session.builder().make_raw_block(
            _session.scope_manager().current_scope(),
            context.node->token.value);
        return true;
    }

    bool ast_evaluator::attribute(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        result.element = _session.builder().make_attribute(
            _session.scope_manager().current_scope(),
            context.node->token.value,
            evaluate(context.node->lhs.get()));
        result.element->location(context.node->location);
        return true;
    }

    bool ast_evaluator::directive(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto& builder = _session.builder();
        auto& scope_manager = _session.scope_manager();

        auto expression = evaluate(context.node->lhs.get());
        if (expression == nullptr)
            return false;

        auto directive_element = builder.make_directive(
            scope_manager.current_scope(),
            context.node->token.value,
            expression);
        directive_element->location(context.node->location);
        directive_element->evaluate(_session);
        result.element = directive_element;

        return true;
    }

    bool ast_evaluator::module(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto& builder = _session.builder();
        auto& program = _session.program();
        auto& scope_manager = _session.scope_manager();

        auto module_block = builder.make_block(program.block(), element_type_t::module_block);
        auto module = builder.make_module(program.block(), module_block);
        module->source_file(_session.current_source_file());
        program.block()->blocks().push_back(module_block);

        scope_manager.push_scope(module_block);
        scope_manager.top_level_stack().push(module_block);
        scope_manager.module_stack().push(module);

        for (auto it = context.node->children.begin();
                 it != context.node->children.end();
                 ++it) {
            auto expr = evaluate((*it).get());
            if (expr == nullptr)
                return false;
            add_expression_to_scope(module_block, expr);
            expr->parent_element(module);
        }

        scope_manager.top_level_stack().pop();
        scope_manager.module_stack().pop();

        result.element = module;

        return true;
    }

    bool ast_evaluator::break_expression(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto& builder = _session.builder();
        auto& scope_manager = _session.scope_manager();

        compiler::label* label = nullptr;
        if (context.node->lhs != nullptr) {
            label = builder.make_label(
                scope_manager.current_scope(),
                context.node->lhs->token.value);
        }

        result.element = builder.make_break(scope_manager.current_scope(), label);
        return true;
    }

    bool ast_evaluator::continue_expression(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto& builder = _session.builder();
        auto& scope_manager = _session.scope_manager();

        compiler::label* label = nullptr;
        if (context.node->lhs != nullptr) {
            label = builder.make_label(
                scope_manager.current_scope(),
                context.node->lhs->token.value);
        }

        result.element = builder.make_continue(scope_manager.current_scope(), label);
        return true;
    }

    bool ast_evaluator::module_expression(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto expr = resolve_symbol_or_evaluate(context, context.node->rhs.get());
        auto reference = _session.builder().make_module_reference(
            _session.scope_manager().current_scope(),
            expr);

        std::string path;
        if (expr->is_constant() && expr->as_string(path)) {
            boost::filesystem::path source_path(path);
            auto current_source_file = _session.current_source_file();
            if (current_source_file != nullptr
            &&  source_path.is_relative()) {
                source_path = boost::filesystem::absolute(
                    source_path,
                    current_source_file->path().parent_path());
            }
            auto source_file = _session.add_source_file(source_path);
            auto module = _session.compile_module(source_file);
            if (module == nullptr) {
                _session.error(
                    "C021",
                    "unable to load module.",
                    context.node->rhs->location);
                return false;
            }
            reference->module(module);
            result.element = reference;
        } else {
            _session.error(
                "C021",
                "expected string literal or constant string variable.",
                context.node->rhs->location);
            return false;
        }

        return true;
    }

    bool ast_evaluator::line_comment(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        result.element = _session.builder().make_comment(
            _session.scope_manager().current_scope(),
            comment_type_t::line,
            context.node->token.value);
        return true;
    }

    bool ast_evaluator::block_comment(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        result.element = _session.builder().make_comment(
            _session.scope_manager().current_scope(),
            comment_type_t::block,
            context.node->token.value);
        return true;
    }

    bool ast_evaluator::string_literal(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        result.element = _session.builder().make_string(
            _session.scope_manager().current_scope(),
            context.node->token.value);
        result.element->location(context.node->location);
        return true;
    }

    bool ast_evaluator::number_literal(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        switch (context.node->token.number_type) {
            case syntax::number_types_t::integer: {
                uint64_t value;
                if (context.node->token.parse(value) == syntax::conversion_result_t::success) {
                    if (context.node->token.is_signed()) {
                        result.element = _session.builder().make_integer(
                            _session.scope_manager().current_scope(),
                            common::twos_complement(value));
                    } else {
                        result.element = _session.builder().make_integer(
                            _session.scope_manager().current_scope(),
                            value);
                    }
                    result.element->location(context.node->location);
                    return true;
                } else {
                    _session.error(
                        "P041",
                        "invalid integer literal",
                        context.node->location);
                }
                break;
            }
            case syntax::number_types_t::floating_point: {
                double value;
                if (context.node->token.parse(value) == syntax::conversion_result_t::success) {
                    result.element = _session.builder().make_float(
                        _session.scope_manager().current_scope(),
                        value);
                    result.element->location(context.node->location);
                    return true;
                } else {
                    _session.error(
                        "P041",
                        "invalid float literal",
                        context.node->location);
                }
                break;
            }
            default:
                break;
        }

        return false;
    }

    bool ast_evaluator::boolean_literal(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto& builder = _session.builder();
        auto bool_value = context.node->token.as_bool();

        result.element = bool_value ?
            builder.true_literal() :
            builder.false_literal();

        return true;
    }

    bool ast_evaluator::character_literal(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto& builder = _session.builder();

        std::vector<uint8_t> _buffer;
        for (auto c : context.node->token.value)
            _buffer.push_back(c);

        auto ch = _buffer[0];
        rune_t rune = ch;
        if (ch >= 0x80) {
            auto cp = common::utf8_decode(
                (char*)(_buffer.data()),
                _buffer.size());
            rune = cp.value;
        }

        result.element = builder.make_character(
            _session.scope_manager().current_scope(),
            rune);

        return true;
    }

    bool ast_evaluator::array_constructor(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto args = dynamic_cast<compiler::argument_list*>(evaluate(context.node->lhs.get()));
        // XXX: walk args and build subscripts
        //
        // [1, 2, 3] => [3]
        //
        // [[1,2,3], [1,2,3], [1,2,3]] => [1][3]
        //
        //
        // XXX: determine entry type
        // different type families, use any
        // integer/float, widen/narrow as needed
        //
        result.element = _session.builder().make_array_constructor(
            _session.scope_manager().current_scope(),
            args);
        return true;
    }

    bool ast_evaluator::namespace_expression(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        result.element = _session.builder().make_namespace(
            _session.scope_manager().current_scope(),
            evaluate(context.node->rhs.get()));
        return true;
    }

    bool ast_evaluator::expression(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        result.element = _session.builder().make_expression(
            _session.scope_manager().current_scope(),
            evaluate(context.node->lhs.get()));
        result.element->location(context.node->location);
        return true;
    }

    bool ast_evaluator::argument_list(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto args = _session.builder().make_argument_list(_session.scope_manager().current_scope());
        for (const auto& arg_node : context.node->children) {
            auto arg = resolve_symbol_or_evaluate(context, arg_node.get());
            args->add(arg);
            arg->parent_element(args);
        }
        result.element = args;
        return true;
    }

    bool ast_evaluator::unary_operator(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto it = s_unary_operators.find(context.node->token.type);
        if (it == s_unary_operators.end())
            return false;
        result.element = _session.builder().make_unary_operator(
            _session.scope_manager().current_scope(),
            it->second,
            resolve_symbol_or_evaluate(context, context.node->rhs.get()));
        return true;
    }

    bool ast_evaluator::spread_operator(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        compiler::element* expr = nullptr;
        if (context.node->lhs != nullptr) {
            expr = resolve_symbol_or_evaluate(context, context.node->lhs.get());
        }
        result.element = _session.builder().make_spread_operator(
            _session.scope_manager().current_scope(),
            expr);
        return true;
    }

    bool ast_evaluator::binary_operator(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto it = s_binary_operators.find(context.node->token.type);
        if (it == s_binary_operators.end())
            return false;
        auto scope = _session.scope_manager().current_scope();

        compiler::element* lhs = nullptr;
        compiler::element* rhs = nullptr;

        if (is_logical_conjunction_operator(it->second)) {
            lhs = convert_predicate(context, context.node->lhs.get(), scope);
            rhs = convert_predicate(context, context.node->rhs.get(), scope);
        } else {
            compiler::block* type_scope = nullptr;

            lhs = resolve_symbol_or_evaluate(
                context,
                context.node->lhs.get());
            infer_type_result_t infer_type_result {};
            if (!lhs->infer_type(_session, infer_type_result)) {
                _session.error(
                    lhs,
                    "P052",
                    "unable to infer type.",
                    lhs->location());
                return false;
            }

            if (infer_type_result.inferred_type->is_composite_type()) {
                compiler::composite_type* composite_type = nullptr;
                if (infer_type_result.inferred_type->is_pointer_type()) {
                    auto pointer_type = dynamic_cast<compiler::pointer_type*>(infer_type_result.inferred_type);
                    composite_type = dynamic_cast<compiler::composite_type*>(pointer_type->base_type_ref()->type());
                } else {
                    composite_type = dynamic_cast<compiler::composite_type*>(infer_type_result.inferred_type);
                }
                type_scope = composite_type->scope();
            } else {
                if (it->second == operator_type_t::member_access) {
                    _session.error(
                        lhs,
                        "P053",
                        "member access requires lhs composite type.",
                        lhs->location());
                    return false;
                }
            }

            rhs = resolve_symbol_or_evaluate(
                context,
                context.node->rhs.get(),
                type_scope);
        }

        result.element = _session.builder().make_binary_operator(
            scope,
            it->second,
            lhs,
            rhs);

        return true;
    }

    bool ast_evaluator::while_expression(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto& builder = _session.builder();
        auto& scope_manager = _session.scope_manager();
        auto scope = scope_manager.current_scope();

        auto predicate = convert_predicate(
            context,
            context.node->lhs.get(),
            scope);
        auto body = evaluate(context.node->rhs.get());

        result.element = builder.make_while(
            scope,
            dynamic_cast<compiler::binary_operator*>(predicate),
            dynamic_cast<compiler::block*>(body));

        return true;
    }

    bool ast_evaluator::cast_expression(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto& builder = _session.builder();
        auto& scope_manager = _session.scope_manager();

        qualified_symbol_t type_name {
            .name = context.node->lhs->lhs->children[0]->token.value
        };
        auto type = scope_manager.find_type(type_name);
        if (type == nullptr) {
            _session.error(
                "P002",
                fmt::format("unknown type '{}'.", type_name.name),
                context.node->lhs->lhs->location);
            return false;
        }
        auto cast_element = builder.make_cast(
            scope_manager.current_scope(),
            builder.make_type_reference(scope_manager.current_scope(), type_name, type),
            resolve_symbol_or_evaluate(context, context.node->rhs.get()));
        cast_element->location(context.node->location);
        cast_element->type_location(context.node->lhs->lhs->location);
        result.element = cast_element;
        return true;
    }

    bool ast_evaluator::return_statement(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto return_element = _session.builder().make_return(_session.scope_manager().current_scope());
        auto& expressions = return_element->expressions();
        for (const auto& arg_node : context.node->rhs->children) {
            auto arg = resolve_symbol_or_evaluate(context, arg_node.get());
            expressions.push_back(arg);
            arg->parent_element(return_element);
        }
        result.element = return_element;
        return true;
    }

    bool ast_evaluator::import_expression(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto& builder = _session.builder();
        auto& scope_manager = _session.scope_manager();

        qualified_symbol_t qualified_symbol {};
        builder.make_qualified_symbol(qualified_symbol, context.node->lhs.get());

        compiler::identifier_reference* from_reference = nullptr;
        if (context.node->rhs != nullptr) {
            from_reference = dynamic_cast<compiler::identifier_reference*>(
                resolve_symbol_or_evaluate(context, context.node->rhs.get()));
            qualified_symbol.namespaces.insert(
                qualified_symbol.namespaces.begin(),
                from_reference->symbol().name);
        }

        auto identifier_reference = builder.make_identifier_reference(
            scope_manager.current_scope(),
            qualified_symbol,
            scope_manager.find_identifier(qualified_symbol));
        auto import = builder.make_import(
            scope_manager.current_scope(),
            identifier_reference,
            from_reference,
            dynamic_cast<compiler::module*>(scope_manager.current_top_level()->parent_element()));
        add_expression_to_scope(scope_manager.current_scope(), import);

        result.element = import;

        return true;
    }

    bool ast_evaluator::subscript_operator(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        result.element = _session.builder().make_binary_operator(
            _session.scope_manager().current_scope(),
            operator_type_t::subscript,
            resolve_symbol_or_evaluate(context, context.node->lhs.get()),
            evaluate(context.node->rhs.get()));
        return true;
    }

    bool ast_evaluator::basic_block(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto& scope_manager = _session.scope_manager();
        auto active_scope = scope_manager.push_new_block();

        for (auto it = context.node->children.begin();
                 it != context.node->children.end();
                 ++it) {
            auto current_node = *it;
            auto expr = evaluate(current_node.get());
            if (expr == nullptr) {
                _session.error(
                    "C024",
                    "invalid statement",
                    current_node->location);
                return false;
            }
            add_expression_to_scope(active_scope, expr);
            expr->parent_element(active_scope);
        }

        result.element = scope_manager.pop_scope();
        return true;
    }

    bool ast_evaluator::proc_call(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto& builder = _session.builder();
        auto& scope_manager = _session.scope_manager();

        qualified_symbol_t qualified_symbol {};
        builder.make_qualified_symbol(qualified_symbol, context.node->lhs.get());

        compiler::argument_list* args = nullptr;
        auto expr = evaluate(context.node->rhs.get());
        if (expr != nullptr) {
            args = dynamic_cast<compiler::argument_list*>(expr);
        }

        auto intrinsic = compiler::intrinsic::intrinsic_for_call(
            _session,
            scope_manager.current_scope(),
            args,
            qualified_symbol);
        if (intrinsic != nullptr) {
            result.element = intrinsic;
            return true;
        }

        auto proc_identifier = scope_manager.find_identifier(qualified_symbol);
        result.element = builder.make_procedure_call(
            scope_manager.current_scope(),
            builder.make_identifier_reference(
                scope_manager.current_scope(),
                qualified_symbol,
                proc_identifier),
            args);
        result.element->location(context.node->location);

        return true;
    }

    bool ast_evaluator::statement(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto& builder = _session.builder();
        auto& scope_manager = _session.scope_manager();

        label_list_t labels {};

        for (const auto& label : context.node->labels) {
            labels.push_back(builder.make_label(
                scope_manager.current_scope(),
                label->token.value));
        }

        compiler::element* expr = nullptr;
        if (context.node->rhs != nullptr) {
            expr = evaluate(context.node->rhs.get());
            if (expr == nullptr)
                return false;

            if (expr->element_type() == element_type_t::symbol) {
                auto type_ref = dynamic_cast<compiler::type_reference*>(evaluate(context.node->rhs->rhs.get()));
                if (type_ref == nullptr)
                    return false;
                expr = add_identifier_to_scope(
                    context,
                    dynamic_cast<compiler::symbol_element*>(expr),
                    type_ref,
                    nullptr,
                    0);
            }
        }

        result.element = builder.make_statement(
            scope_manager.current_scope(),
            labels,
            expr);

        return true;
    }

    bool ast_evaluator::enum_expression(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto& builder = _session.builder();
        auto& scope_manager = _session.scope_manager();

        auto active_scope = scope_manager.current_scope();
        auto enum_type = builder.make_enum_type(
            active_scope,
            builder.make_block(active_scope, element_type_t::block));
        active_scope->types().add(enum_type);
        add_composite_type_fields(
            context,
            enum_type,
            context.node->rhs.get());
        if (!enum_type->initialize(_session))
            return false;
        result.element = enum_type;

        return true;
    }

    bool ast_evaluator::with_expression(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto& builder = _session.builder();
        auto& scope_manager = _session.scope_manager();

        result.element = builder.make_with(
            scope_manager.current_scope(),
            dynamic_cast<compiler::identifier_reference*>(
                resolve_symbol_or_evaluate(
                    context,
                    context.node->lhs.get())),
            dynamic_cast<compiler::block*>(evaluate(context.node->rhs.get())));
        return true;
    }

    bool ast_evaluator::defer_expression(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto& builder = _session.builder();
        auto& scope_manager = _session.scope_manager();

        result.element = builder.make_defer(
            scope_manager.current_scope(),
            evaluate(context.node->lhs.get()));

        return true;
    }

    bool ast_evaluator::struct_expression(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto& builder = _session.builder();
        auto& scope_manager = _session.scope_manager();

        auto active_scope = scope_manager.current_scope();
        auto struct_type = builder.make_struct_type(
            active_scope,
            builder.make_block(active_scope, element_type_t::block));
        active_scope->types().add(struct_type);
        add_composite_type_fields(
            context,
            struct_type,
            context.node->rhs.get());
        if (!struct_type->initialize(_session))
            return false;
        result.element = struct_type;
        return true;
    }

    bool ast_evaluator::union_expression(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto& builder = _session.builder();
        auto& scope_manager = _session.scope_manager();

        auto active_scope = scope_manager.current_scope();
        auto union_type = builder.make_union_type(
            active_scope,
            builder.make_block(active_scope, element_type_t::block));
        active_scope->types().add(union_type);
        add_composite_type_fields(
            context,
            union_type,
            context.node->rhs.get());
        if (!union_type->initialize(_session))
            return false;
        result.element = union_type;
        return true;
    }

    bool ast_evaluator::else_expression(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        result.element = evaluate(context.node->children[0].get());
        return true;
    }

    bool ast_evaluator::if_expression(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto scope = _session.scope_manager().current_scope();

        auto predicate = convert_predicate(context, context.node->lhs.get(), scope);
        auto true_branch = evaluate(context.node->children[0].get());
        auto false_branch = evaluate(context.node->rhs.get());
        result.element = _session.builder().make_if(
            scope,
            predicate,
            true_branch,
            false_branch,
            context.node->type == syntax::ast_node_types_t::elseif_expression);
        return true;
    }

    bool ast_evaluator::proc_expression(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto& builder = _session.builder();
        auto& scope_manager = _session.scope_manager();

        auto active_scope = scope_manager.current_scope();
        auto block_scope = builder.make_block(active_scope, element_type_t::block);
        auto proc_type = builder.make_procedure_type(active_scope, block_scope);
        active_scope->types().add(proc_type);

        auto count = 0;
        compiler::field* return_field = nullptr;
        if (!context.node->lhs->children.empty()) {
            auto return_identifier = builder.make_identifier(
                block_scope,
                builder.make_symbol(block_scope, fmt::format("_{}", count++)),
                nullptr);
            return_identifier->usage(identifier_usage_t::stack);

            auto type_ref = dynamic_cast<compiler::type_reference*>(evaluate_in_scope(
                context,
                context.node->lhs->children[0].get(),
                block_scope));
            if (type_ref->is_unknown_type()) {
                _session.scope_manager()
                    .identifiers_with_unknown_types()
                    .push_back(return_identifier);
            }
            return_identifier->type_ref(type_ref);
            return_field = builder.make_field(
                proc_type,
                block_scope,
                builder.make_declaration(block_scope, return_identifier, nullptr),
                return_field != nullptr ? return_field->end_offset() : 0);
            proc_type->return_type(return_field);
        }

        compiler::field* param_field = nullptr;
        for (const auto& param_node : context.node->rhs->children) {
            switch (param_node->type) {
                case syntax::ast_node_types_t::assignment: {
                    element_list_t list {};
                    auto success = add_assignments_to_scope(
                        context,
                        param_node.get(),
                        list,
                        block_scope);
                    if (success) {
                        auto param_decl = dynamic_cast<compiler::declaration*>(list.front());
                        param_decl->identifier()->usage(identifier_usage_t::stack);
                        param_field = builder.make_field(
                            proc_type,
                            block_scope,
                            param_decl,
                            param_field != nullptr ? param_field->end_offset() : 0);
                        proc_type->parameters().add(param_field);
                    } else {
                        return false;
                    }
                    break;
                }
                case syntax::ast_node_types_t::symbol: {
                    auto param_decl = declare_identifier(
                        context,
                        param_node.get(),
                        block_scope);
                    if (param_decl != nullptr) {
                        param_decl->identifier()->usage(identifier_usage_t::stack);
                        param_field = builder.make_field(
                            proc_type,
                            block_scope,
                            param_decl,
                            param_field != nullptr ? param_field->end_offset() : 0);
                        proc_type->parameters().add(param_field);
                    } else {
                        return false;
                    }
                    break;
                }
                default: {
                    break;
                }
            }
        }

        result.element = proc_type;

        return true;
    }

    bool ast_evaluator::assignment(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto assignment = _session
            .builder()
            .make_assignment(_session.scope_manager().current_scope());
        auto success = add_assignments_to_scope(
            context,
            context.node,
            assignment->expressions(),
            nullptr);
        result.element = assignment;
        return success;
    }

    bool ast_evaluator::type_identifier(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto& builder = _session.builder();
        auto& scope_manager = _session.scope_manager();
        auto scope = scope_manager.current_scope();

        auto is_dynamic = false;
        element_list_t array_subscripts {};

        for (const auto& node : context.node->rhs->children) {
            auto expr = resolve_symbol_or_evaluate(
                context,
                node.get());
            if (expr->element_type() == element_type_t::spread) {
                is_dynamic = true;
            } else if (!expr->is_constant()) {
                _session.error(
                    "P002",
                    "subscript size expressions must be constant.",
                    expr->location());
                return false;
            }
            if (expr != nullptr)
                array_subscripts.emplace_back(expr);
        }

        if (is_dynamic && array_subscripts.size() > 1) {
            _session.error(
                "P002",
                "only one subscript is allowed for dynamic arrays.",
                context.node->location);
            return false;
        }

        type_find_result_t find_type_result {};
        scope_manager.find_identifier_type(
            find_type_result,
            context.node,
            array_subscripts,
            scope);

        compiler::type_reference* type_ref = nullptr;
        if (find_type_result.type == nullptr) {
            auto unknown_type = builder.make_unknown_type_from_find_result(
                scope,
                find_type_result);
            type_ref = builder.make_type_reference(
                scope,
                unknown_type->symbol()->qualified_symbol(),
                unknown_type);
        } else {
            type_ref = find_type_result.make_type_reference(builder, scope);
        }

        result.element = type_ref;

        return true;
    }

    bool ast_evaluator::for_in_statement(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto& builder = _session.builder();
        auto& scope_manager = _session.scope_manager();

        auto rhs = evaluate(context.node->rhs.get());
        auto body = dynamic_cast<compiler::block*>(evaluate(context.node->children[0].get()));

        auto lhs = evaluate(context.node->lhs.get());
        if (lhs == nullptr || lhs->element_type() != element_type_t::symbol) {
            // XXX: error
            return false;
        }

        compiler::type_reference* type_ref = nullptr;
        if (context.node->lhs->rhs != nullptr) {
            type_ref = dynamic_cast<compiler::type_reference*>(evaluate(context.node->lhs->rhs.get()));
        } else {
            infer_type_result_t infer_type_result {};
            if (rhs->infer_type(_session, infer_type_result)) {
                type_ref = infer_type_result.reference;
            } else {
                // XXX: error
                return false;
            }
        }

        auto induction_decl = add_identifier_to_scope(
            context,
            dynamic_cast<compiler::symbol_element*>(lhs),
            type_ref,
            nullptr,
            0,
            body);

        result.element = builder.make_for(
            scope_manager.current_scope(),
            induction_decl,
            rhs,
            body);

        return true;
    }

    bool ast_evaluator::tuple_expression(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        return false;
    }

    bool ast_evaluator::transmute_expression(
            evaluator_context_t& context,
            evaluator_result_t& result) {
        auto& builder = _session.builder();
        auto& scope_manager = _session.scope_manager();

        qualified_symbol_t type_name {
            .name = context.node->lhs->lhs->children[0]->token.value
        };
        auto type = scope_manager.find_type(type_name);
        if (type == nullptr) {
            _session.error(
                "P002",
                fmt::format("unknown type '{}'.", type_name.name),
                context.node->lhs->lhs->location);
            return false;
        }
        auto transmute_element = builder.make_transmute(
            scope_manager.current_scope(),
            builder.make_type_reference(scope_manager.current_scope(), type_name, type),
            resolve_symbol_or_evaluate(context, context.node->rhs.get()));
        transmute_element->location(context.node->location);
        transmute_element->type_location(context.node->lhs->lhs->location);
        result.element = transmute_element;
        return true;
    }

    bool ast_evaluator::add_assignments_to_scope(
            const evaluator_context_t& context,
            const syntax::ast_node_t* node,
            element_list_t& expressions,
            compiler::block* scope) {
        auto& builder = _session.builder();
        auto& scope_manager = _session.scope_manager();

        const auto& target_list = node->lhs;
        const auto& source_list = node->rhs;

        const bool is_constant_assignment = node->type == syntax::ast_node_types_t::constant_assignment;

        if (target_list->children.size() != source_list->children.size()) {
            _session.error(
                "P027",
                "the number of left-hand-side targets must match"
                " the number of right-hand-side expressions.",
                source_list->location);
            return false;
        }

        for (size_t i = 0; i < target_list->children.size(); i++) {
            const auto& target_symbol = target_list->children[i];

            qualified_symbol_t qualified_symbol {};
            builder.make_qualified_symbol(
                qualified_symbol,
                target_symbol.get());
            auto existing_identifier = scope_manager.find_identifier(
                qualified_symbol,
                scope);
            if (existing_identifier != nullptr) {
                if (existing_identifier->symbol()->is_constant()) {
                    _session.error(
                        "P028",
                        "constant variables cannot be modified.",
                        target_symbol->location);
                    return false;
                }

                auto rhs = resolve_symbol_or_evaluate(
                    context,
                    source_list->children[i].get(),
                    scope);
                if (rhs == nullptr)
                    return false;

                auto binary_op = builder.make_binary_operator(
                    scope_manager.current_scope(),
                    operator_type_t::assignment,
                    existing_identifier,
                    rhs);
                expressions.emplace_back(binary_op);
            } else {
                auto lhs = evaluate_in_scope(
                    context,
                    target_symbol.get(),
                    scope);

                auto symbol = dynamic_cast<compiler::symbol_element*>(lhs);
                symbol->constant(is_constant_assignment);

                auto type_ref = dynamic_cast<compiler::type_reference*>(evaluate_in_scope(
                    context,
                    target_symbol->rhs.get(),
                    scope));
                auto decl = add_identifier_to_scope(
                    context,
                    symbol,
                    type_ref,
                    node,
                    i,
                    scope);
                if (decl == nullptr)
                    return false;

                expressions.emplace_back(decl);
            }
        }

        return true;
    }

    compiler::declaration* ast_evaluator::declare_identifier(
            const evaluator_context_t& context,
            const syntax::ast_node_t* node,
            compiler::block* scope) {
        return add_identifier_to_scope(
            context,
            dynamic_cast<compiler::symbol_element*>(evaluate_in_scope(
                context,
                node,
                scope)),
            dynamic_cast<compiler::type_reference*>(evaluate_in_scope(
                context,
                node->rhs.get(),
                scope)),
            nullptr,
            0,
            scope);
    }

    element* ast_evaluator::convert_predicate(
            const evaluator_context_t& context,
            const syntax::ast_node_t* node,
            compiler::block* scope) {
        auto& builder = _session.builder();

        auto predicate = resolve_symbol_or_evaluate(context, node, scope);
        if (predicate->element_type() != element_type_t::binary_operator) {
            infer_type_result_t infer_type_result {};
            if (!predicate->infer_type(_session, infer_type_result))
                return nullptr;

            if (infer_type_result.inferred_type->element_type() != element_type_t::bool_type) {
                _session.error(
                    "P002",
                    "expected a boolean expression.",
                    predicate->location());
                return nullptr;
            }

            predicate = builder.make_binary_operator(
                scope,
                operator_type_t::equals,
                predicate,
                builder.true_literal());
        }

        return predicate;
    }

};