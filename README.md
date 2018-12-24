# Task 15 - k-means
[![Build Status](https://travis-ci.com/mkvdv/otus-cpp-2018.svg?branch=task15)](https://travis-ci.com/mkvdv/otus-cpp-2018)
[![Download](https://api.bintray.com/packages/mkvdv/otus-cpp-2018/ha/images/download.svg?version=kkmeans) ](https://bintray.com/mkvdv/otus-cpp-2018/ha/kkmeans/link)
## Notes
* All build files must be done in `build` directory (created by `.travis.yml`), all binaries are in `build/bin`
* Requirements:
    * python3, matplotlib library
    * dlib
    * openblas

## Usage

#### Generate data

* Generate random clusters-like input data

```bash
cd testing
 
python3 ./gen_clustered_dots.py --k=5 --out=out/circle_clusters/input_5clusters.txt
 
# also u can generate scatter plot of input data
cat out/circle_clusters/input_5clusters.txt | ./draw_input_dots.py --out=./out/circle_clusters/input_5clusters.png
```

* Or generate random dots in square [-100, 100]

```bash
cd testing
 
python3 ./gen_rand_dots.py --out=out/rand/input.txt
 
# also u can generate scatter plot of input data
cat out/rand/input.txt | ./draw_input_dots.py --out=./out/rand/input.png
```
#### Process data
* Run binary with generated input file as stdin data, and visualize results
```bash
cat out/rand/input.txt | ../build/bin/kkmeans --k=3 >./out/rand/out.txt
cat out/circle_clusters/input_5clusters.txt | ../build/bin/kkmeans --k=5 >./out/circle_clusters/out.txt

```

#### Visualize results
* Visualize output data as bubble plot or clustered dots plot
```bash
# bubble plot
cat out/rand/out.txt            |  python3 ./draw_clustered_out_as_buble.py --out=./out/rand/out_buble.png
cat out/circle_clusters/out.txt |  python3 ./draw_clustered_out_as_buble.py --out=./out/circle_clusters/out_buble.png


# clustered dots plot
cat out/rand/out.txt            |  python3 ./draw_clustered_output.py --out=./out/rand/out_clustered.png
cat out/circle_clusters/out.txt |  python3 ./draw_clustered_output.py --out=./out/circle_clusters/out_clustered.png
```
