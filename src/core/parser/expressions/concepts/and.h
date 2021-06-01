#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_AND_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_AND_H_

#include "../concept.h"
#include "../../../elements/concepts/and.h"

namespace dlp {
namespace core {
namespace parser {

class AndConcept : public Concept {
protected:
    virtual element::Concept_Ptr parse_concept_element_impl(const VocabularyInfoImpl& vocabulary, ElementCache &cache) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("AndConcept::parse_concept_element - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        // 1. Parse children
        element::Concept_Ptr l = m_children[0]->parse_concept_element(vocabulary, cache);
        element::Concept_Ptr r = m_children[1]->parse_concept_element(vocabulary, cache);
        if (!(l && r)) {
            throw std::runtime_error("AndConcept::parse_concept_element - children are not of type Concept.");
        }
        // 2. Construct element
        return std::make_shared<element::AndConcept>(vocabulary, l, r);
    }

public:
    AndConcept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Concept(name, sort_children_lexicographically(std::move(children))) { }
};

}
}
}

#endif
