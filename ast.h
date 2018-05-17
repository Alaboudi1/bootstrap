#pragma once

#include <map>
#include <stack>
#include <memory>
#include <vector>
#include <string>
#include <cerrno>
#include <variant>
#include <functional>
#include "token.h"

namespace basecode {

    struct ast_node_t;

    using ast_node_shared_ptr = std::shared_ptr<ast_node_t>;
    using ast_node_list = std::vector<ast_node_shared_ptr>;

    enum class ast_node_types_t {
        program,
        statement,
        variable,
        assignment,
        expression,
        line_comment,
        block_comment,
        if_expression,
        map_constructor,
        array_constructor,
        elseif_expression,
        else_expression,
        for_statement,
        while_statement,
        continue_statement,
        alias_statement,
        extend_statement,
        break_statement,
        switch_expression,
        with_expression,
        namespace_statement,
        function_call,
        function_expression,
        basic_block,
        unary_operator,
        binary_operator,
    };

    struct ast_node_t {
        token_t token;
        ast_node_types_t type;
        ast_node_list children;
        ast_node_shared_ptr lhs = nullptr;
        ast_node_shared_ptr rhs = nullptr;
        ast_node_shared_ptr parent = nullptr;
    };

    class ast_builder {
    public:
        ast_builder();

        virtual ~ast_builder();

        ast_node_shared_ptr pop_scope();

        ast_node_t* current_scope() const;

        ast_node_shared_ptr program_node();

        void push_scope(const ast_node_shared_ptr& node);

        ast_node_shared_ptr line_comment_node(const token_t& token);

    private:
        std::stack<ast_node_shared_ptr> _scope_stack {};
    };
}