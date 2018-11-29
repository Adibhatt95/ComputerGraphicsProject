/***************************
 * File: vshader42.glsl:FIRE
 *   A simple vertex shader.
 *
 * - Vertex attributes (positions & colors) for all vertices are sent
 *   to the GPU via a vertex buffer object created in the OpenGL program.
 *
 * - This vertex shader uses the Model-View and Projection matrices passed
 *   on from the OpenGL program as uniform variables of type mat4.
 ***************************/

 #version 150  // YJC: Comment/un-comment this line to resolve compilation errors
                 //      due to different settings of the default GLSL version

in  vec3 vVelocity;
in  vec3 vColor;
uniform float Time;
out float Pos;
out vec4 color;

uniform mat4 model_view;
uniform mat4 projection;

void main() 
{
	float x = 0.0;
	float y = 0.1;
	float z = 0.0;
	x = x + 0.001 * vVelocity.x * Time;
	y = y + 0.001 * vVelocity.y * Time - 0.5 * 0.00000049 * Time * Time;
	z = z + 0.001 * vVelocity.z * Time;
	vec4 vPosition4 = vec4(x, y, z, 1.0);
	vec4 vColor4 = vec4(vColor.r, vColor.g, vColor.b, 1.0); 

    // YJC: Original, incorrect below:
    //      gl_Position = projection*model_view*vPosition/vPosition.w;


    gl_Position = projection * model_view * vPosition4;
	Pos = y;
    color = vColor4;
} 
