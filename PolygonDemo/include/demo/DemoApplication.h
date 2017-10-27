#pragma once

#include "stdafx.h"

namespace demo
{
	struct Model
	{
		std::shared_ptr<plgn::VertexArray> mesh;
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		glm::vec3 materialColor;

		inline glm::mat4 getModelMatrix() const
		{
			glm::mat4 modelMatrix = glm::translate(position);
			modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1, 0, 0));
			modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0, 1, 0));
			modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0, 0, 1));
			modelMatrix = glm::scale(modelMatrix, scale);
			return modelMatrix;
		}
	};

	class DemoApplication : public plgn::Application
	{
	private:
		glm::mat4 m_projectionMatrix;
		plgn::Camera m_camera;

		std::unique_ptr<plgn::Shader> m_simpleShader;
		std::unique_ptr<plgn::Texture2D> m_texture;
		std::unique_ptr<plgn::Texture2D> m_normalMap;

		std::unordered_map<std::string, std::shared_ptr<plgn::VertexArray>> m_meshes;
		std::vector<Model> m_models;

	public:
		DemoApplication();
		DemoApplication(const DemoApplication&) = delete;
		DemoApplication& operator=(const DemoApplication&) = delete;

	private:
		void init() override;
		void update(double deltaTime) override;
		void render() override;
		void dispose() override;
	};
}
