#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

class Mesh
{
public:
	enum class eVertexType
	{
		POSITION,
		NORMAL,
		COLOR,
		TEXCOORD,
		TANGENT
	};

	struct BufferInfo
	{
		eVertexType type;
		GLuint vbo;
		size_t numElements;
		size_t elementSizeInBytes;
		GLvoid* data;
	};

public:
	Mesh();
	~Mesh();

	void Use();
	void Render();

	void BindVertexAttrib(GLuint attrib, eVertexType type);
	bool Load(const std::string& filename, bool createTangents = false);

public:

	static void CalculateTangent(glm::vec3& tangent, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec2& uv0, const glm::vec2& uv1, const glm::vec2& uv2, const glm::vec3& normal);
	static void CalculateNormal(glm::vec3& normal, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);

private:
	void AddBuffer(eVertexType type, size_t numElements, size_t elementSize, void* data);

private:
	GLuint m_vao = 0;
	int m_numVertices = 0;
	std::vector<BufferInfo> m_buffers;
};

