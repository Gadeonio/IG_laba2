// IG_2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <string>
#include <fstream>

GLuint VBO;
GLint gScalingLocation;


static void ScalingExample()
{
    static float Scale = 1.0f;
    static float Delta = 0.001f;

    Scale += Delta;
    if ((Scale >= 1.5f) || (Scale <= 0.5)) {
        Delta *= -1.0f;
    }

    glm::mat4 Scaling(Scale, 0.0f, 0.0f, 0.0f,
        0.0f, Scale, 0.0f, 0.0f,
        0.0f, 0.0f, Scale, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

    glUniformMatrix4fv(gScalingLocation, 1, GL_TRUE, &Scaling[0][0]);
}


static void CombiningTransformationsExample1()
{
    static float Scale = 1.5f;

    glm::mat4 Scaling(Scale, 0.0f, 0.0f, 0.0f,
        0.0f, Scale, 0.0f, 0.0f,
        0.0f, 0.0f, Scale, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

    static float Loc = 0.0f;
    static float Delta = 0.01f;

    Loc += Delta;
    if ((Loc >= 0.5f) || (Loc <= -0.5f)) {
        Delta *= -1.0f;
    }

    glm::mat4 Translation(1.0f, 0.0f, 0.0f, Loc,
        0.0f, 1.0f, 0.0f, 0.0,
        0.0f, 0.0f, 1.0f, 0.0,
        0.0f, 0.0f, 0.0f, 1.0f);

    //Matrix4f FinalTransform = Translation * Scaling;
    glm::mat4 FinalTransform = Scaling * Translation;

    glUniformMatrix4fv(gScalingLocation, 1, GL_TRUE, &FinalTransform[0][0]);
}


static void CombiningTransformationsExample2()
{
    static float Scale = 0.25f;

    glm::mat4 Scaling(Scale, 0.0f, 0.0f, 0.0f,
        0.0f, Scale, 0.0f, 0.0f,
        0.0f, 0.0f, Scale, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

    static float AngleInRadians = 0.0f;
    static float Delta = 0.01f;

    AngleInRadians += Delta;

    glm::mat4 Rotation(cosf(AngleInRadians), -sinf(AngleInRadians), 0.0f, 0.0f,
        sinf(AngleInRadians), cosf(AngleInRadians), 0.0f, 0.0f,
        0.0, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

    static float Loc = 0.5f;

    glm::mat4 Translation(1.0f, 0.0f, 0.0f, Loc,
        0.0f, 1.0f, 0.0f, 0.0,
        0.0f, 0.0f, 1.0f, 0.0,
        0.0f, 0.0f, 0.0f, 1.0f);

    //Matrix4f FinalTransform = Translation * Rotation * Scaling;
    glm::mat4 FinalTransform = Rotation * Translation * Scaling;

    glUniformMatrix4fv(gScalingLocation, 1, GL_TRUE, &FinalTransform[0][0]);
}

void RenderSceneCB() {
    glClear(GL_COLOR_BUFFER_BIT);

    ScalingExample();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    glDisableVertexAttribArray(0);

    glutPostRedisplay();

    glutSwapBuffers();
}

void CreateVertexBuffer() {
    glm::vec3 Vertices[3];
    Vertices[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
    Vertices[1] = glm::vec3(1.0f, -1.0f, 0.0f);
    Vertices[2] = glm::vec3(0.0f, 1.0f, 0.0f);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType) {
    GLuint ShaderObj = glCreateShader(ShaderType);
    if (ShaderObj == 0) {
        std::cout << "Error creating shader type" << ShaderType;
        exit(1);
    }

    const GLchar* p[1];
    p[0] = pShaderText;

    GLint Lengths[1];
    Lengths[0] = (GLint)strlen(pShaderText);

    glShaderSource(ShaderObj, 1, p, Lengths);

    glCompileShader(ShaderObj);

    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        std::cout << "Error compiling shader type" << " " << ShaderType << " " << InfoLog << std::endl;
        exit(1);
    }
    glAttachShader(ShaderProgram, ShaderObj);
}

const char* pVSFileName = "D:\\Учеба УГАТУ\\2 курс\\2 семестр\\ИГ\\IG_laba2\\IG_2\\shader.vs";
const char* pFSFileName = "D:\\Учеба УГАТУ\\2 курс\\2 семестр\\ИГ\\IG_laba2\\IG_2\\shader.fs";

void Shader_in_file(const char* name, std::string& str) {
    std::ifstream file;
    file.open(name);
    if (!(file.is_open())) {
        exit(10);
    }
    std::string parse;
    
    while (getline(file, parse)) { 
        str += parse + "\n"; 
    }
    std::cout << str << std::endl;
    file.close();
    
}

void CompileShader() {
    GLuint ShaderProgram = glCreateProgram();

    if (ShaderProgram == 0) {
        std::cout << "Error creating shader program" << std::endl;
        exit(11);
    }

    std::string vs, fs;
    Shader_in_file(pVSFileName, vs);
    AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
    Shader_in_file(pFSFileName, fs);
    AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(ShaderProgram);

    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        std::cout << "Error linking shader program: " << ErrorLog << std::endl;
        exit(12);
    }

    gScalingLocation = glGetUniformLocation(ShaderProgram, "gScaling");
    if (gScalingLocation == -1) {
        printf("Error getting uniform location of 'gScaling'\n");
        exit(1);
    }

    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        std::cout << "Invalid shader program: " << ErrorLog << std::endl;
        exit(13);
    }

    glUseProgram(ShaderProgram);   
}


void Window(int &argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    int width = 800;
    int height = 600;
    glutInitWindowSize(width, height);
    glutCreateWindow("Tutorial 8");

    //glutInitWindowPosition(x, y);
}

void Check_GLenum() {
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        std::cout << "Error: Yaroooooo... " << glewGetErrorString(res) << std::endl;
        exit(-1);
    }
    
}

int main(int argc, char** argv)
{
    Window(argc, argv);
    Check_GLenum();
    GLclampf Red = 0.0f, Green = 0.0f, Blue = 0.0f, Alpha = 0.0f;
    glClearColor(Red, Green, Blue, Alpha);

    CreateVertexBuffer();

    CompileShader();

    glutDisplayFunc(RenderSceneCB);

    glutMainLoop();

    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
