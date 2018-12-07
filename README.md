# Task 16 - Realty (Clusterization, classification and regression)
[![Build Status](https://travis-ci.com/mkvdv/otus-cpp-2018.svg?branch=task16)](https://travis-ci.com/mkvdv/otus-cpp-2018)
[![Download](https://api.bintray.com/packages/mkvdv/otus16/realty/images/download.svg)](https://bintray.com/mkvdv/otus16/realty/_latestVersion)

## Notes
* All build files must be done in `build` directory (created by `.travis.yml`), all binaries are in `build/bin`
* Requirements:
    * dlib
    * openblas

## Usage

* Create, train and save classificator and input data
```bash
cd build/bin
# Will create output.XXX and clusters.XXX files
cat ../../16.dataset-12995-8e405d.csv | ./rclst 100 output clusters
```

* Run classfifcator with generated data and classificator files
```bash
# Will read output.XXX and clusters.XXX files
./rclss output cluster
```
