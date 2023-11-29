//
// Created by 18746 on 2023/11/16.
//

#ifndef C___WEBSERVER2_0_ALLOC_H
#define C___WEBSERVER2_0_ALLOC_H


//#include <cstddef>

#include <cstdio>

void* operator new(size_t size);
void operator delete(void* p)noexcept;
void* operator new[](size_t size);
void operator delete[](void* p)noexcept;
void* mem_alloc(size_t size);
void mem_free(void* p);



#endif //C___WEBSERVER2_0_ALLOC_H
