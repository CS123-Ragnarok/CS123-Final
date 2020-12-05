
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

    LSystem();
    ~LSystem();
    void add_rules(char precessor, std::string successor);
    void change_base( std::string new_base);
    std::string derivation(int depth);


private:

    std::string m_base = "";
    std::unordered_map<char, std::string> m_rules;



};


#endif

