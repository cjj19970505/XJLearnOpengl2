#pragma once
#include <map>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glad\glad.h>

#include "Shader.h"
using namespace std;
namespace xjg
{
	class Material
	{

	public:
		map<string, float> floatProperties;
		map<string, bool> boolProperties;
		map<string, glm::vec4> vec4Properties;
		map<string, glm::mat4> mat4Properties;
		map<string, glm::vec3> vec3Properties;
		map<string, int> intProperties;
		Shader *shader;
		Material() :shader()
		{
			shader = new Shader();
		}
		void setFloat(string name, float value)
		{
			floatProperties.insert_or_assign(name, value);
		}
		void setBool(string name, bool value)
		{
			boolProperties.insert_or_assign(name, value);
		}
		void setVec4(string name, glm::vec4 value)
		{
			vec4Properties.insert_or_assign(name, value);
		}
		void setMat4(string name, glm::mat4 value)
		{
			mat4Properties.insert_or_assign(name, value);
		}
		void setVec3(string name, glm::vec3 value)
		{
			vec3Properties.insert_or_assign(name, value);
		}
		void setInt(string name, int value)
		{
			intProperties.insert_or_assign(name, value);
		}
	};

}

