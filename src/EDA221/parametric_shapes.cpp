#include "parametric_shapes.hpp"
#include "core/Log.h"
#include "core/utils.h"

#include <glm/glm.hpp>

#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

eda221::mesh_data
parametric_shapes::createQuad(unsigned int width, unsigned int height)
{

	//! \todo Fill in the blank
    //auto const verticesNB = 2500;
    auto const verticesNB = width*height;
    auto vertices = std::vector<glm::vec3>(verticesNB);
    //auto vertices = std::array<glm::vec3, 1000>();
    auto texcoords = std::vector<glm::vec3>(verticesNB);
    /*{
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 0.0f, 0.0f),
        glm::vec3(3.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 1.0f),
        glm::vec3(2.0f, 0.0f, 1.0f),
        glm::vec3(3.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 2.0f),
        glm::vec3(1.0f, 0.0f, 2.0f),
        glm::vec3(2.0f, 0.0f, 2.0f),
        glm::vec3(3.0f, 0.0f, 2.0f),
        glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(1.0f, 0.0f, 3.0f),
        glm::vec3(2.0f, 0.0f, 3.0f),
        glm::vec3(3.0f, 0.0f, 3.0f)
    };*/
    
    int verticesIndex = 0;
    for (float i = 0; i < height; i++) {
        for (float j = 0; j < width; j++) {
            vertices[verticesIndex] = glm::vec3(j, 0, i);
            texcoords[verticesIndex] = glm::vec3(j/(width-1),i/(height-1),0);
            verticesIndex++;
        }
    }
    
    
    
	/*auto const vertices = std::array<glm::vec3, 4>{
		glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(static_cast<float>(width), 0.0f, 0.0f),
			glm::vec3(static_cast<float>(width), static_cast<float>(height), 0.0f),
			glm::vec3(0.0f, static_cast<float>(height), 0.0f)
	};*/
    
    auto const indicesNB = (width-1)*(height-1)*2;
    
    auto indices = std::vector<glm::uvec3>(indicesNB);/*{
        glm::uvec3(0u, 1u, 5u),
        glm::uvec3(0u, 5u, 4u),
        glm::uvec3(1u, 2u, 6u),
        glm::uvec3(1u, 6u, 5u),
        glm::uvec3(2u, 3u, 7u),
        glm::uvec3(2u, 7u, 6u),
        glm::uvec3(4u, 5u, 9u),
        glm::uvec3(4u, 9u, 8u),
        glm::vec3(5u, 6u, 10u),
        glm::vec3(5u, 10u, 9u),
        glm::vec3(6u, 7u, 11u),
        glm::vec3(6u, 11u, 10u),
        glm::vec3(8u, 9u, 13u),
        glm::vec3(8u, 13u, 12u),
        glm::vec3(9u, 10u, 14u),
        glm::vec3(9u, 14u, 13u),
        glm::vec3(10u, 11u, 15u),
        glm::vec3(10u, 15u, 14u)
    };*/
    
    /*indices[0] = glm::vec3(0u, 1u, 5u);
    indices[1] = glm::vec3(0u, 5u, 4u);
    indices[2] = glm::vec3(1u, 2u, 6u);
    indices[3] = glm::vec3(1u, 6u, 5u);
    indices[4] = glm::vec3(2u, 3u, 7u);
    indices[5] = glm::vec3(2u, 7u, 6u);
    indices[6] = glm::vec3(4u, 5u, 9u);
    indices[7] = glm::vec3(4u, 9u, 8u);
    indices[8] = glm::vec3(5u, 6u, 10u);
    indices[9] = glm::vec3(5u, 10u, 9u);
    indices[10] = glm::vec3(6u, 7u, 11u);
    indices[11] = glm::vec3(6u, 11u, 10u);
    indices[12] = glm::vec3(8u, 9u, 13u);
    indices[13] = glm::vec3(8u, 13u, 12u);
    indices[14] = glm::vec3(9u, 10u, 14u);
    indices[15] = glm::vec3(9u, 14u, 13u);
    indices[16] = glm::vec3(10u, 11u, 15u);
    indices[17] = glm::vec3(10u, 15u, 14u);*/
    
    int indicesIndex = 0;
    int lineCount = 0;
    
    for (int i = 0; i < verticesNB - width; i++) {
        if (lineCount == width-1) {
            lineCount = 0;
        } else {
            indices[indicesIndex++] =  glm::vec3(i, i+1, i+width+1);
            indices[indicesIndex++] = glm::vec3(i, i+width+1, i+width);
            lineCount++;
        }
        
    }
    
    /*for (unsigned int i = 0; i < 9; i++) {
        for (unsigned int j = 0; j < 9; j++) {
            indices[indicesIndex] =  glm::vec3(i+j, i+j+1, i+j+11);
            indices[indicesIndex+1] = glm::vec3(i+j, i+j+11, i+j+10);
            indicesIndex += 2;
        }
    }*/
    

	/*auto indices = std::array<glm::uvec3, 2>{
		glm::uvec3(0u, 1u, 2u),
			glm::uvec3(0u, 2u, 3u)
	};*/

	eda221::mesh_data data;

	//
	// NOTE:
	//
	// Only the values preceeded by a `\todo` tag should be changed, the
	// other ones are correct!
	//

	// Create a Vertex Array Object: it will remember where we stored the
	// data on the GPU, and  which part corresponds to the vertices, which
	// one for the normals, etc.
	//
	// The following function will create new Vertex Arrays, and pass their
	// name in the given array (second argument). Since we only need one,
	// pass a pointer to `data.vao`.
	glGenVertexArrays(1, &data.vao);

	// To be able to store information, the Vertex Array has to be bound
	// first.
	glBindVertexArray(data.vao);
    
    auto const vertices_offset = 0u;
    auto const vertices_size = static_cast<GLsizeiptr>(vertices.size() * sizeof(glm::vec3));
    auto const texcoords_offset = vertices_offset + vertices_size;
    auto const texcoords_size = static_cast<GLsizeiptr>(texcoords.size() * sizeof(glm::vec3));
    auto const bo_size = static_cast<GLsizeiptr>(vertices_size
                                                 + texcoords_size
                                                 );
    

	// To store the data, we need to allocate buffers on the GPU. Let's
	// allocate a first one for the vertices.
	//
	// The following function's syntax is similar to `glGenVertexArray()`:
	// it will create multiple OpenGL objects, in this case buffers, and
	// return their names in an array. Have the buffer's name stored into
	// `data.bo`.
	glGenBuffers(1, &data.bo);

	// Similar to the Vertex Array, we need to bind it first before storing
	// anything in it. The data stored in it can be interpreted in
	// different ways. Here, we will say that it is just a simple 1D-array
	// and therefore bind the buffer to the corresponding target.
	glBindBuffer(GL_ARRAY_BUFFER, data.bo);

	glBufferData(GL_ARRAY_BUFFER, /*! \todo how many bytes should the buffer contain? */bo_size,
		/* where is the data stored on the CPU? */nullptr,
		/* inform OpenGL that the data is modified once, but used often */GL_STATIC_DRAW);

	// Vertices have been just stored into a buffer, but we still need to
	// tell Vertex Array where to find them, and how to interpret the data
	// within that buffer.
	//
	// You will see shaders in more detail in lab 3, but for now they are
	// just pieces of code running on the GPU and responsible for moving
	// all the vertices to clip space, and assigning a colour to each pixel
	// covered by geometry.
	// Those shaders have inputs, some of them are the data we just stored
	// in a buffer object. We need to tell the Vertex Array which inputs
	// are enabled, and this is done by the following line of code, which
	// enables the input for vertices:
    
    
    
    
    glBufferSubData(GL_ARRAY_BUFFER, vertices_offset, vertices_size, static_cast<GLvoid const*>(vertices.data()));
	glEnableVertexAttribArray(static_cast<unsigned int>(eda221::shader_bindings::vertices));

	// Once an input is enabled, we need to explain where the data comes
	// from, and how it interpret it. When calling the following function,
	// the Vertex Array will automatically use the current buffer bound to
	// GL_ARRAY_BUFFER as its source for the data. How to interpret it is
	// specified below:
	glVertexAttribPointer(static_cast<unsigned int>(eda221::shader_bindings::vertices),
		/*! \todo how many components do our vertices have? */3,
		/* what is the type of each component? */GL_FLOAT,
		/* should it automatically normalise the values stored */GL_FALSE,
		/* once all components of a vertex have been read, how far away (in bytes) is the next vertex? */0,
		/* how far away (in bytes) from the start of the buffer is the first vertex? */reinterpret_cast<GLvoid const*>(vertices_offset));
    
    
    
    
    glBufferSubData(GL_ARRAY_BUFFER, texcoords_offset, texcoords_size, static_cast<GLvoid const*>(texcoords.data()));
    glEnableVertexAttribArray(static_cast<unsigned int>(eda221::shader_bindings::texcoords));
    glVertexAttribPointer(static_cast<unsigned int>(eda221::shader_bindings::texcoords), 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid const*>(texcoords_offset));

	// Now, let's allocate a second one for the indices.
	//
	// Have the buffer's name stored into `data.ibo`.
	glGenBuffers(1, /*! \todo fill me */&data.ibo);

	// We still want a 1D-array, but this time it should be a 1D-array of
	// elements, aka. indices!
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, /*! \todo bind the previously generated Buffer */data.ibo);

	data.indices_nb = /*! \todo how many indices do we have? */indices.size() * 3u;

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, /*! \todo how many bytes should the buffer contain? */static_cast<GLsizeiptr>(indices.size() * sizeof(glm::uvec3)),
		/* where is the data stored on the CPU? */indices.data(),
		/* inform OpenGL that the data is modified once, but used often */GL_STATIC_DRAW);



	// All the data has been recorded, we can unbind them.
	glBindVertexArray(0u);
	glBindBuffer(GL_ARRAY_BUFFER, 0u);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);

	return data;
}

eda221::mesh_data
parametric_shapes::createSphere(unsigned int const res_theta,
	unsigned int const res_phi, float const radius)
{
	auto const vertices_nb = res_phi * res_theta;
	auto vertices = std::vector<glm::vec3>(vertices_nb);
	auto normals = std::vector<glm::vec3>(vertices_nb);
	auto tangents = std::vector<glm::vec3>(vertices_nb);
	auto binormals = std::vector<glm::vec3>(vertices_nb);
	auto texcoords = std::vector<glm::vec3>(vertices_nb);


	float theta = 0.0f,                                                        // 'stepping'-variable for theta: will go 0 - 2PI
		dtheta = 2.0f * bonobo::pi / (static_cast<float>(res_theta) - 1.0f); // step size, depending on the resolution

	float phi = 0.0f,                                                                     // 'stepping'-variable for radius: will go inner_radius - outer_radius
		dphi = (bonobo::pi) / (static_cast<float>(res_phi) - 1.0f); // step size, depending on the resolution

	size_t index = 0u;
	for (unsigned int i = 0u; i < res_theta; ++i) {
		float cos_theta = std::cos(theta),
			sin_theta = std::sin(theta);


		phi = 0;
		for (unsigned int j = 0u; j < res_phi; ++j) {
			float cos_phi = std::cos(phi),
				sin_phi = std::sin(phi);
			// vertex
			vertices[index] = glm::vec3(radius * sin_theta * sin_phi,
				(-radius) * cos_phi,
				radius * cos_theta * sin_phi);


			// texture coordinates
			texcoords[index] = glm::vec3(static_cast<float>(i) / (static_cast<float>(res_phi) - 1.0f),
				static_cast<float>(j) / (static_cast<float>(res_theta) - 1.0f),
				0.0f);



			auto t = glm::vec3(cos_theta , 0, - sin_theta );
			t = glm::normalize(t);
			tangents[index] = t;



			// binormal
			auto b = glm::vec3(radius * cos_phi * sin_theta, radius * sin_phi, radius * cos_theta * cos_phi);
			b = glm::normalize(b);
			binormals[index] = b;

			// normal

			auto const n = glm::cross(t, b);
			normals[index] = n;

			phi += dphi;
			++index;
		}

		theta += dtheta;
	}

	auto indices = std::vector<glm::uvec3>(2u * (res_theta - 1u) * (res_phi - 1u));

	index = 0u;
	for (unsigned int i = 0u; i < res_theta - 1u; ++i)
	{
		for (unsigned int j = 0u; j < res_phi - 1u; ++j)
		{
			indices[index] = glm::uvec3(res_phi * i + j,
				res_phi * i + j + 1u,
				res_phi * i + j + 1u + res_phi);
			++index;

			indices[index] = glm::uvec3(res_phi * i + j,
				res_phi * i + j + res_phi + 1u,
				res_phi * i + j + res_phi);
			++index;
		}
	}

	eda221::mesh_data data;
	glGenVertexArrays(1, &data.vao);
	assert(data.vao != 0u);
	glBindVertexArray(data.vao);


	auto const vertices_offset = 0u;
	auto const vertices_size = static_cast<GLsizeiptr>(vertices.size() * sizeof(glm::vec3));
	auto const normals_offset = vertices_size;
	auto const normals_size = static_cast<GLsizeiptr>(normals.size() * sizeof(glm::vec3));
	auto const texcoords_offset = normals_offset + normals_size;
	auto const texcoords_size = static_cast<GLsizeiptr>(texcoords.size() * sizeof(glm::vec3));
	auto const tangents_offset = texcoords_offset + texcoords_size;
	auto const tangents_size = static_cast<GLsizeiptr>(tangents.size() * sizeof(glm::vec3));
	auto const binormals_offset = tangents_offset + tangents_size;
	auto const binormals_size = static_cast<GLsizeiptr>(binormals.size() * sizeof(glm::vec3));
	auto const bo_size = static_cast<GLsizeiptr>(vertices_size
		+ normals_size
		+ texcoords_size
		+ tangents_size
		+ binormals_size
		);
	glGenBuffers(1, &data.bo);
	assert(data.bo != 0u);
	glBindBuffer(GL_ARRAY_BUFFER, data.bo);
	glBufferData(GL_ARRAY_BUFFER, bo_size, nullptr, GL_STATIC_DRAW);





	glBufferSubData(GL_ARRAY_BUFFER, vertices_offset, vertices_size, static_cast<GLvoid const*>(vertices.data()));
	glEnableVertexAttribArray(static_cast<unsigned int>(eda221::shader_bindings::vertices));
	glVertexAttribPointer(static_cast<unsigned int>(eda221::shader_bindings::vertices), 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid const*>(0x0));

	glBufferSubData(GL_ARRAY_BUFFER, normals_offset, normals_size, static_cast<GLvoid const*>(normals.data()));
	glEnableVertexAttribArray(static_cast<unsigned int>(eda221::shader_bindings::normals));
	glVertexAttribPointer(static_cast<unsigned int>(eda221::shader_bindings::normals), 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid const*>(normals_offset));

	glBufferSubData(GL_ARRAY_BUFFER, texcoords_offset, texcoords_size, static_cast<GLvoid const*>(texcoords.data()));
	glEnableVertexAttribArray(static_cast<unsigned int>(eda221::shader_bindings::texcoords));
	glVertexAttribPointer(static_cast<unsigned int>(eda221::shader_bindings::texcoords), 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid const*>(texcoords_offset));

	glBufferSubData(GL_ARRAY_BUFFER, tangents_offset, tangents_size, static_cast<GLvoid const*>(tangents.data()));
	glEnableVertexAttribArray(static_cast<unsigned int>(eda221::shader_bindings::tangents));
	glVertexAttribPointer(static_cast<unsigned int>(eda221::shader_bindings::tangents), 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid const*>(tangents_offset));

	glBufferSubData(GL_ARRAY_BUFFER, binormals_offset, binormals_size, static_cast<GLvoid const*>(binormals.data()));
	glEnableVertexAttribArray(static_cast<unsigned int>(eda221::shader_bindings::binormals));
	glVertexAttribPointer(static_cast<unsigned int>(eda221::shader_bindings::binormals), 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid const*>(binormals_offset));



	glBindBuffer(GL_ARRAY_BUFFER, 0u);

	data.indices_nb = indices.size() * 3u;
	glGenBuffers(1, &data.ibo);
	assert(data.ibo != 0u);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(glm::uvec3)), reinterpret_cast<GLvoid const*>(indices.data()), GL_STATIC_DRAW);

	glBindVertexArray(0u);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);

	return data;

	//////////////////////////////////////////////////////






	//! \todo (Optional) Implement this function
	return eda221::mesh_data{ 0u, 0u, 0u, 0u };
}

eda221::mesh_data
parametric_shapes::createTorus(unsigned int const res_theta,
	unsigned int const res_phi, float const rA,
	float const rB)
{
    auto const vertices_nb = res_phi * res_theta;
    int radius = 0;
    
    auto vertices = std::vector<glm::vec3>(vertices_nb);
    auto normals = std::vector<glm::vec3>(vertices_nb);
    auto texcoords = std::vector<glm::vec3>(vertices_nb);
    auto tangents = std::vector<glm::vec3>(vertices_nb);
    auto binormals = std::vector<glm::vec3>(vertices_nb);
    
    float theta = 0.0f,                                                        // 'stepping'-variable for theta: will go 0 - 2PI
    dtheta = 2.0f * bonobo::pi / (static_cast<float>(res_theta) - 1.0f); // step size, depending on the resolution
    
    float phi = 0.0f,                                                                     // 'stepping'-variable for radius: will go inner_radius - outer_radius
    dphi = (bonobo::pi) / (static_cast<float>(res_phi) - 1.0f); // step size, depending on the resolution
    
    size_t index = 0u;
    for (unsigned int i = 0u; i < res_theta; ++i) {
        float cos_theta = std::cos(theta),
        sin_theta = std::sin(theta);
        
        
        phi = 0;
        for (unsigned int j = 0u; j < res_phi; ++j) {
            float cos_phi = std::cos(phi),
            sin_phi = std::sin(phi);
            // vertex
            vertices[index] = glm::vec3(radius * sin_theta * sin_phi,
                                        (-radius) * cos_phi,
                                        radius * cos_theta * sin_phi);
            
            
            // texture coordinates
            texcoords[index] = glm::vec3(static_cast<float>(i) / (static_cast<float>(res_phi) - 1.0f),
                                         static_cast<float>(j) / (static_cast<float>(res_theta) - 1.0f),
                                         0.0f);
            
            
            
            auto t = glm::vec3(cos_theta , 0, - sin_theta );
            t = glm::normalize(t);
            tangents[index] = t;
            
            
            
            // binormal
            auto b = glm::vec3(radius * cos_phi * sin_theta, radius * sin_phi, radius * cos_theta * cos_phi);
            b = glm::normalize(b);
            binormals[index] = b;
            
            // normal
            
            auto const n = glm::cross(t, b);
            normals[index] = n;
            
            phi += dphi;
            ++index;
        }
        
        theta += dtheta;
    }
    
    
    
	return eda221::mesh_data{ 0u, 0u, 0u, 0u };
}

eda221::mesh_data
parametric_shapes::createCircleRing(unsigned int const res_radius,
	unsigned int const res_theta,
	float const inner_radius,
	float const outer_radius)
{
	auto const vertices_nb = res_radius * res_theta;

	auto vertices = std::vector<glm::vec3>(vertices_nb);
	auto normals = std::vector<glm::vec3>(vertices_nb);
	auto texcoords = std::vector<glm::vec3>(vertices_nb);
	auto tangents = std::vector<glm::vec3>(vertices_nb);
	auto binormals = std::vector<glm::vec3>(vertices_nb);

	float theta = 0.0f,                                                        // 'stepping'-variable for theta: will go 0 - 2PI
		dtheta = 2.0f * bonobo::pi / (static_cast<float>(res_theta) - 1.0f); // step size, depending on the resolution

	float radius = 0.0f,                                                                     // 'stepping'-variable for radius: will go inner_radius - outer_radius
		dradius = (outer_radius - inner_radius) / (static_cast<float>(res_radius) - 1.0f); // step size, depending on the resolution

																						   // generate vertices iteratively
	size_t index = 0u;
	for (unsigned int i = 0u; i < res_theta; ++i) {
		float cos_theta = std::cos(theta),
			sin_theta = std::sin(theta);

		radius = inner_radius;

		for (unsigned int j = 0u; j < res_radius; ++j) {
			// vertex
			vertices[index] = glm::vec3(radius * cos_theta,
				radius * sin_theta,
				0.0f);

			// texture coordinates
			texcoords[index] = glm::vec3(static_cast<float>(j) / (static_cast<float>(res_radius) - 1.0f),
				static_cast<float>(i) / (static_cast<float>(res_theta) - 1.0f),
				0.0f);

			// tangent
			auto t = glm::vec3(cos_theta, sin_theta, 0.0f);
			t = glm::normalize(t);
			tangents[index] = t;

			// binormal
			auto b = glm::vec3(-sin_theta, cos_theta, 0.0f);
			b = glm::normalize(b);
			binormals[index] = b;

			// normal
			auto const n = glm::cross(t, b);
			normals[index] = n;

			radius += dradius;
			++index;
		}

		theta += dtheta;
	}

	// create index array
	auto indices = std::vector<glm::uvec3>(2u * (res_theta - 1u) * (res_radius - 1u));

	// generate indices iteratively
	index = 0u;
	for (unsigned int i = 0u; i < res_theta - 1u; ++i)
	{
		for (unsigned int j = 0u; j < res_radius - 1u; ++j)
		{
			indices[index] = glm::uvec3(res_radius * i + j,
				res_radius * i + j + 1u,
				res_radius * i + j + 1u + res_radius);
			++index;

			indices[index] = glm::uvec3(res_radius * i + j,
				res_radius * i + j + res_radius + 1u,
				res_radius * i + j + res_radius);
			++index;
		}
	}

	eda221::mesh_data data;
	glGenVertexArrays(1, &data.vao);
	assert(data.vao != 0u);
	glBindVertexArray(data.vao);

	auto const vertices_offset = 0u;
	auto const vertices_size = static_cast<GLsizeiptr>(vertices.size() * sizeof(glm::vec3));
	auto const normals_offset = vertices_size;
	auto const normals_size = static_cast<GLsizeiptr>(normals.size() * sizeof(glm::vec3));
	auto const texcoords_offset = normals_offset + normals_size;
	auto const texcoords_size = static_cast<GLsizeiptr>(texcoords.size() * sizeof(glm::vec3));
	auto const tangents_offset = texcoords_offset + texcoords_size;
	auto const tangents_size = static_cast<GLsizeiptr>(tangents.size() * sizeof(glm::vec3));
	auto const binormals_offset = tangents_offset + tangents_size;
	auto const binormals_size = static_cast<GLsizeiptr>(binormals.size() * sizeof(glm::vec3));
	auto const bo_size = static_cast<GLsizeiptr>(vertices_size
		+ normals_size
		+ texcoords_size
		+ tangents_size
		+ binormals_size
		);
	glGenBuffers(1, &data.bo);
	assert(data.bo != 0u);
	glBindBuffer(GL_ARRAY_BUFFER, data.bo);
	glBufferData(GL_ARRAY_BUFFER, bo_size, nullptr, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, vertices_offset, vertices_size, static_cast<GLvoid const*>(vertices.data()));
	glEnableVertexAttribArray(static_cast<unsigned int>(eda221::shader_bindings::vertices));
	glVertexAttribPointer(static_cast<unsigned int>(eda221::shader_bindings::vertices), 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid const*>(0x0));

	glBufferSubData(GL_ARRAY_BUFFER, normals_offset, normals_size, static_cast<GLvoid const*>(normals.data()));
	glEnableVertexAttribArray(static_cast<unsigned int>(eda221::shader_bindings::normals));
	glVertexAttribPointer(static_cast<unsigned int>(eda221::shader_bindings::normals), 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid const*>(normals_offset));

	glBufferSubData(GL_ARRAY_BUFFER, texcoords_offset, texcoords_size, static_cast<GLvoid const*>(texcoords.data()));
	glEnableVertexAttribArray(static_cast<unsigned int>(eda221::shader_bindings::texcoords));
	glVertexAttribPointer(static_cast<unsigned int>(eda221::shader_bindings::texcoords), 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid const*>(texcoords_offset));

	glBufferSubData(GL_ARRAY_BUFFER, tangents_offset, tangents_size, static_cast<GLvoid const*>(tangents.data()));
	glEnableVertexAttribArray(static_cast<unsigned int>(eda221::shader_bindings::tangents));
	glVertexAttribPointer(static_cast<unsigned int>(eda221::shader_bindings::tangents), 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid const*>(tangents_offset));

	glBufferSubData(GL_ARRAY_BUFFER, binormals_offset, binormals_size, static_cast<GLvoid const*>(binormals.data()));
	glEnableVertexAttribArray(static_cast<unsigned int>(eda221::shader_bindings::binormals));
	glVertexAttribPointer(static_cast<unsigned int>(eda221::shader_bindings::binormals), 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid const*>(binormals_offset));

	glBindBuffer(GL_ARRAY_BUFFER, 0u);

	data.indices_nb = indices.size() * 3u;
	glGenBuffers(1, &data.ibo);
	assert(data.ibo != 0u);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(glm::uvec3)), reinterpret_cast<GLvoid const*>(indices.data()), GL_STATIC_DRAW);

	glBindVertexArray(0u);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);

	return data;
}