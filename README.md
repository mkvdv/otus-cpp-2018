# Task07 - Bulk
[![Build Status](https://travis-ci.com/mkvdv/otus-cpp-2018.svg?branch=task07)](https://travis-ci.com/mkvdv/otus-cpp-2018)
[![Download](https://api.bintray.com/packages/mkvdv/otus-cpp-2018/ha/images/download.svg?version=bulk) ](https://bintray.com/mkvdv/otus-cpp-2018/ha/bulk/link)


### Notes
* All build files must be done in `build` directory (created by `.travis.yml`), all binaries are in `build/bin`

### Usage
```bash
$ seq 0 9 | ./bin/bulk 1
bulk: 0
bulk: 1
bulk: 2
bulk: 3
bulk: 4
bulk: 5
bulk: 6
bulk: 7
bulk: 8
bulk: 9
$ seq 0 9 | ./bin/bulk 2
bulk: 0, 1
bulk: 2, 3
bulk: 4, 5
bulk: 6, 7
bulk: 8, 9
$ seq 0 9 | ./bin/bulk 3
bulk: 0, 1, 2
bulk: 3, 4, 5
bulk: 6, 7, 8
bulk: 9
$ seq 0 9 | ./bin/bulk 11
bulk: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
```