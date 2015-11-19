#include "FrameBuffer.h"
#include <MemoryAllocatorAligned.h>
#include <cassert>

namespace Raytracer
{
	using namespace Core;

	FrameBuffer::FrameBuffer(unsigned int width, unsigned int height)
	{
		m_Width = width;
		m_Height = height;
		m_Data = nullptr;

		Allocate();
	}

	FrameBuffer::FrameBuffer(const FrameBuffer& other)
	{
		m_Data = nullptr;
		CopyFrameBuffer(other);
	}

	FrameBuffer::FrameBuffer(FrameBuffer&& other)
	{
		m_Data = nullptr;
		MoveFrameBuffer(std::forward<FrameBuffer>(other));
	}

	FrameBuffer::~FrameBuffer()
	{
		FreeMemory();
	}

	FrameBuffer& FrameBuffer::operator=(const FrameBuffer& other)
	{
		CopyFrameBuffer(other);
		return *this;
	}

	FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other)
	{
		MoveFrameBuffer(std::forward<FrameBuffer>(other));
		return *this;
	}

	void FrameBuffer::CopyFrameBuffer(const FrameBuffer& other)
	{
		m_Width = other.m_Width;
		m_Height = other.m_Height;

		Allocate();
		Copy(other);
	}

	void FrameBuffer::MoveFrameBuffer(FrameBuffer&& other)
	{
		FreeMemory();

		m_Width = other.m_Width;
		m_Height = other.m_Height;
		m_Data = other.m_Data;
		other.m_Data = nullptr;
	}

	void FrameBuffer::Allocate()
	{
		FreeMemory();

		m_Data = reinterpret_cast<float*>(MemoryAllocatorAligned::Allocate(m_Width * m_Height * 4 * sizeof(float)));
	}

	void FrameBuffer::FreeMemory()
	{
		if (nullptr != m_Data)
			MemoryAllocatorAligned::Deallocate(reinterpret_cast<void*>(m_Data));
	}

	void FrameBuffer::Copy(const FrameBuffer& other)
	{
		assert(other.m_Width == m_Width);
		assert(other.m_Height == m_Height);
		assert(nullptr != m_Data);

		memcpy(reinterpret_cast<void*>(m_Data),
			   reinterpret_cast<void*>(other.m_Data),
			   m_Width * m_Height * 4 * sizeof(float));
	}

	float* FrameBuffer::GetData() const
	{
		return m_Data;
	}

	unsigned int FrameBuffer::GetWidth() const
	{
		return m_Width;
	}

	unsigned int FrameBuffer::GetHeight() const
	{
		return m_Height;
	}

	void FrameBuffer::SaveAsTga(const std::string& fileName) const
	{
		using namespace std;

		FILE* file;
		fopen_s(&file, fileName.c_str(), "wb");
		if (nullptr == file)
		{
		//	cout << "Error: Unable to create file [" << filePath << "] for writing!" << endl;
			exit(1);
		}

		uint8_t idLength = 0;	// Identifies the number of bytes contained in the imageId field.
		fwrite(&idLength, sizeof(idLength), 1, file);

		uint8_t colorMapType = 0;	// Identifies the type of color map (if any) used by the image. This should be 0;
		fwrite(&colorMapType, sizeof(colorMapType), 1, file);

		uint8_t imageType = 2;	// We don't currently encode RLE.
		fwrite(&imageType, sizeof(imageType), 1, file);

		uint8_t colorMapSpecification[5] = { 0, 0, 0, 0, 0 };
		fwrite(colorMapSpecification, sizeof(colorMapSpecification), 1, file);

		// Image specification field.
		uint16_t xOrigin = 0;
		uint16_t yOrigin = 0;
		uint16_t imageWidth = m_Width;
		uint16_t imageHeight = m_Height;
		uint8_t pixelDepth = 32;
		uint8_t imageDescriptor = 8;

		fwrite(&xOrigin, sizeof(xOrigin), 1, file);
		fwrite(&yOrigin, sizeof(yOrigin), 1, file);
		fwrite(&imageWidth, sizeof(imageWidth), 1, file);
		fwrite(&imageHeight, sizeof(imageHeight), 1, file);
		fwrite(&pixelDepth, sizeof(pixelDepth), 1, file);
		fwrite(&imageDescriptor, sizeof(imageDescriptor), 1, file);

		// Now we can write out the image itself.
		size_t imageDataLength = m_Width * m_Height * 4;
		size_t imageStride = m_Width * 4;

		// Convert data.
		auto buffer = new uint8_t[imageDataLength];
		auto currentPixel = buffer;
		for (unsigned y = 0; y < m_Height; y++)
		{
			auto currentBufferPixel = m_Data + (m_Height - 1 - y) * imageStride;

			for (unsigned i = 0; i < 4; i++)
			if (currentBufferPixel[i] > 1.0f)
				currentBufferPixel[i] = 1.0f;

			for (unsigned x = 0; x < m_Width; x++)
			{
				// Need to transpose values as tga format is BGRA in memory.
				currentPixel[0] = (uint8_t)(currentBufferPixel[2] * 255.0f);
				currentPixel[1] = (uint8_t)(currentBufferPixel[1] * 255.0f);
				currentPixel[2] = (uint8_t)(currentBufferPixel[0] * 255.0f);
				currentPixel[3] = (uint8_t)(currentBufferPixel[3] * 255.0f);

				currentPixel += 4;
				currentBufferPixel += 4;
			}
		}

		fwrite(buffer, imageDataLength, 1, file);

		fclose(file);

		delete[] buffer;
	}
}