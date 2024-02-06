/**
 * README:
 * - Program to demonstrate the working of the hash function in the hash table.
 *
 * COMPILE:
 * ~/cpp/misc$ g++ 1_hash_table.cpp
 *
 * RUN:
 * ~/cpp/misc$ ./a.out
 *  The hash table after insertion of elements:
 *  Values at index 0: 42
 *  Values at index 1: 29
 *  Values at index 2: 44
 *  Values at index 3: 52 66
 *  Values at index 4: 25 32
 *  Values at index 5:
 *  Values at index 6:
 * ~/cpp/misc$
 * */

#include <iostream>
#include <list>

using namespace std;

// Creating class hash table.
class hash_table {
    // Initializing a list for every index.
    list<int> *tbl;
    int ht_size;

    // Hash function to get the index of the keys.
    int get_hash(int key) {
        return key % ht_size;
    }
    
public:
    // Constructor for the class hash_table.
    hash_table(int n) { 
        ht_size = n;
        tbl = new list<int>[ht_size];
    }

    // Function to insert keys in the hash table.
    void insert_key(int key) {
        tbl[get_hash(key)].push_back(key);
    }

    // Function to display all the keys at their indexes.
    void show() {
        for(int i = 0; i < ht_size; i++) {
            cout << "Values at index " << i << ": ";
            for(int num : tbl[i])
                cout << num << " ";
            cout << endl;
        }
    }
};

int main() {
    // Creating an array of keys.
    int arr[] = {52, 42, 44, 66, 25, 29, 32};
    
    // Find the size of the array.
    int sz_arr = sizeof(arr)/sizeof(int);
    
    // Initializing the hash table.
    hash_table ht(sz_arr);
    
    // Inserting the elements into the hash table.
    for(int i = 0; i < sz_arr; i++) {
        ht.insert_key(arr[i]);
    }
    cout << "The hash table after insertion of elements: " << endl;
    // Calling function to display the hash table.
    ht.show();
    
    return 0;
}

