#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D sprite;
uniform vec3 spriteColor;
uniform vec2 player_pos;
in vec4 frag_position;
uniform int light;


void main()
{
    float dist = length(-vec4(player_pos,0.0f,1.0f) + frag_position);
    if(dist<=300.0f || light==0) {
        color =vec4(spriteColor, 1.0) * texture(sprite, TexCoords);
    } else {
        color = vec4(0.0f);
    }
}
