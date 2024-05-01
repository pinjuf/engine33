    ENGINE33 README

This is my attempt at learning some 3D graphics basics,
using OpenGL. Please, be free to judge me for my ambition
of actually creating an engine. (I know that's not gonna happen)

The 33 stands for the 3 dimensions in which the 3-sided polygons ("triangles" for those with an advanced vocabulary range) reside.

Wish me luck, I'm gonna be using https://www.opengl-tutorial.org 's beginner tuto.


    SHADER VARIABLES

(a) = vertex attribute
(u) = uniform

- (a) vec3 e33_vertexposition   : Vertex position (in model space)
- (a) vec3 e33_vertexnormal     : Vertex normal
- (a) vec2 e33_vertexuv         : Vertex UV
- (a) vec3 e33_vertexcolor      : Vertex color
- (u) mat4 e33_mvp              : Complete Model space -> World space -> Camera space -> Projection Space transformation matrix
- (u) mat4 e33_modelmatrix      : Model space -> World space transformation matrix
- (u) mat4 e33_cameramatrix     : World space -> Camera Space transformation matrix
- (u) mat4 e33_projectionmatrix : Camera Space -> Projection Space transformation matrix
- (u) sampler2D e33_texture0-15 : Texture #0-15
