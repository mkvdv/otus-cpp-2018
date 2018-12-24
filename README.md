# Task03 - Allocator

[![Build Status](https://travis-ci.com/mkvdv/otus-cpp-2018.svg?branch=task03)](https://travis-ci.com/mkvdv/otus-cpp-2018)
[ ![Download](https://api.bintray.com/packages/mkvdv/otus-cpp-2018/ha/images/download.svg?version=3.0.155) ](https://bintray.com/mkvdv/otus-cpp-2018/ha/3.0.155/link)


### How allocator designed
* Allocator use list of caches (contiguous memory blocks), and on every block there is and intrusive list of free blocks. More preciesly, it looks like very simplified version of [Slab allocator](https://en.wikipedia.org/wiki/Slab_allocation) with fixed block size.

### Notes
* All build files must be done in `build` directory (created by `.travis.yml`), all binaries are in `build/bin`
