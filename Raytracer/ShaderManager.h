#pragma once

#include "RaytracerCommon.h"
#include "ShaderProgram.h"
#include <string>
#include <map>

namespace Raytracer
{
	class ShaderManager
	{
	public:

		/// <summary>
		/// Returns the ShaderProgram specified by name.
		/// If the program does not exist, NULL is returned.
		/// </summary>
		ShaderProgram* GetShader(std::string name);

		/// <summary>
		/// On destruction, the shader manager will delete all of the resident ShaderProgram objects.
		/// </summary>
		~ShaderManager();

		static ShaderManager& GetInstance();

		/// - Shader Types. - 

		static const std::string SHADER_SIMPLE_TEXTURE;
		static const std::string SHADER_CORE_TRACE;

	protected:

		std::map<std::string, std::unique_ptr<ShaderProgram>> m_ProgramMap;

		static ShaderManager* s_Instance;

		ShaderManager();

		ShaderManager(const ShaderManager& rvalue) = delete;
		ShaderManager& operator=(const ShaderManager& rvalue) = delete;

	};
}
