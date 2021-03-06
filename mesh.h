struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    GLuint id;
    string type;
};

class Mesh {
public:
    /*  Mesh Data  */
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;
    /*  Functions  */
    Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
    void Draw(Shader shader);
private:
    /*  Render data  */
    GLuint VAO, VBO, EBO;
    /*  Functions    */
    void setupMesh();
};