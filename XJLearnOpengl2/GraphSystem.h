#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Camera.h"
#include "Model.h"
#include <vector>

namespace xjg
{
	class GraphSystem
	{
	private:
		Camera *currentCamera;
		vector<Model*> renderModels;
	public:
		int screenWidth = 800;
		int screenHeight = 600;
		
		void setScreenSize(int screenWidth, int screenHeight)
		{
			this->screenWidth = screenWidth;
			this->screenHeight = screenHeight;
		}
		void setCurrentCamera(Camera *camera)
		{
			currentCamera = camera;
		}
		glm::mat4 getProjMat()
		{
			glm::mat4 projection;
			projection = glm::perspective(glm::radians(currentCamera->fov), (float)(screenWidth / screenHeight), 0.1f, 100.0f);
			return projection;
		}
		void addModel(Model *model)
		{
			renderModels.push_back(model);
		}
		void drawModels()
		{
			for (auto i = renderModels.begin(); i != renderModels.end(); i++)
			{
				drawModel(*i);
			}
		}
		void drawModel(Model *model)
		{
			if (model->meshes.size() <= 0)
			{
				model->loadModel();
			}
			
			vector<Material> &mats = model->materials;
			auto itMesh = model->meshes.begin();
			for (auto i = mats.begin(); i != mats.end(); i++)
			{
				Shader &shader = *((*i).shader);
				shader.use();
				shader.setup();
				//===GLOBE PROP================
				shader.setMatrix4("app.model", model->getModelMatrix());
				shader.setMatrix4("app.view", currentCamera->getViewMatrix());
				shader.setMatrix4("app.projection", getProjMat());

				//===============================
				map<string, float> &floatProperties = (*i).floatProperties;
				for (auto i = floatProperties.begin(); i != floatProperties.end(); i++)
				{
					shader.setFloat((*i).first, (*i).second);
				}
				map<string, bool> &boolProperties = (*i).boolProperties;
				for (auto i = boolProperties.begin(); i != boolProperties.end(); i++)
				{
					shader.setBool((*i).first, (*i).second);
				}
				map<string, glm::vec4> &vec4Properties = (*i).vec4Properties;
				for (auto i = vec4Properties.begin(); i != vec4Properties.end(); i++)
				{
					shader.setVector4((*i).first, (*i).second);
				}
				map<string, glm::mat4> &mat4Properties = (*i).mat4Properties;
				for (auto i = mat4Properties.begin(); i != mat4Properties.end(); i++)
				{
					shader.setMatrix4((*i).first, (*i).second);
				}
				map<string, glm::vec3> &vec3Properties = (*i).vec3Properties;
				for (auto i = vec3Properties.begin(); i != vec3Properties.end(); i++)
				{
					shader.setVector3((*i).first, (*i).second);
				}
				map<string, int> &intProperties = (*i).intProperties;
				for (auto i = intProperties.begin(); i != intProperties.end(); i++)
				{
					shader.setInt((*i).first, (*i).second);
				}
				Mesh &mesh = (*itMesh);
				if (mesh.VAO == 0)
				{
					mesh.setup();
				}
				
				glBindVertexArray(mesh.VAO);
				glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
				//glBindVertexArray(0);
				itMesh++;
			}
		}
		
	};

}

