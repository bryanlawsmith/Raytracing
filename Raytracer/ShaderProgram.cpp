#include "ShaderProgram.h"
#include <cassert>
#include <iostream>
#include <cstdlib>

namespace Raytracer
{
	GLuint ShaderProgram::s_CurrentlyActiveShaderProgram = 0;

	ShaderProgram::ShaderProgram(std::unique_ptr<Shader> vertexShader, 
								 std::unique_ptr<Shader> fragmentShader)
	{
		std::cerr << "ShaderProgram::creating a new shader program: ";

		m_VertexShader = std::move(vertexShader);
		m_FragmentShader = std::move(fragmentShader);

		m_Handle = glCreateProgram();
		assert(m_Handle);	

		// At least one shader must be provided.
		assert(nullptr != m_VertexShader &&
			   nullptr != m_FragmentShader);

		if (nullptr != m_VertexShader)
		{
			assert(GL_VERTEX_SHADER == m_VertexShader->GetType());
			glAttachShader(m_Handle, m_VertexShader->GetHandle());
		}

		if (nullptr != m_FragmentShader)
		{
			assert(GL_FRAGMENT_SHADER == m_FragmentShader->GetType());
			glAttachShader(m_Handle, m_FragmentShader->GetHandle());
		}

		m_IsLinked = false;

		std::cerr << "success" << std::endl << std::endl;
	}

	ShaderProgram::ShaderProgram(std::unique_ptr<Shader> computeShader)
	{
		std::cerr << "ShaderProgram::creating a new (compute) shader program: ";

		m_ComputeShader = std::move(computeShader);

		m_Handle = glCreateProgram();
		assert(m_Handle);
		assert(nullptr != m_ComputeShader);

		if (nullptr != m_ComputeShader)
		{
			assert(GL_COMPUTE_SHADER == m_ComputeShader->GetType());
			glAttachShader(m_Handle, m_ComputeShader->GetHandle());
		}

		m_IsLinked = false;

		std::cerr << "success" << std::endl << std::endl;
	}

	ShaderProgram::~ShaderProgram()
	{
		if (s_CurrentlyActiveShaderProgram == m_Handle)
		{
			s_CurrentlyActiveShaderProgram = 0;
			glUseProgram(0);
		}

		// Detach shaders.
		if (nullptr != m_VertexShader)
			glDetachShader(m_Handle, m_VertexShader->GetHandle());

		if (nullptr != m_FragmentShader)
			glDetachShader(m_Handle, m_FragmentShader->GetHandle());

		if (nullptr != m_ComputeShader)
			glDetachShader(m_Handle, m_ComputeShader->GetHandle());

		glDeleteProgram(m_Handle);
	}

	void ShaderProgram::Link()
	{
		std::cerr << "ShaderProgram::linking program object: ";

		// Compile the shaders if they haven't already been compiled.
		if (nullptr != m_VertexShader && !(m_VertexShader->GetCompiled()))
			m_VertexShader->Compile();

		if (nullptr != m_FragmentShader && !(m_FragmentShader->GetCompiled()))
			m_FragmentShader->Compile();

		if (nullptr != m_ComputeShader && !(m_ComputeShader->GetCompiled()))
			m_ComputeShader->Compile();

		// Attempt to link the shader(s).
		glLinkProgram(m_Handle);

		GLint linkSuccessful;
		GetParameter(GL_LINK_STATUS, &linkSuccessful);
		if (linkSuccessful)
		{
			m_IsLinked = true;

			std::cerr << "success" << std::endl;
		}
		else
		{
			m_IsLinked = false;

			std::cerr << "failed, stopping execution and outputting log" << std::endl;

			std::string programLog;
			GetProgramLog(programLog);
			std::cerr << programLog << std::endl;
			exit(1);
		}

		std::cerr << std::endl;
	}

	void ShaderProgram::Use()
	{
		// Optionally, link the program if not already done
		if (!m_IsLinked)
			Link();

		glUseProgram(m_Handle);

		s_CurrentlyActiveShaderProgram = m_Handle;
	}

	void ShaderProgram::GetParameter(GLenum param, GLint* data) const
	{
		glGetProgramiv(m_Handle, param, data);
	}

	void ShaderProgram::GetProgramLog(std::string& log) const
	{
		assert(GL_TRUE == glIsProgram(m_Handle));

		GLint infoLogLength;
		GetParameter(GL_INFO_LOG_LENGTH, &infoLogLength);

		std::unique_ptr<GLchar[]> infoLog(new GLchar[infoLogLength]);
		glGetProgramInfoLog(m_Handle, infoLogLength, &infoLogLength, infoLog.get());

		log = infoLog.get();
	}

	void ShaderProgram::SetUniform1f(const std::string& uniformName, GLfloat v0) const
	{
		assert(m_Handle == s_CurrentlyActiveShaderProgram);

		GLint uniformIndex = glGetUniformLocation(m_Handle, uniformName.c_str());
		assert(-1 != uniformIndex);

		glUniform1f(uniformIndex, v0);
	}

	void ShaderProgram::SetUniform2f(const std::string& uniformName, GLfloat v0, GLfloat v1) const
	{
		assert(m_Handle == s_CurrentlyActiveShaderProgram);

		GLint uniformIndex = glGetUniformLocation(m_Handle, uniformName.c_str());
		assert(-1 != uniformIndex);

		glUniform2f(uniformIndex, v0, v1);
	}

	void ShaderProgram::SetUniform2fv(const std::string& uniformName, int32_t count, GLfloat* v) const
	{
		assert(m_Handle == s_CurrentlyActiveShaderProgram);

		GLint uniformIndex = glGetUniformLocation(m_Handle, uniformName.c_str());
		assert(-1 != uniformIndex);

		glUniform2fv(uniformIndex, count, v);
	}

	void ShaderProgram::SetUniform3f(const std::string& uniformName, GLfloat v0, GLfloat v1, GLfloat v2) const
	{
		assert(m_Handle == s_CurrentlyActiveShaderProgram);

		GLint uniformIndex = glGetUniformLocation(m_Handle, uniformName.c_str());
		assert(-1 != uniformIndex);

		glUniform3f(uniformIndex, v0, v1, v2);
	}

	void ShaderProgram::SetUniform4f(const std::string& uniformName, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const
	{
		assert(m_Handle == s_CurrentlyActiveShaderProgram);

		GLint uniformIndex = glGetUniformLocation(m_Handle, uniformName.c_str());
		assert(-1 != uniformIndex);

		glUniform4f(uniformIndex, v0, v1, v2, v3);
	}

	void ShaderProgram::SetUniform1i(const std::string& uniformName, GLint v0) const
	{
		assert(m_Handle == s_CurrentlyActiveShaderProgram);

		GLint uniformIndex = glGetUniformLocation(m_Handle, uniformName.c_str());
		assert(-1 != uniformIndex);

		glUniform1i(uniformIndex, v0);
	}

	void ShaderProgram::SetUniform2i(const std::string& uniformName, GLint v0, GLint v1) const
	{
		assert(m_Handle == s_CurrentlyActiveShaderProgram);

		GLint uniformIndex = glGetUniformLocation(m_Handle, uniformName.c_str());
		assert(-1 != uniformIndex);

		glUniform2i(uniformIndex, v0, v1);
	}

	void ShaderProgram::SetUniform3i(const std::string& uniformName, GLint v0, GLint v1, GLint v2) const
	{
		assert(m_Handle == s_CurrentlyActiveShaderProgram);

		GLint uniformIndex = glGetUniformLocation(m_Handle, uniformName.c_str());
		assert(-1 != uniformIndex);

		glUniform3i(uniformIndex, v0, v1, v2);
	}

	void ShaderProgram::SetUniform4i(const std::string& uniformName, GLint v0, GLint v1, GLint v2, GLint v3) const
	{
		assert(m_Handle == s_CurrentlyActiveShaderProgram);

		GLint uniformIndex = glGetUniformLocation(m_Handle, uniformName.c_str());
		assert(-1 != uniformIndex);

		glUniform4i(uniformIndex, v0, v1, v2, v3);
	}

	void ShaderProgram::SetUniformMatrix2fv(const std::string& uniformName, const GLfloat* values, GLboolean transpose) const
	{
		assert(m_Handle == s_CurrentlyActiveShaderProgram);

		GLint uniformIndex = glGetUniformLocation(m_Handle, uniformName.c_str());
		assert(-1 != uniformIndex);

		glUniformMatrix2fv(uniformIndex, 1, transpose, values);
	}

	void ShaderProgram::SetUniformMatrix3fv(const std::string& uniformName, const GLfloat* values, GLboolean transpose) const
	{
		assert(m_Handle == s_CurrentlyActiveShaderProgram);

		GLint uniformIndex = glGetUniformLocation(m_Handle, uniformName.c_str());
		assert(-1 != uniformIndex);

		glUniformMatrix3fv(uniformIndex, 1, transpose, values);
	}

	void ShaderProgram::SetUniformMatrix4fv(const std::string& uniformName, const GLfloat* values, GLboolean transpose) const
	{
		assert(m_Handle == s_CurrentlyActiveShaderProgram);

		GLint uniformIndex = glGetUniformLocation(m_Handle, uniformName.c_str());
		assert(-1 != uniformIndex);

		glUniformMatrix4fv(uniformIndex, 1, transpose, values);
	}

	GLuint ShaderProgram::GetProgramHandle() const
	{
		return m_Handle;
	}

	GLuint ShaderProgram::GetCurrentlyActiveProgramHandle()
	{
		return s_CurrentlyActiveShaderProgram;
	}

	void ShaderProgram::DeactivateShaders()
	{
		glUseProgram(0);
		s_CurrentlyActiveShaderProgram = 0;
	}

}