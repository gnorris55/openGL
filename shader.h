#ifndef SHADER_H
#define SHADER_H


class Shader {

	public:
		unsigned int ID;
		Shader(const char *vertexShaderFileName, const char *fragmentShaderFileName) {
						
			std::ifstream vertexShaderFile;
			std::ifstream fragmentShaderFile;
			std::string vertexCode; 
			std::string fragmentCode; 
			vertexShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
			fragmentShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

			
			try {
				std::stringstream vertexShaderStream;
				vertexShaderFile.open(vertexShaderFileName);
				vertexShaderStream << vertexShaderFile.rdbuf();
				vertexShaderFile.close();
				vertexCode = vertexShaderStream.str();
				
				std::stringstream fragmentShaderStream;
				fragmentShaderFile.open(fragmentShaderFileName);
				fragmentShaderStream << fragmentShaderFile.rdbuf();
				fragmentShaderFile.close();
				fragmentCode = fragmentShaderStream.str();
			}       catch (std::ifstream::failure& e) {
				std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
			}
			
			const char *vertexShaderCode = vertexCode.c_str();
			const char *fragmentShaderCode = fragmentCode.c_str();

			unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
			glCompileShader(vertexShader);
			checkCompileErrors(vertexShader, "VERTEX");
			
			unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
			glCompileShader(fragmentShader);
			checkCompileErrors(fragmentShader, "FRAGMENT");
			
			// creating the program object and adding/linking shaders
			ID = glCreateProgram();
			glAttachShader(ID, vertexShader);
			glAttachShader(ID, fragmentShader);
			glLinkProgram(ID);
			checkCompileErrors(ID, "PROGRAM");
			//deleting shaders after adding them to the program
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

		}

		

	private:
		void checkCompileErrors(unsigned int shader, std::string type) {
		int success;
		char infoLog[1024];
		if (type != "PROGRAM") {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
		    glGetProgramiv(shader, GL_LINK_STATUS, &success);
		    if (!success)
		    {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		    }
		}
	}



};

#endif
