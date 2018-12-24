# Task12 - Bulk Server
[![Build Status](https://travis-ci.com/mkvdv/otus-cpp-2018.svg?branch=task12)](https://travis-ci.com/mkvdv/otus-cpp-2018)
[![Download](https://api.bintray.com/packages/mkvdv/otus-cpp-2018/ha/images/download.svg?version=bulk_server) ](https://bintray.com/mkvdv/otus-cpp-2018/ha/bulk_server/link)

### Notes
* All build files must be done in `build` directory (created by `.travis.yml`), all binaries are in `build/bin`

### Usage
```bash
./build/bin/bulk_server --port=9000 --bulk_size=3
```

* You can enable inner loggingm if you want, using  `logging` parameter:
```bash
./build/bin/bulk_server --port=9000 --bulk_size=3 --logging=true
```