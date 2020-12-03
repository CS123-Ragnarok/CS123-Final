
#ifndef DOL_H
#define DOL_H
#include <vector>
#include "Shape.h"
#include "Cylinder.h"
#include <unordered_map>
#include <string>
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr


class DOL
{
public:

    DOL();
    ~DOL();
    void add_rules(char precessor, std::string& successor);
    void change_base( std::string& new_base);
    void derivation(int depth);
    void interpretation();
    void draw();



private:

    glm::mat4 RotationMatrix(glm::vec3 headding, glm::vec3 left, glm::vec3 up);
    glm::mat3 rotate_up(float rad);
    glm::mat3 rotate_left(float rad);
    glm::mat3 rotate_head(float rad);


    float dist = 3;
    float m_degree = 30;
    float m_rad = m_degree * (M_PI / 180.0f);
    glm::vec3 heading = glm::vec3(0,1,0);
    glm::vec3 left = glm::vec3(0,0,1);
    glm::vec3 up = glm::vec3(1,0,0);

    std::string m_base = "";
    std::unordered_map<char, std::string> m_rules;
    std::vector<Shape> m_shapes;
    std::vector<glm::mat4> m_transform;


};


#endif

