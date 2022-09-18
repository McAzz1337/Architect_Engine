#pragma once


#include <memory>



template<class T>
using ptr = std::shared_ptr<T>;


template<class T, class ..._Types>
ptr<T> make_ptr(_Types&& ..._Args) {
	return std::make_shared<T>(_STD forward<_Types>(_Args)...);
}

