#ifndef OPENGL_RENDERER
#define OPENGL_RENDERER

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <functional>
#include <string>
#include "matrix.h"
#include "physics.h"
#include "game.h"
#include "renderer.h"
#include "debug.h"
#include "wavefront.h"

// stores information on how to render a specific vertex buffer (vbo)
class OpenGLView {
protected:
    unsigned int shaderProgram;
    unsigned int shaderProgram3d;
    size_t vertices_size;
    GLuint mode;
    GLuint vao;
    bool is3d;

public:
    OpenGLView(GLuint vbo, unsigned int shaderProgram, size_t vertices_size, GLuint mode = GL_LINE_LOOP,
               bool is3d = false);
    ~OpenGLView();
    void render(SquareMatrix<float, 4>& matrice);
};

class TypedBodyView : public OpenGLView {
    TypedBody* typed_body;
    float scale;
    std::function<bool()> draw;
    std::function<void(TypedBodyView*)> modify;

    SquareMatrix4df create_object_transformation(Vector3df direction, float angle, float scale);

public:
    TypedBodyView(TypedBody* typed_body, GLuint vbo, unsigned int shaderProgram, size_t vertices_size,
                  float scale = 1.0f, GLuint mode = GL_LINE_LOOP, bool is3d = false,
                  std::function<bool()> draw = []() -> bool { return true; },
                  std::function<void(TypedBodyView*)> modify = [](TypedBodyView*) -> void {});
    void render(SquareMatrix<float, 4>& world);
    TypedBody* get_typed_body();
    void set_scale(float scale);
};

class OpenGLRenderer : public Renderer {
    std::string title;
    int window_width;
    int window_height;
    SDL_Window* window = nullptr;
    SDL_GLContext context;
    unsigned int shaderProgram;
    unsigned int shaderProgram3d;
    std::vector<GLuint> vbos;
    std::vector<GLuint> vbos3d;
    std::vector<std::vector<float>> vertice_3d_data;
    std::vector<std::unique_ptr<TypedBodyView>> views;
    std::unique_ptr<OpenGLView> spaceship_view;
    std::array<std::unique_ptr<OpenGLView>, 10> digit_views;

    void createVbos();
    void create3dVbos();
    void createSpaceShipView();
    void createDigitViews();
    void create_shader_programs();
    void create_3dshader_programs();
    GLuint compile_and_link_shader(const char* vertex_src, const char* fragment_src);

    void create(Spaceship* ship, std::vector<std::unique_ptr<TypedBodyView>>& views);
    void create(Torpedo* torpedo, std::vector<std::unique_ptr<TypedBodyView>>& views);
    void create(Asteroid* asteroid, std::vector<std::unique_ptr<TypedBodyView>>& views);
    void create(Saucer* saucer, std::vector<std::unique_ptr<TypedBodyView>>& views);
    void create(SpaceshipDebris* debris, std::vector<std::unique_ptr<TypedBodyView>>& views);
    void create(Debris* debris, std::vector<std::unique_ptr<TypedBodyView>>& views);
    void renderFreeShips(const SquareMatrix4df& matrice) const;
    void renderScore(const SquareMatrix4df& matrice) const;

public:
    OpenGLRenderer(Game& game, std::string title, int window_width = 1024, int window_height = 768);
    ~OpenGLRenderer();
    bool load_objects(const std::vector<std::string>& object_files);
    virtual bool init();
    virtual void render();
    virtual void exit();
};

#endif
