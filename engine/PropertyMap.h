#pragma once
#include <map>
#include "signals/Signal.h"

namespace coral
{
    /**
     * A PropertyMap embed a map of values
     */
    template <typename Key, typename Value, typename Map = std::map<Key, Value>>
    class PropertyMap
    {
    public:
        // Construction
        PropertyMap() = default;
        PropertyMap(const Map& items);

        // Access
        int size() const;
        void insert(const Key& key, const Value& value);
        void remove(const Key& key);
        void clear();

        // Search
        auto find(const Key& key) const { return items.find(key); }
        bool contains(const Key& key) const { return find(key) != items.end(); }

        // Item access
        const Value& operator[](int index) const { return items[index]; }
        Value& operator[](int index) { return items[index]; }

        // Iterate
        const auto begin() const { return items.begin(); }
        auto begin() { return items.begin(); }
        const auto end() const { return items.end(); }
        auto end() { return items.end(); }

        // Modified signal
        Signal<const Key&, const Value&> itemAdded;
        Signal<const Key&, const Value&> itemRemoved;

    private:
        Map items;
    };

    template <typename Key, typename Value, typename Map>
    PropertyMap<Key, Value, Map>::PropertyMap(const Map& items) : items(items)
    {
    }

    template <typename Key, typename Value, typename Map>
    int PropertyMap<Key, Value, Map>::size() const
    {
        return static_cast<int>(items.size());
    }

    template <typename Key, typename Value, typename Map>
    void PropertyMap<Key, Value, Map>::insert(const Key& key, const Value& value)
    {
        items.insert(key, value);
        itemAdded(key, value);
    }

    template <typename Key, typename Value, typename Map>
    void PropertyMap<Key, Value, Map>::remove(const Key& key)
    {
        auto it = find(key);
        if (it != end())
        {
            auto value = it->second;
            items.erase(it);
            itemRemoved(key, value);
        }
    }

    template <typename Key, typename Value, typename Map>
    void PropertyMap<Key, Value, Map>::clear()
    {
        while (size() > 0)
        {
            remove(begin()->first);
        }
    }
}
