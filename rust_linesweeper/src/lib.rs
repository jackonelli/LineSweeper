#[macro_use]
extern crate serde_derive;
extern crate serde;
extern crate serde_json;

use std::collections::HashMap;
use std::collections::HashSet;
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

    pub fn length_nearest_neighbour_path(&self, start_node: Option<usize>) -> f32 {
        let mut unvisited_nodes = self.reset_unvisited_nodes();
        let mut current_node = start_node.unwrap_or(0);
        unvisited_nodes.remove(&current_node);
        let mut shortest_length = std::f32::INFINITY;
        let mut length: f32;
        for next_node in &unvisited_nodes {
            length = self.get_edge_length(current_node, *next_node);
        }
        let total_length = 0.0;
        total_length
    }

    pub fn reset_unvisited_nodes(&self) -> HashSet<usize> {
        let mut unvisited_nodes = HashSet::new();
        for key in self.nodes.keys() {
            unvisited_nodes.insert(*key);
        }
        unvisited_nodes
    }
    pub fn get_edge_length(&self, node_1: Node, node_2: Node) -> f32 {
        0.0
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

    #[test]
    fn calculate_nearest_path() {
        let mut tmp_nodes = HashMap::new();
        tmp_nodes.insert(0, Node{x: 0.0, y: 0.0});
        tmp_nodes.insert(1, Node{x: 0.0, y: 1.0});
        tmp_nodes.insert(2, Node{x: 1.0, y: 1.0});
        tmp_nodes.insert(3, Node{x: 1.0, y: 2.0});
        let test_graph =  Graph{
            number_of_nodes: 4,
            nodes: tmp_nodes,
            edges_connected: vec![],
        };
        assert_eq!(test_graph.length_nearest_neighbour_path(Some(1)), 3.0);
    }

    fn calculate_edge_length() {
        let node_1 = Node{x: -1.0, y: 0.0};
        let node_2 = Node{x: 0.0, y: 1.0};
        assert_eq!(Graph::get_edge_length(node_1, node_2), 2.0_f32.sqrt());
    }
}
