#include "opengl_renderer.h"
#include <cassert>
#include <span>
#include <utility>
#include "wavefront.h"

// Geometrische Daten für die 2D-Darstellung der Spielobjekte (z.B. Raumschiff, Asteroiden, Ziffern)
// Diese Vektoren definieren die Eckpunkte der jeweiligen Formen im Spielkoordinatensystem
std::vector<Vector2df> spaceship = {
    Vector2df{-6.0f, 3.0f},
    Vector2df{-6.0f, -3.0f},
    Vector2df{-10.0f, -6.0f},
    Vector2df{14.0f, 0.0f},
    Vector2df{-10.0f, 6.0f},
    Vector2df{-6.0f, 3.0f}};

std::vector<Vector2df> flame = {
    Vector2df{-6, 3},
    Vector2df{-12, 0},
    Vector2df{-6, -3}};

std::vector<Vector2df> torpedo_points = {
    Vector2df{0, 0},
    Vector2df{0, 1}};

std::vector<Vector2df> saucer_points = {
    Vector2df{-16, -6},
    Vector2df{16, -6},
    Vector2df{40, 6},
    Vector2df{-40, 6},
    Vector2df{-16, 18},
    Vector2df{16, 18},
    Vector2df{40, 6},
    Vector2df{16, -6},
    Vector2df{8, -18},
    Vector2df{-8, -18},
    Vector2df{-16, -6},
    Vector2df{-40, 6}};

std::vector<Vector2df> asteroid_1 = {
    Vector2df{0, -12},
    Vector2df{16, -24},
    Vector2df{32, -12},
    Vector2df{24, 0},
    Vector2df{32, 12},
    Vector2df{8, 24},
    Vector2df{-16, 24},
    Vector2df{-32, 12},
    Vector2df{-32, -12},
    Vector2df{-16, -24},
    Vector2df{0, -12}};

std::vector<Vector2df> asteroid_2 = {
    Vector2df{6, -6},
    Vector2df{32, -12},
    Vector2df{16, -24},
    Vector2df{0, -16},
    Vector2df{-16, -24},
    Vector2df{-24, -12},
    Vector2df{-16, -0},
    Vector2df{-32, 12},
    Vector2df{-16, 24},
    Vector2df{-8, 16},
    Vector2df{16, 24},
    Vector2df{32, 6},
    Vector2df{16, -6},
};

std::vector<Vector2df> asteroid_3 = {
    Vector2df{-16, 0},
    Vector2df{-32, 6},
    Vector2df{-16, 24},
    Vector2df{0, 6},
    Vector2df{0, 24},
    Vector2df{16, 24},
    Vector2df{32, 6},
    Vector2df{32, 6},
    Vector2df{16, -24},
    Vector2df{-8, -24},
    Vector2df{-32, -6}};

std::vector<Vector2df> asteroid_4 = {
    Vector2df{8, 0},
    Vector2df{32, -6},
    Vector2df{32, -12},
    Vector2df{8, -24},
    Vector2df{-16, -24},
    Vector2df{-8, -12},
    Vector2df{-32, -12},
    Vector2df{-32, 12},
    Vector2df{-16, 24},
    Vector2df{8, 16},
    Vector2df{16, 24},
    Vector2df{32, 12},
    Vector2df{8, 0}};

std::vector<Vector2df> spaceship_debris = {
    Vector2df{-2, -1},
    Vector2df{-10, 7},
    Vector2df{3, 1},
    Vector2df{7, 8},
    Vector2df{0, 3},
    Vector2df{6, 1},
    Vector2df{3, -1},
    Vector2df{-5, -7},
    Vector2df{0, -4},
    Vector2df{-6, -6},
    Vector2df{-2, 2},
    Vector2df{2, 5}};

std::vector<Vector2df> spaceship_debris_direction = {
    Vector2df{-40, -23},
    Vector2df{50, 15},
    Vector2df{0, 45},
    Vector2df{60, -15},
    Vector2df{10, -52},
    Vector2df{-40, 30}};

std::vector<Vector2df> debris_points = {
    Vector2df{-32, 32},
    Vector2df{-32, -16},
    Vector2df{-16, 0},
    Vector2df{-16, -32},
    Vector2df{-8, 24},
    Vector2df{8, -24},
    Vector2df{24, 32},
    Vector2df{24, -24},
    Vector2df{24, -32},
    Vector2df{32, -8}};

std::vector<Vector2df> digit_0 = {{0, -8}, {4, -8}, {4, 0}, {0, 0}, {0, -8}};
std::vector<Vector2df> digit_1 = {{4, 0}, {4, -8}};
std::vector<Vector2df> digit_2 = {{0, -8}, {4, -8}, {4, -4}, {0, -4}, {0, 0}, {4, 0}};
std::vector<Vector2df> digit_3 = {{0, 0}, {4, 0}, {4, -4}, {0, -4}, {4, -4}, {4, -8}, {0, -8}};
std::vector<Vector2df> digit_4 = {{4, 0}, {4, -8}, {4, -4}, {0, -4}, {0, -8}};
std::vector<Vector2df> digit_5 = {{0, 0}, {4, 0}, {4, -4}, {0, -4}, {0, -8}, {4, -8}};
std::vector<Vector2df> digit_6 = {{0, -8}, {0, 0}, {4, 0}, {4, -4}, {0, -4}};
std::vector<Vector2df> digit_7 = {{0, -8}, {4, -8}, {4, 0}};
std::vector<Vector2df> digit_8 = {{0, -8}, {4, -8}, {4, 0}, {0, 0}, {0, -8}, {0, -4}, {4, -4}};
std::vector<Vector2df> digit_9 = {{4, 0}, {4, -8}, {0, -8}, {0, -4}, {4, -4}};

// Zeiger auf die oben definierten Vektordaten, um sie später für die OpenGL-Puffer zu verwenden
std::vector<std::vector<Vector2df> *> vertice_data = {
    &spaceship, &flame,
    &torpedo_points, &saucer_points,
    &asteroid_1, &asteroid_2, &asteroid_3, &asteroid_4,
    &spaceship_debris, &spaceship_debris_direction,
    &debris_points,
    &digit_0, &digit_1, &digit_2, &digit_3, &digit_4, &digit_5, &digit_6, &digit_7, &digit_8, &digit_9};

// ===================== OpenGLView =====================

// Konstruktor für OpenGLView: Initialisiert ein Vertex Array Object (VAO) und bindet den Vertex Buffer Object (VBO).
// Je nach 2D/3D werden die Attribute unterschiedlich gesetzt.
OpenGLView::OpenGLView(GLuint vbo, unsigned int shaderProgram, size_t vertices_size, GLuint mode, bool is3d)
    : shaderProgram(shaderProgram), vertices_size(vertices_size), mode(mode), is3d(is3d)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    if (!is3d)
    {
        // Für 2D: Nur Positionsdaten (x, y)
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void *>(nullptr));
        glEnableVertexAttribArray(0);
    }
    else
    {
        // Für 3D: Position, Farbe, Normalen
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), static_cast<void *>(nullptr));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Hilfsfunktion zum Kompilieren eines Shaders und Fehlerausgabe
void compile_shader(const GLint shader, const char *source)
{
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        std::cerr << " Shader did not compile." << std::endl;
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        error(log);
        throw EXIT_FAILURE;
    }
}

// Überprüft, ob das Shader-Programm erfolgreich gelinkt wurde
void check_link_status(const GLint shader_program)
{
    GLint status;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint length;
        glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &length);
        auto *log = new char[length + 1];
        glGetProgramInfoLog(shader_program, length, &length, &log[0]);
        error(log);
        throw EXIT_FAILURE;
    }
}

// Kompiliert und linkt Vertex- und Fragment-Shader zu einem Shader-Programm
GLuint OpenGLRenderer::compile_and_link_shader(const char *vertex_src, const char *fragment_src)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    compile_shader(vertexShader, vertex_src);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    compile_shader(fragmentShader, fragment_src);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    check_link_status(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

// Destruktor: Gibt das VAO frei
OpenGLView::~OpenGLView()
{
    glDeleteVertexArrays(1, &vao);
}

// Rendert das Objekt mit der übergebenen Transformationsmatrix
void OpenGLView::render(SquareMatrix<float, 4> &matrice)
{
    glBindVertexArray(vao);
	//Shader Programm auswählen
    glUseProgram(shaderProgram);
    unsigned int transformLoc;
    if (is3d)
    {
        transformLoc = glGetUniformLocation(shaderProgram, "model");
    }
    else
    {
        transformLoc = glGetUniformLocation(shaderProgram, "transform");
    }
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &matrice[0][0]);

    const int vertex_division = is3d ? 9 : 1;
    // Zeichnet die Geometrie
    glDrawArrays(mode, 0, vertices_size / vertex_division);
}

// ===================== TypedBodyView =====================

// Erweiterung von OpenGLView für Objekte mit Typ (z.B. Raumschiff, Asteroid)
// draw: Funktion, die entscheidet, ob das Objekt gezeichnet wird
// modify: Funktion, die vor dem Zeichnen Modifikationen erlaubt (z.B. Skalierung)
TypedBodyView::TypedBodyView(TypedBody *typed_body, GLuint vbo, unsigned int shaderProgram, size_t vertices_size,
                             float scale, GLuint mode, bool is3d,
                             std::function<bool()> draw, std::function<void(TypedBodyView *)> modify)
    : OpenGLView(vbo, shaderProgram, vertices_size, mode, is3d), typed_body(typed_body), scale(scale), draw(draw),
      modify(modify)
{
}

// Erstellt eine Transformationsmatrix für Position, Rotation und Skalierung eines Objekts
SquareMatrix4df TypedBodyView::create_object_transformation(Vector3df direction, float angle, float scale)
{
    SquareMatrix4df translation = {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {direction[0], direction[1], direction[2], 1.0f}};
    SquareMatrix4df rotation = {
        {std::cos(angle), std::sin(angle), 0.0f, 0.0f},
        {-std::sin(angle), std::cos(angle), 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}};
    SquareMatrix4df scaling = {
        {scale, 0.0f, 0.0f, 0.0f},
        {0.0f, scale, 0.0f, 0.0f},
        {0.0f, 0.0f, scale, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}};

    return translation * rotation * scaling;
}

// Rendert das Objekt, falls draw() true liefert, und ruft ggf. modify() auf
void TypedBodyView::render(SquareMatrix<float, 4> &world)
{
    if (draw())
    {
        modify(this);
        Vector2df position_2d = typed_body->get_position();
        Vector3df position_3d = {position_2d[0], position_2d[1], 0.0f};
        auto transform = world * create_object_transformation(position_3d, typed_body->get_angle(), scale);
        OpenGLView::render(transform);
    }
}

TypedBody *TypedBodyView::get_typed_body()
{
    return typed_body;
}

void TypedBodyView::set_scale(float scale)
{
    this->scale = scale;
}

// ===================== OpenGLRenderer =====================

// Erzeugt VBOs für alle 2D-Objekte und lädt die Daten in die GPU
void OpenGLRenderer::createVbos()
{
    vbos.resize(vertice_data.size());
    glGenBuffers(vbos.size(), vbos.data());

    for (size_t i = 0; i < vertice_data.size(); i++)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
        glBufferData(GL_ARRAY_BUFFER, vertice_data[i]->size() * sizeof(Vector2df), vertice_data[i]->data(),
                     GL_STATIC_DRAW);
    }
}

// Erzeugt VBOs für alle 3D-Objekte und lädt die Daten in die GPU
void OpenGLRenderer::create3dVbos()
{
    vbos3d.resize(vertice_3d_data.size());
    glGenBuffers(vbos3d.size(), vbos3d.data());

    for (size_t i = 0; i < vertice_3d_data.size(); i++)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbos3d[i]);
        glBufferData(GL_ARRAY_BUFFER, vertice_3d_data[i].size() * sizeof(float), vertice_3d_data[i].data(),
                     GL_STATIC_DRAW);
    }
}

// Erstellt die Views für das Raumschiff und dessen Flamme (3D)
void OpenGLRenderer::create(Spaceship *ship, std::vector<std::unique_ptr<TypedBodyView>> &views)
{
    debug(4, "create(Spaceship *) entry...");
    // Raumschiff
    views.push_back(std::make_unique<TypedBodyView>(ship, vbos3d[0], shaderProgram3d, vertice_3d_data[0].size(), 1.0f,
                                                    GL_TRIANGLES, true,
                                                    [ship]() -> bool
                                                    {
                                                        return !ship->is_in_hyperspace();
                                                    }) // nur anzeigen, wenn nicht im Hyperspace
    );
    // Flamme
    views.push_back(std::make_unique<TypedBodyView>(ship, vbos3d[5], shaderProgram3d, vertice_3d_data[5].size(), 1.0f,
                                                    GL_TRIANGLES, true,
                                                    [ship]() -> bool
                                                    {
                                                        return !ship->is_in_hyperspace() && ship->is_accelerating();
                                                    }) // nur anzeigen, wenn beschleunigt wird
    );

    debug(4, "create(Spaceship *) exit.");
}

// Erstellt die View für die fliegende Untertasse (Saucer)
void OpenGLRenderer::create(Saucer *saucer, std::vector<std::unique_ptr<TypedBodyView>> &views)
{
    debug(4, "create(Saucer *) entry...");
    float scale = 0.5;
    if (saucer->get_size() == 0)
    {
        scale = 0.25;
    }
    views.push_back(std::make_unique<TypedBodyView>(saucer, vbos3d[1], shaderProgram3d, vertice_3d_data[1].size(), scale, GL_TRIANGLES, true));
    debug(4, "create(Saucer *) exit.");
}

// Erstellt die View für das Torpedo-Objekt
void OpenGLRenderer::create(Torpedo *torpedo, std::vector<std::unique_ptr<TypedBodyView>> &views)
{
    debug(4, "create(Torpedo *) entry...");
    views.push_back(std::make_unique<TypedBodyView>(torpedo, vbos3d[4], shaderProgram3d, vertice_3d_data[4].size(), 1.0f,
                                                    GL_TRIANGLES, true));
    debug(4, "create(Torpedo *) exit.");
}

// Erstellt die View für einen Asteroiden
void OpenGLRenderer::create(Asteroid *asteroid, std::vector<std::unique_ptr<TypedBodyView>> &views)
{
    debug(4, "create(Asteroid *) entry...");
    float scale = (asteroid->get_size() == 3 ? 1.0 : (asteroid->get_size() == 2 ? 0.5 : 0.25));

    views.push_back(std::make_unique<TypedBodyView>(asteroid, vbos3d[2], shaderProgram3d,
                                                    vertice_3d_data[2].size(),
                                                    scale, GL_TRIANGLES, true));
    debug(4, "create(Asteroid *) exit.");
}

// Erstellt die View für Raumschiff-Trümmer (3D)
void OpenGLRenderer::create(SpaceshipDebris *debris, std::vector<std::unique_ptr<TypedBodyView>> &views)
{
    debug(4, "create(SpaceshipDebris *) entry...");
    views.push_back(std::make_unique<TypedBodyView>(debris, vbos3d[3], shaderProgram3d, vertice_3d_data[3].size(), 0.1f, GL_POINTS, false, []() -> bool
                                                    { return true; }, [debris](TypedBodyView *view) -> void
                                                    { view->set_scale(
                                                          0.2f * (SpaceshipDebris::TIME_TO_DELETE - debris->get_time_to_delete())); }));
    debug(4, "create(SpaceshipDebris *) exit.");
}

// Erstellt die View für allgemeine Trümmer (Debris, 3D)
void OpenGLRenderer::create(Debris *debris, std::vector<std::unique_ptr<TypedBodyView>> &views)
{
    debug(4, "create(Debris *) entry...");
    views.push_back(std::make_unique<TypedBodyView>(debris, vbos3d[3], shaderProgram3d, vertice_3d_data[3].size(), 0.1f, GL_TRIANGLES, true, []() -> bool
                                                    { return true; }, [debris](TypedBodyView *view) -> void
                                                    { view->set_scale(
                                                          Debris::TIME_TO_DELETE - debris->get_time_to_delete()); }));
    debug(4, "create(Debris *) exit.");
}

// Erstellt die Views für die Ziffern (0-9)
void OpenGLRenderer::createDigitViews()
{
    for (size_t i = 0; i < 10; i++)
    {
        digit_views[i] = std::make_unique<OpenGLView>(vbos[11 + i], shaderProgram, vertice_data[11 + i]->size(),
                                                      GL_LINE_STRIP, false);
    }
}

// Zeichnet die verbleibenden Leben (Schiffe) am oberen Bildschirmrand
void OpenGLRenderer::renderFreeShips(const SquareMatrix4df &matrice) const
{
    constexpr float FREE_SHIP_X = 128;
    constexpr float FREE_SHIP_Y = 64;
    constexpr float LIFE_SCALE = 0.8f;
    const float SHIP_SPACING = 40.0f;
    const int num_ships = game.get_no_of_ships();
    const auto PIf = static_cast<float>(PI);
    // Startposition so berechnen, dass die Anzeige rechtsbündig ist
    float start_x = FREE_SHIP_X - (SHIP_SPACING * (num_ships - 1));
    Vector2df position = {FREE_SHIP_X, FREE_SHIP_Y};
    SquareMatrix4df rotation = {
        {std::cos(-PIf / 2.0f), std::sin(-PIf / 2.0f), 0.0f, 0.0f},
        {-std::sin(-PIf / 2.0f), std::cos(-PIf / 2.0f), 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}};
    if (vbos3d.empty() || vertice_3d_data.empty() || vertice_3d_data[0].empty())
    {
        // Fallback: 2D-Schiff
        for (int i = 0; i < num_ships; i++)
        {
            float x = start_x + i * SHIP_SPACING;
            SquareMatrix4df scale = {
                {LIFE_SCALE * 40.0f, 0.0f, 0.0f, 0.0f},
                {0.0f, LIFE_SCALE * 40.0f, 0.0f, 0.0f},
                {0.0f, 0.0f, 1.0f, 0.0f},
                {0.0f, 0.0f, 0.0f, 1.0f}};
            SquareMatrix4df translation = {
                {1.0f, 0.0f, 0.0f, 0.0f},
                {0.0f, 1.0f, 0.0f, 0.0f},
                {0.0f, 0.0f, 1.0f, 0.0f},
                {x, FREE_SHIP_Y, 0.0f, 1.0f}};
            SquareMatrix4df render_matrice = matrice * translation * rotation * scale;
            if (spaceship_view)
                spaceship_view->render(render_matrice);
        }
        return;
    }
    // 3D-Schiff
    for (int i = 0; i < num_ships; i++)
    {
        float x = start_x + i * SHIP_SPACING;
        SquareMatrix4df scale = {
            {LIFE_SCALE, 0.0f, 0.0f, 0.0f},
            {0.0f, LIFE_SCALE, 0.0f, 0.0f},
            {0.0f, 0.0f, LIFE_SCALE, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}};
        SquareMatrix4df translation = {
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {x, FREE_SHIP_Y, 0.0f, 1.0f}};
        SquareMatrix4df render_matrice = matrice * translation * rotation * scale;
        OpenGLView ship3d_view(vbos3d[0], shaderProgram3d, vertice_3d_data[0].size(), GL_TRIANGLES, true);
        ship3d_view.render(render_matrice);
    }
}

// Zeichnet die aktuelle Punktzahl als Ziffern am oberen Bildschirmrand
void OpenGLRenderer::renderScore(const SquareMatrix4df &matrice) const
{
    constexpr float SCORE_X = 128 - 48;
    constexpr float SCORE_Y = 48 - 4;

    long long score = game.get_score();
    int no_of_digits = 0;
    if (score > 0)
    {
        no_of_digits = std::trunc(std::log10(score)) + 1;
    }

    Vector2df position = {SCORE_X + 20.0f * no_of_digits, SCORE_Y};
    do
    {
        int d = score % 10;
        score /= 10;
        SquareMatrix4df scale_translation = {
            {4.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 4.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {position[0], position[1], 0.0f, 1.0f}};
        SquareMatrix4df render_matrice = matrice * scale_translation;
        digit_views[d]->render(render_matrice);
        no_of_digits--;
        position[0] -= 20;
    } while (no_of_digits > 0);
}

// Erstellt und linkt die Shader-Programme für die 3D-Objekte
void OpenGLRenderer::create_3dshader_programs()
{
    // Vertex-Shader für 3D-Objekte
    const auto vertexShaderSource3d = "#version 330 core\n"
                                      "layout (location = 0) in vec3 position;\n"
                                      "layout (location = 1) in vec3 incolor;\n"
                                      "layout (location = 2) in vec3 innormal;\n"
                                      "out vec3 color;\n"
                                      "out vec4 normal;\n"
                                      "uniform mat4 model;\n"
                                      "void main()\n"
                                      "{\n"
                                      "gl_Position = model * vec4(position, 1.0);\n"
                                      "color = incolor;\n"
                                      "normal = normalize( model * vec4(innormal, 1.0));\n"
                                      "}\0";

    // Fragment-Shader für 3D-Objekte
    auto const fragmentShaderSource3d = "#version 330 core\n"
                                        "out vec4 outColor;\n"
                                        "in vec3 color;\n"
                                        "in vec4 normal;\n"
                                        "void main () {\n"
                                        "  outColor = vec4(color * (0.4 + 0.6 * max(0.0, dot(normal, normalize( vec4(0.0, 1.0, -4.0, 0.0))))) , 1.0);\n"
                                        "}\n\0";

    // Kompilieren und Linken der Shader
    const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    compile_shader(vertexShader, vertexShaderSource3d);
    const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    compile_shader(fragmentShader, fragmentShaderSource3d);
    shaderProgram3d = glCreateProgram();
    glAttachShader(shaderProgram3d, vertexShader);
    glAttachShader(shaderProgram3d, fragmentShader);
    glBindFragDataLocation(shaderProgram3d, 0, "outColor");
    glLinkProgram(shaderProgram3d);
    GLint status;
    glGetProgramiv(shaderProgram3d, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLint length;
        glGetProgramiv(shaderProgram3d, GL_INFO_LOG_LENGTH, &length);
        auto *log = new char[length + 1];
        glGetProgramInfoLog(shaderProgram3d, length, &length, &log[0]);
        error(log);
        throw EXIT_FAILURE;
    }
}

// Konstruktor für den Renderer: Initialisiert Fensterparameter und Titel
OpenGLRenderer::OpenGLRenderer(Game &game, std::string title, int window_width, int window_height)
    : Renderer(game), title(std::move(title)), window_width(window_width), window_height(window_height) {}

// Erstellt und linkt die Shader-Programme für die 2D-Objekte
void OpenGLRenderer::create_shader_programs()
{
    static auto vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec2 p;\n"
                                     "uniform mat4 transform;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = transform * vec4(p, 1.0, 1.0);\n"
                                     "}\0";

    static auto fragmentShaderSource = "#version 330 core\n"
                                       "out vec4 FragColor;\n"
                                       "void main()\n"
                                       "{\n"
                                       "   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
                                       "}\n\0";

    // Kompilieren und Linken der Shader (analog zu oben)
    const unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        error(std::string("vertex shader compilation failed") + infoLog);
    }

    const unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        error(std::string("fragment shader compilation failed") + infoLog);
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        error(std::string("linking shader programs failed") + infoLog);
    }
}

// Standardmaterial für Wavefront-Objekte
Material default_material = {{1.0f, 1.0f, 1.0f}};

// Wandelt die von WavefrontImporter geladenen Daten in ein Vertex-Array für OpenGL um
std::vector<float> create_vertices(WavefrontImporter &wi)
{
    std::vector<float> vertices;
    for (auto &face : wi.get_faces())
    {
        if (face.material == nullptr)
            face.material = &default_material;
        for (const auto &group : face.reference_groups)
        {
            for (size_t i = 0; i < 3; i++)
            {
                vertices.push_back(group.vertice[i]);
            }
            for (size_t i = 0; i < 3; i++)
            {
                vertices.push_back(group.normal[i]);
            }
            for (size_t i = 0; i < 3; i++)
            {
                vertices.push_back(face.material->ambient[i]);
            }
        }
    }
    return vertices;
}

// Lädt die 3D-Objekte aus Wavefront-Obj-Dateien
bool OpenGLRenderer::load_objects(const std::vector<std::string> &object_files)
{
    vertice_3d_data.clear();
    bool success = true;

    for (const auto &file : object_files)
    {
        std::ifstream in(file);
        if (!in)
        {
            std::cerr << "Failed to open file: " << file << std::endl;
            success = false;
            continue;
        }
        WavefrontImporter wi(in);
        wi.parse();
        vertice_3d_data.push_back(create_vertices(wi));
    }

    return success;
}

// Initialisiert SDL, OpenGL, GLEW, lädt Objekte und erstellt alle benötigten Ressourcen
bool OpenGLRenderer::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        error(std::string("Could not initialize SDL. SDLError: ") + SDL_GetError());
    }
    else
    {
        window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width,
                                  window_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
        if (window == nullptr)
        {
            error(std::string("Could not create Window. SDLError: ") + SDL_GetError());
        }
        else
        {
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
            SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

            context = SDL_GL_CreateContext(window);

            GLenum err = glewInit(); // Muss nach dem Erstellen des OpenGL-Kontexts aufgerufen werden
            if (GLEW_OK != err)
            {
                error("Could not initialize Glew. Glew error message: ");
                error(glewGetErrorString(err));
            }
            debug(1, "Using GLEW Version: ");
            debug(1, glewGetString(GLEW_VERSION));

            SDL_GL_SetSwapInterval(1);

            // Aufgabe_3: Lade alle benötigten 3D-Objekte
            load_objects({{"spaceship.obj"}, {"saucer.obj"}, {"asteroid.obj"}, {"debris.obj"}, {"torpedo.obj"}, {"spaceship_boost.obj"}});

            create_shader_programs();
            create_3dshader_programs();
            createVbos();
            create3dVbos();
            createDigitViews();
            return true;
        }
    }
    return false;
}

// ===================== Render-Loop =====================

// Rendert die komplette Szene: Transformation, Objekte, Lebensanzeige, Punktzahl
void OpenGLRenderer::render()
{
    debug(2, "render() entry...");

    //Spielwelt Koordinaten in OpenGL-Koordinaten umwandeln
    SquareMatrix4df world_transformation =
        SquareMatrix4df{
            {2.0f / window_width, 0.0f, 0.0f, 0.0f},
            {0.0f, -2.0f / window_height, 0.0f, 0.0f},
            {0.0f, 0.0f, 2.0f / window_width, 0.0f},
            {-1.0f, 1.0f, -1.0f, 1.0f}};

    // Mittelpunkt des Fensters berechnen
    float center_x = window_width / 2.0f;
    float center_y = window_height / 2.0f;

    // Welttransformation ggf. um die Schiffsposition verschieben (Kamera folgt dem Schiff)
    SquareMatrix4df combined_transformation = world_transformation;
    if (game.ship_exists())
    {
        Spaceship *spaceship = game.get_ship();
        Vector2df current_position = spaceship->get_position();

        // Verschieben des Schiffs in die Mitte des Fensters
        SquareMatrix4df spaceship_position_translation =
            SquareMatrix4df{
                {1.0f, 0.0f, 0.0f, 0.0f},
                {0.0f, 1.0f, 0.0f, 0.0f},
                {0.0f, 0.0f, 1.0f, 0.0f},
                {center_x - current_position[0], center_y - current_position[1], 0.0f, 1.0f}};

        combined_transformation = world_transformation * spaceship_position_translation;
    }

    // Bildschirm löschen
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    debug(2, "remove views for deleted objects");

    // Entferne alle Views, deren Objekte gelöscht werden sollen
    erase_if(views, [](std::unique_ptr<TypedBodyView> &view)
             { return view->get_typed_body()->is_marked_for_deletion(); });

    // Neue Objekte aus der Physik-Engine hinzufügen
    auto new_bodies = game.get_physics().get_recently_added_bodies();
    for (Body2df *body : new_bodies)
    {
        assert(body != nullptr);
        TypedBody *typed_body = static_cast<TypedBody *>(body);
        auto type = typed_body->get_type();
        if (type == BodyType::spaceship)
        {
            create(static_cast<Spaceship *>(typed_body), views);
        }
        else if (type == BodyType::torpedo)
        {
            create(static_cast<Torpedo *>(typed_body), views);
        }
        else if (type == BodyType::asteroid)
        {
            create(static_cast<Asteroid *>(typed_body), views);
        }
        else if (type == BodyType::saucer)
        {
            create(static_cast<Saucer *>(typed_body), views);
        }
        else if (type == BodyType::spaceship_debris)
        {
            create(static_cast<SpaceshipDebris *>(typed_body), views);
        }
        else if (type == BodyType::debris)
        {
            create(static_cast<Debris *>(typed_body), views);
        }
    }

    debug(2, "render all views");
    // Kacheln für das nahtlose Zeichnen am Rand (Wrap-Around)
    const std::vector<Vector2df> kachels = {
        {static_cast<float>(-window_width), static_cast<float>(window_height)},
        {0.0f, static_cast<float>(window_height)},
        {static_cast<float>(window_width), static_cast<float>(window_height)},
        {static_cast<float>(-window_width), 0.0f},
        {0.0f, 0.0f},
        {static_cast<float>(window_width), 0.0f},
        {static_cast<float>(-window_width), static_cast<float>(-window_height)},
        {0.0f, static_cast<float>(-window_height)},
        {static_cast<float>(window_width), static_cast<float>(-window_height)}};
    for (auto &kachel : kachels)
    {
        SquareMatrix4df kachel_position_translation = {
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {kachel[0], kachel[1], 0.0f, 1.0f}};

        SquareMatrix4df configured_transformation = combined_transformation * kachel_position_translation;
        for (auto &view : views)
        {
            view->render(configured_transformation);
        }
    }

    // Lebensanzeige und Punktzahl rendern
    renderFreeShips(world_transformation);
    renderScore(world_transformation);

    SDL_GL_SwapWindow(window);
    debug(2, "render() exit.");
}

// Gibt alle Ressourcen frei und beendet SDL
void OpenGLRenderer::exit()
{
    views.clear();
    glDeleteBuffers(vbos.size(), vbos.data());
    glDeleteBuffers(vbos3d.size(), vbos3d.data());
    glDeleteProgram(shaderProgram);
    glDeleteProgram(shaderProgram3d);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Destruktor: Gibt alle Ressourcen frei
OpenGLRenderer::~OpenGLRenderer()
{
    views.clear();
    glDeleteBuffers(vbos.size(), vbos.data());
    glDeleteBuffers(vbos3d.size(), vbos3d.data());
    glDeleteProgram(shaderProgram);
    glDeleteProgram(shaderProgram3d);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
