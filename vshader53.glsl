/* 
File Name: "vshader53.glsl":
Vertex shader:
  - Per vertex shading for a single point light source;
    distance attenuation is Yet To Be Completed.
  - Entire shading computation is done in the Eye Frame.
*/

// #version 150  // YJC: Comment/un-comment this line to resolve compilation errors
                 //      due to different settings of the default GLSL version

in  vec4 vPosition;
in  vec2 vTexCoord;
in  vec3 vNormal;
in  vec3 vColor;

out float zfog;
out float strip;
out vec2 texCoord;
out vec4 color;
out float x;
out float xtry;
out float ztry;
out vec2 sphereCoord;
out vec2 latticeCoord;
uniform int shadowFlag;
uniform int spotlight;
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct, GlobalAmbientProduct;
uniform vec4 AmbientProductDis, DiffuseProductDis, SpecularProductDis;
uniform mat4 ModelView;
uniform mat4 ShadowView;
uniform mat4 Projection;
uniform mat3 Normal_Matrix;
uniform vec4 LightPosition;   // Must be in Eye Frame
uniform float Shininess;
uniform vec4 LightDestination;
uniform int eye1OrObject0;
uniform int Vertical1OrSlantedStrip2;
uniform int Vertical1OrSlantedChecker2;
uniform int Upright1OrTiltedLattice2;
vec4 dist_light_direction = vec4(0.1, 0.0, -1.0, 0.0);
vec4 spotlight_point = vec4(-6.0,0.0,-4.5,1.0);

uniform float ConstAtt;  // Constant Attenuation
uniform float LinearAtt; // Linear Attenuation
uniform float QuadAtt;   // Quadratic Attenuation

void main()
{
	texCoord = vTexCoord;
	//s = 2.5*vPosition.x;
	vec3 pos = (ModelView * vPosition).xyz;
	vec3 trying = (ShadowView * vPosition).xyz;
	ztry = trying.z;
	xtry = trying.x;
	zfog = length(pos);
	vec3 temp;
	if(eye1OrObject0 == 1)
	{
		temp = pos;
	}
	else
	{
		temp = vPosition.xyz; 
	}
   if(shadowFlag > 0){
				// Transform vertex  position into eye coordinates
				
				
				vec3 L = normalize( LightPosition.xyz - pos );
				vec3 E = normalize( -pos );
				vec3 H = normalize( L + E );

				// Transform vertex normal into eye coordinates
				  // vec3 N = normalize( ModelView*vec4(vNormal, 0.0) ).xyz;
				vec3 N = normalize(Normal_Matrix * vNormal);

			/*--- To Do: Compute attenuation ---*/
			float attenuation = 1.0; 
			float distance = length( LightPosition.xyz - pos );
			 attenuation = 1.0 / (ConstAtt + LinearAtt * distance + QuadAtt * distance * distance);
			 // Compute terms in the illumination equation
				vec4 ambient = AmbientProduct;

				float d = max( dot(L, N), 0.0 );
				vec4  diffuse = d * DiffuseProduct;

				float s = pow( max(dot(N, H), 0.0), Shininess );
				vec4  specular = s * SpecularProduct;
    
				if( dot(L, N) < 0.0 ) {
				specular = vec4(0.0, 0.0, 0.0, 1.0);
				} 

				vec4 color1 = attenuation * (ambient + diffuse + specular);


				//color2---------------------------------------------------
				vec3 pos2 = vec3(0.1,0.0,-0.1);
					L = normalize(-pos);
					vec3 LDist = normalize( -dist_light_direction.xyz );
				vec3 HDist = normalize( LDist + E );
				/*--- To Do: Compute attenuation ---*/
				attenuation = 1.0; 

				// Compute terms in the illumination equation
					vec4 ambient2 = AmbientProductDis;

					d = max( dot(LDist, N), 0.0 );
					vec4 diffuse2 = d * DiffuseProductDis;

					s = pow( max(dot(N, HDist), 0.0), Shininess );
				vec4 specular2 = s * SpecularProductDis;
    
				if( dot(LDist, N) < 0.0 ) {
				specular2 = vec4(0.0, 0.0, 0.0, 1.0);
				} 
				
				vec4 color2 = (ambient2 + diffuse2 + specular2);
				//---------------------------------------------------------------





				if(spotlight == 1)
				{
							
							color = GlobalAmbientProduct+color1 + color2;
				}
				gl_Position = Projection * ModelView * vPosition;
				
				if(spotlight == 2)
				{
							float attenuation_spotlight;
							vec3 pos2 = (ModelView * spotlight_point).xyz;//in eye co-orindates
							vec3 LSpot = normalize( - LightPosition.xyz + LightDestination.xyz);
							vec3 LP = normalize( - LightPosition.xyz + pos );//vector P from light source to point on sphere/ground.
							float A = max(0.0, dot(LSpot,LP));

							if(A < cos(20.0 * 3.14159 / 180.0))
							{
							     attenuation_spotlight = 0.0;
							}
							else
							{
							    attenuation_spotlight = pow(A, 15.0);
							}
							color = GlobalAmbientProduct+ attenuation_spotlight * color1 + color2;
				}
				x = 4;
				
				if(Vertical1OrSlantedStrip2 == 1)
				{
					strip = 2.5*temp.x;
				}
				else 
				{
					strip = 1.5*(temp.x + temp.y + temp.z);
				}
				if(Vertical1OrSlantedChecker2 == 1)
				{
					float svar = 0.5*(temp.x + 1);
					float tvar = 0.5*(temp.y + 1);
					sphereCoord = vec2(svar, tvar);
				}
				else
				{
					float svar = 0.3 * (temp.x + temp.y + temp.z);//0.5*(vPosition.x + 1);
					float tvar = 0.3 * (temp.x - temp.y + temp.z);//0.5*(vPosition.y + 1);
					sphereCoord = vec2(svar, tvar);
				}
				

				 //strip = 2.5*vPosition.x;
				 
				 
				 
				 //lattice
				 if(Upright1OrTiltedLattice2 == 1)
				 {
					float svar = 0.5*(vPosition.x + 1);
					float tvar = 0.5*(vPosition.y + 1);
					latticeCoord = vec2(svar, tvar);	  
				 }
				 else
				 {
					 float svar = 0.3 * (vPosition.x + vPosition.y + vPosition.z);
					 float tvar = 0.3 * (vPosition.x - vPosition.y + vPosition.z);
					 latticeCoord = vec2(svar,tvar);
				 }
				 
				 //latticeCoord
			/*--- attenuation below must be computed properly ---*/
				//color = GlobalAmbientProduct+color1 + color2;
				//color =color1;
	}
	else if(shadowFlag == -1)
	{
	//std::cout << "ADITYA BHATT ASSIGNMENT 3 APB462 enter file" << std::endl;
	//printf("Helllo");
				vec4 vPosition4 = vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
			vec4 vColor4 = vec4(vColor.r, vColor.g, vColor.b, 0.65); 

				// YJC: Original, incorrect below:
				//      gl_Position = Projection*ModelView*vPosition/vPosition.w;

				gl_Position = Projection * ModelView * vPosition4;
				color = vColor4;
				//color = vec4(0.25,0.25,0.25,0.01);
				x = vPosition4.x;
				 texCoord = vTexCoord;
				 if(Upright1OrTiltedLattice2 == 1)
				 {
					float svar = 0.5*(vPosition.x + 1);
					float tvar = 0.5*(vPosition.y + 1);
					latticeCoord = vec2(svar, tvar);	  
				 }
				 else
				 {
					 float svar = 0.3 * (vPosition.x + vPosition.y + vPosition.z);
					 float tvar = 0.3 * (vPosition.x - vPosition.y + vPosition.z);
					 latticeCoord = vec2(svar,tvar);
				 }
				 
				 //latticeCoord
				// s = 2.5*vPosition4.x;
	}
	else if(shadowFlag == -2)
	{
			vec4	 vPosition4 = vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
			vec4 vColor4 = vec4(vColor.r, vColor.g, vColor.b, 1.0); 

				// YJC: Original, incorrect below:
				//      gl_Position = Projection*ModelView*vPosition/vPosition.w;

				gl_Position = Projection * ModelView * vPosition4;

			   color = vColor4;
				//color = vec4(0.25,0.25,0.25,0.65);
				x = 4;
				 texCoord = vTexCoord;
	}
}
