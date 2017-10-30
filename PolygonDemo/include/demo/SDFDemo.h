#pragma once

#include "stdafx.h"

namespace demo
{
	class SDFDemo : public plgn::Application
	{
	private:
		std::unique_ptr<plgn::Shader> m_sdfShader;
		std::unique_ptr<plgn::VertexArray> m_screenVao;

	public:
		SDFDemo();
		SDFDemo(const SDFDemo&) = delete;
		SDFDemo& operator=(const SDFDemo&) = delete;

	private:
		void init() override;
		void update(double deltaTime) override;
		void render() override;
		void dispose() override;
	};
}
