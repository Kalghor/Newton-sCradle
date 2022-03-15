#include <glad/glad.h>
#include <glfw3.h>
#include <string>
#include <fstream>
#include <iostream>

std::string loadShaderSource(const std::string& _filepath);
GLuint compileShader(const GLchar* _source, GLenum _stage, const std::string& _msg);

constexpr int WIN_SZ_X = 1550, WIN_SZ_Y = 850;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIN_SZ_X, WIN_SZ_Y, "Kulki Newtona", nullptr, nullptr);
	if (!window)
		return 1;

	glfwMakeContextCurrent(window);
	gladLoadGL();

	GLuint vs = compileShader(loadShaderSource("vs.vert").c_str(), GL_VERTEX_SHADER, "VS log");
	GLuint fs = compileShader(loadShaderSource("fs.frag").c_str(), GL_FRAGMENT_SHADER, "FS log");
	GLuint pipeline;
	glCreateProgramPipelines(1, &pipeline);
	glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vs);
	glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fs);

	float vertices[]{
		0.f, 0.5f, 1.f, 0.f, 0.f,
		0.5f, -0.5f, 0.f, 1.f, 0.f,
		-0.5f, -0.5f, 0.f, 0.f, 1.f,
	};

	GLuint vao;
	glCreateVertexArrays(1, &vao);

	GLuint vertexBuffer;
	glCreateBuffers(1, &vertexBuffer);
	glNamedBufferStorage(vertexBuffer, sizeof(vertices), vertices, 0);

	constexpr GLuint ATTR_POS = 0u;
	glEnableVertexArrayAttrib(vao, ATTR_POS);
	glVertexArrayAttribFormat(vao, ATTR_POS, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayVertexBuffer(vao, ATTR_POS, vertexBuffer, 0, 5*sizeof(float));
	glVertexArrayAttribBinding(vao, ATTR_POS, ATTR_POS);

	constexpr GLuint ATTR_COLOR = 1u;
	glEnableVertexArrayAttrib(vao, ATTR_COLOR);
	glVertexArrayAttribFormat(vao, ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayVertexBuffer(vao, ATTR_COLOR, vertexBuffer, 2*sizeof(float), 5 * sizeof(float));
	glVertexArrayAttribBinding(vao, ATTR_COLOR, ATTR_COLOR);

	glBindProgramPipeline(pipeline);
	glBindVertexArray(vao);

	while (!glfwWindowShouldClose(window)) {
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, 1);
		}
		glClearColor(0.2f, 0.3f, 0.7f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);
;		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &vertexBuffer);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgramPipelines(1, &pipeline);
	glDeleteProgram(vs);
	glDeleteProgram(fs);
	glfwTerminate();
	return 1;
}

//Funkcja �aduje kod �r�d�owy shader�w z plik�w
std::string loadShaderSource(const std::string& _filepath) {
	std::ifstream f(_filepath, std::ios::ate | std::ios::in); // tworzymy obiekt pliku. Ate powoduje, �e wska�nik ustawiamy na ko�cu pliku dzi�ki czemu znamy d�ugo�� tego pliku
	const size_t len = f.tellg(); //d�ugo�� pliku
	f.seekg(0); // ustawiamy wska�nik na pocz�tek pliku

	std::string shdr; // string, w kt�rym trzymamy kod �r�d�owy
	shdr.resize(len); // zwi�kszamy rozmiar buforu stringa, �eby zmie�ci� kod �r�d�owy pliku

	f.read(&shdr.front(), len); // odczytujemy kod z pliku
	return shdr;
}

GLuint compileShader(const GLchar* _source, GLenum _stage, const std::string& _msg) {
	GLuint shdr = glCreateShaderProgramv(_stage, 1, &_source); // kompiluje kod

	std::string log; //  bufor na log kompilacji
	log.resize(1024); // ustawienie wielko�ci stringa
	glGetProgramInfoLog(shdr, log.size(), nullptr, &log.front()); // pobieranie log�w kompilacji
	printf("%s: %s\n", _msg.c_str(), log.c_str());

	return shdr;
}