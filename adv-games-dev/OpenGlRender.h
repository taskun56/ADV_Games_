#pragma once

#include "../include/GL/glew.h"
#include <vector>
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"
#include "../include/assimp/Importer.hpp"
#include "../include/assimp/scene.h"
#include "../include/assimp/postprocess.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>

namespace GFX
{
	struct Shader
	{
		std::string ShaderName;
		void *Data;
	};

	struct Mesh
	{
		std::vector<glm::vec3> Position;
		std::vector<glm::vec3> Normal;
		std::vector<glm::vec2> TexCoords;
		std::vector<glm::vec4> colour;

		void *Data;
		std::vector<unsigned int> indices;
	};

	struct opengl_shader
	{
		GLuint vert;
		GLuint frag;
		GLuint program;
	};

	struct opengl_mesh
	{
		// The primitive geometry type used by the geometry
		GLenum type;
		// The OpenGL ID of the vertex array object
		GLuint vao;
		// The OpenGL IDs of the buffers used within the vertex array object
		std::map<GLuint, GLuint> buffers;
		// The OpenGL ID of the index buffer
		GLuint index_buffer;
		bool has_indices;
		// The number of vertices in the geometry
		// GLuint vertices;
		// The number of indices in the index buffer
		GLuint indice_count;
		GLuint vertex_count;
	};

	enum BUFFERS
	{
		POSITION_BUFFER = 0,
		COLOUR_BUFFER = 1,
		NORMAL_BUFFER = 2,
		TEXTURE_COORDS_0 = 10
	};

	////Random Colour used if no other colour thingy found
	//const static glm::vec4 RandC[3] = { glm::vec4(0.533f, 0.898f, 0.231, 1.0f), glm::vec4(1.0f, 0.698f, 0.259, 1.0f),
	//glm::vec4(0.412f, 0.227f, 0.702f, 1.0f) };

	bool add_buffer(opengl_mesh &om, const void *data, GLuint stride, GLuint size, GLuint index, GLenum buffer_type);

	bool add_index_buffer(opengl_mesh &om, const std::vector<GLuint> &buffer);

	//TexCoords
	bool add_buffer(opengl_mesh &om, const std::vector<glm::vec2> &buffer, GLuint index, GLenum buffer_type = GL_STATIC_DRAW);

	//Position and Normals
	bool add_buffer(opengl_mesh &om, const std::vector<glm::vec3> &buffer, GLuint index, GLenum buffer_type = GL_STATIC_DRAW);

	//Colours
	bool add_buffer(opengl_mesh &om, const std::vector<glm::vec4> &buffer, GLuint index, GLenum buffer_type = GL_STATIC_DRAW);

	bool FindFile(std::string &path);

	bool ReadFile(const std::string &filename, std::string &content);

	void LoadShader(Shader *shade, const std::string &vert, const std::string &frag);

	void LoadModel(Mesh *msh);

	void GLRender(Mesh *msh, Shader *shade, glm::mat4 MVP);

	Shader *GetShaders(const std::string &file);

	Mesh *GetMesh(const std::string &file);

};