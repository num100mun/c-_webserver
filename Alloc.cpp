//
// Created by 18746 on 2023/11/16.
//

#include "include/Alloc.h"
#include "include/MemoryMgr.h"

void* operator new(size_t nSize){printf("new !\n");return MemoryMgr::Instance().allocMem(nSize);}
void operator delete(void* p){MemoryMgr::Instance().freeMem(p);}
void* operator new[](size_t nSize){return MemoryMgr::Instance().allocMem(nSize);}
void operator delete[](void* p){MemoryMgr::Instance().freeMem(p);}
void* mem_alloc(size_t size){return malloc(size);}
void mem_free(void* p){free(p);}
