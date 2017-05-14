/*
 * ruby_gdiplus.h
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
#ifndef SIMPLEMAP_H
#define SIMPLEMAP_H

#include "ruby_gdiplus.h"
#include <cstring>

using namespace std;

class MapBase {
public:
    virtual ~MapBase() {}
};

template<typename TKey, typename TVal>
class IMap : public MapBase {
public:
    virtual bool append(TKey key, TVal val) = 0;
    virtual bool set(TKey key, TVal val) = 0;
    virtual bool get(TKey key, TVal& val) = 0;
    virtual bool each(bool(*func)(TKey, TVal)) = 0;
    bool get(TKey key, TVal& val, TVal default_val) {
        if (this->get(key, val)) {
            return true;
        }
        else {
            val = default_val;
            return false;
        }
    }
};

template<typename TKey, typename TVal>
class ArrayMap : public IMap<TKey, TVal> {
protected:
    int Capa;
    int Len;
    TKey *KeyTable;
    TVal *ValTable;
    virtual int compare(TKey key1, TKey key2) {
        if (key1 == key2) return 0;
        else return key1 > key2 ? 1 : -1;
    }
public:
    ArrayMap(int capa) {
        dp("ArrayMap(%d)", capa);
        Capa = capa;
        Len = 0;
        KeyTable = static_cast<TKey *>(ruby_xcalloc(capa, sizeof(TKey)));
        ValTable = static_cast<TVal *>(ruby_xcalloc(capa, sizeof(TVal)));
    }
    virtual ~ArrayMap() {
        dp("~ArrayMap() Capa=%d, Len=%d", Capa, Len);
        ruby_xfree(KeyTable);
        ruby_xfree(ValTable);
    }
    virtual bool append(TKey key, TVal val) {
        if (Len < Capa) {
            KeyTable[Len] = key;
            ValTable[Len] = val;
            Len += 1;
            return true;
        }
        else {
            rb_warning("ArrayMap: capacity over");
            return false;
        }
    }
    virtual bool set(TKey key, TVal val) {
        for (int i = 0; i < Len; ++i) {
            if (this->compare(key, KeyTable[i])) {
                ValTable[i] = val;
                return true;
            }
        }
        return append(key, val);
    }
    virtual bool get(TKey key, TVal& val) {
        for (int i = 0; i < Len; ++i) {
            if (key == KeyTable[i]) {
                val = ValTable[i];
                return true;
            }
        }
        return false;
    }
    virtual bool each(bool(*func)(TKey, TVal)) {
        for (int i = 0; i < Len; ++i) {
            if (func(KeyTable[i], ValTable[i]) == false) {
                return false;
            }
        }
        return true;
    }
};

template<typename TVal>
class IndexArrayMap : public ArrayMap<int, TVal> {
    int InitKey;
public:
    IndexArrayMap(int capa, int init_key=-1) : ArrayMap<int, TVal>(capa) {
        this->Len = capa;
        InitKey = init_key;
        for (int i = 0; i < capa; ++i) {
            this->KeyTable[i] = InitKey;
        }
    }
    virtual ~IndexArrayMap() {}
    virtual bool append(int key, TVal val) {
        if (key < this->Capa) {
            this->KeyTable[key] = key;
            this->ValTable[key] = val;
            return true;
        }
        else {
            rb_warning("IndexArrayMap: index over");
            return false;
        }
    }
    virtual bool set(int key, TVal val) {
        if (key < this->Capa) {
            this->KeyTable[key] = key;
            this->ValTable[key] = val;
            return true;
        }
        else {
            return false;
        }
    }
    virtual bool get(int key, TVal& val) {
        if (this->KeyTable[key] != InitKey) {
            val = this->ValTable[key];
            return true;
        }
        else {
            return false;
        }
    }
    virtual bool each(bool(*func)(int, TVal)) {
        for (int i = 0; i < this->Capa; ++i) {
            if (this->KeyTable[i] != InitKey) {
                if (func(this->KeyTable[i], this->ValTable[i]) == false) {
                    return false;
                }
            }
        }
        return true;
    }
};

template<typename TKey, typename TVal>
class SortedArrayMap : public ArrayMap<TKey, TVal> {
public:
    SortedArrayMap(int capa) : ArrayMap<TKey, TVal>(capa) {}
    virtual ~SortedArrayMap() {}
    virtual bool append(TKey key, TVal val) {
        if (this->Len < this->Capa) {
            this->KeyTable[this->Len] = key;
            this->ValTable[this->Len] = val;
            int idx = this->Len - 1;
            while (idx >= 0) {
                if (this->compare(this->KeyTable[idx], this->KeyTable[idx + 1]) > 0) {
                    TKey tmp_key = this->KeyTable[idx];
                    TVal tmp_val = this->ValTable[idx];
                    this->KeyTable[idx] = this->KeyTable[idx + 1];
                    this->ValTable[idx] = this->ValTable[idx + 1];
                    this->KeyTable[idx + 1] = tmp_key;
                    this->ValTable[idx + 1] = tmp_val;
                }
                else {
                    break;
                }
                idx -= 1;
            }
            this->Len += 1;
            return true;
        }
        else {
            rb_warning("SortedArrayMap: capacity over");
            return false;
        }
    }

    virtual bool set(TKey key, TVal val) {
        for (int i = 0; i < this->Len; ++i) {
            if (this->compare(key, this->KeyTable[i]) == 0) {
                this->ValTable[i] = val;
                return true;
            }
        }
        return append(key, val);
    }
    
    virtual bool get(TKey key, TVal& val) {
        if (this->Len == 0) return false;
        int left = 0;
        int right = this->Len - 1;
        while (left < right) {
            int middle = (left + right) / 2;
            if (this->compare(this->KeyTable[middle], key) < 0) left = middle + 1;
            else if (this->compare(this->KeyTable[middle], key) > 0) right = middle - 1;
            else {
                val = this->ValTable[middle];
                return true;
            }
        }
        if (this->compare(this->KeyTable[left], key) == 0) {
            val = this->ValTable[left];
            return true;
        }
        return false;
    }
};

template<typename TKey, typename TVal>
class MemPtrSortedArrayMap : public SortedArrayMap<TKey, TVal> {
protected:
    virtual int compare(TKey key1, TKey key2) {
        return memcmp(key1, key2, sizeof(*key1));
    }
public:
    MemPtrSortedArrayMap(int capa) : SortedArrayMap<TKey, TVal>(capa) {}
    virtual ~MemPtrSortedArrayMap() {}
};

#endif /* SIMPLEMAP_H */