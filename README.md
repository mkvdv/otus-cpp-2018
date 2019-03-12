# otus-cpp-2018
[![Build Status](https://travis-ci.com/mkvdv/otus-cpp-2018.svg?branch=master)](https://travis-ci.com/mkvdv/otus-cpp-2018)
[ ![Download](https://api.bintray.com/packages/mkvdv/otus-cpp-2018/ha/images/download.svg) ](https://bintray.com/mkvdv/otus-cpp-2018/ha/_latestVersion)

My solutions for [C++ Course at otus.ru](https://otus.ru/learning/9745)  
Look at branches for more details.


### Contents

* [Allocator](https://github.com/mkvdv/otus-cpp-2018/tree/task03)
-- simplified version of [Slab allocator](https://en.wikipedia.org/wiki/Slab_allocation) with fixed block size.

* [Bayan](https://github.com/mkvdv/otus-cpp-2018/tree/task08_2) - utility for detecting duplicate files, use 
Boost::Filesystem

* [Bulk Server](https://github.com/mkvdv/otus-cpp-2018/tree/task12) - Async Server, that execute tasks in parallel. 
Use Boost::ASIO for network.

* [YAMR](https://github.com/mkvdv/otus-cpp-2018/tree/task14) - tool for launching tasks as MapReduce programming model

* [k-means](https://github.com/mkvdv/otus-cpp-2018/tree/task15) - example of usage DLib library for clustering

* [Realty](https://github.com/mkvdv/otus-cpp-2018/tree/task16) - tool for realty clusterization and classification

* [Nickname](https://github.com/mkvdv/otus-cpp-2018/tree/task07_2) - Radix tree implementation

* [Editor](https://github.com/mkvdv/otus-cpp-2018/tree/task05) - example of MVC pattern, graphical editor on stubs =)

* [PrintIp](https://github.com/mkvdv/otus-cpp-2018/tree/task04) - example of 
[SFINAE](https://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error) usage 

* [Range](https://github.com/mkvdv/otus-cpp-2018/tree/task09_2) - example of usage [Range v3](https://github.com/ericniebler/range-v3) 
library, loaded from [Conan package manager](https://conan.io/)


### Installation

* Resolving debian packages from [bintray](https://bintray.com/mkvdv/otus-cpp-2018/ha)

```bash
 wget -qO -https://bintray.com/user/downloadSubjectPublicKey?username=bintray | sudo apt-key add -
 echo "deb https://dl.bintray.com/mkvdv/otus-cpp-2018 trusty  main" | sudo tee -a /etc/apt/sources.list
 apt update
 apt install package_name
``` 
