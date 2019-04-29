#include <iostream>

#include <glad/glad.h>

#include "Canvas.h"
#include "Window.h"


Canvas::Canvas()
{
	//use fbo as render target instead of the default framebuffer
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//use texture as color attachment
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	width = Window::width;
	height = Window::height;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	//use rbo as depth-stencil attachment
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Failed to create canvas" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	float vertices[] = {
		-1.0, -1.0,  0.0, 0.0,
		-1.0,  1.0,  0.0, 1.0,
		 1.0, -1.0,  1.0, 0.0,
		 1.0,  1.0,  1.0, 1.0,
		 1.0, -1.0,  1.0, 0.0,
		-1.0,  1.0,  0.0, 1.0,
	};

	//Init vertex data
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Canvas::copyToScreen(ShaderProgram * program)
{
	glDisable(GL_BLEND);

	glClear(GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	program->use();
	program->setUniformInt("image", 0);

	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glFlush();

	glEnable(GL_BLEND);
}

void Canvas::use()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, width, height);
}

void Canvas::unuse()
{
	//make the default framebuffer active again
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_FRONT);
	glViewport(0, 0, Window::width, Window::height);
}

void Canvas::resize()
{
	glBindTexture(GL_TEXTURE_2D, texture);

	width = Window::width;
	height = Window::height;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Canvas::changeVisibleArea(std::vector<float> vertices)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
}
