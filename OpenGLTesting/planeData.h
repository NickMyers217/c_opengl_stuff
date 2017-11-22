#pragma once

#include "Mesh.h"

#include <vector>
#include <glm\glm.hpp>


using namespace std;

void generatePlaneData(vector<Vertex>& vertices, vector<unsigned int>& indices, unsigned int width, unsigned int length)
{
	// right now we are hard coded at 8 floats per vertex and 6 vertices per cell of the plane
	const int VERTICES_PER_CELL = 6;
	const Vertex CELL_VERTICES[VERTICES_PER_CELL] = {
		Vertex {
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec2(0.0f, 0.0f)
		},
		Vertex {
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec2(0.0f, 1.0f)
		},
		Vertex {
			glm::vec3(1.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f)
		},
		Vertex {
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec2(0.0f, 0.0f)
		},
		Vertex {
			glm::vec3(1.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f)
		},
		Vertex {
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec2(1.0f, 0.0f)
		}
	};
	// Use these offsets to center the plane on the origin, instead of having it's corner there
	const float Z_OFF = (float)length / 2.0f * -1.0f;
	const float X_OFF = (float)width / 2.0f * -1.0f;

	unsigned int curIndex = 0;

	for (unsigned int z = 0; z < length; z++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			for (unsigned int v = 0; v < VERTICES_PER_CELL; v++)
			{
				Vertex vert = CELL_VERTICES[v];
				vert.Position += glm::vec3((float)x + X_OFF, 0.0f, (float)z + Z_OFF);
				vertices.push_back(vert);
				indices.push_back(curIndex++);
			}
		}
	}
}
