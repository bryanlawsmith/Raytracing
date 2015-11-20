#include "ShaderManager.h"

namespace Raytracer 
{
	ShaderManager* ShaderManager::s_Instance = nullptr;

	const std::string ShaderManager::SHADER_SIMPLE_TEXTURE	= "Shaders/ShaderSimpleTexture";
	const std::string ShaderManager::SHADER_CORE_TRACE		= "Shaders/ShaderCoreTrace";
	
	static const std::string SHADER_VERTEX_EXTENSION	= ".vert";
	static const std::string SHADER_FRAGMENT_EXTENSION	= ".frag";
	static const std::string SHADER_COMPUTE_EXTENSION	= ".comp";

	ShaderManager::ShaderManager()
	{
		ShaderProgram* currentProgram;

		// Simple texture.
		{
			std::unique_ptr<Shader> vertexShader(new Shader(SHADER_SIMPLE_TEXTURE + SHADER_VERTEX_EXTENSION, GL_VERTEX_SHADER));
			std::unique_ptr<Shader> fragmentShader(new Shader(SHADER_SIMPLE_TEXTURE + SHADER_FRAGMENT_EXTENSION, GL_FRAGMENT_SHADER));

			currentProgram = new ShaderProgram(std::move(vertexShader), std::move(fragmentShader));
			currentProgram->Link();

			m_ProgramMap.insert(std::make_pair(SHADER_SIMPLE_TEXTURE, std::unique_ptr<ShaderProgram>(currentProgram)));
		}

		// Core trace.
		{
			std::unique_ptr<Shader> computeShader(new Shader(SHADER_CORE_TRACE + SHADER_COMPUTE_EXTENSION, GL_COMPUTE_SHADER));

			currentProgram = new ShaderProgram(std::move(computeShader));
			currentProgram->Link();

			m_ProgramMap.insert(std::make_pair(SHADER_CORE_TRACE, std::unique_ptr<ShaderProgram>(currentProgram)));
		}
	}

	ShaderProgram* ShaderManager::GetShader(std::string name)
	{
		auto program = m_ProgramMap.find(name);
		if (m_ProgramMap.end() != program)
			return program->second.get();

		return nullptr;
	}

	ShaderManager::~ShaderManager()
	{
	}

	ShaderManager& ShaderManager::GetInstance()
	{
		if (nullptr == s_Instance)
			s_Instance = new ShaderManager;

		return *s_Instance;
	}
}
