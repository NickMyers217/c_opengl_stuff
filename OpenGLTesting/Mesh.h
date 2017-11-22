#pragma once

#include <string>
#include <vector>
#include <glad\glad.h>
#include <glm\glm.hpp>

#include "texture.h"
#include "ShaderProgram.h"

using namespace std;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};


class Mesh
{
public:
	vector<Vertex> Vertices;
	vector<GLuint> Indices;
	vector<Texture> Textures;

	Mesh(vector<Vertex>& vertices, vector<Texture>& textures);
	Mesh(vector<Vertex>& vertices, vector<GLuint>& indices, vector<Texture>& textures);

	~Mesh();

	void Draw(ShaderProgram& shader);
private:
	GLuint VAO, VBO, EBO;
		
	void InitializeMesh();
};


Mesh::Mesh(vector<Vertex>& vertices, vector<Texture>& textures)
{
	Vertices = vertices;
	for (GLuint i = 0; i < vertices.size(); i++)
		Indices.push_back(i);
	Textures = textures;

	InitializeMesh();
}

Mesh::Mesh(vector<Vertex>& vertices, vector<GLuint>& indices, vector<Texture>& textures)
{
	Vertices = vertices;
	Indices = indices;
	Textures = textures;

	InitializeMesh();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Mesh::InitializeMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(GLuint), &Indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, Normal)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, TexCoords)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Mesh::Draw(ShaderProgram& shader)
{
	GLuint diffuseCount = 1;
	GLuint specularCount = 1;
	int textureSlot = 0;

	for (int i = 0; i < Textures.size(); i++)
	{
		string number;
		TextureType type = Textures[i].type;
		std::string name = "";

		switch (type)
		{
		case DIFFUSE:
			name = "texture_diffuse";
			number = std::to_string(diffuseCount++);
			shader.SetUniform(("material." + name + number).c_str(), textureSlot);
			textureUse(&Textures[i], textureSlot++);
			break;
		case SPECULAR:
			name = "texture_specular";
			number = std::to_string(specularCount++);
			shader.SetUniform(("material." + name + number).c_str(), textureSlot);
			textureUse(&Textures[i], textureSlot++);
			break;
		case DIFFUSE_AND_SPECULAR:
			string numberDiff = std::to_string(diffuseCount++);
			string numberSpec = std::to_string(specularCount++);
			shader.SetUniform(("material.texture_diffuse" + numberDiff).c_str(), textureSlot);
			textureUse(&Textures[i], textureSlot++);
			shader.SetUniform(("material.texture_specular" + numberSpec).c_str(), textureSlot);
			textureUse(&Textures[i], textureSlot++);
			break;
		}
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
