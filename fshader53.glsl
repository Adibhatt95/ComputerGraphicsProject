/* 
File Name: "fshader53.glsl":
           Fragment Shader
*/

// #version 150  // YJC: Comment/un-comment this line to resolve compilation errors
                 //      due to different settings of the default GLSL version

in  vec4 color;
in float x;
in float strip;
in  vec2 texCoord;
in vec2 sphereCoord;
in float zfog;
in float xtry;
in float ztry;
in vec2 latticeCoord;
uniform int shadowFlag;
uniform sampler2D texture_2D;
uniform sampler1D texture_1D;
uniform int twoDor1D;
uniform int textorNot;
uniform int sphereOrFloor;//sphere =1, floor =2
uniform int fog10yes20no11lin12exp13expsqu;
uniform int latYes10No20;
//in float z;
out vec4 fColor;

void main() 
{ 
	
	vec4 colorBase;
	
	
	if(textorNot == 1 && sphereOrFloor == 2)
	{    colorBase = color * texture( texture_2D, texCoord );  }
	else if(textorNot == 1 && sphereOrFloor == 1)
	{  
		if(latYes10No20 == 10 && fract(4*latticeCoord.x) < 0.35 && fract(4*latticeCoord.y) < 0.35) discard; 
		if(twoDor1D == 1)
			colorBase = color * texture( texture_1D, strip );
		if(twoDor1D == 2){
			vec4 colortemp = texture( texture_2D, sphereCoord );
			if(colortemp.r == 0.0){
			colortemp.r = 0.9;
			colortemp.g = 0.1;
			colortemp.b = 0.1;
			colortemp.a = 1.0;
			}
			colorBase = color * colortemp;
			}
	}
	else if(textorNot == 1 && sphereOrFloor == 0)
	{ 
		if(latYes10No20 == 10 && fract(4*latticeCoord.x) < 0.35 && fract(4*latticeCoord.y) < 0.35) discard; 
		colorBase = color;
	}
	else
	{
	if(shadowFlag == -1){
		if(latYes10No20 == 10 && fract(4*latticeCoord.x) < 0.35 && fract(4*latticeCoord.y) < 0.35) discard;
		//if(ztry < -40 ) discard;
		}
	colorBase = color;}
	if(fog10yes20no11lin12exp13expsqu/10 == 1)
	{
	  float start = 0.0;
	  float end = 18.0;
	  float density = 0.09;
	  vec4 fogcolor = vec4(0.7,0.7,0.7,0.5);
	  float f;
	  if(fog10yes20no11lin12exp13expsqu == 11)//linear fog
	  {
		f = (end-zfog)/(end-start);
	  }
	  else if(fog10yes20no11lin12exp13expsqu == 12)//exponential
	  {
		f = exp(-density*zfog);
	  }
	  else if(fog10yes20no11lin12exp13expsqu == 13)
	  {
		f = exp(-((density*zfog)*(density*zfog)));
	  }
	  f = clamp(f,0,1);
	  colorBase = mix(fogcolor,colorBase,f);
	}
	fColor = colorBase;
} 

