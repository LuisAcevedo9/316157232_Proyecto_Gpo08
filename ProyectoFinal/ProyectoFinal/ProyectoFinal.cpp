// Std. Includes
#include <string>
#include <math.h>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"


// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );
void animacion();
void animacion2();


// Camera
Camera camera( glm::vec3( 0.0f, 0.0f, 3.0f ) );
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
bool esta_abajo = true;
bool rotar = false;
bool active1 = false;
float animacion1RotY = -79.0f;
bool sentido1 = true;

//Segunda animaci�n
bool active2 = false;
bool sentido2 = true;
float animacion2posY = 3.6f;

//Tercera animaci�n
bool active3 = false;
bool sentido3bill = true;
float animacion3scale = 1.0f;
float animacion3posY = 0.0f;
float animacion3rot1X = 0.0f;
float animacion3rot2Y = 0.0f;
float animacion3rot3Z = 0.0f;

//Cuarta Animaci�n
glm::vec3 PosIni(-16.0f, 0.65f, -16.0f);
float movKitX = 0.0;
float movKitZ = 0.0;
float rotKit = 0.0f;

bool circuito = false;
bool recorrido1 = true;
bool recorrido2 = false;
bool recorrido3 = false;
bool recorrido4 = false;
bool recorrido5 = false;
bool recorrido6 = false;
bool recorrido7 = false;
bool recorrido8 = false;
bool recorrido9 = false;

//Quinta animaci�n
bool circuito2 = false;
bool recorrido1_2 = true;
bool recorrido2_2 = false;
bool recorrido3_2 = false;


glm::vec3 PosIniLata(-1.1f, 3.6f, 0.0f);
float posX = 0.0f;
float posY = 0.0f;
float RotBill = 0.0f;


int main( )
{
    // Init GLFW
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Practica 4", nullptr, nullptr );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // OpenGL options
    glEnable( GL_DEPTH_TEST );
    
    // Setup and compile our shaders
    Shader shader( "Shaders/modelLoading.vs", "Shaders/modelLoading.frag" );
    Shader lampshader( "Shaders/lamp.vs", "Shaders/lamp.frag" );
    



    // Load models
    //Model Pizza((char*)"Models/Pizza/PizzaSteve.obj");
    //Model brader((char*)"Models/Pizza/brader.obj");
    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    
    GLfloat vertices[] =
    {
        // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 

    };

    GLuint indices[] =
    {  // Note that we start from 0!
        0,1,3,
        1,2,3

    };

    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Load textures
    //Model pokeArriba((char*)"Models/Pokeball/pokearriba.obj");
    //Model pokeAbajo((char*)"Models/Pokeball/pokeabajo.obj");
    Model casa((char*)"Models/ModelosProyecto/casa/Casa-gravity-falls.obj");
    Model cama1((char*)"Models/ModelosProyecto/cama/cama.obj");
    Model cama2((char*)"Models/ModelosProyecto/cama/cama2.obj");
    Model mesa((char*)"Models/ModelosProyecto/mesa/mesa.obj");
    Model buro((char*)"Models/ModelosProyecto/Buro/buro.obj");
    Model lata((char*)"Models/ModelosProyecto/lata/lata.obj");
    Model libro((char*)"Models/ModelosProyecto/libro/libro.obj");
    Model maleta((char*)"Models/ModelosProyecto/maleta/maleta.obj");
    Model bill((char*)"Models/ModelosProyecto/Bill/bill.obj");
    Model ventana((char*)"Models/ModelosProyecto/ventana/ventana.obj");
    Model piso((char*)"Models/ModelosProyecto/piso/piso_pasto.obj");
    Model pato((char*)"Models/ModelosProyecto/pato/pato.obj");
    /*Model bill2((char*)"Models/ModelosProyecto/Bill/bill.obj");*/
    Model lata2((char*)"Models/ModelosProyecto/lata/lata.obj");

    //Modelos para el proyecto de teor�a
    Model mostrador((char*)"Models/ModelosProyecto/mostrador/mostrador.obj");
    Model barril((char*)"Models/ModelosProyecto/barril/barril.obj");
    Model canasta((char*)"Models/ModelosProyecto/canasta/canasta.obj");
    Model letrero((char*)"Models/ModelosProyecto/letrero/letrero.obj");
    Model caja((char*)"Models/ModelosProyecto/caja_roja/caja_roja.obj");

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  


    //Cambiar lo de alfa para PNG----------------------------------------------------------------------------------
    //image = stbi_load("images/goku.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    //glGenerateMipmap(GL_TEXTURE_2D);
    //if (image)
    //{
    //    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    //    //glGenerateMipmap(GL_TEXTURE_2D);
    //}
    //else
    //{
    //    std::cout << "Failed to load texture" << std::endl;
    //}
    //stbi_image_free(image);


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model
        glm::mat4 model(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        //glDrawElements(GL_TRIANGLES, 6, GL_FLAT, 0);
        //Pizza.Draw(shader);

        model = glm:: mat4(1);
        model = glm::rotate(model, glm::radians(-rot), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        casa.Draw(shader);


        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.963f, 3.254f, 1.488f));
        model = glm::scale(model, glm::vec3(0.445f, 0.445f, 0.445f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        cama1.Draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.238f, 3.355f, -1.5f));
        model = glm::scale(model, glm::vec3(0.417f, 0.417f, 0.417f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        cama2.Draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.8f, 3.85f, 0.45f));
        model = glm::scale(model, glm::vec3(0.353f, 0.370f, 0.370f));
        model = glm::rotate(model, glm::radians(-82.917f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        mesa.Draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.8f, 3.25f, -0.812f));
        model = glm::scale(model, glm::vec3(0.503f, 0.503f, 0.503f));
        model = glm::rotate(model, glm::radians(-31.169f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        buro.Draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.806f, 3.355f + animacion3posY, 1.249f));
        model = glm::scale(model, glm::vec3(0.355f, 0.355f, 0.355f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(animacion3rot1X), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        lata.Draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.2, 3.361f + animacion3posY, -0.537f));
        model = glm::scale(model, glm::vec3(0.164f, 0.164f, 0.164f));
        model = glm::rotate(model, glm::radians(animacion3rot2Y), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        libro.Draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.5f, 3.399f + animacion3posY, 0.355f));
        model = glm::scale(model, glm::vec3(0.343f, 0.343f, 0.343f));
        model = glm::rotate(model, glm::radians(-195.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(animacion3rot3Z), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        maleta.Draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.1f, animacion2posY, 0.0f));
        model = glm::scale(model, glm::vec3(animacion3scale, animacion3scale, animacion3scale));
        model = glm::rotate(model, glm::radians(90.0f+ RotBill), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        bill.Draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-4.7f, 6.0f, -0.05f));
        model = glm::rotate(model, glm::radians(animacion1RotY), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        ventana.Draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(7.086f, 1.508f, 1.061f));
        model = glm::scale(model, glm::vec3(0.617f, 0.617f, 0.617f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        mostrador.Draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(5.553f, 1.432f, 1.033f));
        model = glm::scale(model, glm::vec3(0.246f, 0.246f, 0.246f));
        //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        barril.Draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(9.241f, 1.44f, 1.087f));
        model = glm::scale(model, glm::vec3(0.308f, 0.308f, 0.308f));
        //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        canasta.Draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(6.87f, 2.58f, 0.816f));
        model = glm::scale(model, glm::vec3(0.308f, 0.24f, 0.308f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        letrero.Draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(8.117f, 1.285f, 0.89f));
        model = glm::scale(model, glm::vec3(0.202f, 0.202f, 0.202f));
        //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        caja.Draw(shader);


        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(3.0f, 1.0f, 3.0f));
        //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        piso.Draw(shader);


        model = glm::mat4(1.0f);
        model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
        model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
        model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pato.Draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.806f + posX, 3.355f + posY, -1.3f));
        model = glm::scale(model, glm::vec3(0.355f, 0.355f, 0.355f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
       /* model = glm::rotate(model, glm::radians(animacion3rot1X), glm::vec3(1.0f, 0.0f, 0.0f));*/
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        lata2.Draw(shader);

        //if (circuito2) {
        //    model = glm::mat4(1.0f);
        //    model = glm::translate(model, PosIni2 + glm::vec3(movKitX2, movKitY2, movKitZ2));
        //    model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
        //    model = glm::rotate(model, glm::radians(rotKit2), glm::vec3(0.0f, 1.0f, 0.0f));
        //    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        //    bill2.Draw(shader);
        //}
        
        
        //Animaci�n 1
        if (active1)
        {
            if (animacion1RotY > -20.0) {
                sentido1 = false;
            }
            else if (animacion1RotY < -80.0) {
                sentido1 = true;
            }
            if (sentido1) {
                animacion1RotY += 0.5f;
            }
            else {
                animacion1RotY -= 0.5f;
            }


        }
        else
        {
            sentido1 = true;
            animacion1RotY = 0.0f;
        }
        //Animaci�n 2
        if (active2)
        {
            if (animacion2posY >= 3.9) {
                sentido2 = false;
            }
            else if (animacion2posY < 3.3) {
                sentido2 = true;
            }
            if (sentido2) {
                animacion2posY += 0.005f;
            }
            else {
                animacion2posY -= 0.005f;
            }


        }
        else
        {
            sentido2 = true;
            animacion2posY = 3.6f;
        }


        //Animacion 3
        if (active3)
        {
            //Animaci�n de bill
            if (animacion3scale >= 1.3) {
                sentido3bill = false;
            }
            else if (animacion3scale < 0.8){
                sentido3bill = true;
                }
                if (sentido3bill) {
                    animacion3scale += 0.006f;
                }
                else {
                    animacion3scale -= 0.006f;
                }

            //Animaci�n de objetos
            if (animacion3posY >= 2.0) {
                animacion3posY = 2.0f;
                //Rotaci�n de los objetos
                animacion3rot1X += 0.5f;
                animacion3rot2Y += 0.5f;
                animacion3rot3Z += 0.5f;

            }
            else {
                animacion3posY += 0.003;
            }

        }
        else
        {
            sentido3bill = true;
            animacion3scale = 1.0f;
        }

        animacion();
        animacion2();

        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        lampshader.Use();
        //glm::mat4 model(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate( );
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement( )
{
    // Camera controls
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }


    if (keys[GLFW_KEY_M])
    {
        circuito = true;
    }

    if (keys[GLFW_KEY_N])
    {
        circuito = false;
    }


    if (keys[GLFW_KEY_B])
    {
        circuito2 = true;
    }



}

void animacion()
{

    //Movimiento del coche
    if (circuito)
    {
        if (recorrido1)
        {
            movKitZ+= 0.05f;
            if (movKitZ > 32)
            {
                recorrido1 = false;
                recorrido2 = true;
            }
        }
        if (recorrido2)
        {
            rotKit = 90;
            movKitX += 0.05f;
            if (movKitX > 32)
            {
                recorrido2 = false;
                recorrido3 = true;

            }
        }

        if (recorrido3)
        {
            rotKit = 180;
            movKitZ -= 0.05f;
            if (movKitZ < 0)
            {
                recorrido3 = false;
                recorrido4 = true;
            }
        }

        if (recorrido4)
        {
            rotKit = 270;
            movKitX -= 0.05f;
            if (movKitX < 30)
            {
                recorrido4 = false;
                recorrido5 = true;
            }
        }
        if (recorrido5)
        {
            rotKit = 0;
            movKitZ+= 0.05f;
            if (movKitZ > 28)
            {
                recorrido5 = false;
                recorrido6 = true;
            }
        }
        if (recorrido6)
        {
            rotKit = 270;
            movKitX -= 0.05f;
            if (movKitX < 5)
            {
                recorrido6 = false;
                recorrido7 = true;
            }
        }
        if (recorrido7)
        {
            rotKit = 180;
            movKitZ -= 0.05f;
            if (movKitZ < 0)
            {
                recorrido7 = false;
                recorrido8 = true;
            }
        }
        if (recorrido8)
        {
            rotKit = 270;
            movKitX -= 0.05f;
            if (movKitX < 0)
            {
                recorrido8 = false;
                recorrido9 = true;
            }
        }
        if (recorrido9)
        {
            rotKit = 0;
            movKitZ += 0.1f;
            if (movKitZ > 0)
            {
                recorrido9 = false;
                recorrido1 = true;
            }
        }


    }
}

void animacion2()
{
    //glm::vec3 PosIni2(-1.1f, 3.6f, 0.0f);
    //Movimiento del coche
    if (circuito2)
    {
        if (recorrido1_2)
        {
            RotBill += 0.5f;
            if (RotBill > 30.0)
            {
                recorrido1_2 = false;
                recorrido2_2 = true;
            }
        }
        if (recorrido2_2)
        {
            RotBill -= 0.5f;

            float v = 8.0f;
            float rad = 40 / 57.3;
            posX += 0.05f;
            posY = tan(rad) * posX - (9.8 / (2 * v * v * cos(rad) * cos(rad))) * posX * posX;
            if (RotBill < -30.0 && posY <=0.05)
            {
                recorrido2_2 = false;
                recorrido3_2 = true;
            }
        }
        if (recorrido3_2) {
            RotBill = 0.0f;
            circuito2 = false;
        }

    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }

    }

    if (keys[GLFW_KEY_O])
    {
        active1 = !active1;
    }
    if (keys[GLFW_KEY_P])
    {
        active2 = !active2;
    }
    if (keys[GLFW_KEY_L])
    {
        active3 = !active3;
    }
}

 


void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}

