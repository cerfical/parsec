#include "regex/nodes/AlternExpr.hpp"
#include "regex/nodes/ConcatExpr.hpp"
#include "regex/nodes/NodeVisitor.hpp"
#include "regex/nodes/OptionalExpr.hpp"
#include "regex/nodes/PlusClosure.hpp"
#include "regex/nodes/StarClosure.hpp"
#include "regex/nodes/SymbolAtom.hpp"

namespace parsec::regex {
    void SymbolAtom::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void StarClosure::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void PlusClosure::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void OptionalExpr::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void ConcatExpr::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }

    void AlternExpr::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }
}
