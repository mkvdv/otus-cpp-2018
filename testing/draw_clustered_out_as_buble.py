#!/usr/bin/python3

import argparse
import sys
from functools import reduce
from typing import Tuple, List

import matplotlib.pyplot as plt

Point = Tuple[float, float]
Points = List[Point]


def gen_info(points: Points) -> (float, float, int):
    sum_x = reduce(lambda p1, p2: (p1[0] + p2[0], 0), points, (0.0, 0.0))[0]
    res_x = sum_x / len(points)
    sum_y = reduce(lambda p1, p2: (0, p1[1] + p2[1]), points, (0.0, 0.0))[1]
    res_y = sum_y / len(points)

    return res_x, res_y, len(points)


def main(out_fname: str):
    """
    read data from input as %f;%f;%d as x;y;cluster_no
    """
    groups = []

    for line in sys.stdin:
        data = line.split(';')
        group_ix = int(data[2])
        x = float(data[0])
        y = float(data[1])
        while group_ix + 1 > len(groups):
            groups.append([])
        groups[group_ix].append((x, y))

    # calculate center of mass for every group,
    # number of elements in group and draw bubble chart

    cluster_info = [gen_info(points) for points in groups]

    x = [info[0] for info in cluster_info]
    y = [info[1] for info in cluster_info]
    count = [info[2] for info in cluster_info]

    plt.scatter(x=x, y=y, s=count, alpha=0.75)
    plt.savefig(out_fname)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description='Draw bubble chart from lines formatted as '
                    'x;y;cluster_no')
    parser.add_argument('--out', action='store',
                        dest='out_fname', help='Output file_name')
    args = parser.parse_args()

    main(str(args.out_fname))
