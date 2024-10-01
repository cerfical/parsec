#pragma once

#include "Node.hpp"

namespace parsec::pars {

    /**
     * @brief Rule matching empty string.
     */
    class EmptyRule : public Node {
    public:

        void accept(NodeVisitor& visitor) const override;
    };

}
