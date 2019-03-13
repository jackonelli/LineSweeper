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

    parser.add_argument('--in', type=Path, required=True,
                        dest="in_",
                        help='Path to input .json file')

    parser.add_argument('--out', type=Path, required=True,
                        help='Path to output .json file')

    parser.add_argument('-f', '--force', action='store_true')

    return parser.parse_args()


def read_json(json_file_path: Path):
    """Read json file"""
    try:
        with json_file_path.open('r') as json_file:
            data = json.load(json_file)
    except FileNotFoundError:
        print("File not found: {}".format(json_file_path))
        sys.exit(1)

    return data


def extract_id_as_keys(data: dict):
    """Transfer raw nested list data to dict"""
    new_dict = dict()
    nodes = data.get("nodes")
    if not nodes:
        print("No 'nodes' key in dict.")
        sys.exit(1)
    for node in nodes:
        new_node = {"x": node["x"],
                    "y": node["y"]
                   }
        new_dict[node["id"]] = new_node
    return new_dict


def write_dict_to_json(json_file_path: Path, graph_dict: dict,
                       overwrite: bool):
    """Write graph data in dict to json"""
    if json_file_path.exists() and not overwrite:
        print("File already exists, use --force to overwrite")
        sys.exit(1)
    with json_file_path.open('w') as json_file:
        json.dump(graph_dict, json_file, sort_keys=True, indent=4)


def main():
    """Main entry point for script"""
    args = parse_args()

    graph_data = read_json(args.in_)
    graph_dict = extract_id_as_keys(graph_data)
    write_dict_to_json(args.out, graph_dict, args.force)


if __name__ == '__main__':
    main()
