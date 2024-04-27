#include "shaders.h"

GLuint loadShader(const char * vertp, const char * fragp) {
    GLuint vertid = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragid = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint programid = glCreateProgram();

    std::string vertcode;
    std::string fragcode;

    std::stringstream tmp_vert;
    std::stringstream tmp_frag;

	GLint result = GL_FALSE;
	int info_log_len;

    std::ifstream vertfile(vertp);
    if (!vertfile.is_open()) {
        std::cout << "Couldn't open vertex shader " << vertp << std::endl;
        return -1;
    }
    tmp_vert << vertfile.rdbuf();
    vertcode = tmp_vert.str();

    std::ifstream fragfile(fragp);
    if (!fragfile.is_open()) {
        std::cout << "Couldn't open fragment shader " << fragp << std::endl;
        return -1;
    }
    tmp_frag << fragfile.rdbuf();
    fragcode = tmp_frag.str();

    // Compile vertex shader
    const char * vertcode_ptr = vertcode.c_str();
    glShaderSource(vertid, 1, &vertcode_ptr, NULL);
    glCompileShader(vertid);

	// Check Vertex Shader
	glGetShaderiv(vertid, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE) {
        glGetShaderiv(vertid, GL_INFO_LOG_LENGTH, &info_log_len);
        char log[info_log_len + 1];
        glGetShaderInfoLog(vertid, info_log_len, NULL, log);
        std::cout << log;
	}

    // Compile fragment shader
    const char * fragcode_ptr = fragcode.c_str();
    glShaderSource(fragid, 1, &fragcode_ptr, NULL);
    glCompileShader(fragid);

	// Check fragment Shader
	glGetShaderiv(fragid, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE) {
        glGetShaderiv(fragid, GL_INFO_LOG_LENGTH, &info_log_len);
        char log[info_log_len + 1];
        glGetShaderInfoLog(fragid, info_log_len, NULL, log);
        std::cout << log;
	}
	// Link the program
	glAttachShader(programid, vertid);
	glAttachShader(programid, fragid);
	glLinkProgram(programid);

	// Check the program
	glGetProgramiv(programid, GL_LINK_STATUS, &result);
	if (result != GL_TRUE){
        glGetProgramiv(programid, GL_INFO_LOG_LENGTH, &info_log_len);
        char log[info_log_len + 1];
        glGetProgramInfoLog(programid, info_log_len, NULL, log);
        std::cout << log;
	}
	
    // Program is linked, don't need the individual shaders anymore
	glDetachShader(programid, vertid);
	glDetachShader(programid, fragid);
	glDeleteShader(vertid);
	glDeleteShader(fragid);

	return programid;
}
