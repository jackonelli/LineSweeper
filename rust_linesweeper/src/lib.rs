#[macro_use]
extern crate serde_derive;
extern crate serde;
extern crate serde_json;

use std::collections::HashMap;
use std::fs;
use std::io::BufReader;
use std::io::prelude::*;
use serde_json::Deserializer;

mod tools;

#[derive(Deserialize)]
pub struct Node{
    x: f32,
    y: f32,
}

#[derive(Deserialize)]
pub struct Edge{
    to: usize,
    from: usize,
}

pub struct Graph{
    pub number_of_nodes: usize,
    pub nodes: HashMap<usize, Node>,
    pub edges_connected: Vec<Edge>,
    pub data_file: String,
}

#[derive(Deserialize)]
struct GraphData {
    nodes: Option<HashMap<usize, Node>>,
    edges: Option<Vec<Edge>>,
}


impl Graph {
    pub fn new(data_file: String) -> Graph {
        let json_string = tools::io::read_file(&data_file);
        let tmp_nodes = Graph::nodes_from_json(&json_string);
        Graph {
            number_of_nodes: tmp_nodes.len(),
            nodes: tmp_nodes,
            edges_connected: Graph::edges_from_json(&json_string),
            data_file: data_file,
        }
    }

    pub fn nodes_from_json(json_string: &String) -> HashMap<usize, Node> {
        let graph_data: GraphData = serde_json::from_str(json_string).
            expect("Could not parse json");
        let nodes = match graph_data.nodes {
            Some(nodes) => nodes,
            None => panic!("No nodes"),
        };
        nodes
    }

    pub fn edges_from_json(json_string: &String) -> Vec<Edge> {
        let graph_data: GraphData = serde_json::from_str(json_string).
            expect("Could not parse json");
        let edges = match graph_data.edges {
            Some(edges) => edges,
            None => vec![],
        };
        edges
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn read_json_to_nodes() {
    let data = String::from(
                r#"{
                    "nodes": {
                        "1": {
                            "x": 43.0,
                            "y": 2.1
                        },
                        "2": {
                            "x": 1.15,
                            "y": 0.1
                        }
                    }
                  }"#
                  );
        let test_graph = Graph::nodes_from_json(&data);
        assert_eq!(test_graph.len(), 2);
    }
}
