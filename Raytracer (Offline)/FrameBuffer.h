#pragma once

#include <memory>
#include <string>

namespace Raytracer
{
	/// <summary>
	/// Defines an area of memory that can be rendered to. 
	/// Elements of a frame buffer are stored in 32-bit FP, and are 
	/// ordered RGBA in memory.
	/// </summary>
	class FrameBuffer
	{
	public:

		FrameBuffer(unsigned int width, unsigned int height);

		~FrameBuffer();

		FrameBuffer(const FrameBuffer& other);
		FrameBuffer(FrameBuffer&& other);

		FrameBuffer& operator= (const FrameBuffer& other);
		FrameBuffer& operator= (FrameBuffer&& other);

		float* GetData() const;

		unsigned int GetWidth() const;
		unsigned int GetHeight() const;

		void SaveAsTga(const std::string& fileName) const;

	private:

		unsigned int m_Width;
		unsigned int m_Height;

		float* m_Data;

		void CopyFrameBuffer(const FrameBuffer& frameBuffer);
		void MoveFrameBuffer(FrameBuffer&& other);

		void Allocate();
		void Copy(const FrameBuffer& other);

		void FreeMemory();
	};
}