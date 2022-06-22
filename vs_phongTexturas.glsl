	#version 330 core
	
	layout (location = 0) in vec3 pos;
	layout (location = 1) in vec3 normalVertice;
	layout (location = 2) in vec2 textCoord;

	out vec3 fragPos;
	out vec3 fragNormal;
	out vec3 fragColor;
	out vec2 fragTextColor;
	
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;
	
	void main()
	{
		gl_Position = projection * view * model * vec4(pos, 1.0);
		
		fragPos = vec3(model * vec4(pos, 1.0));
		fragNormal = mat3(transpose(inverse(model))) * normalVertice;
		fragTextColor = textCoord;
		fragColor = vec3(1.0, 1.0, 1.0);
	}	
