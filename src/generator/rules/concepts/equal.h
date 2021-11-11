#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_EQUAL_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_EQUAL_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class EqualConcept : public Rule {
public:
    EqualConcept() : Rule("c_equal") { }

    virtual void generate(const States& states, int iteration, FeatureGeneratorData& data) override {
        for (int i = 1; i < iteration; ++i) {
            int j = iteration - i;
            for (const auto& role_left : data.get_role_elements_by_complexity()[i]) {
                for (const auto& role_right : data.get_role_elements_by_complexity()[j]) {
                    if (data.reached_limit()) return;
                    else if (data.add_concept(states, data.get_factory().make_equal_concept(role_left, role_right))) {
                        m_count_instantiations += 1;
                    }
                }
            }
        }
    }
};

}

#endif