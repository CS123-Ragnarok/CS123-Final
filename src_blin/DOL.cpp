#include "DOL.h"

DOL::DOL(){
}

DOL::~DOL(){

}

 void DOL::add_rules(char precessor, std::string& successor)
{
    if(m_rules.find(precessor) != m_rules.end())
        return;
    m_rules[precessor] = successor;
}

void DOL::change_base(std::string& new_base)
{
   m_base = new_base;
}

void DOL::derivation(int depth)
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

void DOL::interpretation()
{
    glm::vec3 pos(0,0,0);
    m_shapes.clear();
    m_transform.clear();
    for(int i = 0; i < m_base.size(); i++)
    {
        char instru = m_base.at(i);
        if(instru == 'f')
        {
            pos += heading * dist;
        }
        else if(instru == 'F')
        {
            Cylinder token(60, 60);
            m_shapes.push_back(token);
            glm::vec3 old_pos = pos;
            pos += heading * dist;
            glm::vec3 mid = (pos + old_pos)/2.0f;
            glm::mat4 translate = glm::translate(glm::vec3(mid));
            glm::mat4 rotate = RotationMatrix(heading, left, up);
            glm::mat4 scale = glm::scale(glm::vec3(1,1,dist));
            glm::mat4 move = rotate * translate * scale;
            m_transform.push_back(move);

        }
        else
        {
            glm::mat3 R;
            if(instru == '+')
                R = rotate_up(m_rad);
            else if(instru == '-')
                 R = rotate_up(-m_rad);

            else if(instru == '&')
                 R = rotate_left(m_rad);
            else if(instru == '^')
                 R = rotate_left(-m_rad);

            else if(instru == '\\')
                 R = rotate_head(m_rad);
            else if(instru == '/')
                 R = rotate_head(-m_rad);

            else if(instru == '|')
                R = rotate_up(M_PI);

            heading = heading * R;
            left = left * R;
            up = up * R;
        }

    }

}

void DOL::draw(){

}



glm::mat4 DOL::RotationMatrix(glm::vec3 headding, glm::vec3 left, glm::vec3 up)
{
    glm::mat4 token = glm::mat4(up.x, up.y, up.z, 0,
                                headding.x, headding.y, headding.z, 0,
                                left.x, left.y, left.z, 0,
                                0,     0,     0,      1);
    return  glm::transpose(token);
}

glm::mat3 DOL::rotate_up(float rad)
{
    glm::mat3 token = glm::mat3( cos(rad),   sin(rad),  0,
                                -sin(rad),   cos(rad),  0,
                                0,           0,         1);
    return glm::transpose(token);

}

glm::mat3 rotate_left(float rad)
{

    glm::mat3 token = glm::mat3(cos(rad),   0,     -sin(rad),
                                   0,        1,         0,
                                sin(rad),     0,      cos(rad));
    return glm::transpose(token);

}

glm::mat3 rotate_head(float rad)
{

    glm::mat3 token = glm::mat3(   1,       0,          0,
                                   0,    cos(rad),   -sin(rad),
                                   0,    sin(rad),    cos(rad));
    return glm::transpose(token);

}

void DOL::draw()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    for(int i = 0; i < num_shape; i++)
    {
        glm::mat4x4 token_trans =  m_transform.at(i);
        m_phongShader->setUniform("m", m_transformation_list.at(i));
       // std::cout << glm::to_string(token_trans) << std::endl<<std::endl;
        CS123SceneMaterial token = m_material_list.at(i);
        token.cAmbient *= m_global.ka;
        token.cDiffuse *= m_global.kd;
        token.cSpecular*= m_global.ks;

        m_phongShader->applyMaterial(token);
        m_shape_list.at(i)->draw();
    }
}
