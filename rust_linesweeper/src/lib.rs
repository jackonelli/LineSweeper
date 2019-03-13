#[macro_use]
extern crate serde_derive;
extern crate serde;
extern crate serde_json;

use serde_json::Deserializer;
use std::collections::HashMap;
use std::collections::HashSet;
use std::fs;
use std::io::prelude::*;
use std::io::BufReader;

mod tools;

#[derive(Deserialize, Clone)]
pub struct Node {
    x: f32,
    y: f32,
}

#[derive(Deserialize)]
pub struct Edge {
    to: usize,
    from: usize,
}

pub struct Graph {
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
        let graph_data: GraphData =
            serde_json::from_str(json_string).expect("Could not parse json");
        let nodes = match graph_data.nodes {
            Some(nodes) => nodes,
            None => panic!("No nodes"),
        };
        nodes
    }

    pub fn edges_from_json(json_string: &String) -> Vec<Edge> {
        let graph_data: GraphData =
            serde_json::from_str(json_string).expect("Could not parse json");
        let edges = match graph_data.edges {
            Some(edges) => edges,
            None => vec![],
        };
        edges
    }

    pub fn length_nearest_neighbour_path(&self, start_id: Option<usize>) -> f32 {
        let mut unvisited_nodes = self.nodes.clone();
        let mut current_node = unvisited_nodes.remove(&start_id.unwrap_or(0)).unwrap();
        let mut total_length = 0.0;
        for _ in 0..unvisited_nodes.len() {
            let (nearest_id, length) = Graph::get_nearest_node(&unvisited_nodes, &current_node);
            total_length += length;
            current_node = unvisited_nodes.remove(&nearest_id).unwrap();
        }
        total_length
    }

    fn get_nearest_node(
        unvisited_nodes: &HashMap<usize, Node>,
        current_node: &Node,
    ) -> (usize, f32) {
        let mut shortest_length = std::f32::INFINITY;
        let mut length: f32;
        let mut nearest_id = 0;
        for (next_id, next_node) in unvisited_nodes {
            length = Graph::get_edge_length(current_node, next_node);
            if length < shortest_length {
                shortest_length = length;
                nearest_id = *next_id;
            }
        }
        (nearest_id, shortest_length)
    }

    fn get_edge_length(node_1: &Node, node_2: &Node) -> f32 {
        let dx = node_1.x - node_2.x;
        let dy = node_1.y - node_2.y;
        (dx * dx + dy * dy).sqrt()
    }

    pub fn reset_unvisited_nodes(&self) -> HashSet<usize> {
        let mut unvisited_nodes = HashSet::new();
        for key in self.nodes.keys() {
            unvisited_nodes.insert(*key);
        }
        unvisited_nodes
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
                  }"#,
        );
        let test_graph = Graph::nodes_from_json(&data);
        assert_eq!(test_graph.len(), 2);
    }

    #[test]
    fn calculate_nearest_path() {
        let mut tmp_nodes = HashMap::new();
        tmp_nodes.insert(0, Node { x: 0.0, y: 0.0 });
        tmp_nodes.insert(1, Node { x: 0.0, y: 1.0 });
        tmp_nodes.insert(2, Node { x: 1.0, y: 1.0 });
        tmp_nodes.insert(3, Node { x: 1.0, y: 2.0 });
        let test_graph = Graph {
            number_of_nodes: 4,
            nodes: tmp_nodes,
            edges_connected: vec![],
        };
        assert_eq!(test_graph.length_nearest_neighbour_path(Some(0)), 3.0);
    }

    #[test]
    fn get_next_node() {
        let mut tmp_nodes = HashMap::new();
        tmp_nodes.insert(1, Node { x: 0.0, y: 1.0 });
        tmp_nodes.insert(2, Node { x: 1.0, y: 1.0 });
        tmp_nodes.insert(3, Node { x: 1.0, y: 2.0 });
        let test_graph = Graph {
            number_of_nodes: 4,
            nodes: tmp_nodes,
            edges_connected: vec![],
        };
        let current_node = Node { x: 0.0, y: 0.0 };
        let (id, length) = Graph::get_nearest_node(&test_graph.nodes, &current_node);
        assert_eq!(length, 1.0);
        assert_eq!(id, 1);
    }

    #[test]
    fn calculate_edge_length() {
        let node_1 = Node { x: -1.0, y: 0.0 };
        let node_2 = Node { x: 0.0, y: 1.0 };
        assert_eq!(Graph::get_edge_length(&node_1, &node_2), 2.0_f32.sqrt());
    }
}
