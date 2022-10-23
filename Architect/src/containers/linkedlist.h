#pragma once



namespace archt {

	template<typename T>
	struct Node {

		Node<T>* previous = nullptr;
		Node<T>* next = nullptr;

		T data;

		Node() {

		}

		Node(const T& data) : data(data) {

		}

		~Node() {
		}

		void operator<<(Node<T>* node) {
			previous = node;
		}

		void operator>>(Node<T>* node) {
			next = node;
		}
	};

	template<typename T>
	class LinkedList {

		Node<T>* start = nullptr;
		Node<T>* end = nullptr;


	public:
		LinkedList() {

		}

		LinkedList(Node<T>* start) : start(start) {}

		~LinkedList() {

			Node<T>* node = start;
			while (node) {
				Node<T>* next = node->next;
				delete node;
				node = next;
			}
		}

		int size() {
			
			int i = 0;
			Node<T>* node = start;
			while (node) {
			
				i++;
				node = node->next;
			}

			return i;
		}

		Node<T>* operator[](int index) {

			if (size() == 0) {
				return nullptr;
			}

			Node<T>* node = start;
			for (int i = 0; i < index; i++) {

				node = node->next;
			}

			return node;
		}

		void operator<<(const T& data) {
			
			Node<T>* node = new Node<T>(data);
			

			if (size() > 0) {
				end->next = node;
				node->previous = end;
				end = node;
			}
			else {
				start = node;
				end = node;
			}
		}

		void operator>>(const T& data) {

			Node<T>* node = new Node<T>(data);

			if (size() > 0) {
				start->previous = node;
				node->next = start;
				start = node;
			}
			else {
				start = node;
				end = node;
			}
		}
	};


}