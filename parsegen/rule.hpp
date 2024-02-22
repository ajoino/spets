#ifndef __SPETS_RULE_H__
#define __SPETS_RULE_H__

#include <string>
#include <vector>

struct Rule {
    std::string name;
    std::vector<std::vector<std::string>> alts;
};

#endif // !__SPETS_RULE_H__
