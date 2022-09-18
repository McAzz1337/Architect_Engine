#pragma once
#include "../vertex.h"
#include <typeindex>
#include "glinclude.h"

namespace archt {

	class VBO {

		uint32_t id = 0;
		Vertex* data = nullptr;
		uint32_t size = 0;

	public:
		VBO();
		VBO(Vertex* verteces, uint32_t size);
		~VBO();

		void allocateOnGPU(int size = -1, int mode = GL_DYNAMIC_DRAW);
		void deallocateOnGPU();
		void bind() const;
		void write(uint32_t offset, Vertex* d, uint32_t size);
		void upload(int offset = 0, int length = 0) const;

		void setTexId(float id);
		void setMatrixId(float id);

		void print(int end) const;

		glm::vec3* getPositionalCoordinates(int& out_size) const;
		
		void setVerteces(Vertex* verteces, uint32_t size);


		inline Vertex* getData() const { return data; }
		inline int getSize() const { return size; }
		inline int bytes() const { return size * sizeof(Vertex); }
		inline int hasGPUAllocation() const { return id != 0; }

	};



	class IBO {

		uint32_t id = 0;
		uint32_t* data = nullptr;
		uint32_t size = 0;

	public:
		IBO();
		IBO(uint32_t* indeces, uint32_t size);
		~IBO();


		void allocateOnGPU(int bytes = -1, int mode = GL_DYNAMIC_DRAW);
		void deallocateOnGPU();
		void bind() const;
		void write(uint32_t offset, uint32_t* d, uint32_t size, int vertexOffset);
		void upload(int offset = 0, int length = 0, int mode = GL_DYNAMIC_DRAW) const;

		void print(int end) const;

		void setIndeces(uint32_t* indeces, uint32_t size);

		inline uint32_t* getData() const { return data; }
		inline int getSize() const { return size; }
		inline int bytes() const { return size * sizeof(uint32_t); }
		inline int hasGPUAllocation() const { return id != 0; }
	};





}