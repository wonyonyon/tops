#include "ProbabilityNode.hpp"

#include "ToPSLangVisitor.hpp"

namespace tops {
  namespace lang {
    void ProbabilityNode::accept(ToPSLangVisitor& visitor) {
      _cond->accept(visitor);
      _value->accept(visitor);
      visitor.visitProbabilityNode(this);
    }
    
    std::string ProbabilityNode::str() {
      return std::string("(ProbabilityNode " + _cond->str() + " " + _value->str() + ")");
    }
  }
}
