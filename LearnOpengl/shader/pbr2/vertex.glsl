#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aCoords;

 uniform mat4 model;
 uniform mat3 normalMatrix;
 uniform mat4 modelViewProjection;

 out vec3 fragWorldPos;
 out vec3 vNormal;
 out vec2 vCoords;

 void main()
 {
	fragWorldPos = vec3(model * vec4(aPosition, 1.0));
	vNormal = normalMatrix * aNormal;
	vCoords = aCoords;
	gl_Position = modelViewProjection * vec4(aPosition, 1.0);
 }