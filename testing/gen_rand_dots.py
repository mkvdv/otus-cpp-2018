#!/usr/bin/python3

import argparse
import operator
import random
from functools import reduce


def main(filename: str):
    res = []

    for i in range(500):
        res.append("{0};{1}\n".format(random.randint(-100, 100), random.randint(-100, 100)))

    with open(filename, 'w') as file:
        s = reduce(operator.add, res)
        file.write(s)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Generate random dots in [-100, 100]')
    parser.add_argument('--out', action='store', type=str, dest='filename', help='Output file_name')
    args = parser.parse_args()

    main(str(args.filename))
