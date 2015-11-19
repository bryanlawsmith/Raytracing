#ifndef SHADERPROGRAM_H_INCLUDED
#define SHADERPROGRAM_H_INCLUDED

#include "RaytracerCommon.h"
#include "Shader.h"
#include <memory>

namespace Raytracer
{
	class ShaderProgram
	{
	public:

		/// <summary>
		/// Constructor which accepts two shaders, one vertex and one fragment.
		/// Assumes that vertexShader and fragmentShader are of the types GL_VERTEX_SHADER and GL_FRAGMENT_SHADER respectively.
		/// NOTE:	If a field is NULL then that stage of the pipeline will be fixed function, however, we require that at least one shader
		///			be loaded with this program.
		/// </summary>
		ShaderProgram(std::unique_ptr<Shader> vertexShader,
					  std::unique_ptr<Shader> fragmentShader);

		/// <summary>
		/// Constructor which accepts a compute shader.
		/// </summary>
		ShaderProgram(std::unique_ptr<Shader> computeShader);

		/// <summary>
		///	On destruction, ShaderProgram objects delete any shaders attached to them.
		/// </summary>
		~ShaderProgram();

		/// <summary>
		/// Attempts to link this program object. If the attached shaders have not been compiled then this method will call their compile
		/// methods. The linked flag will be set to either true or false depending on whether the link operation was successful or not.
		/// </summary>
		void Link();

		/// <summary>
		/// Sets this program as the currently active shader program in OpenGL.
		/// If the programs' linked flag is false, this method will attempt to link the program object before use.
		/// </summary>
		void Use();

		/// <summary>
		/// Wrapper around the glGetProgramiv function.
		/// </summary>
		void GetParameter(GLenum, GLint* data) const;

		/// <summary>
		/// Retrieves the program's log, the programs' log is stored in @param programLog upon return.
		/// </summary.
		void GetProgramLog(std::string& programLog) const;

		/// <summary>
		/// Returns the OpenGL program handle.
		/// </summary>
		GLuint GetProgramHandle() const;

		void SetUniform1f(const std::string& uniformName, GLfloat v0) const;
		void SetUniform2f(const std::string& uniformName, GLfloat v0, GLfloat v1) const;
		void SetUniform2fv(const std::string& uniformName, int32_t count, GLfloat* v) const;
		void SetUniform3f(const std::string& uniformName, GLfloat v0, GLfloat v1, GLfloat v2) const;
		void SetUniform4f(const std::string& uniformName, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const;
		void SetUniform1i(const std::string& uniformName, GLint v0) const;
		void SetUniform2i(const std::string& uniformName, GLint v0, GLint v1) const;
		void SetUniform3i(const std::string& uniformName, GLint v0, GLint v1, GLint v2) const;
		void SetUniform4i(const std::string& uniformName, GLint v0, GLint v1, GLint v2, GLint v3) const;
		void SetUniformMatrix2fv(const std::string& uniformName, const GLfloat* values, GLboolean transpose = false) const;
		void SetUniformMatrix3fv(const std::string& uniformName, const GLfloat* values, GLboolean transpose = false) const;
		void SetUniformMatrix4fv(const std::string& uniformName, const GLfloat* values, GLboolean transpose = false) const;

		/// <summary>
		/// Retrieves the currently active shader program handle. 0 is returned if no shaders are currently active.
		/// </summary>
		static GLuint GetCurrentlyActiveProgramHandle();

		/// <summary>
		/// Deactivates all OpenGL shader functionality.
		/// </summary>
		static void DeactivateShaders();

	protected:

		std::unique_ptr<Shader> m_VertexShader;
		std::unique_ptr<Shader> m_FragmentShader;
		std::unique_ptr<Shader> m_ComputeShader;

		/// <summary>
		/// The OpenGL shader program handle.
		/// </summary>
		GLuint m_Handle;

		/// <summary>
		/// Tracks whether the most recent link operation was successful.
		/// </summary>
		bool m_IsLinked;

		/// <summary>
		/// Static to keep track of the currently active shader program. If no shader program is currently active (i.e shaders are disabled)
		/// this value is 0.
		/// </summary>
		static GLuint s_CurrentlyActiveShaderProgram;
	};

}


#endif // SHADERPROGRAM_H_INCLUDED
