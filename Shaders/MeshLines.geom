#version 420 core
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

void main()
{
    vec2 screenSize = vec2(800, 600);
    float width = 50.0f;

    vec4 beg = gl_in[0].gl_Position; 
    vec4 end = gl_in[1].gl_Position; 

    beg.xy /= beg.w;
    end.xy /= end.w;

    vec2 normal = normalize(end.xy - beg.xy) /screenSize;
    normal = vec2(normal.y, -normal.x);

    gl_Position = beg;
    gl_Position.xy -= normal * width;
    gl_Position.xy *= gl_Position.w;
    EmitVertex();   

    gl_Position = beg;
    gl_Position.xy += normal * width;
    gl_Position.xy *= gl_Position.w;
    EmitVertex();  
    
    gl_Position = end;
    gl_Position.xy -= normal * width;
    gl_Position.xy *= gl_Position.w;
    EmitVertex();   

    gl_Position = end;
    gl_Position.xy += normal * width;
    gl_Position.xy *= gl_Position.w;
    EmitVertex();   
}