#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glad\glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;
namespace xjg
{
	class Shader
	{
	public:
		unsigned int id;
		string vertexPath;
		string geometryPath;
		string fragmentPath;
		Shader():id(0), vertexPath("DefaultVertexShader.glsl"), fragmentPath("DefaultFragmentShader.glsl"), geometryPath("DefaultGeometryShader.glsl") {}
		Shader(string vertexPath, string fragmentPath):id(0), vertexPath(vertexPath), fragmentPath(fragmentPath), geometryPath("DefaultGeometryShader.glsl"){}
		Shader(string vertexPath, string geometryPath, string fragmentPath):id(0), vertexPath(vertexPath), fragmentPath(fragmentPath), geometryPath(geometryPath) {}
		
		//һ����System����
		void loadShader()
		{
			if (id != 0)
			{
				cout << "WARRING::SHADER " << id << " HAS ALREADY BEEN SETUP" << endl;
				return;
			}
			std::string vertexCode;
			std::string fragmentCode;
			std::string geometryCode;
			std::ifstream vShaderFile;
			std::ifstream fShaderFile;
			std::ifstream gShaderFile;
			vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			try
			{
				vShaderFile.open(vertexPath);
				fShaderFile.open(fragmentPath);
				gShaderFile.open(geometryPath);
				std::stringstream vShaderStream, fShaderStream, gShaderStream;
				vShaderStream << vShaderFile.rdbuf();
				fShaderStream << fShaderFile.rdbuf();
				gShaderStream << gShaderFile.rdbuf();
				vShaderFile.close();
				fShaderFile.close();
				gShaderFile.close();
				vertexCode = vShaderStream.str();
				fragmentCode = fShaderStream.str();
				geometryCode = gShaderStream.str();

			}
			catch (std::ifstream::failure e)
			{
				std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
			}
			const char *vShaderCode = vertexCode.c_str();
			const char *fShaderCode = fragmentCode.c_str();
			const char *gShaderCode = geometryCode.c_str();

			unsigned int vertex, fragment, geometry;
			int success;
			char infoLog[512];

			vertex = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex, 1, &vShaderCode, NULL);
			glCompileShader(vertex);
			glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(vertex, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			}

			fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment, 1, &fShaderCode, NULL);
			glCompileShader(fragment);
			glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

			if (!success)
			{
				glGetShaderInfoLog(fragment, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			}

			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &gShaderCode, NULL);
			glCompileShader(geometry);
			glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(geometry, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
			}
			id = glCreateProgram();
			glAttachShader(id, vertex);
			glAttachShader(id, geometry);
			glAttachShader(id, fragment);
			
			glLinkProgram(id);

			glGetProgramiv(id, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(id, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			}
			glDeleteShader(vertex);
			glDeleteShader(geometry);
			glDeleteShader(fragment);

		}
		void use()
		{
			if (id == 0)
			{
				loadShader();
			}
			glUseProgram(id);
		}
		void setup()
		{
			glEnable(GL_DEPTH_TEST);
		}
		void setBool(const std::string & name, bool value) const
		{
			glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
		}

		void setInt(const std::string & name, int value) const
		{
			glUniform1i(glGetUniformLocation(id, name.c_str()), value);
		}

		void setFloat(const std::string & name, float value) const
		{
			glUniform1f(glGetUniformLocation(id, name.c_str()), value);
		}

		void setVector4(const std::string & name, glm::vec4 value) const
		{
			glUniform4f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z, value.w);
		}

		void setMatrix4(const std::string & name, glm::mat4 value) const
		{
			glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
		}

		void setVector3(const std::string & name, glm::vec3 value) const
		{
			glUniform3f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z);
		}

	};
}