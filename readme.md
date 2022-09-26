# PackedVector #

## Description ##

    PackedVector is a kind of vectors,
    but for storing elements uses defined
    number of bits

## Building ##

    mkdir build
    cd build
    cmake ..
    cmake --build .
    ./bin/libcsc_test

## How to use ##

### Initialisation ###

    libcsc::PackedVector<unsigned short, 5> vec1;

    /*
     *    Default constructor
     *
     *    Memory not allocated, vector is empty ( vec.empty() = true)
     *
     *    But each new element will be 5 bits of size
     */

    libcsc::PackedVector<unsigned short, 5> vec2 (5, 17);

    /* In this case vector will be initialised with 5 elements equaled to 17 */

### Access elements ###

    /*
     * To get access to existing elements you can use overrided operator[i] or method at(i)
     * In both cases it's not possible to change element's value like that:
     *     vec[1] = 4
     *     vec.at(1) = 4;
     * To change element's value you should use method assign(index, value)
     */

    vec2.assign(2, 4);

    std::cout << vec2[0] << " " << vec2.at(1) << vec2[2] << "\n";


    /* Output: 17 17 4 */

    /*
     * To put new element in the vector you can use methods push_back(value) & insert(i, value) like in std::vecto
     * To remove: method pop_back(), that also returns value of the removed element
     *
     */

    vec2.push_back(4);
    vec2.push_back(7);
    vec2.pop_back(); /* returns 7 */

     /*
      * Also you can use methods resize & reserve
      * and change values by using method assign
      *
      */

    vec1.resize(3); // now you have 3 elements equalled to 0
    vec1.assign(2, 4);

    vec1.reserve(6); // size is 3, but capacity is 6

    vec1.shrink_to_fit(); // to set capacity to value of size

### Other methods you may want to use: ###

    size() - to get number of used elements
    capacity() - to get maximum number of elements before reallocation
    bits() - to get number of bits requered to store 1 element
    used_bytes() - to get number of bytes vector's using to store it's used elements
    reserved_bytes() - to get number of bytes to store maximum number of elements before reallocation
    clear() - set all element to 0
    erase() - free all vector's memory
