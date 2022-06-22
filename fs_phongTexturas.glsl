	#version 330 core
	
	in vec3 fragPos;
	in vec3 fragNormal;
	in vec3 fragColor;
	in vec2 fragTextColor;
	
	uniform sampler2D Texture;	
	
	vec4 Light(vec3 fragColor);
	
	void main()
	{
		gl_FragColor = texture(Texture, fragTextColor) * Light(fragColor);
	}
	
	vec4 Light(vec3 fragColor)
	{
		vec3 colorLuz = vec3(1.0f, 1.0f, 1.0f);
		vec3 posicionLuz = vec3(1.2f, 3.0f, -5.0f);
		vec3 posicionVista = vec3(0.0f, 0.0f, -4.0f);
		
		//luz ambiental
		float fuerzaAmbiental = 1.0;
		vec3 ambiental = fuerzaAmbiental * colorLuz;
		
		//luz difusa
		vec3 norm = normalize(fragNormal);
		vec3 direccionLuz = normalize(posicionLuz - fragPos);
		float diff = max(dot(norm, direccionLuz), 0.0);
		vec3 difusa = diff * colorLuz;
		
		//luz especular
		float fuerzaEspecular = 0.5;
		vec3 direccionVista = normalize(posicionVista - fragPos);
		vec3 direccionReflejo = reflect(-direccionLuz , norm);
		float espec = pow(max(dot(direccionVista, direccionReflejo), 0.0), 32);
		vec3 especular = fuerzaEspecular * espec * colorLuz;
		
		vec3 resultado = (ambiental + difusa + especular) * fragColor;
		
		return vec4(resultado, 1.0);
		
	}
