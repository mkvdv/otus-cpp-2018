#!/usr/bin/python3

import argparse
import math
import operator
import random
from functools import reduce


def main(k: int, output_file: str):
    res = []
    MAX_R = 25

    for _ in range(k):
        center_x = random.uniform(-150, 150)
        center_y = random.uniform(-150, 150)

        for _ in range(50):
            R = MAX_R * math.sqrt(random.uniform(0, 1))
            angle = random.uniform(0, 2 * math.pi)

            res.append("{0};{1}\n".format(center_x + math.cos(angle) * R,
                                          center_y + math.sin(angle) * R))

    s = reduce(operator.add, res)
    with open(output_file, "w") as file:
        file.write(s)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description='Generate random dots like the are in k clusters (in circles')
    parser.add_argument('--k', dest='clusters', action='store',
                        help='Number of clusters you want')
    parser.add_argument('--out', action='store', dest='output_file',
                        type=str, help='Output file_name')
    args = parser.parse_args()

    main(int(args.clusters), str(args.output_file))
