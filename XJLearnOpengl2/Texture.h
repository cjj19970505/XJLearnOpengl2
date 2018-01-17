#pragma once
#include <string>
#include <iostream>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <glad\glad.h>

#include <stb_image.h>
using namespace std;
namespace xjg
{
	class Texture
	{
	public:
		unsigned int glID;
		string type;
		string path;
		bool gammaCorrection;
		Texture():glID(0){}
		Texture(string path, bool gamma = false) :Texture()
		{
			this->path = path;
			this->gammaCorrection = gamma;
		}
		void glSetup()
		{
			if (glID != 0)
			{
				cout << "WARRING: TEXTURE(" << path << ") HAS ALREADY BEEN LOADED" << endl;
				return;
			}
			glID = TextureFromFile(path, gammaCorrection);
		}
	private:
		unsigned int TextureFromFile(const string path, bool gamma)
		{
			string filename = string(path);
			//filename = directory + '/' + filename;

			unsigned int textureID;
			glGenTextures(1, &textureID);

			int width, height, nrComponents;
			unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
			if (data)
			{
				GLenum format;
				if (nrComponents == 1)
					format = GL_RED;
				else if (nrComponents == 3)
					format = GL_RGB;
				else if (nrComponents == 4)
					format = GL_RGBA;

				glBindTexture(GL_TEXTURE_2D, textureID);
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				stbi_image_free(data);
			}
			else
			{
				std::cout << "Texture failed to load at path: " << filename << std::endl;
				stbi_image_free(data);
			}
			return textureID;
		}

	};
}