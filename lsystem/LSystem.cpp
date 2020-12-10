#include "LSystem.h"
#include "random"
#include <ctime>
#include <iostream>


LSystem::LSystem(int seed){
    f_rules.push_back("F");
    f_rules.push_back("F[Fz[zFZXFZYF]R[ZFxzFyzF]R[XFyzFZxF]C+]");
    f_rules.push_back("F[XR][YR][ZR]");
    f_rules.push_back("F[xF][yF][zF]");
    f_rules.push_back("F[xxxRF][yyyRF][zzzRF]");

    r_rules.push_back("FFF[FXYZ[FxRxF[zFRzXFC]R[ZFZyFC]]yFRyF]");
    r_rules.push_back("F[xFR][XFR]");
    r_rules.push_back("f[[XFyR][YFzR][ZFxR]]");
   // r_rules.push_back("XRF");
   // r_rules.push_back("YRF");
   // r_rules.push_back("ZRF");

    r_rules.push_back("F[xx[ZYR]yyF][zz[YXR]XXF][YY[xZR]]");
    m_rules['F'] = f_rules;
    m_rules['R'] = r_rules;
    this->seed = seed;

}

LSystem::~LSystem(){

}

 void LSystem::add_rules(char precessor, std::string successor)
{
//    if(m_rules.find(precessor) != m_rules.end())
//        return;
//    m_rules[precessor] = successor;
}

void LSystem::change_base(std::string new_base)
{
   m_base = new_base;
}

std::string LSystem::derivation(int depth)
{
    srand(seed);
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
                int index = rand() % m_rules.at(key).size();
                std::string rule = m_rules.at(key)[index];
                token += rule;
            }
        }
        m_base = token;
        depth--;
    }
    return m_base;
}

void LSystem::clear_rules()
{
    m_rules.clear();
}

void LSystem::remove_rule(char key){
    m_rules.erase(key);
}

