# task13 - Join Server
[![Build Status](https://travis-ci.com/mkvdv/otus-cpp-2018.svg?branch=task13)](https://travis-ci.com/mkvdv/otus-cpp-2018)
[![Download](https://api.bintray.com/packages/mkvdv/otus13/join_server/images/download.svg)](https://bintray.com/mkvdv/otus13/join_server/_latestVersion)

### Notes
* All build files must be done in `build` directory (created by `.travis.yml`), all binaries are in `build/bin`

### Usage
```bash
./build/bin/join_server --port=9000
```

* You can enable inner loggingm if you want, using  `logging` parameter:
```bash
./build/bin/join_server --port=9000 --logging=true
```

### Testing
* There are simple test client `tests/simple_test_client.py`. How to test:
```bash
# in first terminal
./build/bin/join_server --port=9000

# in second terminal
python tests/simple_test_client.py --port 9000

```
