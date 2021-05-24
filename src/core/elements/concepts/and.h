#ifndef DLP_SRC_CORE_ELEMENTS_CONCEPTS_AND_H_
#define DLP_SRC_CORE_ELEMENTS_CONCEPTS_AND_H_

#include "../concept.h"


namespace dlp {
namespace core {
namespace element {

class AndConceptElement : public ConceptElement {
protected:
    ConceptElement_Ptr m_l;
    ConceptElement_Ptr m_r;

protected:
    virtual Concepts evaluate_impl(const Index_Vec& atoms) override {
        // TODO(dominik):
    }

public:
    AndConceptElement(std::shared_ptr<TaskInfo> task_info, bool goal, ConceptElement_Ptr l, ConceptElement_Ptr r)
    : ConceptElement(task_info, goal), m_l(l), m_r(r) {}

    virtual unsigned complexity() const override {
        return m_l->complexity() + m_r->complexity() + 1;
    }
};

}
}
}

#endif