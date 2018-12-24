# Task 14 - YAMR
[![Build Status](https://travis-ci.com/mkvdv/otus-cpp-2018.svg?branch=task14)](https://travis-ci.com/mkvdv/otus-cpp-2018)
[![Download](https://api.bintray.com/packages/mkvdv/otus-cpp-2018/ha/images/download.svg?version=yamr) ](https://bintray.com/mkvdv/otus-cpp-2018/ha/yamr/link)

### Notes
* All build files must be done in `build` directory (created by `.travis.yml`), all binaries are in `build/bin`

### Usage
```bash
./yamr --src=input.txt --mnum=5 --rnum=6 
```

* You can enable inner logging if you want, using `logging` parameter:
```bash
./yamr --src=input.txt --mnum=5 --rnum=6 --logging=true
```
