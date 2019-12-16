#ifndef _HTABLE
#define _HTABLE
#include <iostream>
#include <vector>
#include <string>
#include <math.h>

#define REMOVED "XXX"
using namespace std;

class HashStringTable{
public:
    // constructor that initializes the elements as a vector of size 11
    // with "" values.It also initializes oher private data members
    HashStringTable();

    // Adds string value to elements. It first checks the load factor.
    // If the load factor of elements is >=0.75 then its size is doubled and
    // all data are rehashed. During insertion duplicate values are ignored
    // (i.e. they are not added to the hash table)
    void add(string value) ;

    // returns the size of the hash table (i.e. vector elements)
    int get_size();

    //returns the number of data values in the hash table
    int get_count();

    //returns the average number of probes for successful search
    double get_avgProbe();

    // returns the average number of probes for unsuccessful search
    double get_unsuccessProbe();

    // returns true if the string value is in the hash table; false otherwise
    bool contains(string value);

    // returns true if value is removed successfully from the hash table; false
    // otherwise
    bool remove(string value);

private:
    vector<string> elements; // the hash table implemented as a vector
    int cnt;	//current number of items in the table

    int total_probes;   //total number of probes that helps calculating the
    //average number of probes for successful search.


    // Hash function that finds the hash code corresponding to string str.
    // It should map the given string to an integer value between 0 and
    // hash table size -1.
    // Make sure that your hash function uses all characters of the string in
    // the computation.
    int hashcode(string str);

    // resizes the hash table by doubling its size. The new size will be
    //(oldsize*2)+1
    void rehash() ;

}; //end of class HashStringTable

HashStringTable::HashStringTable() {
    elements.resize(11,"");
    cnt = 0;
    total_probes = 0;
}

void HashStringTable::add(string value) {
    if ((double) cnt / elements.size() >= 0.75)
        rehash();

    int h = hashcode(value);
    while (elements[h] != "" && elements[h] != value &&
           elements[h] != REMOVED) {           // linear probing
        h = (h + 1) % elements.size();  // for empty slot
        total_probes++;
    }
    if (elements[h] == value)
        cout << "Duplicate: " << value << endl;
    if (elements[h] != value) {         // avoid duplicates
        elements[h] = value;
        cnt++;
        total_probes++;
    }
}

int HashStringTable::get_size() {
    return elements.size();
}

int HashStringTable::get_count() {
    return cnt;
}

double HashStringTable::get_avgProbe() {
    return (cnt == 0)?1:(double)total_probes/(double)cnt;
}

double HashStringTable::get_unsuccessProbe() {
    int total = 0;
    for (unsigned i = 0; i< elements.size(); i++){
        int pr = 1;
        int h = i;
        while(elements[h] != "") {
            pr ++;
            h = (h+1)% elements.size();
        }
        total += pr;
    }
    return (double)total/elements.size();
}

bool HashStringTable::contains(string value) {
    int h = hashcode(value);
    while (elements[h] != "") {
        if (elements[h] == value) {     // linear probing
            return true;                // to search
        }
        h = (h + 1) % elements.size();
    }
    return false;                       // not found
}

bool HashStringTable::remove(string value) {
    bool flag = false;
    int h = hashcode(value);
    while (elements[h] != "" && elements[h] != value) {
        h = (h + 1) % elements.size();
    }
    if (elements[h] == value) {
        elements[h] = REMOVED;   // "removed" flag value
        cnt--;
        flag = true;
    }
    return flag;
}

int HashStringTable::hashcode(string str) {
    int h = 0;
    for (unsigned i = 0; i < str.length(); i++) {
        h = 31 * h + str[i];
    }
    h %= elements.size();
    if (h < 0)   /* in case overflows occurs */
        h += elements.size();

    return h;
}

void HashStringTable::rehash() {
    vector<string> old (elements);

    elements.resize(2 * old.size()+1);
    for (unsigned i =0 ; i < elements.size() ; i++)
        elements[i] = "";
    cnt = 0;
	total_probes = 0;

    for (unsigned i=0; i < old.size(); i++) {
        if (old[i] != "" && old[i] != REMOVED) {
            add(old[i]);
        }
    }
}


#endif // _HTABLE

