# -*- coding: utf-8 -*-
"""
Created on Mon Oct 08 10:00:00 2018

@author: jakob.lindqvist
"""
import sys
from copy import deepcopy
import json
import csv
import argparse
from pathlib import Path


def parse_args():
    """Handle command line arguments"""
    parser = argparse.ArgumentParser(
        description='Convert graph data from .txt to .json')

    parser.add_argument('--txt', type=Path, required=True,
                        help='Path to input .txt file')

    parser.add_argument('--json', type=Path, required=True,
                        help='Path to output .json file')

    parser.add_argument('-f', '--force', action='store_true')

    return parser.parse_args()


def read_txt(txt_file_path: Path):
    """Read txt file"""
    try:
        with open(txt_file_path, 'r') as txt_file:
            reader = csv.reader(txt_file, delimiter=' ')
            graph_data = list(reader)
    except FileNotFoundError:
        print("File not found: {}".format(txt_file_path))
        sys.exit(1)

    return graph_data


def raw_data_to_dict(graph_data: list):
    """Transfer raw nested list data to dict"""
    graph_dict = dict()
    graph_dict["nodes"] = list()
    nodes = graph_dict["nodes"]
    node = dict()
    for id_, sublist in enumerate(graph_data):
        node["id"] = id_
        node["x"] = float(sublist[0])
        node["y"] = float(sublist[1])
        id_ += 1
        nodes.append(deepcopy(node))
    print(nodes)
    return graph_dict


def write_dict_to_json(json_file_path: Path, graph_dict: dict,
                       overwrite: bool):
    """Write graph data in dict to json"""
    if json_file_path.exists() and not overwrite:
        print("File already exists, use --force to overwrite")
        sys.exit(1)
    with open(json_file_path, 'w') as json_file:
        json.dump(graph_dict, json_file, sort_keys=True, indent=4)


def main():
    """Main entry point for script"""
    args = parse_args()

    graph_data = read_txt(args.txt)
    graph_dict = raw_data_to_dict(graph_data)
    write_dict_to_json(args.json, graph_dict, args.force)


if __name__ == '__main__':
    main()
