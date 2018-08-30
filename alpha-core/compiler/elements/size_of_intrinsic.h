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

#include "intrinsic.h"

namespace basecode::compiler {

    class size_of_intrinsic : public intrinsic {
    public:
        size_of_intrinsic(
            compiler::module* module,
            compiler::block* parent_scope,
            compiler::argument_list* args);

    protected:
        compiler::element* on_fold(compiler::session& session) override;

        compiler::type* on_infer_type(const compiler::session& session) override;
    };

};

