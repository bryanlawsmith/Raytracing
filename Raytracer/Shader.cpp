#include "Shader.h"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>

namespace Raytracer
{
	Shader::Shader(const std::string& fileName, GLenum shaderType)
	{
		m_IsCompiled = false;

		m_Type = shaderType;

		VerifyAndGenerateShaderHandle();

		// Read shader source code, upload, and compile.
		auto sourceCode = ReadShader(fileName);
		const char* sourceArray[] = { sourceCode.get() };
		glShaderSource(m_Handle, 
					   1, 
					   reinterpret_cast<const GLchar**>(sourceArray),
					   nullptr);

		Compile();
	}

	Shader::Shader(GLenum shaderType)
	{
		m_IsCompiled = false;

		m_Type = shaderType;

		VerifyAndGenerateShaderHandle();
	}

	Shader::~Shader()
	{
		if (glIsShader(m_Handle))
			glDeleteShader(m_Handle);
	}

	void Shader::VerifyAndGenerateShaderHandle()
	{
		// Ensure that the shader is of the correct type.
		assert(m_Type == GL_VERTEX_SHADER ||
			   m_Type == GL_FRAGMENT_SHADER ||
			   m_Type == GL_COMPUTE_SHADER);

		// Create the shdaer.
		m_Handle = glCreateShader(m_Type);
	}

	void Shader::Compile()
	{
		assert(glIsShader(m_Handle));

		std::cerr << "Shader::compiling: ";

		GLint compilationSuccessful;

		glCompileShader(m_Handle);

		GetParameter(GL_COMPILE_STATUS, &compilationSuccessful);
		if (GL_TRUE == compilationSuccessful)
		{
			m_IsCompiled = true;

			std::cerr << "success" << std::endl << std::endl;
		}
		else
		{
			m_IsCompiled = false;
			std::cerr << "failed, stopping execution and outputting shader log" << std::endl;

			std::string shaderLog;
			GetShaderLog(shaderLog);
			std::cerr << shaderLog << std::endl;

			exit(1);
		}
	}

	bool Shader::GetCompiled() const
	{
		return m_IsCompiled;
	}

	void Shader::SetShaderSource(std::string& code) const
	{
		const char* sourceCode = code.c_str();

		glShaderSource(m_Handle, 1, (const GLchar**)&sourceCode, nullptr);
	}

	void Shader::GetShaderLog(std::string& log) const
	{
		assert(GL_TRUE == glIsShader(m_Handle));

		GLint infoLogLength;
		GetParameter(GL_INFO_LOG_LENGTH, &infoLogLength);

		std::unique_ptr<GLchar[]> infoLog(new GLchar[infoLogLength]);
		glGetShaderInfoLog(m_Handle, infoLogLength, &infoLogLength, infoLog.get());

		log = infoLog.get();
	}

	void Shader::GetShaderSource(std::string& source) const
	{
		assert(GL_TRUE == glIsShader(m_Handle));

		GLint sourceCodeLength;
		GetParameter(GL_SHADER_SOURCE_LENGTH, &sourceCodeLength);

		std::unique_ptr<GLchar[]> sourceCode(new GLchar[sourceCodeLength]);
		glGetShaderSource(m_Handle, sourceCodeLength, &sourceCodeLength, sourceCode.get());

		source = sourceCode.get();
	}

	void Shader::GetParameter(GLenum param, GLint* data) const
	{
		assert(GL_TRUE == glIsShader(m_Handle));
		glGetShaderiv(m_Handle, param, data);
	}

	std::unique_ptr<char[]> Shader::ReadShader(const std::string& fileName)
	{
		std::cerr << "Shader::loading shader file " << fileName << ": ";
		std::string programString;
		std::ifstream shaderFile;

		// Check file extensions.
		std::string shaderFileExtension = fileName.substr(fileName.size() - 3, 3);

		if (GL_VERTEX_SHADER == m_Type && ".vs" != shaderFileExtension)
			std::cerr << "attempting to load vertex shader source from a non .vs file ... ";

		if (GL_FRAGMENT_SHADER == m_Type && ".fs" != shaderFileExtension)
			std::cerr << "attempting to load fragment shader source from a non .fs file ... ";

		if (GL_COMPUTE_SHADER == m_Type && ".cs" != shaderFileExtension)
			std::cerr << "attempting to load compute shader source from a non .cs file ... ";

		// Open file.
		shaderFile.open(fileName.c_str());
		if (!shaderFile.is_open())
		{
			std::cerr << "failed [unable to open source file]" << std::endl;
			exit(1);
		}

		// Parse file contents.
		while (!shaderFile.eof())
		{
			char currentChar;
			shaderFile.get(currentChar);

			programString += currentChar;

			shaderFile.peek();
		}

		shaderFile.close();

		std::cerr << "success" << std::endl;

		std::unique_ptr<char[]> sourceCodeString(new char[programString.size() + 1]);
		memcpy(sourceCodeString.get(), programString.c_str(), programString.size() + 1);

		return sourceCodeString;
	}

	GLuint Shader::GetHandle() const
	{
		return m_Handle;
	}

	GLuint Shader::GetType() const
	{
		return m_Type;
	}
}