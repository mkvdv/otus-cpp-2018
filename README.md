# Task06 - Matrix
[![Build Status](https://travis-ci.com/mkvdv/otus-cpp-2018.svg?branch=task07)](https://travis-ci.com/mkvdv/otus-cpp-2018)
[ ![Download](https://api.bintray.com/packages/mkvdv/otus07/bulk/images/download.svg) ](https://bintray.com/mkvdv/otus07/bulk/_latestVersion)

### Notes
* All build files must be done in `build` directory (created by `.travis.yml`), all binaries are in `build/bin`

### Usage
You can use it with redirecting `stdin` to file. For example, if `cmd.txt` file contain this text
```txt
cmd1
cmd2
cmd3
cmd4
cmd5
```

Calling application with this file as stdin
    
```sh
./bulk 3 <cmd.txt
```

Will give this output:
```
cmd1    |
cmd2    |
cmd3    |
        | bulk: cmd1, cmd2, cmd3
cmd4    |
cmd5    |
        | bulk: cmd4, cmd5

```