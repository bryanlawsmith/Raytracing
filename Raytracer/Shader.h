#pragma once

#include "RaytracerCommon.h"
#include <string>
#include <memory>

namespace Raytracer
{
	class Shader
	{
	public:

		/// <summary>
		/// Initializes the shader to a specific shader type and loads the
		/// shader's source code from a file specified by fileName.
		/// </summary>
		Shader(const std::string& fileName, GLenum shaderType);

		/// <summary>
		/// Initializes the shader to a specific type. No source code is loaded
		/// at this point.
		/// </summary>
		Shader(GLenum shaderType);

		/// <summary>
		/// Destructor.
		/// </summary>
		~Shader();

		/// </ummary>
		/// Returns is the last compilation of this shader object was successful or not.
		/// </summary>
		bool GetIsCompiled() const;

		/// <summary>
		/// Returns the OpenGL handle for this shader.
		/// </summary>
		GLuint GetHandle() const;

		/// <summary>
		/// Returns the shader type of this shader object. This will be one of GL_VERTEX_SHADER,
		/// GL_FRAGMENT_SHADER, GL_COMPUTE_SHADER.
		/// </summary>
		GLuint GetType() const;

		/// <summary>
		/// Attempts to compile this shader object.
		/// </summary>
		void Compile();

		/// <summary>
		/// Sets this shader's source code.
		/// WARNING: A recompile is needed after calling this method before you can
		///          successfully use the new shader.
		/// </summary>
		void SetShaderSource(std::string& code) const;

		/// <summary>
		/// Retrieves the shader's log. It is stored in log upon
		/// return.
		/// </summary>
		void GetShaderLog(std::string& log) const;

		/// <summary>
		/// Retrieves the shader's source code. It is stored in @param source upon
		/// return.
		/// </summary>
		void GetShaderSource(std::string& source) const;

		/// <summary>
		/// Returns a boolean specifying if this shader is successfully compiled or not.
		/// </summary>
		bool GetCompiled() const;

	protected:

		/// <summary>
		/// Indicates the type of shader program this is. Options are
		/// GL_VERTEX_SHADER
		/// GL_FRAGMENT_SHADER
		/// GL_COMPUTE_SHADER
		/// </summary>
		GLenum m_Type;

		/// <summary>
		/// Handle to the shader object provided by OpenGL.
		/// </summary>
		GLuint m_Handle;

		/// <summary>
		/// Flag set if the compilation was successful.
		/// </summary>
		bool m_IsCompiled;

		/// <summary>
		/// Reads the shader source code from a file.
		/// </summary>
		std::unique_ptr<char[]> ReadShader(const std::string& fileName);

		/// <summary>
		/// Ensures that the shader is of the right type and generates a new 
		/// handle for it.
		/// </summary>
		void VerifyAndGenerateShaderHandle();

		/// <summary>
		/// Wrapper around glGetShaderiv.
		/// </summary>
		void GetParameter(GLenum param, GLint* data) const;
	};
}
