#!/usr/bin/python3

import argparse
import sys

import matplotlib.pyplot as plt


def main(out_filename: str):
    clusters = dict()

    for line in sys.stdin:
        data = line.split(';')
        x = float(data[0])
        y = float(data[1])
        n = int(data[2])

        if n not in clusters.keys():
            clusters[n] = dict()
            clusters[n]['x'] = []
            clusters[n]['y'] = []

        clusters[n]['x'].append(x)
        clusters[n]['y'].append(y)

    for cluster in clusters.values():
        plt.scatter(x=cluster['x'], y=cluster['y'], marker="o")
    plt.savefig(out_filename)

    print("Saved plot to {0}".format(out_filename))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description='Draw clustered results of k-means algo, '
                    'in x;y;cluster_no\\n format')
    parser.add_argument('--out', action='store', dest='output_file',
                        type=str, help='Output file_name')
    args = parser.parse_args()

    main(str(args.output_file))
