#pragma once
#include "base/Signal.h"
#include <vector>

namespace coral
{
    /**
     * A PropertyArray embedded array of values
     */
    template <typename Type>
    class PropertyArray
    {
    public:
        // Iterator for the property array
        struct Iterator
        {
            Iterator(int index, PropertyArray<Type>& parent) : parent(parent), index(index) {}
            const Type& operator*() const { return parent[index]; }
            const Type* operator->() const { return parent[index]; }
            Type& operator*() { return parent[index]; }
            Type* operator->() { return parent[index]; }
            Iterator& operator++() { index++; return *this; }
            Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
            friend bool operator== (const Iterator& a, const Iterator& b) { return a.index == b.index; };
            friend bool operator!= (const Iterator& a, const Iterator& b) { return a.index != b.index; };
        private:
            PropertyArray<Type>& parent;
            int index;
        };

        // Construction
        PropertyArray() = default;
        PropertyArray(const std::vector<Type>& items);

        // Access
        int size() const;
        void insert(const Type& item, int index = -1);
        void remove(const Type& item);
        void remove(int index);
        void clear();

        // Search
        int find(const Type& item) const;
        bool contains(const Type& item) const { return find(item) != -1; }

        // Item access
        const Type&operator[](int index) const { return items[index]; }
        Type operator[](int index) { return items[index]; }

        // Iterate
        const Iterator begin() const { return ConstIterator(*this, 0); }
        Iterator begin() { return Iterator(*this, 0); }
        const Iterator end() const { return ConstIterator(*this, size()); }
        Iterator end() { return Iterator(*this, size()); }

        // Modified signal
        Signal<const Type&, int> itemAdded;
        Signal<const Type&, int> itemRemoved;

    private:
        std::vector<Type> items;
    };

    template <typename Type>
    PropertyArray<Type>::PropertyArray(const std::vector<Type>& items) : items(items)
    {
    }

    template <typename Type>
    int PropertyArray<Type>::size() const
    {
        return static_cast<int>(items.size());
    }

    template <typename Type>
    void PropertyArray<Type>::insert(const Type& item, int index)
    {
        if (index >= 0 && index < size())
        {
            items.insert(items.begin() + index, item);
            itemAdded(item, index);
        }
        else
        {
            items.push_back(item);
            itemAdded(item, size());
        }
    }

    template <typename Type>
    void PropertyArray<Type>::remove(const Type& item)
    {
        auto it = std::find(begin(), end(), item);
        if (it != end())
        {
            int index = static_cast<int>(std::distance(begin(), it));
            items.erase(it);
            itemRemoved(item, index);
        }
    }

    template <typename Type>
    void PropertyArray<Type>::remove(int index)
    {
        if (index >= 0 && index < size())
        {
            Type item = items[index];
            items.erase(items.begin() + index);
            itemRemoved(item, index);
        }
    }

    template <typename Type>
    void PropertyArray<Type>::clear()
    {
        while(size() > 0)
        {
            remove(size() - 1);
        }
    }

    template <typename Type>
    int PropertyArray<Type>::find(const Type& item) const
    {
        auto it = std::find(begin(), end(), item);
        if (it != end())
        {
            return static_cast<int>(std::distance(begin(), it));
        }
        return -1;
    }
}
