#include "LSystem.h"

LSystem::LSystem(){
}

LSystem::~LSystem(){

}

 void LSystem::add_rules(char precessor, std::string successor)
{
    if(m_rules.find(precessor) != m_rules.end())
        return;
    m_rules[precessor] = successor;
}

void LSystem::change_base(std::string new_base)
{
   m_base = new_base;
}

void LSystem::derivation(int depth)
{
    while(depth > 0)
    {

        std::string token = "";
        for(int i = 0; i < m_base.size(); i++)
        {
            char key = m_base.at(i);
            if(m_rules.find(key) == m_rules.end())
                token += key;
            else
            {
                std::string rule = m_rules.at(key);
                token += rule;
            }
        }
        m_base = token;
    }
    return;
}

