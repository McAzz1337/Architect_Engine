#pragma once

#include "../ptr.h"

#include <glm/mat4x4.hpp>

#include <vector>

#include <iostream>


namespace archt {


	class Component {

		ptr<Component> parent = nullptr;
		std::vector<ptr<Component>> children;

	public:
		Component();
		virtual ~Component();

		template<class T>
		void addComponent(ptr<T> component) {
			if (ptr<Component>  c = std::dynamic_pointer_cast<Component>(component)) {
				children.push_back(c);
			}
		}

		template<class T>
		void removeComponent(ptr<T> component) {
			if (ptr<Component>  c = std::dynamic_pointer_cast<Component>(component)) {
				int size = children.size();
				for (int i = 0; i < size; i++) {
					if (children[i].get() == c.get()) {
						children.erase(children.begin() + i);
						return;
					}
				}
			}

		}

		template<class T>
		ptr<T> getComponent() {
			
			int size = children.size();
			for (int i = 0; i < size; i++) {
				if (ptr<T> t = std::dynamic_pointer_cast<T>(children[i])) {
					//std::cout << "returned component as < " << typeid(T).name() << ">" << std::endl;
					return t;
				}
			}
			__debugbreak();
			return nullptr;
		}

		template<class T>
		ptr<T> getComponent() const {

			int size = children.size();
			for (int i = 0; i < size; i++) {
				if (ptr<T> t = std::dynamic_pointer_cast<T>(children[i])) {
					//std::cout << "returned component as < " << typeid(T).name() << ">" << std::endl;
					return t;
				}
			}
			//__debugbreak();
			return nullptr;
		}

		template<class T>
		std::vector<ptr<T>> getComponents() {
		
			std::vector<ptr<T>> list;
			int size = children.size();
			for (int i = 0; i < size; i++) {
				if (ptr<T> t = std::dynamic_pointer_cast<T>(children[i])) {
					return t;
				}
			}
			return list;
		}
	};


}