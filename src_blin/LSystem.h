
#ifndef LSYSTEM_H
#define LSYSTEM_H
#include <vector>
#include <unordered_map>
#include <string>
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr


class LSystem
{
public:

    LSystem(int seed);
    ~LSystem();
    void add_rules(char precessor, std::string successor);
    void change_base( std::string new_base);
    std::string derivation(int depth);
    void clear_rules();
    void remove_rule(char key);



private:

    std::string m_base = "";
    std::unordered_map<char, std::vector<std::string>> m_rules;

    std::vector<std::string> f_rules;
    std::vector<std::string> r_rules;

    int seed;

};


#endif

