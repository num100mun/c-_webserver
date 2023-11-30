
#ifndef C___WEBSERVER2_0_M_H
#define C___WEBSERVER2_0_M_H

#include <mutex>
#include <assert.h>
#include<stdlib.h>
using namespace std;
#ifdef _DEBUG
#include<stdio.h>
	#define xPrintf(...) printf(__VA_ARGS__)
#else
#define xPrintf(...)
#endif // _DEBUG
#define MAX_MEMORY_SIZE 1024

class MemoryAlloc;
class MemoryBlock{
public:
    MemoryAlloc* pAlloc;
    MemoryBlock* pNext;
    int nID;
    int nRef;
    int bPool;
private:
    char c1;
    char c2;
    char c3;
};

class MemoryAlloc{
protected:
    char * _pBuff = nullptr;
    MemoryBlock* _pHeader = nullptr;
    size_t _nSize;
    size_t _nBlockSize;
    mutex _mutex;
public:
    MemoryAlloc(){};
    ~MemoryAlloc(){if(_pBuff) free(_pBuff);}
    void InitMemory(){
        assert(_pBuff == nullptr);
        if(_pBuff) return;
        size_t realSize = _nSize + sizeof(MemoryBlock);
        size_t bufSize = realSize * _nBlockSize;
        _pBuff = (char *)malloc(bufSize);
        _pHeader = (MemoryBlock*)_pBuff;
        _pHeader->bPool = true;
        _pHeader->pNext = nullptr;
        _pHeader->nRef = 0;
        _pHeader->pAlloc = this;
        _pHeader->nID = 0;
        MemoryBlock * h = _pHeader;
        for(size_t i = 1; i<_nBlockSize; i++){
            MemoryBlock* temp = (MemoryBlock*)(_pBuff + i*realSize);
            temp->bPool = true;
            temp->nID = i;
            temp->nRef = 0;
            temp->pAlloc = this;
            temp->pNext = nullptr;
            h->pNext = temp;
            h = temp;
        }
    }
    void * allocMemory(size_t nSize){

        return (char*) malloc(nSize);
        lock_guard<mutex> lg(_mutex);
        if (!_pBuff) InitMemory();

        MemoryBlock* pReturn = nullptr;
        if (nullptr == _pHeader){
            pReturn = (MemoryBlock*)malloc(nSize+sizeof(MemoryBlock));
            pReturn->bPool = false;
            pReturn->nID = -1;
            pReturn->nRef = 1;
            pReturn->pAlloc = nullptr;
            pReturn->pNext = nullptr;
            printf("allocMem: %llx, id=%d, size=%d\n", pReturn, pReturn->nID, nSize);
        }
        else{
            pReturn = _pHeader;
            _pHeader = _pHeader->pNext;
//            pReturn->pNext = nullptr;
            assert(0 == pReturn->nRef);
            pReturn->nRef = 1;
        }
        return ((char *)pReturn + sizeof(MemoryBlock));
    }
    void freeMemory(void * pMem){
        MemoryBlock* pBlock = (MemoryBlock*) ((char*)pMem - sizeof(MemoryBlock));
        assert(pBlock->nRef == 1);
        if(pBlock->bPool){
            lock_guard<mutex> lg(_mutex);
            if(--pBlock->nRef != 0) return ;
            pBlock->pNext = _pHeader;
            _pHeader = pBlock;
        }else if(--pBlock->nRef !=0) return;free(pBlock);
    }
};

template<size_t nSize, size_t nBlockSize>
class MemoryAlloctor: public MemoryAlloc{
public:
    MemoryAlloctor(){
        const size_t n = sizeof(void *);
        _nSize = (nSize/n)*n + (nSize % n ?n : 0);
        _nBlockSize = nBlockSize;
    }
};

class MemoryMgr{
private:
    MemoryAlloctor<64, 100> _mem64;
    MemoryAlloctor<128, 100> _mem128;
    MemoryAlloctor<256, 100> _mem256;
    MemoryAlloctor<512, 100> _mem512;
    MemoryAlloctor<1024, 100> _mem1024;
    MemoryAlloc* _szAlloc[MAX_MEMORY_SIZE + 1];

    void init_szAlloc(int nBegin, int nEnd, MemoryAlloc* pMemA){
        for(int n= nBegin; n <= nEnd; ++n){
            _szAlloc[n] = pMemA;
        }
    }
    MemoryMgr(){
        init_szAlloc(0, 64, &_mem64);
        init_szAlloc(65, 128, &_mem128);
        init_szAlloc(129, 256, &_mem256);
        init_szAlloc(257, 512, &_mem512);
        init_szAlloc(513, 1024, &_mem1024);
    }
public:
    int sum = 0;
    static MemoryMgr& Instance(){
        static MemoryMgr mgr;
        return mgr;
    }
    MemoryMgr(const MemoryMgr&) = delete;
    MemoryMgr & operator=(const MemoryMgr&) = delete;

    void * allocMem(size_t nSize){
        printf("nsize : %d   sum:%d\n",nSize,sum++);
        return (char*) malloc(nSize);
        if(nSize < MAX_MEMORY_SIZE)
            return _szAlloc[nSize]->allocMemory(nSize);
        else{
            MemoryBlock* pReturn = (MemoryBlock*)malloc(nSize + sizeof(MemoryBlock));
            pReturn->bPool = false;
            pReturn->nID = -1;
            pReturn->nRef = 1;
            pReturn->pAlloc = nullptr;
            pReturn->pNext = nullptr;
            printf("allocMem: %llx, id=%d, size=%d\n", pReturn , pReturn->nID, nSize);
            return ((char*)pReturn + sizeof(MemoryBlock));

        }
    }
    void addRef(void *pMem){
        MemoryBlock * block = (MemoryBlock *)((char*)pMem - sizeof(MemoryBlock));
        ++block->nRef;
    }
    void freeMem(void* pMem){
        MemoryBlock * block = (MemoryBlock *)((char*)pMem - sizeof(MemoryBlock));
        if(block->bPool) block->pAlloc->freeMemory(pMem);
        else{
            if(--block->nRef == 0) free(block);
        }
    }

};

#endif