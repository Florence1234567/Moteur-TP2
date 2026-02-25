#pragma once
#include <stack>
#include <cassert>

struct SMemoryPage
{
    void* pData = nullptr;
};

template<int NBytes, int NObjects>
class CPageAllocator
{
    public:
    CPageAllocator()
    {
        for (int i = 0; i < NObjects; i++)
        {
            SMemoryPage page;
            //Malloc alloue un bloc de mémoire sur la heap eet retourne un pointer void* vrs celui-ci.
            page.pData = malloc(NBytes);
            m_pool.push(page);
        }
    }

    ~CPageAllocator()
    {
        while (m_pool.size() > 0)
        {
            //Free libère le bloc de mémoire alloué par Malloc, donc l'inverse de Malloc.
            free(m_pool.top().pData);
            m_pool.pop();
        }
    }

    //Construction d'un object dans le bloc de mémoire alloué par Malloc.
    template<typename T, typename... Args>
    T* NewObject(Args&&... args)
    {
        if (sizeof(T) > NBytes)
            return nullptr;

        SMemoryPage page = m_pool.top();
        m_pool.pop();
        m_used++;

        //Construction de T dans la mémoire de la page.
        return new (page.pData) T(std::forward<Args>(args)...);
    }

    //Détruit l'objet et le remet dans la pile.
    template<typename T>
    void FreeObject(T* Object)
    {
        Object->~T();

        SMemoryPage page;
        page.pData = Object;
        m_pool.push(page);
        m_used--;
    }

    int GetAvailable() { return (int)m_pool.size() - m_used; }
    int GetUsed() { return m_used; }

private:
    std::stack<SMemoryPage> m_pool;
    int m_used = 0;
};
