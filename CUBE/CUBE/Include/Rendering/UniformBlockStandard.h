#pragma once

#include <cstdint>
#include <vector>
#include <GL\glew.h>

#include "..\Math\Matrix4.h"
#include "..\Math\PerspectiveMatrix.h"
#include "..\Math\OrthoMatrix.h"
#include "..\Math\Vector4.h"
#include "..\Math\Vector3.h"
#include "..\Math\Vector2.h"
#include "..\Math\Color.h"

#include <memory>


/**
* Abstracts manipulating GLSL uniform buffer blocks.
*/
class FUniformBlock
{
public:
	/**
	* Retrieve the size of a uniform block that resides
	* in the currently active shader program.
	* @param BlockName - Name of the uniform block.
	* @param SizeOut - Location to put the block size.
	*/
	static void GetBlockSize(const char* BlockName, GLint& SizeOut);

public:
	/**
	* Ctor
	* Construct a std140 uniform buffer object.
	* If the binding index of the uniform block is know, use the appropriate constructor.
	* @param BlockName - The name of the uniform block.
	* @param BindingIndexToSet - The binding index that will be set for this block.
	* @param BlockSize - The size in bytes of the uniform block.
	*/
	FUniformBlock(const char* BlockName, GLuint BindingIndexToSet, const uint32_t BlockSize);

	/**
	* Ctor
	* Construct a std140 uniform buffer object.
	* @param BindingIndex - The binding index of the uniform block
	* @param BlockSize - The size in bytes of the uniform block.
	*/
	FUniformBlock(const uint32_t BindingIndex, const uint32_t BlockSize);

	/**
	* Dtor
	* Deletes the buffer bound to this object.
	*/
	~FUniformBlock();

	/**
	* Retrieve a pointer to the buffer held by this object.
	* @param Access - Type of access required for the memory.
	* @return The pointer is returned in a unique_ptr object with a custom deleter
	* that unbinds and unmappeds this buffer.
	*/
	void* MapBuffer(GLenum Access) const;

	/**
	* Add data to the buffer at a specific offset within the uniform block.
	* Data layout must correspond with: https://www.opengl.org/registry/specs/ARB/uniform_buffer_object.txt
	* @param DataOffset - Position of the data in the uniform block.
	* @param Data - Data to place in the buffer
	* @param DataSize - Size, in bytes, of the data to be placed.
	*/
	void SetData(const uint32_t DataOffset, const uint8_t* Data, const uint32_t DataSize);

	template <typename T>
	/**
	* Adds data to the buffer at a specific offset within the uniform block from a
	* specific object type. Classes that wish to use this function must implement a
	* specialization of this function template.
	* @param Data - Data to place in the buffer
	* @param Type - Object to copy data from.
	*/
	void SetData(const uint32_t DataOffset, const T& Type);

	GLuint GetID() const { return mBufferID; }

private:
	GLuint mBufferID; // ID of the buffer object bound to this uniform block
};

template <>
inline void FUniformBlock::SetData<float>(const uint32_t DataOffset, const float& Type)
{
	SetData(DataOffset, (uint8_t*)&Type, sizeof(float));
}

template <>
inline void FUniformBlock::SetData<FMatrix4>(const uint32_t DataOffset, const FMatrix4& Type)
{
	SetData(DataOffset, (uint8_t*)&Type.M[0][0], sizeof(FMatrix4));
}

template <>
inline void FUniformBlock::SetData<FPerspectiveMatrix>(const uint32_t DataOffset, const FPerspectiveMatrix& Type)
{
	SetData(DataOffset, (uint8_t*)&Type.M[0][0], sizeof(FPerspectiveMatrix));
}

template <>
inline void FUniformBlock::SetData<FOrthoMatrix>(const uint32_t DataOffset, const FOrthoMatrix& Type)
{
	SetData(DataOffset, (uint8_t*)&Type.M[0][0], sizeof(FOrthoMatrix));
}

template <>
inline void FUniformBlock::SetData<LookAtMatrix>(const uint32_t DataOffset, const LookAtMatrix& Type)
{
	SetData(DataOffset, (uint8_t*)&Type.M[0][0], sizeof(LookAtMatrix));
}

template <>
inline void FUniformBlock::SetData<Vector2f>(const uint32_t DataOffset, const Vector2f& Type)
{
	SetData(DataOffset, (uint8_t*)&Type.x, 8);
}

template <>
inline void FUniformBlock::SetData<Vector2ui>(const uint32_t DataOffset, const Vector2ui& Type)
{
	SetData(DataOffset, (uint8_t*)&Type.x, 8);
}

template <>
inline void FUniformBlock::SetData<Vector3f>(const uint32_t DataOffset, const Vector3f& Type)
{
	SetData(DataOffset, (uint8_t*)&Type.x, 12);
}

template <>
inline void FUniformBlock::SetData<Vector3i>(const uint32_t DataOffset, const Vector3i& Type)
{
	SetData(DataOffset, (uint8_t*)&Type.x, 12);
}

template <>
inline void FUniformBlock::SetData<Vector4f>(const uint32_t DataOffset, const Vector4f& Type)
{
	SetData(DataOffset, (uint8_t*)&Type.x, 16);
}

template <>
inline void FUniformBlock::SetData<Vector4i>(const uint32_t DataOffset, const Vector4i& Type)
{
	SetData(DataOffset, (uint8_t*)&Type.x, 16);
}

template <>
inline void FUniformBlock::SetData<FColor>(const uint32_t DataOffset, const FColor& Type)
{
	SetData(DataOffset, (uint8_t*)(&Type.R), 16);
}
