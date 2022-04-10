#pragma once
#include <vector>
#include <algorithm>
#include "Signal.h"

namespace coral
{
    /**
     * A PropertyArray embedded array of values
     */
    template <typename Type, typename Array = std::vector<Type>>
    class PropertyArray
    {
    public:
        // Construction
        PropertyArray() = default;
        PropertyArray(const Array& items);

        // Access
        int size() const;
        void insert(const Type& item, int index = -1);
        void remove(const Type& item);
        void remove(int index);
        void clear();

        // Search
        auto find(const Type& item) { return std::find(begin(), end(), item); }
        bool contains(const Type& item) const { return find(item) != end(); }

        // Item access
        const Type& operator[](int index) const { return items[index]; }
        Type& operator[](int index) { return items[index]; }

        // Iterate
        auto begin() const { return items.begin(); }
        auto begin() { return items.begin(); }
        const auto end() const { return items.end(); }
        auto end() { return items.end(); }

        // Modified signal
        Signal<const Type&, int> itemAdded;
        Signal<const Type&, int> itemRemoved;

    private:
        Array items;
    };

    template <typename Type, typename Array>
    PropertyArray<Type, Array>::PropertyArray(const Array& items) : items(items)
    {
    }

    template <typename Type, typename Array>
    int PropertyArray<Type, Array>::size() const
    {
        return static_cast<int>(items.size());
    }

    template <typename Type, typename Array>
    void PropertyArray<Type, Array>::insert(const Type& item, int index)
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

    template <typename Type, typename Array>
    void PropertyArray<Type, Array>::remove(const Type& item)
    {
        auto it = find(item);
        if (it != end())
        {
            int index = static_cast<int>(std::distance(begin(), it));
            items.erase(it);
            itemRemoved(item, index);
        }
    }

    template <typename Type, typename Array>
    void PropertyArray<Type, Array>::remove(int index)
    {
        if (index >= 0 && index < size())
        {
            Type item = items[index];
            items.erase(items.begin() + index);
            itemRemoved(item, index);
        }
    }

    template <typename Type, typename Array>
    void PropertyArray<Type, Array>::clear()
    {
        while(size() > 0)
        {
            remove(size() - 1);
        }
    }
}
