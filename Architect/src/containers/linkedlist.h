#pragma once



namespace archt {

	template<typename T>
	struct Node {

		Node<T>* previous = nullptr;
		Node<T>* next = nullptr;

		T* data = nullptr;

		Node() {

		}

		Node(T* data) : data(data) {

		}

		~Node() {
			delete data;
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

			Node<T>* node = start;
			for (int i = 0; i < index; i++) {

				node = node->next;
			}

			return node;
		}

		void operator<<(T* data) {
			
			Node<T>* node = new Node<T>(data);
			start->previous = node;
			node->next = start;
			start = node;
		}

		void operator>>(T* data) {

			if (!start) {
				start = new Node<T>(data);
				return;
			}
			else if (!end) {
				start->next = new Node<T>(data);
				end = start->next;
				return;
			}

			Node<T>* node = new Node<T>(data);
			end->next = node;
			node->previous = end;
			end = node;
		}
	};


}