//
// Created by 18746 on 2023/11/22.
//

#ifndef C___WEBSERVER2_0_CELLOBJECTPOOL_H
#define C___WEBSERVER2_0_CELLOBJECTPOOL_H
#include<stdlib.h>
#include<assert.h>
#include<mutex>
#include "CELLlog.h"
#ifdef _DEBUG
#ifndef xPrintf
		#include<stdio.h>
		#define xPrintf(...) printf(__VA_ARGS__)
	#endif
#else
#ifndef xPrintf
#define xPrintf(...)
#endif
#endif // _DEBUG

using namespace std;
/**
 *NodeHeader* _pHeader：对象池头部
 * mutex _mutex：锁
 * char* _pBuf：对象池大小
 * void initPool()：初始化每个对象池块
 * CELLObjectPool()：调用initPool()
 * ~CELLObjectPool()：释放掉_pBuf
 *void freeObjMemory: 释放掉对象，把内存块重新还给header
 *void* allocObjMemory:分配内存块
 * */
 /**
  *
  * class classA : public  ObjectPoolBase<classA, 100>{
public:
    int num = 0;
};
int main() {
    CELLlog::Instance().setLogPath("log.txt", "w");
    typedef ObjectPoolBase<classA,100> cl;
    cl * a = new cl();
    delete a;
    classA * a1 = new classA();
    delete a1;
    classA * a2 = cl::createObject();
    cl::destroyObject(a2);
    return 0;
}
  *
  *
  * */
template<class Type, int nPoolSize>
class CELLObjectPool{
private:
    class NodeHeader{
    public:
        NodeHeader* pNext = nullptr;
        int nID = -1;
        char nRef = -1;
        bool bPool = false;
    private:
        char c1;
        char c2;
    };
private:
    NodeHeader* _pHeader;
    char* _pBuf;
    mutex _mutex;
    void initPool(){
        CELLlog::Info("create object block %d every size %d\n", nPoolSize, sizeof(Type));
        assert(nullptr == _pBuf);
        if (_pBuf)return;
        size_t realSzie = sizeof(Type) + sizeof(NodeHeader);
        size_t n = nPoolSize*realSzie;
        _pBuf = new char[n];
        _pHeader = (NodeHeader*)_pBuf;
        _pHeader->bPool = true;
        _pHeader->nID = 0;
        _pHeader->nRef = 0;
        _pHeader->pNext = nullptr;
        NodeHeader* re = _pHeader;
        for (size_t n = 1; n < nPoolSize; n++){
            NodeHeader* p = (NodeHeader*)(_pBuf + (n* realSzie));
            p->bPool = true;
            p->nID = n;
            p->nRef = 0;
            p->pNext = nullptr;
            re->pNext = p;
            re = p;
        }
    }
public:
    CELLObjectPool(){initPool();}
    ~CELLObjectPool(){if(_pBuf)delete[] _pBuf;}
    void freeObjMemory(void* pMem){
        NodeHeader* pBlock = (NodeHeader*)((char*)pMem - sizeof(NodeHeader));
        CELLlog::Info("freeObjMemory: %llx, id=%d, size=%d\n", pBlock, pBlock->nID, sizeof(pMem));
        int ci = sizeof(NodeHeader);
        printf("%d\n",ci );
        assert(1 == pBlock->nRef);
        if (pBlock->bPool){
            lock_guard<std::mutex> lg(_mutex);
            if (--pBlock->nRef != 0)return;
            pBlock->pNext = _pHeader;
            _pHeader = pBlock;
        }
        else {
            if (--pBlock->nRef != 0)return;
            delete[] pBlock;
        }
    }
    void* allocObjMemory(size_t nSize){
        lock_guard<std::mutex> lg(_mutex);
        NodeHeader* pReturn = nullptr;
        if (nullptr == _pHeader){
            pReturn = (NodeHeader*)new char[sizeof(Type) + sizeof(NodeHeader)];
            pReturn->bPool = false;
            pReturn->nID = -1;
            pReturn->nRef = 1;
            pReturn->pNext = nullptr;
        }
        else {
            pReturn = _pHeader;
            _pHeader = _pHeader->pNext;
            assert(0 == pReturn->nRef);
            pReturn->nRef = 1;
        }
        CELLlog::Info("allocObjMemory: %llx, id=%d, size=%d\n", pReturn, pReturn->nID, nSize);
        return ((char*)pReturn + sizeof(NodeHeader));
    }

};
/**
 *
 * static ClassTypePool & objectPool() ： 返回单例池
 * void* operator new(size_t nSize):重写new方法在池中创建对象
 * void operator delete(void* p)：重写delete方法删除池中对象
 * static Type* createObject：使用普通new方法创建对象
 * static void destroyObject：使用普通的delete方法
 * */
template<class Type, size_t nPoolSize>
class ObjectPoolBase{
private:
    typedef CELLObjectPool<Type, nPoolSize> ClassTypePool;
    static ClassTypePool & objectPool(){
        static ClassTypePool sPool;
        return sPool;
    }
public:
    void* operator new(size_t nSize){return objectPool().allocObjMemory(nSize);}
    void operator delete(void* p){objectPool().freeObjMemory(p);}
    template<typename ...Args>
    static Type* createObject(Args ... args){
        Type* obj = new Type(args...);
        CELLlog::Info("create object\n");
        return obj;
    }
    static void destroyObject(Type* obj){
        delete obj;
        CELLlog::Info("destroy object\n");
    }
};





#endif //C___WEBSERVER2_0_CELLOBJECTPOOL_H
