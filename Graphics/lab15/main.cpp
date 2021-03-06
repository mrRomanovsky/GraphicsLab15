#include<Windows.h>    
// first include Windows.h header file which is required    
#include<stdio.h>
#include "GL/glew.h"
#include<gl/GL.h>   // GL.h header file    
#include<gl/GLU.h> // GLU.h header file    
#include<gl/glut.h>  // glut.h header file from freeglut\include\GL folder    
#include<conio.h>
#include<math.h>
#include<string>
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <SOIL.h>
#include <glm/gtc/matrix_transform.hpp>

unsigned char* image;
GLuint texture1;
int width = 0, height = 0;
//1 - ��������
//2 - ������
//3 - Minnaert
//4 - Toon shading
//5 - ��� ���
//6 - Rim
int mode = 1;
GLuint LambertProgram, BlinnProgram, MinnaertProgram, ToonShadingProgram, AmiGoochProgram, RimProgram;
GLuint VAO;
//std::vector<GLushort> indices;
GLuint shader_program;
//transform
glm::mat4 model, viewProjection;
glm::mat3 normaltr;

float viewPosition[]{ 0,0,-50 };

//light
float light_angle = 0, light_pos = 0, light_rad = 50;
float light[]{ 0, 5, 0 };
float ambient[]{ 0.2f, 0.2f,0.2f,1.0f };
float diffuse[]{ 1.0f,1.0f,1.0f,1.0f };
float specular[]{ 1.0f,1.0f,1.0f,1.0f };
float attenuation[]{ 1.0f,0.0f,0.0f };

//tryig model
std::vector<glm::vec3> indexed_vertices;
std::vector<glm::vec2> indexed_uvs;
std::vector<glm::vec3> indexed_normals;
std::vector<unsigned short> indices;
GLuint vertexbuffer;
GLuint uvbuffer;
GLuint normalbuffer;
GLuint elementbuffer;

std::string objname = "dog.obj";
std::string objtex = "dog.jpg";
double obj_scale = 0.5;
float rotateX = -105, rotateY = 0, rotateZ = 180;


void makeTextureImage() {
	texture1 = SOIL_load_OGL_texture
	(
		"dog.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

std::string readfile(const char* path)
{
	std::string res = "";
	std::ifstream file(path);
	std::string line;
	getline(file, res, '\0');
	while (getline(file, line))
		res += "\n " + line;
	return res;
}

//1 - ��������
void initLambertShaders()
{
	std::string readed = readfile("vertex1.shader");
	const char* vsSource = readed.c_str();

	std::string readed2 = readfile("fragment1.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	LambertProgram = glCreateProgram();
	glAttachShader(LambertProgram, vShader);
	glAttachShader(LambertProgram, fShader);
	glLinkProgram(LambertProgram);
}

//2 - ������
void initBlinnShaders() {
	std::string readed = readfile("vertex2.shader");
	const char* vsSource = readed.c_str();

	std::string readed2 = readfile("fragment2.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	BlinnProgram = glCreateProgram();
	glAttachShader(BlinnProgram, vShader);
	glAttachShader(BlinnProgram, fShader);
	glLinkProgram(BlinnProgram);
}

//3 - Minnaert
void initMinnaertShaders()
{
	std::string readed = readfile("vertex3.shader");
	const char* vsSource = readed.c_str();

	std::string readed2 = readfile("fragment3.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	MinnaertProgram = glCreateProgram();
	glAttachShader(MinnaertProgram, vShader);
	glAttachShader(MinnaertProgram, fShader);
	glLinkProgram(MinnaertProgram);
}

//4 - Toon shading
void initToonShaders()
{
	std::string readed = readfile("vertex4.shader");
	const char* vsSource = readed.c_str();

	std::string readed2 = readfile("fragment4.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	ToonShadingProgram = glCreateProgram();
	glAttachShader(ToonShadingProgram, vShader);
	glAttachShader(ToonShadingProgram, fShader);
	glLinkProgram(ToonShadingProgram);

	int link_ok;
	glGetProgramiv(ToonShadingProgram, GL_LINK_STATUS, &link_ok);
}

//5 - ��� ���
void initAmiGoochShaders()
{
	std::string readed = readfile("vertex5.shader");
	const char* vsSource = readed.c_str();

	std::string readed2 = readfile("fragment5.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	AmiGoochProgram = glCreateProgram();
	glAttachShader(AmiGoochProgram, vShader);
	glAttachShader(AmiGoochProgram, fShader);
	glLinkProgram(AmiGoochProgram);

	int link_ok;
	glGetProgramiv(AmiGoochProgram, GL_LINK_STATUS, &link_ok);
}

//6 - Rim
void initRimShaders()
{
	std::string readed = readfile("vertex6.shader");
	const char* vsSource = readed.c_str();

	std::string readed2 = readfile("fragment6.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	RimProgram = glCreateProgram();
	glAttachShader(RimProgram, vShader);
	glAttachShader(RimProgram, fShader);
	glLinkProgram(RimProgram);

	int link_ok;
	glGetProgramiv(RimProgram, GL_LINK_STATUS, &link_ok);
}

void initShaders() {
	initLambertShaders();
	initBlinnShaders();
	initMinnaertShaders();
	initToonShaders();
	initAmiGoochShaders();
	initRimShaders();
	shader_program = LambertProgram;
}

void freeShader() {
	glUseProgram(0);
	glDeleteProgram(LambertProgram);
	glDeleteProgram(BlinnProgram);
	glDeleteProgram(MinnaertProgram);
	glDeleteProgram(ToonShadingProgram);
	glDeleteProgram(AmiGoochProgram);
	glDeleteProgram(RimProgram);
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glEnable(GL_DEPTH_TEST);
}

void reshape(int w, int h)
{
	width = w; height = h;
	glViewport(0, 0, w, h);
}

struct VertexInfo
{
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
	bool operator<(const VertexInfo that) const
	{
		return memcmp((void*)this, (void*)&that, sizeof(VertexInfo)) > 0;
	};
}; 

void loadOBJ(const std::string & path, std::vector<glm::vec3> & out_vertices, std::vector<glm::vec2> & out_uvs, std::vector<glm::vec3> & out_normals)
{
	std::vector<unsigned int> vertex_indices, uv_indices, normal_indices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	std::ifstream infile(path);
	std::string line;
	while (getline(infile, line))
	{
		std::stringstream ss(line);
		std::string lineHeader;
		getline(ss, lineHeader, ' ');
		if (lineHeader == "v") {
			glm::vec3 vertex;
			ss >> vertex.x >> vertex.y >> vertex.z;
			vertex.x *= obj_scale;
			vertex.y *= obj_scale;
			vertex.z *= obj_scale;
			temp_vertices.push_back(vertex);
		}
		else if (lineHeader == "vt") {
			glm::vec2 uv;
			ss >> uv.x >> uv.y;
			temp_uvs.push_back(uv);
		}
		else if (lineHeader == "vn") {
			glm::vec3 normal;
			ss >> normal.x >> normal.y >> normal.z;
			temp_normals.push_back(normal);
		}
		else if (lineHeader == "f") {
			unsigned int vertex_index, uv_index, normal_index;
			char slash;
			while (ss >> vertex_index >> slash >> uv_index >> slash >> normal_index) {
				vertex_indices.push_back(vertex_index);
				uv_indices.push_back(uv_index);
				normal_indices.push_back(normal_index);
			}
		}
	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertex_indices.size(); i++) {
		unsigned int vertexIndex = vertex_indices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);

		unsigned int uvIndex = uv_indices[i];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		out_uvs.push_back(uv);

		unsigned int normalIndex = normal_indices[i];
		glm::vec3 normal = temp_normals[normalIndex - 1];
		out_normals.push_back(normal);
	}
}

void indexVBO(std::vector<glm::vec3> & in_vertices, std::vector<glm::vec2> & in_uvs, std::vector<glm::vec3> & in_normals,
	std::vector<unsigned short> & out_indices, std::vector<glm::vec3> & out_vertices, std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals)
{
	std::map<VertexInfo, unsigned short> VertexToOutIndex;

	// For each input vertex
	for (unsigned int i = 0; i < in_vertices.size(); i++)
	{
		VertexInfo packed = { in_vertices[i], in_uvs[i], in_normals[i] };

		unsigned short index;
		auto it = VertexToOutIndex.find(packed);
		if (it != VertexToOutIndex.end()) // check if vertex already exists
			out_indices.push_back(it->second);
		else
		{ // If not, it needs to be added in the output data.
			out_vertices.push_back(in_vertices[i]);
			out_uvs.push_back(in_uvs[i]);
			out_normals.push_back(in_normals[i]);
			unsigned short newindex = (unsigned short)out_vertices.size() - 1;
			out_indices.push_back(newindex);
			VertexToOutIndex[packed] = newindex;
		}
	}
}

void initBuffers() {
	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	loadOBJ(objname.c_str(), vertices, uvs, normals);
	indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

	//gen
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &uvbuffer);
	glGenBuffers(1, &normalbuffer);
	glGenBuffers(1, &elementbuffer);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	//binding
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
	//pointers
	// 1rst attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// 3rd attribute buffer : normals
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// 2nd attribute buffer : UVs
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void freeBuffers() {
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

const double pi = 3.14159265358979323846;
void recountLightPos()
{
	double x = light_rad * glm::cos(light_angle / 180 * pi);
	double z = light_rad * glm::sin(light_angle / 180 * pi);
	light[0] = x;
	light[1] = light_pos;
	light[2] = z;
}

void setTransform()
{
	GLint s_model = glGetUniformLocation(shader_program, "transform.model");
	GLint s_proj = glGetUniformLocation(shader_program, "transform.viewProjection");
	GLint s_normal = glGetUniformLocation(shader_program, "transform.normal");
	GLint s_view = glGetUniformLocation(shader_program, "transform.viewPosition");

	glUniformMatrix4fv(s_model, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(s_proj, 1, GL_FALSE, &viewProjection[0][0]);
	glUniformMatrix3fv(s_normal, 1, GL_FALSE, &normaltr[0][0]);
	glUniform3fv(s_view, 1, viewPosition);

}

void setPointLight() {
	GLint s_position = glGetUniformLocation(shader_program, "light.position");
	GLint s_ambient = glGetUniformLocation(shader_program, "light.ambient");
	GLint s_diffuse = glGetUniformLocation(shader_program, "light.diffuse");
	GLint s_specular = glGetUniformLocation(shader_program, "light.specular");
	GLint s_attenuation = glGetUniformLocation(shader_program, "light.attenuation");

	glUniform4fv(s_position, 1, light);
	glUniform4fv(s_ambient, 1, ambient);
	glUniform4fv(s_diffuse, 1, diffuse);
	glUniform4fv(s_specular, 1, specular);
	glUniform3fv(s_attenuation, 1, attenuation);
}

void setMaterial(float* m_ambient, float* m_diffuse, float* m_specular, float* m_emission, float m_shiness) {
	GLint s_ambient = glGetUniformLocation(shader_program, "material.ambient");
	GLint s_diffuse = glGetUniformLocation(shader_program, "material.diffuse");
	GLint s_specular = glGetUniformLocation(shader_program, "material.specular");
	GLint s_emission = glGetUniformLocation(shader_program, "material.emission");
	GLint s_shiness = glGetUniformLocation(shader_program, "material.shiness");

	glUniform4fv(s_ambient, 1, m_ambient);
	glUniform4fv(s_diffuse, 1, m_diffuse);
	glUniform4fv(s_specular, 1, m_specular);
	glUniform4fv(s_emission, 1, m_emission);
	glUniform1f(s_shiness, m_shiness);
}

void Lambert() {
	glUseProgram(shader_program);
	float fColor[4] = { 0, 1, 0, 0  };
	GLint color = glGetUniformLocation(shader_program, "diffColor");
	glUniform4fv(color, 1, fColor);

	setTransform();
	//set point light
	GLint s_position = glGetUniformLocation(shader_program, "light.position");
	glUniform4fv(s_position, 1, light);
}

void Blinn() {
	glUseProgram(shader_program);
	float fColor[4] = { 0, 1, 0, 0 };
	GLint color = glGetUniformLocation(shader_program, "diffColor");
	glUniform4fv(color, 1, fColor);

	setTransform();
	//set point light
	GLint s_position = glGetUniformLocation(shader_program, "light.position");
	glUniform4fv(s_position, 1, light);

	//set material
	GLint s_diffuse = glGetUniformLocation(shader_program, "material.diffuse");
	GLint s_specular = glGetUniformLocation(shader_program, "material.specular");
	GLint s_shiness = glGetUniformLocation(shader_program, "material.shiness");

	float m_diffuse[]{ 0 , 1, 0, 0 };
	float m_specular[]{ 0 , 1, 0, 0 };
	float m_shiness = 70;

	glUniform4fv(s_diffuse, 1, m_diffuse);
	glUniform4fv(s_specular, 1, m_specular);
	glUniform1f(s_shiness, m_shiness);
}

void Minnaert() {
	glUseProgram(shader_program);
	float fColor[4] = { 0, 1, 0, 0 };
	GLint color = glGetUniformLocation(shader_program, "diffColor");
	glUniform4fv(color, 1, fColor);

	setTransform();
	//set point light
	GLint s_position = glGetUniformLocation(shader_program, "light.position");
	glUniform4fv(s_position, 1, light);

	//set koef
	float koef = 0.8;
	GLint s_k = glGetUniformLocation(shader_program, "k");
	glUniform1f(s_k, koef);
}

void Toon() {
	glUseProgram(shader_program);
	float fColor[4] = { 0, 1, 0, 0 };
	GLint color = glGetUniformLocation(shader_program, "diffColor");
	glUniform4fv(color, 1, fColor);

	setTransform();
	//set point light
	GLint s_position = glGetUniformLocation(shader_program, "light.position");
	glUniform4fv(s_position, 1, light);
}

void Gooch() {
	glUseProgram(shader_program);
	float fColor[4] = { 0, 1, 0, 0 };
	GLint color = glGetUniformLocation(shader_program, "diffColor");
	glUniform4fv(color, 1, fColor);

	setTransform();
	//set point light
	GLint s_position = glGetUniformLocation(shader_program, "light.position");
	glUniform4fv(s_position, 1, light);

	//set other colors
	float oWarm[4] = { 0, 1, 0, 0 };//{ 0.6f,0.6f,0.0f, 1.0f };
	GLint warm = glGetUniformLocation(shader_program, "warm");
	glUniform4fv(warm, 1, oWarm);

	float oCold[4] = { 0, 0, 0.6, 1 }; //{0.0f,0.0f,0.6f, 1.0f};
	GLint cold = glGetUniformLocation(shader_program, "cold");
	glUniform4fv(cold, 1, oCold);

	float oDiffWarm = 0.45;
	GLint dwarm = glGetUniformLocation(shader_program, "diffwarm");
	glUniform1f(dwarm, oDiffWarm);

	float oDiffCold = 0.45;
	GLint dcold = glGetUniformLocation(shader_program, "diffcold");
	glUniform1f(dcold, oDiffCold);
}

void Rim()
{
	glUseProgram(shader_program);

	setTransform();
	//set point light
	GLint s_position = glGetUniformLocation(shader_program, "light.position");
	glUniform4fv(s_position, 1, light);

	//set other colors
	float diffc[4] = { 0, 1, 0, 0 };
	GLint diff = glGetUniformLocation(shader_program, "diff");
	glUniform4fv(diff, 1, diffc);

	float specc[4] = { 0, 1, 0, 0 };
	GLint spec = glGetUniformLocation(shader_program, "spec");
	glUniform4fv(spec, 1, specc);

	float ospecPow = 30.0f;
	GLint specpow = glGetUniformLocation(shader_program, "specPow");
	glUniform1f(specpow, ospecPow);

	float orimPow = 8.0f;
	GLint rimpow = glGetUniformLocation(shader_program, "rimPow");
	glUniform1f(rimpow, orimPow);

	float obias = 0.3f;
	GLint bias = glGetUniformLocation(shader_program, "bias");
	glUniform1f(bias, obias);
}

void display(void)
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	model = glm::mat4(1.0f);
	//rotate model here
	model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(rotateY), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0, 0, 1));

	viewProjection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
	viewProjection *= glm::lookAt(
		glm::vec3(viewPosition[0], viewPosition[1], viewPosition[2]),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);
	normaltr = glm::transpose(glm::inverse(model));
	
	float red[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	
	switch (mode)
	{
	case 1: Lambert();
		break;
	case 2: Blinn();
		break;
	case 3: Minnaert();
		break;
	case 4: Toon();
		break;
	case 5: Gooch();
		break;
	case 6: Rim();
		break;
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
	glutSwapBuffers();
}

/* ���������� ���������� ����� � ������� ��������:
1 - ��������
2 - ������
3 - Minnaert
4 - Toon shading
5 - ��� ���
6 - Rim */

void special(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_F1: mode = 1;
		shader_program = LambertProgram;
		break;
	case GLUT_KEY_F2:
		mode = 2;
		shader_program = BlinnProgram;
		break;
	case GLUT_KEY_F3:
		mode = 3;
		shader_program = MinnaertProgram;
		break;
	case GLUT_KEY_F4:
		mode = 4;
		shader_program = ToonShadingProgram;
		break;
	case GLUT_KEY_F5:
		mode = 5;
		shader_program = AmiGoochProgram;
		break;
	case GLUT_KEY_F6:
		mode = 6;
		shader_program = RimProgram;
		break;
	case GLUT_KEY_UP: light_pos += 0.5; break;
	case GLUT_KEY_DOWN: light_pos -= 0.5; break;
	case GLUT_KEY_RIGHT: light_angle -= 3; break;
	case GLUT_KEY_LEFT: light_angle += 3; break;
	case GLUT_KEY_PAGE_UP: light_rad -= 0.5; break;
	case GLUT_KEY_PAGE_DOWN: light_rad += 0.5; break;
	}
	recountLightPos();
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		rotateX -= 1;
		break;
	case 'd':
		rotateX += 1;
		break;
	case 'w':
		rotateY -= 1;
		break;
	case 's':
		rotateY += 1;
		break;
	case 'e':
		rotateZ -= 1;
		break;
	case 'q':
		rotateZ += 1;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Lab 15");

	GLenum glew_status = glewInit();

	initBuffers();
	makeTextureImage();
	initShaders();
	init();
	recountLightPos();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	freeShader();
	freeBuffers();
}