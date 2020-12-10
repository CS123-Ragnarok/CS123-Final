#version 400 core


layout(location = 0) in vec3 position;

out vec3 color; // Computed color for this vertex

// Transformation matrices
uniform mat4 p;
uniform mat4 v;
uniform mat4 m;

void main(){

    vec4 position_cameraSpace = v * m * vec4(position, 1.0);
     vec4 position_worldSpace = m * vec4(position, 1.0);

     gl_Position = p * position_cameraSpace;

}
