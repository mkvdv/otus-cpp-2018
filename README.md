# Task 8 - bayan
[![Build Status](https://travis-ci.com/mkvdv/otus-cpp-2018.svg?branch=task08_2)](https://travis-ci.com/mkvdv/otus-cpp-2018)
[ ![Download](https://api.bintray.com/packages/mkvdv/otus-cpp-2018/ha/images/download.svg?version=bayan) ](https://bintray.com/mkvdv/otus-cpp-2018/ha/bayan/link)

Utility for detecting duplicate files.

## Notes
* All build files must be done in `build` directory (created by `.travis.yml`), all binaries are in `build/bin`
* Required Boost and OpenSSL libraries
* Task description you can find [here](08.bayan.pdf).

## Usage
* There are such program options
```txt
Allowed options:
  --help                produce help message
  --dirs arg            directories
  --ex-dirs arg         excluded directories
  --rec arg             scanning level: true is recursive search, false is 
                        non-recursive search without nested ones
  --minsize arg (=1)    minimum size of files to be checked 
  --masks arg           masks for files
  --bsize arg           block_size for reading files
  --hash arg            name of hashing algorithm

```

* Required options are dirs and rec. Example: find all duplicate files in
 directories dir1, dir2, dir3 recursively
```bash
bayan --dirs d1 d2 --rec true --ex-dirs d1/d12 d2/d21/d211 --minsize 1 --bsize 1 --hash=sha-1 --masks fi?e_name.*
```
