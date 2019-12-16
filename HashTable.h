#ifndef __HASHTABLE__
#define __HASHTABLE__

#include "HashUtils.h"

// Do not modify the public interface of this class.
// Otherwise, your code will note compile!
template <class T>
class HashTable
{
    struct Entry
    {
        std::string Key; // the key of the entry
        T Value;         // the value of the entry
        bool Deleted;    // flag indicating whether this entry is deleted
        bool Active;     // flag indicating whether this item is currently used

        Entry() : Key(), Value(), Deleted(false), Active(false) {}
    };

    struct Bucket
    {
        Entry entries[3];
    };

    int _capacity; // INDICATES THE SIZE OF THE TABLE
    int _size;     // INDICATES THE NUMBER OF ITEMS IN THE TABLE

    Bucket *_table; // HASH TABLE

    // You can define private methods and variables

public:
    // TODO: IMPLEMENT THESE FUNCTIONS.
    // CONSTRUCTORS, ASSIGNMENT OPERATOR, AND THE DESTRUCTOR
    HashTable();
    HashTable(const HashTable<T> &rhs);
    HashTable<T> &operator=(const HashTable<T> &rhs);
    ~HashTable();

    // TODO: IMPLEMENT THIS FUNCTION.
    // INSERT THE ENTRY IN THE HASH TABLE WITH THE GIVEN KEY & VALUE
    // IF THE GIVEN KEY ALREADY EXISTS, THE NEW VALUE OVERWRITES
    // THE ALREADY EXISTING ONE.
    // IF LOAD FACTOR OF THE TABLE IS BIGGER THAN 0.5,
    // RESIZE THE TABLE WITH THE NEXT PRIME NUMBER.
    void Insert(std::string key, const T &value);

    // TODO: IMPLEMENT THIS FUNCTION.
    // DELETE THE ENTRY WITH THE GIVEN KEY FROM THE TABLE
    // IF THE GIVEN KEY DOES NOT EXIST IN THE TABLE, JUST RETURN FROM THE FUNCTION
    // HINT: YOU SHOULD UPDATE ACTIVE & DELETED FIELDS OF THE DELETED ENTRY.
    void Delete(std::string key);

    // TODO: IMPLEMENT THIS FUNCTION.
    // IT SHOULD RETURN THE VALUE THAT CORRESPONDS TO THE GIVEN KEY.
    // IF THE KEY DOES NOT EXIST, THIS FUNCTION MUST RETURN T()
    T Get(std::string key) const;

    // TODO: IMPLEMENT THIS FUNCTION.
    // AFTER THIS FUNCTION IS EXECUTED THE TABLE CAPACITY MUST BE
    // EQUAL TO newCapacity AND ALL THE EXISTING ITEMS MUST BE REHASHED
    // ACCORDING TO THIS NEW CAPACITY.
    // WHEN CHANGING THE SIZE, YOU MUST REHASH ALL OF THE ENTRIES FROM 0TH ENTRY TO LAST ENTRY
    void Resize(int newCapacity);

    // TODO: IMPLEMENT THIS FUNCTION.
    // RETURNS THE AVERAGE NUMBER OF PROBES FOR SUCCESSFUL SEARCH
    double getAvgSuccessfulProbe();

    // TODO: IMPLEMENT THIS FUNCTION.
    // RETURNS THE AVERAGE NUMBER OF PROBES FOR UNSUCCESSFUL SEARCH
    double getAvgUnsuccessfulProbe();

    // THE IMPLEMENTATION OF THESE FUNCTIONS ARE GIVEN TO YOU
    // DO NOT MODIFY!
    int Capacity() const;
    int Size() const;
};

template <class T>
HashTable<T>::HashTable()
{
    // TODO: CONSTRUCTOR
    _capacity = NextCapacity(0);
    std::cout << "capacity is initialized and its value is " << _capacity << std::endl;

    _size = 0;

    _table = new Bucket[_capacity];

    for (int i = 0; i < _capacity; i++)
    {
        // Initialize each key with a Bucket
        _table[i] = Bucket();

        // Initialize each entry of a Bucket
        for (int j = 0; j < 3; j++)
        {
            _table[i].entries[j] = Entry();
        }
    }
}

template <class T>
HashTable<T>::HashTable(const HashTable<T> &rhs)
{
    // TODO: COPY CONSTRUCTOR
    this->_capacity = rhs->_capacity;
    this->_size = rhs->_size;

    for (int i = 0; i < _capacity; i++)
    {
        // Initialize each key with a Bucket
        _table[i] = Bucket();

        // Initialize each entry of a Bucket
        for (int j = 0; j < 3; j++)
        {
            _table[i].entries[j].key = Entry(rhs->_table[i].entries[j].key);
            _table[i].entries[j].Value = Entry(rhs->_table[i].entries[j].Value);
            _table[i].entries[j].Deleted = Entry(rhs->_table[i].entries[j].Deleted);
            _table[i].entries[j].Active = Entry(rhs->_table[i].entries[j].Active);
        }
    }
}

template <class T>
HashTable<T> &HashTable<T>::operator=(const HashTable<T> &rhs)
{
    // TODO: OPERATOR=
    HashTable<T> temp(rhs);
    std::swap(temp._table, _table);
    return *this;
}

template <class T>
HashTable<T>::~HashTable()
{
    // TODO: DESTRUCTOR
    delete[] _table;
    _table = NULL;
}

template <class T>
void HashTable<T>::Insert(std::string key, const T &value)
{
    // TODO: IMPLEMENT THIS FUNCTION.
    // INSERT THE ENTRY IN THE HASH TABLE WITH THE GIVEN KEY & VALUE
    // IF THE GIVEN KEY ALREADY EXISTS, THE NEW VALUE OVERWRITES
    // THE ALREADY EXISTING ONE. IF LOAD FACTOR OF THE TABLE IS BIGGER THAN 0.5,
    // RESIZE THE TABLE WITH THE NEXT PRIME NUMBER.

    // std::cout << "Insert with the key "<< key << std::endl;

    double load_factor = (double)_size / (3 * _capacity);
    std::cout << "load factory: " << load_factor << std::endl;

    if (load_factor > 0.5)
    {
        std::cout << "----------------------resizing" << std::endl;
        Resize(NextCapacity(_capacity));
        std::cout << "resized ;" << std::endl;
    }
    int index = Hash(key) % _capacity;
    // std::cout << "index: "<< index << std::endl;
    bool update = false;
    bool bucket_full = true;

    for (int i = 0; i < 3; i++)
    {
        if (_table[index].entries[i].Key == key)
        {
            _table[index].entries[i].Value = value;
            _table[index].entries[i].Active = true;
            update = true;
            bucket_full = false;
            _size++;
            std::cout << "existed one updated" << std::endl;
        }
    }

    if (update != true && !bucket_full)
    {
        for (int i = 0; i < 3; i++)
        {
            if (_table[index].entries[i].Active == false)
            {
                std::cout << "normal inserted" << std::endl;
                _table[index].entries[i].Value = value;
                _table[index].entries[i].Key = key;
                _table[index].entries[i].Active = true;

                bucket_full = false;
                _size++;
                break;
            }
        }
    }

    if (bucket_full)
    {
        // _size++;
        // double load_factor = (double)_size/(3*_capacity);
        // std::cout << "--------------------load factory: " << load_factor << "----------------bucket is full--------------------" << std::endl;

        /* quadratic probing */
        int h = 1;

        while (bucket_full)
        {
            int index = (Hash(key) + h * h) % _capacity;
            std::cout << "quadratic probing: " << h << std::endl;
            for (int k = 0; k < 3; k++)
            {
                if (_table[index].entries[k].Active == false)
                {
                    _table[index].entries[k].Value = value;
                    _table[index].entries[k].Key = key;
                    _table[index].entries[k].Active = true;
                    bucket_full = false;
                    _size++;
                    break;
                }
            }
            h++;
        }
    }
}

template <class T>
void HashTable<T>::Delete(std::string key)
{
    // TODO: IMPLEMENT THIS FUNCTION.
    // DELETE THE ENTRY WITH THE GIVEN KEY FROM THE TABLE
    // IF THE GIVEN KEY DOES NOT EXIST IN THE TABLE, JUST RETURN FROM THE FUNCTION
    // HINT: YOU SHOULD UPDATE ACTIVE & DELETED FIELDS OF THE DELETED ENTRY.

    int h = 0;
    bool removed = false;
    std::cout << "Delete function is called" << std::endl;

    while (!removed && (h * h) < _capacity)
    {
        int index = (Hash(key) + h * h) % _capacity;
        std::cout << "trying to remove with key " << index << std::endl;

        for (int k = 0; k < 3; k++)
        {
            std::cout << _table[index].entries[k].Key << " ? " << key << std::endl;
            if (_table[index].entries[k].Key == key)
            {
                std::cout << key << " is in hash table" << std::endl;
                _table[index].entries[k] = Entry();
                _table[index].entries[k].Active = false;
                _table[index].entries[k].Deleted = true;
                removed = true;
                _size--;
                break;
            }
        }
        h++;
    }

    if (removed)
    {
        std::cout << key << " is removed" << std::endl;
    }
    else
    {
        std::cout << key << " is not removed" << std::endl;
    }
}

template <class T>
T HashTable<T>::Get(std::string key) const
{
    // TODO: IMPLEMENT THIS FUNCTION. IT SHOULD RETURN THE VALUE THAT
    // IT SHOULD RETURN THE VALUE THAT CORRESPONDS TO THE GIVEN KEY.
    // IF THE KEY DOES NOT EXIST, THIS FUNCTION MUST RETURN T()
}

template <class T>
void HashTable<T>::Resize(int newCapacity)
{
    // TODO: IMPLEMENT THIS FUNCTION. AFTER THIS FUNCTION IS EXECUTED
    // THE TABLE CAPACITY MUST BE EQUAL TO newCapacity AND ALL THE
    // EXISTING ITEMS MUST BE REHASHED ACCORDING TO THIS NEW CAPACITY.
    // WHEN CHANGING THE SIZE, YOU MUST REHASH ALL OF THE ENTRIES FROM 0TH ENTRY TO LAST ENTRY

    int oldCapacity = _capacity;
    _capacity = newCapacity;
    Bucket *newTable = new Bucket[_capacity];

    for (int i = 0; i < _capacity; i++)
    {
        // Initialize each key with a Bucket
        newTable[i] = Bucket();

        // Initialize each entry of a Bucket
        for (int j = 0; j < 3; j++)
        {
            newTable[i].entries[j] = Entry();
        }
    }

    bool inserted = false;
    for (int i = 0; i < oldCapacity; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (_table[i].entries[j].Active == true)
            {
                int index = Hash(_table[i].entries[j].Key) % _capacity;
                for (int k = 0; k < 3; k++)
                {
                    if (newTable[index].entries[k].Active == false)
                    {
                        newTable[index].entries[k] = _table[i].entries[j];
                        inserted = true;
                        break;
                    }
                }

                if (inserted == false)
                {
                    /* quadratic probing */
                    int h = 1;

                    while (!inserted)
                    {
                        int index = (Hash(_table[i].entries[j].Key) + h * h) % _capacity;

                        for (int k = 0; k < 3; k++)
                        {
                            if (newTable[index].entries[k].Active == false)
                            {
                                newTable[index].entries[k] = _table[i].entries[j];
                                inserted = true;
                                break;
                            }
                        }
                        h++;
                    }
                }
            }
        }
    }

    delete[] _table;

    this->_table = newTable;
    std::cout << "new table size: " << std::endl;
}

template <class T>
double HashTable<T>::getAvgSuccessfulProbe()
{
    // TODO: IMPLEMENT THIS FUNCTION.
    // RETURNS THE AVERAGE NUMBER OF PROBES FOR SUCCESSFUL SEARCH
}

template <class T>
double HashTable<T>::getAvgUnsuccessfulProbe()
{
    // TODO: IMPLEMENT THIS FUNCTION.
    // RETURNS THE AVERAGE NUMBER OF PROBES FOR UNSUCCESSFUL SEARCH
}

template <class T>
int HashTable<T>::Capacity() const
{
    return _capacity;
}

template <class T>
int HashTable<T>::Size() const
{
    return _size;
}

#endif