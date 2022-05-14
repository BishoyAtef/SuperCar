#pragma once

#include <glad/gl.h>
#include "vertex.hpp"

namespace our {

    #define ATTRIB_LOC_POSITION 0
    #define ATTRIB_LOC_COLOR    1
    #define ATTRIB_LOC_TEXCOORD 2
    #define ATTRIB_LOC_NORMAL   3

    class Mesh {
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        unsigned int VBO, EBO;
        unsigned int VAO;
        // We need to remember the number of elements that will be draw by glDrawElements 
        GLsizei elementCount;
    public:

        // The constructor takes two vectors:
        // - vertices which contain the vertex data.
        // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
        // The mesh class does not keep a these data on the RAM. Instead, it should create
        // a vertex buffer to store the vertex data on the VRAM,
        // an element buffer to store the element data on the VRAM,
        // a vertex array object to define how to read the vertex & element buffer during rendering 
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& elements)
        {
            //TODO: (Req 1) Write this function
            // remember to store the number of elements in "elementCount" since you will need it for drawing
            // For the attribute locations, use the constants defined above: ATTRIB_LOC_POSITION, ATTRIB_LOC_COLOR, etc
            // configure the vertex array object

            //generating a vertex array object to store the verices to be drawn and binding it
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            // storing the elementcount
            elementCount = elements.size();
            // configure vertex buffer and attribute locations
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

            glEnableVertexAttribArray(ATTRIB_LOC_POSITION);
            //this line tells opengl that each vertics consents of three float, no normalization needed and the offset to sart reading the position
            glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, position));

            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);
            //this line tells opengl that the color each vertics consents of four float, normalization needed and the offset to sart reading the color
            //is the starting position of the color variable in the struct Vertex
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex), (void*)offsetof(Vertex, color));
        
            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);
            //this line tells opengl that the texture each vertics consents of two float, no normalization needed and the offset to sart reading the texture
            //is the starting position of the tex_coord variable in the struct Vertex
            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));

            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);
            //this line tells opengl that the normal to each vertics consents of three float, no normalization needed and the offset to sart reading the position
            //is the starting position of the normal variable in the struct Vertex
            glVertexAttribPointer(ATTRIB_LOC_NORMAL, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, normal));


            // configure element buffer
            //generating element buffer object and binding it
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            //specifing the size of the buffer object it will not be changed after that so we choose "GL_STATIC_DRAW"
            //computing the needed sapce as the prouct of the number of elements in the elemetns vector by the size of unsigned integer
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementCount*sizeof(unsigned int), elements.data(), GL_STATIC_DRAW);
        }

        // this function should render the mesh
        void draw() 
        {
            //TODO: (Req 1) Write this function
            glBindVertexArray(VAO);
            //specifing the number of elements to be drawn and their type
            glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, (void*)0);
        }

        // this function should delete the vertex & element buffers and the vertex array object
        ~Mesh(){
            //TODO: (Req 1) Write this function
            //deleting the vertics array object as it is not needed after that as well as the vertics buffer object and element buffer object to save recources
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
        }

        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };

}