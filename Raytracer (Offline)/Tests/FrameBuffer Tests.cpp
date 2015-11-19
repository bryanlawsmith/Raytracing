#include <gtest\gtest.h>
#include "..\FrameBuffer.h"

using namespace Raytracer;

TEST(FrameBuffer, FrameBuffer_Allocates_Enough_Memory)
{
	const unsigned int width = 16;
	const unsigned int height = 16;
	const unsigned int numElements = width * height * 4;

	auto frameBuffer = new FrameBuffer(width, height);
	auto bufferData = frameBuffer->GetData();

	// Ensure correct dimensions.
	ASSERT_EQ(frameBuffer->GetWidth(), width);
	ASSERT_EQ(frameBuffer->GetHeight(), height);

	// Ensure that memory has been allocated.
	ASSERT_NE(bufferData, nullptr);
	
	// Ensure that memory has been allocated for all elements.
	for (int i = 0; i < numElements; i++)
		ASSERT_NO_FATAL_FAILURE(bufferData[i] = (float)i);
}

TEST(FrameBuffer, FrameBuffer_Allocates_Memory_Aligned_To_16_Byte_Boundary)
{
	const unsigned int width = 16;
	const unsigned int height = 16;

	auto frameBuffer = new FrameBuffer(width, height);
	auto bufferData = frameBuffer->GetData();
	
	// Ensure correct dimensions.
	ASSERT_EQ(frameBuffer->GetWidth(), width);
	ASSERT_EQ(frameBuffer->GetHeight(), height);

	// Ensure that memory has been allocated.
	ASSERT_NE(bufferData, nullptr);

	// Ensure that memory is properly aligned.
	ASSERT_EQ(reinterpret_cast<int>(bufferData) & 15, 0);
}

TEST(FrameBuffer, FrameBuffer_Copies_Correctly)
{
	const unsigned int width = 16;
	const unsigned int height = 16;
	const unsigned int numElements = width * height * 4;

	auto frameBuffer = new FrameBuffer(width, height);
	auto frameBufferData = frameBuffer->GetData();
	for (int i = 0; i < numElements; i++)
		frameBufferData[i] = (float)i;

	// Ensure that the copy constructor works as desired.
	{
		auto frameBufferCopy = new FrameBuffer(*frameBuffer);

		// Ensure correct dimensions.
		ASSERT_EQ(frameBufferCopy->GetWidth(), frameBuffer->GetWidth());
		ASSERT_EQ(frameBufferCopy->GetHeight(), frameBuffer->GetHeight());

		auto frameBufferCopyData = frameBufferCopy->GetData();

		// Ensure that the memory blocks are different.
		ASSERT_NE(frameBufferData, frameBufferCopyData);

		// Ensure that the memory has been allocated correctly.
		for (int i = 0; i < numElements; i++)
			ASSERT_EQ(frameBufferData[i], frameBufferCopyData[i]);

		delete frameBufferCopy;
	}

	// Ensure that the copy assignment works as desired.
	{
		auto frameBufferAssignCopy = new FrameBuffer(1, 1);
		*frameBufferAssignCopy = *frameBuffer;

		// Ensure correct dimensions.
		ASSERT_EQ(frameBufferAssignCopy->GetWidth(), frameBuffer->GetWidth());
		ASSERT_EQ(frameBufferAssignCopy->GetHeight(), frameBuffer->GetHeight());

		auto frameBufferAssignCopyData = frameBufferAssignCopy->GetData();

		// Ensure that the memory blocks are different.
		ASSERT_NE(frameBufferData, frameBufferAssignCopyData);

		// Ensure that the memory has been allocated correctly.
		for (int i = 0; i < numElements; i++)
			ASSERT_EQ(frameBufferData[i], frameBufferAssignCopyData[i]);

		delete frameBufferAssignCopy;
	}

	delete frameBuffer;
}

TEST(FrameBuffer, FrameBuffer_Moves_Correctly)
{
	const unsigned int width = 16;
	const unsigned int height = 16;
	const unsigned int numElements = width * height * 4;

	// Ensure that the move constructor works as desired.
	{
		auto frameBuffer = new FrameBuffer(width, height);
		auto frameBufferData = frameBuffer->GetData();
		for (int i = 0; i < numElements; i++)
			frameBufferData[i] = (float)i;

		auto frameBufferMove = new FrameBuffer(std::move(*frameBuffer));

		// Ensure correct dimensions.
		ASSERT_EQ(frameBufferMove->GetWidth(), frameBuffer->GetWidth());
		ASSERT_EQ(frameBufferMove->GetHeight(), frameBuffer->GetHeight());

		// Ensure that the moved memory block is the same.
		ASSERT_EQ(frameBufferMove->GetData(), frameBufferData);

		// Ensure that the old memory is set to null.
		ASSERT_EQ(frameBuffer->GetData(), nullptr);

		delete frameBuffer;
		delete frameBufferMove;
	}

	// Ensure that the move assignment works as desired.
	{
		auto frameBuffer = new FrameBuffer(width, height);
		auto frameBufferData = frameBuffer->GetData();
		for (int i = 0; i < numElements; i++)
			frameBufferData[i] = (float)i;

		auto frameBufferMove = new FrameBuffer(1, 1);
		*frameBufferMove = std::move(*frameBuffer);

		// Ensure correct dimensions.
		ASSERT_EQ(frameBufferMove->GetWidth(), frameBuffer->GetWidth());
		ASSERT_EQ(frameBufferMove->GetHeight(), frameBuffer->GetHeight());

		// Ensure that the moved memory block is the same.
		ASSERT_EQ(frameBufferMove->GetData(), frameBufferData);

		// Ensure that the old memory is set to null.
		ASSERT_EQ(frameBuffer->GetData(), nullptr);

		delete frameBuffer;
		delete frameBufferMove;
	}
}