#version 450 core
out vec4 color;
in vec3 normal;
in vec2 tex;

in vec3 FragPos;

struct Materials{
    int map_difusion;
    int map_normals;
    int map_reflection;
    int map_height;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

in flat int i_;

layout(std430, binding = 3) buffer dataMaterial{
   Materials material[];
};

struct Light {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform vec3 viewPos;
void main(){

//    vec3 ambient = light.ambient * material[i_].ambient;

    // diffuse
//    vec3 norm = normalize(normal);

//    vec3 lightDir = normalize(-light.direction);
//    float diff = max(dot(norm, lightDir), 0.0);
//    vec3 diffuse = light.diffuse * (diff * material[i_].diffuse);

    // specular
//    vec3 viewDir = normalize(viewPos - FragPos);
//    vec3 reflectDir = reflect(-lightDir, norm);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material[i_].shininess);
//    vec3 specular = light.specular * (spec * material[i_].specular);

//    vec3 result = ambient + diffuse + specular;

//    color = vec4(result, 1.0);

      color = vec4(material[i_].diffuse, 1.0);

};
