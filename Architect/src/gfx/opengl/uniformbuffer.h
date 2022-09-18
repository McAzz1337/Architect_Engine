#pragma once
#include <typeindex>
#include <string>

namespace archt {

	class Uniformbuffer {

		uint32_t id = 0;
		uint32_t binding = 0;
		void* data = nullptr;
		uint32_t size = 0;
		std::string name;

	public:
		Uniformbuffer();
		Uniformbuffer(const std::string& name, void* data, uint32_t size, uint32_t binding = 0);
		~Uniformbuffer();

		void write(uint32_t offset, void* d, uint32_t size);
		void upload() const;

		int getUniformBlockLocation(uint32_t shaderId, const std::string& blockName) const;

		void bind() const;
		void bindUniformBlock(uint32_t shaderId, const std::string& blockName) const;

		inline const std::string& getName() const { return name; }
	};


}