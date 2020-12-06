#include "Cylinder.h"
#include <math.h>
#include <iostream>

Cylinder::Cylinder(){

}

Cylinder::Cylinder(int p1, int p2):
    mesh_size(p1),
    side_size(p2)
{

    if(p2 < 3)
        side_size = 3;
    create_vertex_data();
    buildVAO();

}

Cylinder::~Cylinder(){
}

void Cylinder::create_vertex_data(){

    create_cycle(0.5, true);
    create_cycle(-0.5, false);
    float side_space = (2*M_PI)/(float)side_size;
    float x = 0.5, y = 0.5, z = 0;
    float barrel_mesh_size = 1/(float)mesh_size;
    for(int i = 1; i <= side_size; i++)
    {
        float old_x = x;
        float old_z = z;
        x = cos(i * side_space) * 0.5;
        z = sin(i * side_space) * 0.5;
        y = 0.5;
        glm:: vec3 normal_ = {(old_x+x)/2, 0, (old_z+z)/2};
        normal_ = glm::normalize(normal_);
      //  glm:: vec3 normal_ = {0,0, 1};
        for(int j = 0; j < mesh_size; j++)
        {
            update_m_vtx(old_x, y - (j * barrel_mesh_size), old_z, normal_);

            update_m_vtx(x, y - (j+1) * barrel_mesh_size, z, normal_);

            update_m_vtx(old_x,y - ((j+1) * barrel_mesh_size), old_z, normal_);

            //second triangle
            update_m_vtx(old_x, y - (j * barrel_mesh_size), old_z, normal_);

            update_m_vtx(x, y - (j * barrel_mesh_size), z, normal_);

            update_m_vtx(x, y - (j+1) * barrel_mesh_size, z, normal_);

        }

    }

}


void Cylinder::create_cycle(float y_axis, bool face_top)
{
    float side_space = (2*M_PI)/(float)side_size;
    float x = 0.5, y = y_axis, z = 0;
    std::vector<float> normal;
    if(face_top)
        normal = {0, 1, 0};
    //    normal = {0, 0, 1};
    else
        normal = {0, -1, 0};
     //    normal = {0, 0, 1};
    for(int i = 1; i <= side_size; i++)
    {
        float old_x = x;
        float old_z = z;
        float old_x_spc = old_x/(float)mesh_size;
        float old_z_spc = old_z/(float)mesh_size;

        x = cos(i * side_space) * 0.5;
        z = sin(i * side_space) * 0.5;
        float new_x_spc = x/(float)mesh_size;
        float new_z_spc = z/(float)mesh_size;

        if(face_top){
            update_m_vtx(old_x_spc, y, old_z_spc, normal);
            update_m_vtx(0, y, 0, normal);
            update_m_vtx(new_x_spc, y, new_z_spc, normal);
        }
        else
        {
            update_m_vtx(0, y, 0, normal);
            update_m_vtx(old_x_spc, y, old_z_spc, normal);
            update_m_vtx(new_x_spc, y, new_z_spc, normal);
        }


        for(int j = 1; j < mesh_size; j++)
        {

            //first triangle
            if(face_top){
            update_m_vtx(j*old_x_spc, y, j*old_z_spc, normal);

            update_m_vtx((j+1)*new_x_spc, y, (j+1)*new_z_spc, normal);

            update_m_vtx((j+1)*old_x_spc, y, (j+1)*old_z_spc, normal);

            //second triangle
            update_m_vtx(j*old_x_spc, y, j*old_z_spc, normal);

            update_m_vtx(j*new_x_spc, y, j*new_z_spc, normal);

            update_m_vtx((j+1)*new_x_spc, y, (j+1)*new_z_spc, normal);
            }
            else{
                update_m_vtx(j * old_x_spc, y, j * old_z_spc, normal);

                update_m_vtx((j+1) * old_x_spc, y, (j+1) * old_z_spc, normal);

                update_m_vtx((j+1) * new_x_spc, y, (j+1) * new_z_spc, normal);

                //second triangle
                update_m_vtx(j * old_x_spc, y, j * old_z_spc, normal);

                update_m_vtx((j+1) * new_x_spc, y, (j+1) * new_z_spc, normal);

                 update_m_vtx(j * new_x_spc, y, j * new_z_spc, normal);
            }


        }

    }

}
