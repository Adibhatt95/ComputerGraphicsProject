// shadingMain.cpp : Defines the entry point for the console application.
//



// --------------------------------------------------------------------------
// Handout: rotate-cube-shading.cpp (Rotating Cube with shading)
//
// * Originally from Ed Angel's textbook "Interactive Computer Graphics" 6th Ed
//          sample code "example3.cpp" of Chapter 5.
// * Extensively modified by Yi-Jen Chiang for the program structure, 
//   normal matrix, user interface, etc. 
//  (See keyboard() and mouse() functions for user interactions.)
// * Display a rotating cube with shading.
//
// - Light and material properties & Normal Matrix are sent to the shader as 
//   uniform variables.
// - Entire shading computation is done in the Eye Frame (in shader).
// --------------------------------------------------------------------------  
#include "Angel-yjc.h"
#pragma warning(disable:4996);
typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
typedef Angel::vec3  color3;
typedef Angel::vec3  point3;
int size = 0;
int numTriangles;
bool flagError = false;

GLuint program;       /* shader program object id */
GLuint program2;
GLuint program3;
GLuint cube_buffer;   /* vertex buffer object id for cube */
GLuint floor_buffer;
GLuint axes_Buffer;
GLuint fireworks_Buffer;
GLuint selected_BufferSphere;
GLuint selected_BufferFloor;
GLuint cube_buffer_noShade;
GLuint floor_buffer_noShade;
GLuint shadow_Buffer;
GLuint flatOrSmooth_Buff;
GLuint flat_Buff;
GLuint smooth_Buff;
static GLuint texName[2];
					  // Projection transformation parameters
GLfloat  fovy = 45.0;  // Field-of-view in Y direction angle (in degrees)
GLfloat  aspect;       // Viewport aspect ratio
GLfloat  zNear = 0.5, zFar = 399.0;
vec4    at(0.0, 0.0, 0.0, 1.0);                 //VRP + VPN 
vec4    up(0.0, 1.0, 0.0, 0.0);
vec4 init_eye(7.0, 3.0, -10.0, 1.0); // initial viewer position
vec4 eye = init_eye;    // current viewer position

int time0 = 0;
int time;





GLfloat angle = 0.0; // rotation angle in degrees
GLfloat anglenoChange = 0.0;
GLfloat tempangle = 0.0;
GLfloat ABangle = 0.0;
GLfloat BCangle = 0.0;
GLfloat CAangle = 0.0;
GLfloat d = 0.0;
GLfloat tranx = -0.008122;
GLfloat tranz = -0.01827;
bool isTrue = false;
GLfloat trans[2];
GLfloat gettransx();
GLfloat gettransz();
GLfloat transxlter = -0.00008122;
GLfloat transzlter = -0.0001827;
GLfloat currX = 3.0;
GLfloat currZ = 5.0;
bool shadowOrNot = true;
bool notCalled = true;
GLfloat rotateDirX = -9.0;
GLfloat rotateDirZ = 4.0;
GLfloat temprotateDirX = -9.0;
GLfloat temprotateDirZ = 4.0;





#define ImageWidth  64
#define ImageHeight 64
GLubyte Image[ImageHeight][ImageWidth][4];

#define	stripeImageWidth 32
GLubyte stripeImage[4 * stripeImageWidth];




int mydirection = 1;
bool bButton = false;
int cubeFlag = 1;
int animationFlag = 1; // 1: animation; 0: non-animation. Toggled by key 'a' or 'A'
int shadOrNot = 1;
bool floorFlag = 1;
int spotlight = 1; //1 is no spotlight, 2 is spotlight
int fireWorksOrNot = 0;
int fog10yes20no11lin12exp13expsqu = 20;
int blendOrNot = 1;//0 for no
int textOrNot = 1;
int sphereTexture = 0;//0 if no sphere 1 if yes
int floorTexture = -1; //0 if no floor 2 if yes
int twoDor1D = 1;
int latYes10No20 = 20;//10 yes 20 no
int eye1OrObject0 = 0;//eye1OrObject0,Vertical1OrSlantedStrip2,Vertical1OrSlantedChecker2,Upright1OrTiltedLattice2
int Vertical1OrSlantedStrip2 = 1;
int Vertical1OrSlantedChecker2 = 1;
int Upright1OrTiltedLattice2 = 1;
int fogYes1No2 = 2;
int fireWorksInner = 1;




const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)
point4 points[5000];
vec3   normals[5000];
vec3 normalsflat[5000];
const int floor_NumVertices = 6; //(1 face)*(2 triangles/face)*(3 vertices/triangle)
point4 floor_points[6]; // positions for all vertices
point3 floor_points2[6];
color3 floor_colors[floor_NumVertices]; // colors for all vertices
vec3 floorNormals[6];

point3 sphere_Points[5000];
point3 sphere_Color[5000];
color3 shadow_Color[5000];
// Vertices of a unit cube centered at origin, sides aligned with axes
point3 vertices2[8] = {//0,4,4,7,3,3
	point3(5.0, 0.0,  -4.0),
	point3(-0.5,  0.5,  0.5),
	point3(0.5,  0.5,  0.5),
	point3(-5.0, 0.0,  -4.0),
	point3(5.0, 0.0, 8.0),
	point3(-0.5,  0.5, -0.5),
	point3(0.5,  0.5, -0.5),
	point3(-5.0, 0.0, 8.0)
};
point4 vertices[8] = {//0,4,4,7,3,3
	point4(5.0, 0.0,  -4.0, 1.0),//0b
	point4(-0.5,  0.5,  0.5, 1.0),
	point4(0.5,  0.5,  0.5, 1.0),
	point4(-5.0, 0.0, -4.0, 1.0),////3a
	point4(5.0, 0.0, 8.0, 1.0),////4cc
	point4(-0.5,  0.5, -0.5, 1.0),
	point4(0.5,  0.5, -0.5, 1.0),
	point4(-5.0, 0.0, 8.0, 1.0)//7d
};
vec2 quad_texCoord[6] = {
	vec2(1.5, 0.0),  // for a
	vec2(1.5, 1.25),  // for b
	vec2(0.0, 1.25),  // for c

	vec2(0.0, 1.25),  // for c
	vec2(0.0, 0.0),  // for d
	vec2(1.5, 0.0),  // for a 
};
color3 vertex_colors[1] = {
	//color3(0.0, 0.0, 0.0),  // black
	//color3(1.0, 0.0, 0.0),  // red
	//color3(1.0, 1.0, 0.0),  // yellow
	color3(0.0, 1.0, 0.0),  // green
							//color3(0.0, 0.0, 1.0),  // blue
							//color3(1.0, 0.0, 1.0),  // magenta
							//color3(1.0, 1.0, 1.0),  // white
							//color3(0.0, 1.0, 1.0)   // cyan
};
point3 g_vertex_buffer_data[] = {
	point3(0.0,  100.0, 0.0),
	point3(0.0,  0.0, 0.0),
	point3(100.0,  0.0, 0.0),
	point3(0.0,  0.0, 0.0),
	point3(0.0,  0.0, 100.0),
	point3(0.0,  0.0, 0.0)
};
color3 g_vertex_color_data[] = {
	color3(1.0,  0.0, 1.0),
	color3(1.0,  0.0, 1.0),
	color3(1.0,  0.0, 0.0),
	color3(1.0,  0.0, 0.0),
	color3(0.0,  0.0, 1.0),
	color3(0.0,  0.0, 1.0)
};

point3 fireWorksVelocity[400];
color3 fireWorksColors[400];

// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };

// Model-view and projection matrices uniform location
GLuint  ModelView, Projection, ShadowView;












/*----- Shader Lighting Parameters -----*/
color4 light_ambient(0.0,0.0,0.0, 1.0);//done
color4 global_ambient(1, 1, 1,1.0);//done
color4 light_diffuse(1,1,1, 1);//done
color4 light_specular(1,1,1, 1.0);//done
float const_att = 2.0;
float linear_att = 0.01;
float quad_att = 0.001;
point4 light_position(-14.0, 12.0, -3.0, 1.0);//done
point4 light_destination(-6.0, 0.0, -4.5, 1.0);//done

color4 light_ambientDis(0.0, 0.0, 0.0, 1.0);//done
//color4 global_ambientDis(1, 1, 1, 1.0);
color4 light_diffuseDis(0.8, 0.8, 0.8, 1.0);//done
color4 light_specularDis(0.2, 0.2, 0.2, 1.0);//done
// In World frame.
// Needs to transform it to Eye Frame
// before sending it to the shader(s).

const bool shadow = false;

color4 material_ambient(0.2, 0.2, 0.2, 1.0);//done
color4 material_diffuse(1.0, 0.84, 0.0, 1.0);//done
color4 material_specular(1.0, 0.84, 0.0, 1.0);//done
float  material_shininess = 125.0;

color4 global_ambientProduct = global_ambient*material_ambient;
color4 ambient_product = light_ambient * material_ambient;
color4 diffuse_product = light_diffuse * material_diffuse;
color4 specular_product = light_specular * material_specular;

color4 global_ambientProductDis = global_ambient*material_ambient;//this is not used in a different way
color4 ambient_productDis = light_ambientDis * material_ambient;
color4 diffuse_productDis = light_diffuseDis * material_diffuse;
color4 specular_productDis = light_specularDis * material_specular;


color4 fl_material_ambient(0.2, 0.2, 0.2, 1.0);//done
color4 fl_material_diffuse(0.0, 1.0, 0.0, 1.0);//done
color4 fl_material_specular(0.0, 0.0, 0.0, 1.0);//done
float  fl_material_shininess = 125.0;

color4 fl_global_ambientProduct = global_ambient*fl_material_ambient;
color4 fl_ambient_product = light_ambient * fl_material_ambient;
color4 fl_diffuse_product = light_diffuse * fl_material_diffuse;
color4 fl_specular_product = light_specular * fl_material_specular;

color4 fl_ambient_productDis = light_ambientDis * fl_material_ambient;
color4 fl_diffuse_productDis = light_diffuseDis * fl_material_diffuse;
color4 fl_specular_productDis = light_specularDis * fl_material_specular;

void SetUp_Lighting_Uniform_Vars(mat4 mv);

int Index = 0;







void getDataFromFile(char fileloc[200])
{
	fileloc = "D:\\MyProject\\OpenGlTestProj\\triangles.txt";
	float x, y, z;
	FILE *file;
	int numTriang;
	try
	{
		file = fopen(fileloc, "r");
		if (!file)
		{
			printf("\n\tInput File not found: \n \n");
			flagError = true;
			system("pause");
			exit(0);
		}
		else
		{
			fscanf(file, "%d", &numTriang);
			int index = 0;

			{

				int temp = fscanf(file, "%d", &temp);
				numTriangles = numTriang;
				int indexNormal = 0;
				vec4 u;
				vec4 v;
				vec4 w;
				for (int j = 0; j < numTriang; j++)//taking the input from file and adding into the arrays
				{
					for (int i = 0; i < 3; i++)
					{
						int num = fscanf(file, "%f %f %f", &x, &y, &z);
						if (i == 0)
						{
							u = point3(GLfloat(x), GLfloat(y), GLfloat(z));
							normals[indexNormal] = vec3(x, y, z);// normalize(cross(u, v));
						}
						if (i == 1)
						{
							v = point3(GLfloat(x), GLfloat(y), GLfloat(z));
							normals[indexNormal + 1] = vec3(x, y, z);// normalize(cross(u, v));
						}
						if (i == 2)
						{

							
							w = point3(GLfloat(x), GLfloat(y), GLfloat(z));
							vec4 U = v - u;
							vec4 V = w - u;
							vec3 normal = normalize(cross(U, V));
							//normals[indexNormal + 1] = normalize(cross(v,w));
							normals[indexNormal + 2] = vec3(x, y, z); //normalize(cross(w,u));
							normalsflat[indexNormal] = normal;
							normalsflat[indexNormal+1] = normal;
							normalsflat[indexNormal+2] = normal;
							indexNormal = indexNormal + 3;
						}
						points[index] = point4(x, y, z,1.0);
						sphere_Points[index] = point3(x, y, z);
						sphere_Color[index] = color3(1.0, 0.84, 0);
						shadow_Color[index] = color3(0.25, 0.25, 0.25);
						//sphere_Normals
						index++;
					}
					int temp = fscanf(file, "%d", &temp);
					//printf("%d", num);
				}

			}
		}
		if (!flagError) {
			printf("\n\tThe file has been read successfully. \n\n");
		}
	}
	catch (std::exception const& e)
	{
		printf("Please follow the format specified, the error is: ");
		std::cout << e.what() << std::endl;
		flagError = true;
		system("pause");
	}
}


//----------------------------------------------------------------------------
// quad() generates two triangles for each face and assigns normals
//        to the vertices
void quad(int a, int b, int c, int d)
{
	// Initialize temporary vectors along the quad's edges to
	//   compute its face normal 
	vec4 u = vertices[b] - vertices[a];
	vec4 v = vertices[d] - vertices[a];

	vec3 normal = normalize(cross(u, v));

	normals[Index] = normal; points[Index] = vertices[a]; Index++;
	normals[Index] = normal; points[Index] = vertices[b]; Index++;
	normals[Index] = normal; points[Index] = vertices[c]; Index++;
	normals[Index] = normal; points[Index] = vertices[a]; Index++;
	normals[Index] = normal; points[Index] = vertices[c]; Index++;
	normals[Index] = normal; points[Index] = vertices[d]; Index++;
}
//----------------------------------------------------------------------------
// colorcube() generates 6 quad faces (12 triangles): 36 vertices & 36 normals
void colorcube()
{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}
void floor()
{
	floor_colors[0] = vertex_colors[0]; floor_points[0] = vertices[3];
	floor_colors[1] = vertex_colors[0]; floor_points[1] = vertices[0];
	floor_colors[2] = vertex_colors[0]; floor_points[2] = vertices[4];

	floor_colors[3] = vertex_colors[0]; floor_points[3] = vertices[4];
	floor_colors[4] = vertex_colors[0]; floor_points[4] = vertices[7];
	floor_colors[5] = vertex_colors[0]; floor_points[5] = vertices[3];
	vec4 u = floor_points[2] - floor_points[0];
	vec4 v = floor_points[1] - floor_points[0];
	floorNormals[0] =  normalize(cross(u, v));//vec3(0, 1, 0);//
    u = floor_points[0] - floor_points[1];
    v = floor_points[2] - floor_points[1];
	floorNormals[1] = normalize(cross(u,v));//vec3(0, 1, 0);//  
	u = floor_points[1] - floor_points[2];
	v = floor_points[0] - floor_points[2];
	floorNormals[2] = normalize(cross(u,v));//vec3(0, 1, 0);// 
	u = floor_points[5] - floor_points[3];
	v = floor_points[4] - floor_points[3];
	floorNormals[3] = normalize(cross(u,v));//vec3(0, 1, 0);//  
	u = floor_points[3] - floor_points[4];
	v = floor_points[5] - floor_points[4];
	floorNormals[4] = normalize(cross(u,v));//vec3(0, 1, 0);// 
	u = floor_points[4] - floor_points[5];
	v = floor_points[3] - floor_points[5];
	floorNormals[5] = normalize(cross(u,v));//vec3(0, 1, 0);// 
}
void floor2()
{
	floor_colors[0] = vertex_colors[0]; floor_points2[0] = vertices2[3];
	floor_colors[1] = vertex_colors[0]; floor_points2[1] = vertices2[0];
	floor_colors[2] = vertex_colors[0]; floor_points2[2] = vertices2[4];

	floor_colors[3] = vertex_colors[0]; floor_points2[3] = vertices2[4];
	floor_colors[4] = vertex_colors[0]; floor_points2[4] = vertices2[7];
	floor_colors[5] = vertex_colors[0]; floor_points2[5] = vertices2[3];
	vec4 u = floor_points[2] - floor_points[0];
	vec4 v = floor_points[1] - floor_points[0];
	floorNormals[0] = normalize(cross(u, v));//vec3(0, 1, 0);//
	u = floor_points[0] - floor_points[1];
	v = floor_points[2] - floor_points[1];
	floorNormals[1] = normalize(cross(u, v));//vec3(0, 1, 0);//  
	u = floor_points[1] - floor_points[2];
	v = floor_points[0] - floor_points[2];
	floorNormals[2] = normalize(cross(u, v));//vec3(0, 1, 0);// 
	u = floor_points[5] - floor_points[3];
	v = floor_points[4] - floor_points[3];
	floorNormals[3] = normalize(cross(u, v));//vec3(0, 1, 0);//  
	u = floor_points[3] - floor_points[4];
	v = floor_points[5] - floor_points[4];
	floorNormals[4] = normalize(cross(u, v));//vec3(0, 1, 0);// 
	u = floor_points[4] - floor_points[5];
	v = floor_points[3] - floor_points[5];
	floorNormals[5] = normalize(cross(u, v));//vec3(0, 1, 0);// 
}
//----------------------------------------------------------------------------
// OpenGL initialization

GLfloat inRange(GLfloat x, float a, float b)
{
	if (x < a)
		return a;
	if (x > b)
		return b;
	return x;
}
void setUpFireWorkVariables()
{
	for (int i = 0; i < 300; i++)
	{
		GLfloat xVelocity = 2.0 * ((rand() % 256) / 256.0 - 0.5);
		xVelocity = inRange(xVelocity, -1.0, 1.0);
		GLfloat zVelocity = 2.0 * ((rand() % 256) / 256.0 - 0.5);
		zVelocity = inRange(zVelocity, -1.0, 1.0);
		GLfloat yVelocity = 1.2 * 2.0 * ((rand() % 256) / 256.0);
		yVelocity = inRange(yVelocity, 0.0, 2.4);
		fireWorksVelocity[i] = point3(xVelocity, yVelocity, zVelocity);
		GLfloat rColor = (rand() % 256) / 256.0;
		rColor = inRange(rColor, 0.0, 1.0);
		GLfloat gColor = (rand() % 256) / 256.0;
		gColor = inRange(gColor, 0.0, 1.0);
		GLfloat bColor = (rand() % 256) / 256.0;
		bColor = inRange(bColor, 0.0, 1.0);
		fireWorksColors[i] = color3(rColor, gColor, bColor);
	}
}
//------------------

void image_set_up(void)
{
	int i, j, c;

	/* --- Generate checkerboard image to the image array ---*/
	for (i = 0; i < ImageHeight; i++)
		for (j = 0; j < ImageWidth; j++)
		{
			c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0));

			if (c == 1) /* white */
			{
				c = 255;
				Image[i][j][0] = (GLubyte)c;
				Image[i][j][1] = (GLubyte)c;
				Image[i][j][2] = (GLubyte)c;
			}
			else  /* green */
			{
				Image[i][j][0] = (GLubyte)0;
				Image[i][j][1] = (GLubyte)150;
				Image[i][j][2] = (GLubyte)0;
			}

			Image[i][j][3] = (GLubyte)255;
		}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/*--- Generate 1D stripe image to array stripeImage[] ---*/
	for (j = 0; j < stripeImageWidth; j++) {
		/* When j <= 4, the color is (255, 0, 0),   i.e., red stripe/line.
		When j > 4,  the color is (255, 255, 0), i.e., yellow remaining texture
		*/
		stripeImage[4 * j] = (GLubyte)255;
		stripeImage[4 * j + 1] = (GLubyte)((j>4) ? 255 : 0);
		stripeImage[4 * j + 2] = (GLubyte)0;
		stripeImage[4 * j + 3] = (GLubyte)255;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	/*----------- End 1D stripe image ----------------*/

	/*--- texture mapping set-up is to be done in
	init() (set up texture objects),
	display() (activate the texture object to be used, etc.)
	and in shaders.
	---*/

} /* end function */

//-------------------------

void init()
{
	//colorcube();
	setUpFireWorkVariables();
	image_set_up();
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/*--- Create and Initialize a texture object ---*/
	glGenTextures(2, texName);      // Generate texture obj name(s)

	glActiveTexture(GL_TEXTURE0);  // Set the active texture unit to be 0 
	glBindTexture(GL_TEXTURE_2D, texName[0]); // Bind the texture to this texture unit

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ImageWidth, ImageHeight,
		0, GL_RGBA, GL_UNSIGNED_BYTE, Image);


	//glGenTextures(1, &texName);      // Generate texture obj name(s)

	glActiveTexture(GL_TEXTURE1);  // Set the active texture unit to be 0 
	glBindTexture(GL_TEXTURE_1D, texName[1]); // Bind the texture to this texture unit

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, stripeImageWidth,
		0, GL_RGBA, GL_UNSIGNED_BYTE, stripeImage);


	glGenBuffers(1, &cube_buffer_noShade);
	glBindBuffer(GL_ARRAY_BUFFER, cube_buffer_noShade);

#if 0
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_points) + sizeof(cube_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube_points), cube_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cube_points), sizeof(cube_colors),
		cube_colors);
#endif
#if 1
	glBufferData(GL_ARRAY_BUFFER, sizeof(point3)*numTriangles * 3 + sizeof(color3)*numTriangles * 3,
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		sizeof(point3) * numTriangles * 3, sphere_Points);
	glBufferSubData(GL_ARRAY_BUFFER,
		sizeof(point3) * numTriangles * 3,
		sizeof(color3) * numTriangles * 3,
		sphere_Color);
#endif


	floor2();
	// Create and initialize a vertex buffer object for floor, to be used in display()
	glGenBuffers(1, &floor_buffer_noShade);
	glBindBuffer(GL_ARRAY_BUFFER, floor_buffer_noShade);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor_points2) + sizeof(floor_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(floor_points2), floor_points2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(floor_points2), sizeof(floor_colors),
		floor_colors);


	// Create and initialize a vertex buffer object
	glGenBuffers(1, &cube_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, cube_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points),
		sizeof(normals), normals);
	selected_BufferSphere = cube_buffer;//smooth shading buffer
	
	glGenBuffers(1, &flat_Buff);
	glBindBuffer(GL_ARRAY_BUFFER, flat_Buff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(normalsflat),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points),
		sizeof(normalsflat), normalsflat);


	floor();
	// Create and initialize a vertex buffer object for floor, to be used in display()
	glGenBuffers(1, &floor_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, floor_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor_points) + sizeof(floorNormals) + sizeof(quad_texCoord),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(floor_points), floor_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(floor_points), sizeof(floorNormals),
		floorNormals);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(floor_points) + sizeof(floorNormals), sizeof(quad_texCoord),
		quad_texCoord);

	//--------------------------------AXES BUFFER------------------

	glGenBuffers(1, &axes_Buffer);
	glBindBuffer(GL_ARRAY_BUFFER, axes_Buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(point3) * 6 + sizeof(color3) * 6, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point3) * 6, g_vertex_buffer_data);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(point3) * 6, sizeof(color3) * 6, g_vertex_color_data);
	// -----------------------------------------------------------
	glGenBuffers(1, &fireworks_Buffer);
	glBindBuffer(GL_ARRAY_BUFFER, fireworks_Buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fireWorksVelocity) + sizeof(fireWorksColors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(fireWorksVelocity), fireWorksVelocity);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(fireWorksVelocity), sizeof(fireWorksColors),
		fireWorksColors);


	//--------------------------------------------------------------

	glGenBuffers(1, &shadow_Buffer);
	glBindBuffer(GL_ARRAY_BUFFER, shadow_Buffer);

#if 0
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_points) + sizeof(cube_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube_points), cube_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cube_points), sizeof(cube_colors),
		cube_colors);
#endif
#if 1
	glBufferData(GL_ARRAY_BUFFER, sizeof(point3)*numTriangles * 3 + sizeof(color3)*numTriangles * 3,
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		sizeof(point3) * numTriangles * 3, sphere_Points);
	glBufferSubData(GL_ARRAY_BUFFER,
		sizeof(point3) * numTriangles * 3,
		sizeof(color3) * numTriangles * 3,
		shadow_Color);
#endif


	// Load shaders and create a shader program (to be used in display())
	program = InitShader("vshader53.glsl", "fshader53.glsl");
	program2 = InitShader("vshader42.glsl", "fshader42.glsl");
	program3 = InitShader("vshaderfire.glsl", "fshaderFire.glsl");
	size = sizeof(floor_points);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.529, 0.807, 0.92, 0.0);
}



//----------------------------------------------------------------------
// SetUp_Lighting_Uniform_Vars(mat4 mv):
// Set up lighting parameters that are uniform variables in shader.
//
// Note: "LightPosition" in shader must be in the Eye Frame.
//       So we use parameter "mv", the model-view matrix, to transform
//       light_position to the Eye Frame.
//----------------------------------------------------------------------
void SetUp_Lighting_Uniform_Vars(mat4 mv)
{
	float o = 0.0;
	//glUniform1f(glGetUniformLocation(program, "shadowFlag"),
	//	1.0);
	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"),
		1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"),
		1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"),
		1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "GlobalAmbientProduct"),
		1, global_ambientProduct);
	glUniform1i(glGetUniformLocation(program, "shadowFlag"),
		1);
	glUniform1i(glGetUniformLocation(program, "spotlight"),
		spotlight);
	glUniform4fv(glGetUniformLocation(program, "AmbientProductDis"),
		1, ambient_productDis);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProductDis"),
		1, diffuse_productDis);
	glUniform4fv(glGetUniformLocation(program, "SpecularProductDis"),
		1, specular_productDis);
	// The Light Position in Eye Frame
	vec4 light_position_eyeFrame = mv * light_position;
	glUniform4fv(glGetUniformLocation(program, "LightPosition"),
		1, light_position_eyeFrame);
	vec4 light_destination_eyeFrame = mv * light_destination;
	glUniform4fv(glGetUniformLocation(program, "LightDestination"),
		1, light_destination_eyeFrame);
	//LightDestination

	glUniform1f(glGetUniformLocation(program, "ConstAtt"),
		const_att);
	glUniform1f(glGetUniformLocation(program, "LinearAtt"),
		linear_att);
	glUniform1f(glGetUniformLocation(program, "QuadAtt"),
		quad_att);

	glUniform1f(glGetUniformLocation(program, "Shininess"),
		material_shininess);
}


void fl_SetUp_Lighting_Uniform_Vars(mat4 mv)
{
	glUniform1i(glGetUniformLocation(program, "shadowFlag"),
	1);
	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"),
		1, fl_ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"),
		1, fl_diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"),
		1, fl_specular_product);
	glUniform4fv(glGetUniformLocation(program, "GlobalAmbientProduct"),
		1, fl_global_ambientProduct);
	glUniform1i(glGetUniformLocation(program, "spotlight"),
		spotlight);
	glUniform4fv(glGetUniformLocation(program, "AmbientProductDis"),
		1, fl_ambient_productDis);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProductDis"),
		1, fl_diffuse_productDis);
	glUniform4fv(glGetUniformLocation(program, "SpecularProductDis"),
		1, fl_specular_productDis);
	// The Light Position in Eye Frame
	vec4 light_position_eyeFrame = mv * light_position;
	//std::cout << light_position_eyeFrame << std::endl;
	glUniform4fv(glGetUniformLocation(program, "LightPosition"),
		1, light_position_eyeFrame);
	vec4 light_destination_eyeFrame = mv * light_destination;
	glUniform4fv(glGetUniformLocation(program, "LightDestination"),
		1, light_destination_eyeFrame);

	glUniform1f(glGetUniformLocation(program, "ConstAtt"),
		const_att);
	glUniform1f(glGetUniformLocation(program, "LinearAtt"),
		linear_att);
	glUniform1f(glGetUniformLocation(program, "QuadAtt"),
		quad_att);

	glUniform1f(glGetUniformLocation(program, "Shininess"),
		material_shininess);
}



GLfloat gettransx()//get x value to translate here basically d is found from angle using the formula mentioned above, then d is used to get tran value for X and Z
{
	GLfloat tran = 0.0;
	if (mydirection == 1 && (d * d < 97))//097 is distance between A and B 
	{
		tran = d * (-4) / (sqrt(97)); //(-0.4061);//-4 / sqrt(97)
		return tran;
	}
	if (mydirection == 1 && (d * d >= 97))
	{
		mydirection = 2;
		currX = -1;
		currZ = -4;
		tran = d * (4.5) / (sqrt(22.5)); //(0.9486);
										 //mv = mv * Rotate(angle, rotateDirX, 1, rotateDirZ);
		temprotateDirX = rotateDirX;
		temprotateDirZ = rotateDirZ;
		rotateDirX = 1.5;
		rotateDirZ = -4.5;
		tempangle = angle;
		ABangle = tempangle;

		isTrue = true;
		angle = 0.0;
		d = 0.0;
		return tran;
		//currX = -1;
		//currZ = -4;
		//d = 0.0;

	}
	if (mydirection == 2 && (d * d < 22.5))
	{

		tran = d * (4.5) / (sqrt(22.5)); //(0.9486); //4.5 / sqrt(22.5)
		return tran;
	}
	if (mydirection == 2 && (d*d >= 22.5))
	{
		mydirection = 3;
		currX = 3.5;
		currZ = -2.5;
		tran = d * (-0.5) / (sqrt(56.5));//(-0.06652); //-0.5 / sqrt(56.5)
										 //mv = mv * Rotate(angle, rotateDirX, 1, rotateDirZ);
		tempangle = angle;
		BCangle = tempangle;
		temprotateDirX = rotateDirX;
		temprotateDirZ = rotateDirZ;
		rotateDirX = 7.5;
		rotateDirZ = 0.5;
		isTrue = true;
		angle = 0.0;
		d = 0.0;
		return tran;
	}
	if (mydirection == 3 && (d*d < 56.5))
	{
		tran = d * (-0.5) / (sqrt(56.5));
		return tran;
	}
	if (mydirection == 3 && (d * d >= 56.5))
	{
		mydirection = 1;
		currX = 3;
		currZ = 5;
		tran = d * (-4) / (sqrt(97));//(-0.4061);//-4 / sqrt(97)
									 //mv = mv * Rotate(angle, rotateDirX, 1, rotateDirZ);
		tempangle = angle;
		CAangle = tempangle;
		temprotateDirX = rotateDirX;
		temprotateDirZ = rotateDirZ;
		rotateDirX = -9.0;
		rotateDirZ = 4.0;
		isTrue = true;
		angle = 0.0;
		d = 0.0;
		return tran;
	}
}
GLfloat gettransz()//get z value to translate
{
	GLfloat tran = 0.0;
	if (mydirection == 1 && (d * d <= 97))//097 is distance between A and B 
	{
		tran = d * (-0.91381);
		return tran;
	}
	else if (mydirection == 1 && (d * d > 97))
	{
		//mydirection = 2;
		tran = d * (1.5) / (sqrt(22.5)); //(0.3162);
										 //currZ = -4;
										 //angle = 0.0;
		return tran;
		//currX = -1;
		//currZ = -4;
		//d = 0.0;

	}
	if (mydirection == 2 && (d * d <= 22.5))
	{
		tran = d * (0.3162); //4.5 / sqrt(22.5)
		return tran;
	}
	if (mydirection == 2 && (d*d > 22.5))
	{
		//mydirection = 3;
		//currX = 3.5;
		//currZ = -2.5;
		tran = d * (0.99778); //7.5 / sqrt(56.5)
							  //angle = 0.0;
		return tran;
	}
	if (mydirection == 3 && (d*d <= 56.5))
	{
		tran = d * (0.99778); //GM
		return tran;
	}
	if (mydirection == 3 && (d * d > 56.5))
	{
		tran = d * (-0.91381);//-9.0/ sqrt(97)
							  //mv = mv * Rotate(angle, rotateDirX, 1, rotateDirZ);

		return tran;
	}
}

//-----------------------------------DRAW LINES AXES----------------

void drawAxes(GLuint axes_Buffer2)
{

	// Clear the screen
	//glClear(GL_COLOR_BUFFER_BIT);
	glUniform1i(glGetUniformLocation(program, "shadowFlag"),
		-2);//textorNot
	glUniform1i(glGetUniformLocation(program, "textorNot"),
		-2);
	glBindBuffer(GL_ARRAY_BUFFER, axes_Buffer2);

	// 1rst attribute buffer : vertices
	
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(point3) * 6));

	// Draw the lines !
	glDrawArrays(GL_LINES, 0, 6); // 2 indices for the 2 end points of 1 line

	glDisableVertexAttribArray(vPosition);
	glDisableVertexAttribArray(vColor);
	//fireworks_Buffer
}
void drawAxes2(GLuint axes_Buffer2)
{

	// Clear the screen
	//glClear(GL_COLOR_BUFFER_BIT);
	//glUniform1i(glGetUniformLocation(program, "shadowFlag"),
		//-2);

	glBindBuffer(GL_ARRAY_BUFFER, axes_Buffer2);

	// 1rst attribute buffer : vertices

	GLuint vPosition = glGetAttribLocation(program2, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program2, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(point3) * 6));

	// Draw the lines !
	glDrawArrays(GL_LINES, 0, 6); // 2 indices for the 2 end points of 1 line

	glDisableVertexAttribArray(vPosition);
	glDisableVertexAttribArray(vColor);

}


//------------------------------------FIREWORKS--------------------
void drawFireWorks()
{
	glPointSize(3.0);
	
	glBindBuffer(GL_ARRAY_BUFFER, fireworks_Buffer);

	// 1rst attribute buffer : vertices

	GLuint vPosition = glGetAttribLocation(program3, "vVelocity");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program3, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(300));

	glUniform1f(glGetUniformLocation(program3, "Time"),
		(time0 - time)%10000);
	// Draw the lines !
	glDrawArrays(GL_POINTS, 0, 300); // 2 indices for the 2 end points of 1 line
	
	glDisableVertexAttribArray(vPosition);
	glDisableVertexAttribArray(vColor);
	//std::cout << time0-time << std::endl;
	//if((time-time0) >= time0)
	time0 = (int)glutGet(GLUT_ELAPSED_TIME);
	if (time0 > 1000000);
	{
		//time0 = 0;
	}

}
//------------------------------------------------------------------



void drawObj(GLuint buffer, int num_vertices, int bufferOff, int shadeOrNot)
{
	if (shadeOrNot == 1)
	{
		//--- Activate the vertex buffer object to be drawn ---//
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glUniform1i(glGetUniformLocation(program, "texture_2D"), 0);
		glUniform1i(glGetUniformLocation(program, "texture_1D"), 1);
		glUniform1i(glGetUniformLocation(program, "twoDor1D"), twoDor1D);
		glUniform1i(glGetUniformLocation(program, "sphereOrFloor"), sphereTexture);
		glUniform1i(glGetUniformLocation(program, "textorNot"), textOrNot);
		glUniform1i(glGetUniformLocation(program, "latYes10No20"), latYes10No20);
		glUniform1i(glGetUniformLocation(program, "fog10yes20no11lin12exp13expsqu"), fog10yes20no11lin12exp13expsqu);
		glUniform1i(glGetUniformLocation(program, "eye1OrObject0"), eye1OrObject0);
		glUniform1i(glGetUniformLocation(program, "Vertical1OrSlantedStrip2"), Vertical1OrSlantedStrip2);
		glUniform1i(glGetUniformLocation(program, "Vertical1OrSlantedChecker2"), Vertical1OrSlantedChecker2);
		glUniform1i(glGetUniformLocation(program, "Upright1OrTiltedLattice2"), Upright1OrTiltedLattice2);
		//eye1OrObject0,Vertical1OrSlantedStrip2,Vertical1OrSlantedChecker2,Upright1OrTiltedLattice2
		/*----- Set up vertex attribute arrays for each vertex attribute -----*/
		GLuint vPosition = glGetAttribLocation(program, "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));
		//std::cout << num_vertices << std::endl;
		GLuint vNormal = glGetAttribLocation(program, "vNormal");
		glEnableVertexAttribArray(vNormal);
		glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(bufferOff));
		
		glUniform1f(glGetUniformLocation(program, "shadowFlag"),
			1.0);
		// the offset is the (total) size of the previous vertex attribute array(s)

		



		/* Draw a sequence of geometric objs (triangles) from the vertex buffer
		(using the attributes specified in each enabled vertex attribute array) */
		glDrawArrays(GL_TRIANGLES, 0, num_vertices);

		/*--- Disable each vertex attribute array being enabled ---*/
		glDisableVertexAttribArray(vPosition);
		glDisableVertexAttribArray(vNormal);
	}
	else if (shadeOrNot == 2)
	{
		//--- Activate the vertex buffer object to be drawn ---//
		glBindBuffer(GL_ARRAY_BUFFER, buffer);

		/*----- Set up vertex attribute arrays for each vertex attribute -----*/
		GLuint vPosition = glGetAttribLocation(program, "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));
		glUniform1f(glGetUniformLocation(program, "shadowFlag"),-2.0);
		
		GLuint vColor = glGetAttribLocation(program, "vColor");
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(sizeof(point3) * num_vertices));
		// the offset is the (total) size of the previous vertex attribute array(s)

		/* Draw a sequence of geometric objs (triangles) from the vertex buffer
		(using the attributes specified in each enabled vertex attribute array) */
		glDrawArrays(GL_TRIANGLES, 0, num_vertices);
		/*--- Disable each vertex attribute array being enabled ---*/
		glDisableVertexAttribArray(vPosition);
		glDisableVertexAttribArray(vColor);
		//glDisableVertexAttribArray(vNormal);
	}
}

void drawFloorText(GLuint buffer, int num_vertices, int bufferOff, int shadeOrNot)
{
	//--- Activate the vertex buffer object to be drawn ---//
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glUniform1f(glGetUniformLocation(program, "shadowFlag"), 1.0);
	glUniform1i(glGetUniformLocation(program, "textorNot"), textOrNot);
	glUniform1i(glGetUniformLocation(program, "fog10yes20no11lin12exp13expsqu"), fog10yes20no11lin12exp13expsqu);
	glUniform1i(glGetUniformLocation(program, "eye1OrObject0"), eye1OrObject0);
	glUniform1i(glGetUniformLocation(program, "Vertical1OrSlantedStrip2"), Vertical1OrSlantedStrip2);
	glUniform1i(glGetUniformLocation(program, "Vertical1OrSlantedChecker2"), Vertical1OrSlantedChecker2);
	glUniform1i(glGetUniformLocation(program, "Upright1OrTiltedLattice2"), Upright1OrTiltedLattice2);
	/*----- Set up vertex attribute arrays for each vertex attribute -----*/
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	//std::cout << num_vertices << std::endl;
	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(bufferOff));

	GLuint vTextCord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTextCord);
	glVertexAttribPointer(vTextCord, 2, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(bufferOff + sizeof(floorNormals)));

	glUniform1i(glGetUniformLocation(program, "texture_2D"), 0);
	glUniform1i(glGetUniformLocation(program, "texture_1D"), 1);
	glUniform1i(glGetUniformLocation(program, "sphereOrFloor"), floorTexture);
	
	
	// the offset is the (total) size of the previous vertex attribute array(s)





	/* Draw a sequence of geometric objs (triangles) from the vertex buffer
	(using the attributes specified in each enabled vertex attribute array) */
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);

	/*--- Disable each vertex attribute array being enabled ---*/
	glDisableVertexAttribArray(vPosition);
	glDisableVertexAttribArray(vNormal);
	glDisableVertexAttribArray(vTextCord);
}


void drawObj2(GLuint buffer, int num_vertices)
{
	//--- Activate the vertex buffer object to be drawn ---//
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glUniform1i(glGetUniformLocation(program, "shadowFlag"),
		-2);
	/*----- Set up vertex attribute arrays for each vertex attribute -----*/
	GLuint vPosition = glGetAttribLocation(program2, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	/*GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
	BUFFER_OFFSET(sizeof(point3) * num_vertices));*/
	GLuint vColor = glGetAttribLocation(program2, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(point3) * num_vertices));

	//glUniform1i(glGetUniformLocation(program, "shadowFlag"),
		//-2);
	// the offset is the (total) size of the previous vertex attribute array(s)

	/* Draw a sequence of geometric objs (triangles) from the vertex buffer
	(using the attributes specified in each enabled vertex attribute array) */
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);
	/*--- Disable each vertex attribute array being enabled ---*/
	glDisableVertexAttribArray(vPosition);
	glDisableVertexAttribArray(vColor);
	//glDisableVertexAttribArray(vNormal);


}

void drawShd(GLuint buffer, int num_vertices)
{
	//--- Activate the vertex buffer object to be drawn ---//
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glUniform1i(glGetUniformLocation(program, "latYes10No20"), latYes10No20);
	glUniform1i(glGetUniformLocation(program, "shadowFlag"),
		-1);
	glUniform1i(glGetUniformLocation(program, "textorNot"),
		-2);
	/*----- Set up vertex attribute arrays for each vertex attribute -----*/
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));
	
	glUniform1i(glGetUniformLocation(program, "eye1OrObject0"), eye1OrObject0);
	glUniform1i(glGetUniformLocation(program, "Vertical1OrSlantedStrip2"), Vertical1OrSlantedStrip2);
	glUniform1i(glGetUniformLocation(program, "Vertical1OrSlantedChecker2"), Vertical1OrSlantedChecker2);
	glUniform1i(glGetUniformLocation(program, "Upright1OrTiltedLattice2"), Upright1OrTiltedLattice2);
	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(point3) * num_vertices));
	// the offset is the (total) size of the previous vertex attribute array(s)

	/* Draw a sequence of geometric objs (triangles) from the vertex buffer
	(using the attributes specified in each enabled vertex attribute array) */
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);
	/*--- Disable each vertex attribute array being enabled ---*/
	glDisableVertexAttribArray(vPosition);
	glDisableVertexAttribArray(vColor);
	//glDisableVertexAttribArray(vNormal);


}
mat4  mv = mat4(GLfloat(1.0));
mat4  mv2 = mat4(GLfloat(1.0));
mat4  mv3 = mat4(GLfloat(1.0));

mat4 mvS = mat4(vec4(12,14,0,0),vec4(0,0,0,0),vec4(0,3,12,0),vec4(0,-1,0,12));
//mat4 mvS = mat4(GLfloat(10.0), GLfloat(0.0), GLfloat(0.0), GLfloat(0.0), GLfloat(7.0), GLfloat(0.0), GLfloat(1.0), GLfloat(1.0),
	//GLfloat(0.0), GLfloat(0.0), GLfloat(10.0), GLfloat(0.0), GLfloat(0.0), GLfloat(0.0), GLfloat(0.0), GLfloat(10.0));
int count = 0;
//----------------------------------------------------------------------------
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//shadOrNot = 1;
	if (shadOrNot == 1)
	{
		if (cubeFlag == 0)
		{
			//** Important: glUseProgram() must be called *before* any shader variable
			//              locations can be retrieved. This is needed to pass on values to
			//              uniform/attribute variables in shader ("variable binding" in 
			//              shader).
			glUseProgram(program2); // Use the shader program

			ModelView = glGetUniformLocation(program2, "model_view");
			//model_view2 = glGetUniformLocation(program, "model_view");
			Projection = glGetUniformLocation(program2, "projection");
			//glUniform1i(glGetUniformLocation(program2, "shadowFlag"),
			//-2);
			/*---  Set up and pass on Projection matrix to the shader ---*/
			mat4  p = Perspective(fovy, aspect, zNear, zFar);
			glUniformMatrix4fv(Projection, 1, GL_TRUE, p); // GL_TRUE: matrix is row-major

			mat4 mvInitial = LookAt(eye, at, up);
			//SetUp_Lighting_Uniform_Vars(mvInitial);
			if (isTrue)
			{
				mv2 = Rotate(tempangle, temprotateDirX, 1, temprotateDirZ) * mv2;
				isTrue = false;
			}
			//else
			{
				mv = mvInitial * Translate(currX + tranx, 1.0, currZ + tranz) * Rotate(angle, rotateDirX, 1, rotateDirZ) * Scale(1.0, 1.0, 1.0) * mv2;
				//mv3 = mv3 * Rotate(anglenoChange, rotateDirX, 1, rotateDirZ);
			}

			glUniformMatrix4fv(ModelView, 1, GL_TRUE, mv); // GL_TRUE: matrix is row-major
															//mat3 normal_matrix = NormalMatrix(mv, 1);
															//glUniformMatrix3fv(glGetUniformLocation(program, "Normal_Matrix"),
															//1, GL_TRUE, normal_matrix);

			if (cubeFlag == 1) // Filled cube
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			else              // Wireframe cube
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			drawObj2(cube_buffer_noShade, numTriangles * 3);  // draw the cube
		}
		glUseProgram(program);
		glUniform1i(glGetUniformLocation(program, "shadowFlag"),
			1);
		// Retrieve transformation uniform variable locations
		// ** Must be called *after* glUseProgram().
		ModelView = glGetUniformLocation(program, "ModelView");
		Projection = glGetUniformLocation(program, "Projection");
		ShadowView = glGetUniformLocation(program, "ShadowView");
		if (cubeFlag == 1)
		{
			

			/*---  Set up and pass on Projection matrix to the shader ---*/
			mat4 p = Perspective(fovy, aspect, zNear, zFar);
			glUniformMatrix4fv(Projection, 1, GL_TRUE, p); // GL_TRUE: matrix is row-major

														   // Generate the model-view matrix
			const vec3 viewer_pos(0.0, 0.0, 2.0);
			//const vec4 eye(3.0, 2.0, 0.0, 1.0);
			//vec4 at(0.0, 0.0, 0.0, 1.0);
			//vec4 up(0.0, 1.0, 0.0, 0.0);

			mat4 mvInitial = LookAt(eye, at, up); // model-view matrix using Correct LookAt()
												  // model-view matrix for the light position.

			
										   /*--- Set up lighting parameters that are uniform variables in shader ---*/
										   // ** Must be called *after* glUseProgram().
										   // ** Also, "LightPosition" in shader must be in the Eye Frame, so
										   //    we need to use model-view matrix to transform light_position to Eye Frame.
										   //    ==> Must be called *after* the model-view matrix mv for light position is 
										   //        set u
			if (shadOrNot == 1)
			{
				SetUp_Lighting_Uniform_Vars(mvInitial);
			}
			if (isTrue)
			{
				mv2 = Rotate(tempangle, temprotateDirX, 1, temprotateDirZ) * mv2;
				isTrue = false;
			}
			//else

			mat4 model_view = mvInitial * Translate(currX + tranx, 1.0, currZ + tranz) * Rotate(angle, rotateDirX, 1, rotateDirZ) * Scale(1.0, 1.0, 1.0) * mv2;

			//std::cout << mv << std::endl;
			glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);

			// Set up the Normal Matrix from the model-view matrix
			mat3 normal_matrix = NormalMatrix(model_view, 1);
			if (shadOrNot == 1)
			{

				// Flag in NormalMatrix(): 
				//    1: model_view involves non-uniform scaling
				//    0: otherwise.
				// Using 1 is always correct.  
				// But if no non-uniform scaling, 
				//     using 0 is faster (avoids matrix inverse computation).

				glUniformMatrix3fv(glGetUniformLocation(program, "Normal_Matrix"),
					1, GL_TRUE, normal_matrix);
			}
			if (cubeFlag == 1) // Filled cube
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			else              // Wireframe cube
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			//drawShd(shadow_Buffer, numTriangles * 3);
			//selected_BufferSphere = cube_buffer;
			drawObj(selected_BufferSphere, numTriangles * 3, sizeof(points), shadOrNot);  // draw the cube
		}
		mat3 normal_matrix = NormalMatrix(mv, 1);
		glDepthMask(GL_FALSE);
		mv = LookAt(eye, at, up); // model-view matrix using Correct LookAt()
								  // model-view matrix for the light position.

								  /*--- Set up lighting parameters that are uniform variables in shader ---*/
								  // ** Must be called *after* glUseProgram().
								  // ** Also, "LightPosition" in shader must be in the Eye Frame, so
								  //    we need to use model-view matrix to transform light_position to Eye Frame.
								  //    ==> Must be called *after* the model-view matrix mv for light position is 
								  //        set up.
		if (shadOrNot == 1)
		{
			fl_SetUp_Lighting_Uniform_Vars(mv);
		}
		mv = mv *  Translate(0.0, 0.0, 0.0) * Scale(1.0, 1.0, 1.0);

		glUniformMatrix4fv(ModelView, 1, GL_TRUE, mv); // GL_TRUE: matrix is row-major
		if (shadOrNot == 1)
		{
			normal_matrix = NormalMatrix(mv, 1);
			// Flag in NormalMatrix(): 
			//    1: model_view involves non-uniform scaling
			//    0: otherwise.
			// Using 1 is always correct.  
			// But if no non-uniform scaling, 
			//     using 0 is faster (avoids matrix inverse computation).

			glUniformMatrix3fv(glGetUniformLocation(program, "Normal_Matrix"),
				1, GL_TRUE, normal_matrix);
		}
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		selected_BufferFloor = floor_buffer;
		drawFloorText(floor_buffer, floor_NumVertices, sizeof(floor_points), shadOrNot);  // draw the floor
		drawAxes(axes_Buffer);
		if (blendOrNot == 0)
			glDepthMask(GL_TRUE);
		if(blendOrNot == 1)
			glEnable(GL_BLEND);
		//-----------------------============================================ SHADOW========================
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		if (eye[1] >= 0 && shadowOrNot == true)
		{
			mv = LookAt(eye, at, up) * mvS * Translate(currX + tranx, 1.0, currZ + tranz) * Rotate(angle, rotateDirX, 1, rotateDirZ) * Scale(1.0, 1.0, 1.0) * mv2;

			glUniformMatrix4fv(ModelView, 1, GL_TRUE, mv); // GL_TRUE: matrix is row-major
			if (cubeFlag == 1) // Filled cube
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			else              // Wireframe cube
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			drawShd(shadow_Buffer, numTriangles * 3);
		}
		glUniformMatrix4fv(ShadowView, 1, GL_TRUE, mvS * Translate(currX + tranx, 1.0, currZ + tranz) * Rotate(angle, rotateDirX, 1, rotateDirZ) * Scale(1.0, 1.0, 1.0) * mv2);
		//---------------==============================================================================================
		if (blendOrNot == 1)
			glDisable(GL_BLEND);
		if (blendOrNot == 1)
			glDepthMask(GL_TRUE);

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		mv = LookAt(eye, at, up); // model-view matrix using Correct LookAt()
									   // model-view matrix for the light position.

									   /*--- Set up lighting parameters that are uniform variables in shader ---*/
									   // ** Must be called *after* glUseProgram().
									   // ** Also, "LightPosition" in shader must be in the Eye Frame, so
									   //    we need to use model-view matrix to transform light_position to Eye Frame.
									   //    ==> Must be called *after* the model-view matrix mv for light position is 
									   //        set up.
		if (shadOrNot == 1)
		{
		//	fl_SetUp_Lighting_Uniform_Vars(mv);
		}
		mv = mv *  Translate(0.0, 0.0, 0.0) * Scale(1.0, 1.0, 1.0);

		glUniformMatrix4fv(ModelView, 1, GL_TRUE, mv); // GL_TRUE: matrix is row-major
		if (shadOrNot == 1)
		{
			normal_matrix = NormalMatrix(mv, 1);
			// Flag in NormalMatrix(): 
			//    1: model_view involves non-uniform scaling
			//    0: otherwise.
			// Using 1 is always correct.  
			// But if no non-uniform scaling, 
			//     using 0 is faster (avoids matrix inverse computation).

			glUniformMatrix3fv(glGetUniformLocation(program, "Normal_Matrix"),
				1, GL_TRUE, normal_matrix);
		}
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		drawFloorText(floor_buffer, floor_NumVertices, sizeof(floor_points), shadOrNot);  // draw the floor
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		count++;
		if (count > 1000)
		{
			//shadOrNot = 2;
			count = 0;
		}
		
	}
	else if (shadOrNot == 2)
	{
		GLuint  model_view;  // model-view matrix uniform shader variable location
		GLuint  projection;  // projection matrix uniform shader variable location
		GLuint  model_view2;
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(program2); // Use the shader program

		model_view = glGetUniformLocation(program2, "model_view");
		//model_view2 = glGetUniformLocation(program, "model_view");
		projection = glGetUniformLocation(program2, "projection");
		//glUniform1i(glGetUniformLocation(program2, "shadowFlag"),
			//-2);
		/*---  Set up and pass on Projection matrix to the shader ---*/
		mat4  p = Perspective(fovy, aspect, zNear, zFar);
		glUniformMatrix4fv(projection, 1, GL_TRUE, p); // GL_TRUE: matrix is row-major

		mat4 mvInitial = LookAt(eye, at, up);
		//SetUp_Lighting_Uniform_Vars(mvInitial);
		if (isTrue)
		{
			mv2 = Rotate(tempangle, temprotateDirX, 1, temprotateDirZ) * mv2;
			isTrue = false;
		}
		//else
		{
			mv = mvInitial * Translate(currX + tranx, 1.0, currZ + tranz) * Rotate(angle, rotateDirX, 1, rotateDirZ) * Scale(1.0, 1.0, 1.0) * mv2;
			//mv3 = mv3 * Rotate(anglenoChange, rotateDirX, 1, rotateDirZ);
		}
		
		glUniformMatrix4fv(model_view, 1, GL_TRUE, mv); // GL_TRUE: matrix is row-major
														//mat3 normal_matrix = NormalMatrix(mv, 1);
														//glUniformMatrix3fv(glGetUniformLocation(program, "Normal_Matrix"),
														//1, GL_TRUE, normal_matrix);
	
		if (cubeFlag == 1) // Filled cube
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else              // Wireframe cube
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		drawObj2(cube_buffer_noShade, numTriangles * 3);  // draw the cube

		glDepthMask(GL_FALSE);

		mv = LookAt(eye, at, up) * Translate(0.0, 0.0, 0.0) * Scale(1.0, 1.0, 1.0);
	
		glUniformMatrix4fv(model_view, 1, GL_TRUE, mv); // GL_TRUE: matrix is row-major
		
		if (floorFlag == 1) // Filled floor
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else              // Wireframe floor
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		drawObj2(floor_buffer_noShade, floor_NumVertices);  // draw the floor

		
		//blendOrNot = 0;
		if (blendOrNot == 0)
			glDepthMask(GL_TRUE);
		if (blendOrNot == 1)
			glEnable(GL_BLEND);
		//-----------------------============================================ SHADOW========================
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		if (eye[1] >= 0 && shadowOrNot == true)
		{
			mv = LookAt(eye, at, up) * mvS * Translate(currX + tranx, 1.0, currZ + tranz) * Rotate(angle, rotateDirX, 1, rotateDirZ) * Scale(1.0, 1.0, 1.0) * mv2;

			glUniformMatrix4fv(model_view, 1, GL_TRUE, mv); // GL_TRUE: matrix is row-major
			if (cubeFlag == 1) // Filled cube
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			else              // Wireframe cube
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			drawObj2(shadow_Buffer, numTriangles * 3);
		}
		// draw the shadow
		//std::cout << mv << std::endl;
		//------------------------------------------------------
		//---------------==============================================================================================
		if (blendOrNot == 1)
			glDisable(GL_BLEND);
		if (blendOrNot == 1)
			glDepthMask(GL_TRUE);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		mv = LookAt(eye, at, up) * Translate(0.0, 0.0, 0.0) * Scale(1.0, 1.0, 1.0);

		glUniformMatrix4fv(model_view, 1, GL_TRUE, mv); // GL_TRUE: matrix is row-major
		if (floorFlag == 1) // Filled floor
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else              // Wireframe floor
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		drawObj2(floor_buffer_noShade, floor_NumVertices);  // draw the floor

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		drawAxes2(axes_Buffer);
		//glutSwapBuffers();
		count++;
		if (count > 1000)
		{
			//shadOrNot = 1;
			count = 0;
		}
	}
	
	if (fireWorksOrNot == 1)
	{
		if (fireWorksInner == 1 && bButton)
		{
			time = (int)glutGet(GLUT_ELAPSED_TIME);
			time0 = time;
			fireWorksInner = 2;
		}
		glUseProgram(program3); // Use the shader program

		ModelView = glGetUniformLocation(program3, "model_view");
		//model_view2 = glGetUniformLocation(program, "model_view");
		Projection = glGetUniformLocation(program3, "projection");
		//glUniform1i(glGetUniformLocation(program2, "shadowFlag"),
		//-2);
		/*---  Set up and pass on Projection matrix to the shader ---*/
		mat4  p = Perspective(fovy, aspect, zNear, zFar);
		glUniformMatrix4fv(Projection, 1, GL_TRUE, p); // GL_TRUE: matrix is row-major

		mat4 mv = LookAt(eye, at, up);
		glUniformMatrix4fv(ModelView, 1, GL_TRUE, mv);

		glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		drawFireWorks();
	}

	glutSwapBuffers();
	
}
//----------------------------------------------------------------------------
void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		switch (button) {
		case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
		case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
		case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
		}
	}
}
//----------------------------------------------------------------------------
void idle(void)
{
	//time = (float)glutGet(GLUT_ELAPSED_TIME);
	// Theta[Axis] += 0.01;
	tranx = gettransx();
	tranz = gettransz();
	d = angle * 2 * 3.14159 / 360;
	angle += 0.09;
	anglenoChange += 0.02;//not needed
						  //transxlter;//0.0004;
						  //tranz = //transzlter;//0.0009;
						  //angle += 1.0;    //YJC: change this value to adjust the cube rotation speed.
	glutPostRedisplay();
}
//----------------------------------------------------------------------------
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033: // Escape Key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;

	case 'X': eye[0] += 1.0; break;
	case 'x': eye[0] -= 1.0; break;
	case 'Y': eye[1] += 1.0; break;
	case 'y': eye[1] -= 1.0; break;
	case 'Z': eye[2] += 1.0; break;
	case 'z': eye[2] -= 1.0; break;

	case 'b': case 'B': // Toggle between animation and non-animation
						//animationFlag = 1 - animationFlag;
		bButton = true;
		if (animationFlag == 1) glutIdleFunc(idle);
		else                    glutIdleFunc(NULL);
		break;

	case 'c': case 'C': // Toggle between filled and wireframe cube
		cubeFlag = 1 - cubeFlag;
		break;

	case 'f': case 'F': // Toggle between filled and wireframe floor
		floorFlag = 1 - floorFlag;
		break;
	case 'l': case 'L':
		if (latYes10No20 == 10)
			latYes10No20 = 20;
		else
			latYes10No20 = 10;
		break;
	case 'u': case 'U':
			Upright1OrTiltedLattice2 = 1;
		break;
	case 'v': case 'V':
			Vertical1OrSlantedStrip2 = 1;
			Vertical1OrSlantedChecker2 = 1;
		break;
	case 's': case 'S':
		Vertical1OrSlantedStrip2 = 2;
		Vertical1OrSlantedChecker2 = 2;
		break;
	case 't': case 'T':
		Upright1OrTiltedLattice2 = 2;
		break;
	case 'o': case 'O':
		if (eye1OrObject0 == 1)
			eye1OrObject0 = 0;
		else
			eye1OrObject0 = 1;
		break;
	case ' ':  // reset to initial viewer/eye position
		eye = init_eye;
		break;
	}
	glutPostRedisplay();
}
//----------------------------------------------------------------------------
void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	aspect = (GLfloat)width / (GLfloat)height;
	glutPostRedisplay();
}

void OnMouseClick(int button, int state, int x, int y)
{
	/*if (notCalled)
	{
	notCalled = false;
	glutCreateMenu(menu);
	// Add menu items
	glutAddMenuEntry("Default View Point", 1);
	glutAddMenuEntry("Quit", 2);
	// Associate a mouse button with menu
	glutAttachMenu(GLUT_LEFT_BUTTON);

	}*/

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		if (bButton)
		{
			fireWorksInner = 1;
			animationFlag = 1 - animationFlag;
			if (animationFlag == 1) glutIdleFunc(idle);
			else                    glutIdleFunc(NULL);
		}
	}
	//glutPostRedisplay();
}

void menu(int item)
{
	switch (item)
	{
	case 1: eye = init_eye;
		break;
	case 3: cubeFlag = 0;
		break;
	case 4: exit(0);
		break;
	default:
	{       /* Nothing */       }
	break;
	}

	glutPostRedisplay();

	return;
}
void shadowmenu(int item)
{
	switch (item)
	{
	case 1: shadowOrNot = true;
		break;
	case 2: shadowOrNot = false;
		break;
	default:
	{       /* Nothing */       }
	break;
	}

	glutPostRedisplay();

	return;
}
void enablelight(int item)
{
	switch (item)
	{
	case 1: shadOrNot = 1;
		break;
	case 2: shadOrNot = 2;
		break;
	default:
	{       /* Nothing */       }
	break;
	}

	glutPostRedisplay();

	return;
}
void shading(int item)
{
	switch (item)
	{
	case 1: selected_BufferSphere = cube_buffer;
		break;
	case 2: selected_BufferSphere = flat_Buff;
		break;
	default:
	{       /* Nothing */       }
	break;
	}

	glutPostRedisplay();

	return;
}
void lightsource(int item)
{
	switch (item)
	{
	case 1: spotlight = 1;
		break;
	case 2: spotlight = 2;
		break;
	default:
	{       /* Nothing */       }
	break;
	}

	glutPostRedisplay();

	return;
}
void fireworks(int item) {
	switch (item)
	{
	case 1: fireWorksOrNot = 1;
			fireWorksInner = 1;
		break;
	case 2: fireWorksOrNot = 2;
		break;
	default:
	{       /* Nothing */       }
	break;
	}

	glutPostRedisplay();

	return;
}
void blendingshadow(int item) {
	switch (item)
	{
	case 1: blendOrNot = 1;
		break;
	case 2: blendOrNot = 0;
		break;
	default:
	{       /* Nothing */       }
	break;
	}

	glutPostRedisplay();

	return;
}
void fogoptions(int item) {
	switch (item)
	{
	case 1: fog10yes20no11lin12exp13expsqu = 20;
		break;
	case 2: fog10yes20no11lin12exp13expsqu = 11;
		break;
	case 3: fog10yes20no11lin12exp13expsqu = 12;
		break;
	case 4: fog10yes20no11lin12exp13expsqu = 13;
		break;
	default:
	{       /* Nothing */       }
	break;
	}

	glutPostRedisplay();

	return;
}
void texturemappedground(int item)
{
	switch (item)
	{
	case 1: floorTexture = 2;
		textOrNot = 1;
		break;
	case 2: floorTexture = -1;
		textOrNot = 1;
		break;
	default:
	{       /* Nothing */       }
	break;
	}//hello

	glutPostRedisplay();

	return;
}
void spheretextoptions(int item) {
	switch (item)
	{
	case 1: textOrNot = 1;
		sphereTexture = 0;
		break;
	case 2:  sphereTexture = 1;
		twoDor1D = 1;
		break;
	case 3: sphereTexture = 1;
		twoDor1D = 2;
		break;
	default:
	{       /* Nothing */       }
	break;
	}

	glutPostRedisplay();

	return;
}


//----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	int err;
	char filename[200];
	std::cout << "ADITYA BHATT ASSIGNMENT 3 APB462 enter file" << std::endl;
	std::cin >> filename;
	//---- part a: get contents of file and store
	getDataFromFile(filename);
	//-----
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	// glutInitContextVersion( 3, 2 );
	// glutInitContextProfile( GLUT_CORE_PROFILE );
	glutCreateWindow("Rotating Cube with Shading");

	/* Call glewInit() and error checking */
	err = glewInit();
	if (GLEW_OK != err)
	{
		printf("Error: glewInit failed: %s\n", (char*)glewGetErrorString(err));
		exit(1);
	}
	int shadowMenu = glutCreateMenu(shadowmenu);
	glutAddMenuEntry("Yes", 1);
	glutAddMenuEntry("No", 2);

	int enableLight = glutCreateMenu(enablelight);
	glutAddMenuEntry("Yes", 1);
	glutAddMenuEntry("No", 2);

	int shadingMenu = glutCreateMenu(shading);
	glutAddMenuEntry("Smooth Shading", 1);
	glutAddMenuEntry("Flat Shading", 2);

	int lightSource = glutCreateMenu(lightsource);
	glutAddMenuEntry("Point Source", 1);
	glutAddMenuEntry("Spotlight", 2);

	int fireWorks = glutCreateMenu(fireworks);
	glutAddMenuEntry("Yes", 1);
	glutAddMenuEntry("No", 2);

	int blendingShadow = glutCreateMenu(blendingshadow);
	glutAddMenuEntry("Yes", 1);
	glutAddMenuEntry("No", 2);

	int textureMappedGround = glutCreateMenu(texturemappedground);
	glutAddMenuEntry("Yes", 1);
	glutAddMenuEntry("No", 2);

	int fogOptions = glutCreateMenu(fogoptions);
	glutAddMenuEntry("No Fog", 1);
	glutAddMenuEntry("Linear Fog", 2);
	glutAddMenuEntry("Exponential Fog", 3);
	glutAddMenuEntry("Exponential Square Fog", 4);

	int sphereTextOptions = glutCreateMenu(spheretextoptions);
	glutAddMenuEntry("No ", 1);
	glutAddMenuEntry("Yes-contour lines", 2);
	glutAddMenuEntry("Yes-checkerboard", 3);
	

	glutCreateMenu(menu);
	// Add menu items
	glutAddMenuEntry("Default View Point", 1);
	glutAddSubMenu("Shadow", shadowMenu);
	glutAddSubMenu("Enable Lighting?", enableLight);
	glutAddSubMenu("Shading", shadingMenu);
	glutAddSubMenu("Light Source?", lightSource);
	glutAddSubMenu("Fireworks?", fireWorks);
	glutAddSubMenu("Blend the Shadow?", blendingShadow);
	glutAddSubMenu("Fog Options", fogOptions);
	glutAddSubMenu("Texture Mapped Ground?", textureMappedGround);
	glutAddSubMenu("Tecture Mapped Sphere", sphereTextOptions);
	glutAddMenuEntry("WireFrame Sphere", 3);
	glutAddMenuEntry("Quit", 4);
	// Associate a mouse button with menu
	glutAttachMenu(GLUT_LEFT_BUTTON);


	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(NULL);

	glutKeyboardFunc(keyboard);
	glutMouseFunc(OnMouseClick);

	init();
	glutMainLoop();
	system("pause");
	return 0;
}


