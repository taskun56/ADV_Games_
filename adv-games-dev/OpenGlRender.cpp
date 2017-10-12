#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>




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


bool add_buffer(opengl_mesh &om, const void *data, GLuint stride, GLuint size, GLuint index, GLenum buffer_type)
{
	// Check that index is viable
	assert(index < 5);
	// Check that buffer is not empty
	assert(size > 0);
	// Check if geometry initialised
	if (om.vao == 0)
	{
		glGenVertexArrays(1, &om.vao);
	}
	// Now add buffer to the vertex array object.  Bind the vertex array object
	glBindVertexArray(om.vao);
	// Generate buffer with OpenGL
	GLuint id;
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	// Set the buffer data
	glBufferData(GL_ARRAY_BUFFER, size, data, buffer_type);
	// Set the vertex pointer and enable
	glVertexAttribPointer(index, stride, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(index);


	// Add buffer to map
	om.buffers[index] = id;
	return true;
}


bool add_index_buffer(opengl_mesh &om, const std::vector<GLuint> &buffer)
{
	// Check that buffer is not empty
	assert(buffer.size() > 0);
	// Check if vertex array object is valid
	assert(om.vao != 0);
	// Set indices to buffer size
	om.indice_count = static_cast<GLuint>(buffer.size());
	// Bind vertex array object
	glBindVertexArray(om.vao);
	// Add buffer
	glGenBuffers(1, &om.index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, om.index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer.size() * sizeof(GLuint), &buffer[0], GL_STATIC_DRAW);

	return true;
}

//TexCoords
bool add_buffer(opengl_mesh &om, const std::vector<glm::vec2> &buffer, GLuint index, GLenum buffer_type = GL_STATIC_DRAW)
{
	return add_buffer(om, &buffer[0], 2, static_cast<GLuint>(buffer.size() * sizeof(glm::vec2)), index, buffer_type);
}

//Position and Normals
bool add_buffer(opengl_mesh &om, const std::vector<glm::vec3> &buffer, GLuint index, GLenum buffer_type = GL_STATIC_DRAW)
{
	return add_buffer(om, &buffer[0], 3, static_cast<GLuint>(buffer.size() * sizeof(glm::vec3)), index, buffer_type);
}
//Colours
bool add_buffer(opengl_mesh &om, const std::vector<glm::vec4> &buffer, GLuint index, GLenum buffer_type = GL_STATIC_DRAW)
{
	return add_buffer(om, &buffer[0], 4, static_cast<GLuint>(buffer.size() * sizeof(glm::vec4)), index, buffer_type);
}


bool FindFile(std::string &path)
{
	//Directories to check
	static const std::string filedirs[] = { "" };
	for (const auto s : filedirs)
	{
		std::ifstream inFile((s + path).c_str(), std::ifstream::in);
		if (inFile.good())
		{
			path = s + path;
			return true;
		}
	}
	return false;
}

bool ReadFile(const std::string &filename, std::string &content)
{
	std::ifstream file(filename, std::ios_base::in);
	// Check that file exists.  If not, return false
	if (file.bad())
	{
		return false;
	}
	// File is good.  Read contents
	std::stringstream buffer;
	buffer << file.rdbuf();

	// Get contents from the file
	content = buffer.str();

	// Close file and return true
	file.close();
	return true;

}




void LoadShader(Shader *shade, const std::string &vert, const std::string &frag)
{
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	printf("Compiling shader : %s\n", vert);
	auto *VertexSourcePointer = vert.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, 0);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	printf("Compiling shader : %s\n", frag);
	auto *FragmentSourcePointer = frag.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, 0);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	opengl_shader *sh = new opengl_shader();

	sh->frag = FragmentShaderID;
	sh->vert = VertexShaderID;
	sh->program = glCreateProgram();

	glAttachShader(sh->program, VertexShaderID);
	glAttachShader(sh->program, FragmentShaderID);

	glLinkProgram(sh->program);

	// Check the program
	glGetProgramiv(sh->program, GL_LINK_STATUS, &Result);
	glGetProgramiv(sh->program, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(sh->program, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
		glDeleteProgram(sh->program);
	}

	shade->Data = sh;

}

void LoadModel(Mesh *msh)
{
	opengl_mesh *opmesh = new opengl_mesh();
	msh->Data = opmesh;
	opmesh->type = GL_TRIANGLES;
	// Add the buffers to the geometry
	add_buffer(*opmesh, msh->Position, BUFFERS::POSITION_BUFFER);
	add_buffer(*opmesh, msh->colour, BUFFERS::COLOUR_BUFFER);

	if (msh->Normal.size() != 0)
	{
		add_buffer(*opmesh, msh->Normal, BUFFERS::NORMAL_BUFFER);

	}
	if (msh->TexCoords.size() != 0)
	{
		add_buffer(*opmesh, msh->TexCoords, BUFFERS::TEXTURE_COORDS_0);
	}
	if (msh->indices.size() != 0)
	{
		add_index_buffer(*opmesh, msh->indices);
		opmesh->has_indices = true;
	}
	else
	{
		opmesh->vertex_count = msh->Position.size();
	}


}


void GLRender(Mesh *msh, Shader *shade, glm::mat4 MVP)
{
	opengl_mesh *omash = static_cast<opengl_mesh *>(msh->Data);
	opengl_shader *sh = static_cast<opengl_shader *>(shade->Data);

	glUseProgram(sh->program);

	auto id = glGetUniformLocation(sh->program, "MVP");
	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(id, 1, GL_FALSE, value_ptr(MVP));

	// Bind the vertex array object for the
	glBindVertexArray(omash->vao);


	// If there is an index buffer then use to render
	if (omash->has_indices)
	{
		// Bind index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, omash->index_buffer);

		// Draw elements
		glDrawElements(omash->type, omash->indice_count, GL_UNSIGNED_INT, nullptr);
	}
	else
	{
		// Draw arrays
		glDrawArrays(omash->type, 0, omash->vertex_count);
	}
}



Shader *GetShaders(const std::string &file)
{
	//vert and frag shader
	std::string vert = file + ".vert";
	std::string frag = file + ".frag";
	std::string vert_file;
	std::string frag_file;

	//check if vert could be found
	if (!FindFile(vert) || !ReadFile(vert, vert_file))
	{
		std::cerr << "ERROR - could not find file " << vert << std::endl;
		throw std::runtime_error("Error loading vert file");
	}
	//check if frag could be found
	if (!FindFile(frag) || !ReadFile(frag, frag_file))
	{
		std::cerr << "ERROR - could not find file " << frag << std::endl;
		throw std::runtime_error("Error loading frag file");
	}

	auto shade = new Shader();
	//new shader  name = file
	shade->ShaderName = file;

	LoadShader(shade, vert_file, frag_file);

	return shade;
}

Mesh *GetMesh(const std::string &file)
{
	//look for file
	std::string path = file;
	if (!FindFile(path))
	{
		std::cerr << "ERROR - could not find file " << path << std::endl;
		throw std::runtime_error("Error loading model file");
	}

	// Create model importer
	Assimp::Importer model_importer;
	// Read in the model data
	const aiScene* sc = model_importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);

	//new mesh 
	Mesh *nw = new Mesh();

	unsigned int vertex_begin = 0;
	//loop through the meshes in the scene
	for (unsigned int i = 0; i < sc->mNumMeshes; ++i)
	{
		//array of meshes
		auto mesh = sc->mMeshes[i];

		//loop through the vertices in the mesh
		for (unsigned int j = 0; j < mesh->mNumVertices; ++j)
		{

			auto tempPos = mesh->mVertices[j];

			//Set the positions
			tempPos.x = mesh->mVertices[j].x;
			tempPos.y = mesh->mVertices[j].y;
			tempPos.z = mesh->mVertices[j].z;

			nw->Position.push_back(glm::vec3(tempPos.x, tempPos.y, tempPos.z));

			if (mesh->HasVertexColors(0))
			{
				auto tempCol = mesh->mColors[0][j];
				//set colour
				tempCol.r = mesh->mColors[0][j].r;
				tempCol.g = mesh->mColors[0][j].g;
				tempCol.b = mesh->mColors[0][j].b;
				tempCol.a = mesh->mColors[0][j].a;

				nw->colour.push_back(glm::vec4(tempCol.r, tempCol.g, tempCol.b, tempCol.a));
			}
			else
			{

				nw->colour.push_back(glm::vec4(0.0f, 0.10f, 0.23, 1.0f));

			}
			//see if mesh has normals
			if (mesh->HasNormals())
			{
				auto tempNorm = mesh->mNormals[j];
				//set normals
				tempNorm.x = mesh->mNormals[j].x;
				tempNorm.y = mesh->mNormals[j].y;
				tempNorm.z = mesh->mNormals[j].z;

				nw->Normal.push_back(glm::vec3(tempNorm.x, tempNorm.y, tempNorm.z));

			}
			//see if mesh has tex coords
			if (mesh->HasTextureCoords(0))
			{
				auto tempTex = mesh->mTextureCoords[0][j];
				//set tex coords
				tempTex.x = mesh->mTextureCoords[0][j].x;
				tempTex.y = mesh->mTextureCoords[0][j].y;

				nw->TexCoords.push_back(glm::vec2(tempTex.x, tempTex.y));
			}


		}
		//see if mesh has faces
		if (mesh->HasFaces())
		{
			//loop through faces in this mesh
			for (unsigned int j = 0; j < mesh->mNumFaces; ++j)
			{
				//Get the face
				auto face = mesh->mFaces[j];
				//Add the indices of the face to the vector
				for (unsigned int k = 0; k < face.mNumIndices; ++k)
				{
					nw->indices.push_back(vertex_begin + face.mIndices[k]);
				}
			}
		}

		vertex_begin += mesh->mNumVertices;
	}
	//pass mesh into loadModel
	LoadModel(nw);


	return nw;
}


