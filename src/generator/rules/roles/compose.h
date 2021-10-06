#include "../rule.h"


namespace dlplan::generator::rules {

class ComposeRole : public Rule {
public:
    ComposeRole() : Rule("r_compose") { }

    virtual void generate(const States& states, int iteration, FeatureGeneratorData& data) override {
        for (int i = 1; i < iteration; ++i) {
            int j = iteration - i;
            for (const auto& role_left : data.get_role_elements_by_complexity()[i]) {
                for (const auto& role_right : data.get_role_elements_by_complexity()[j]) {
                    if (data.reached_limit()) return;
                    else if (data.add_role(states, data.get_factory().make_compose_role(role_left, role_right))) {
                        m_count_instantiations += 1;
                    }
                }
            }
        }
    }
};

}
