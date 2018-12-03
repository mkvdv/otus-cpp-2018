#!/usr/bin/python3

import argparse
import sys

import matplotlib.pyplot as plt


def main(out_filename: str):
    xs = []
    ys = []

    for line in sys.stdin:
        data = line.split(';')
        x = float(data[0])
        y = float(data[1])

        xs.append(x)
        ys.append(y)

    plt.scatter(x=xs, y=ys, marker="o")
    plt.savefig(out_filename)
    print("Saved scatter plot to {0}".format(out_filename))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description='Draw plot with dots from stdin (in x;y\\n format)')
    parser.add_argument('--out', action='store', dest='output_file',
                        type=str, help='Output file_name')
    args = parser.parse_args()

    main(str(args.output_file))
