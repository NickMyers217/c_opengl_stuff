#pragma once

void generatePlaneVertices(float * vertices, int width, int length)
{
	// right now we are hard coded at 8 floats per vertex and 6 vertices per cell of the plane
	const int VERTICES_PER_CELL = 6;
	const int FLOATS_PER_VERTEX = 8;
	const int FLOATS_PER_CELL = VERTICES_PER_CELL * FLOATS_PER_VERTEX;
	const float CELL_VERTICES[FLOATS_PER_CELL] = {
		// Position        // Normal          // UV
		0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,

		0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		1.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
		1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
	};
	// Use these offsets to center the plane on the origin, instead of having it's corner there
	const float Z_OFF = (float)length / 2.0f * -1.0f;
	const float X_OFF = (float)width / 2.0f * -1.0f;

	for (unsigned int z = 0; z < length; z++)
	{
		for (unsigned int x = 0; x < width; x++)
		{

			int curFloat = z * FLOATS_PER_CELL * width + x * FLOATS_PER_CELL;
			for (unsigned int v = 0; v < VERTICES_PER_CELL; v++)
			{
				for (unsigned int f = 0; f < FLOATS_PER_VERTEX; f++)
				{
					int curVertexFloat = v * FLOATS_PER_VERTEX + f;
					float value = CELL_VERTICES[curVertexFloat];
					if (f == 0) // Tranlate the x
					{
					    value += (float)x + X_OFF;
					}
					if (f == 2) // Translate the z
					{
						value += (float)z + Z_OFF;
					}
					vertices[curFloat + curVertexFloat] = value;
				}
			}
		}
	}
}
