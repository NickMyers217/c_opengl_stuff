#pragma once

#include <stdlib.h>
#include <memory.h>
#include <glad\glad.h>


struct BaseModel {
	GLuint vaoId, vboId, bufferLength;
	GLfloat * buffer;
	GLsizei bufferSize;
};


void modelInit(BaseModel * model, const GLfloat * buffer, unsigned int bufferLength);
void modelRender(BaseModel * model);
void modelFree(BaseModel * model);


void modelInit(BaseModel * model, const GLfloat * buffer, unsigned int bufferLength)
{
	glGenVertexArrays(1, &model->vaoId);
	glGenBuffers(1, &model->vboId);

	model->bufferLength = bufferLength;
	model->bufferSize = model->bufferLength * sizeof(GLfloat);
	model->buffer = (GLfloat *)malloc(model->bufferSize);
	memcpy(model->buffer, buffer, model->bufferSize);

	glBindVertexArray(model->vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, model->vboId);
	glBufferData(GL_ARRAY_BUFFER, model->bufferSize, model->buffer, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void *)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void *)(6 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);
}

void modelRender(BaseModel * model)
{
	glBindVertexArray(model->vaoId);
	glDrawArrays(GL_TRIANGLES, 0, model->bufferLength);
}

void modelFree(BaseModel * model)
{
	free(model->buffer);
	glDeleteVertexArrays(1, &model->vaoId);
	glDeleteBuffers(1, &model->vboId);
}