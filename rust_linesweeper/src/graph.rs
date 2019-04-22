use crate::tools;
use serde_derive::Deserialize;
use serde_json;
use std::collections::HashMap;

// TODO: Node key as its own type instead of usize
#[derive(Deserialize, Clone, PartialEq)]
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
    pub fn new(data_file: &str) -> Graph {
        let json_string = tools::io::read_file(&data_file);
        let tmp_nodes = Graph::nodes_from_json(&json_string);
        Graph {
            number_of_nodes: tmp_nodes.len(),
            nodes: tmp_nodes,
            edges_connected: Graph::edges_from_json(&json_string),
        }
    }

    fn nodes_from_json(json_string: &str) -> HashMap<usize, Node> {
        let graph_data: GraphData =
            serde_json::from_str(json_string).expect("Could not parse json");
        let nodes = match graph_data.nodes {
            Some(nodes) => nodes,
            None => panic!("No nodes"),
        };
        nodes
    }

    fn edges_from_json(json_string: &str) -> Vec<Edge> {
        let graph_data: GraphData =
            serde_json::from_str(json_string).expect("Could not parse json");
        let edges = match graph_data.edges {
            Some(edges) => edges,
            None => vec![],
        };
        edges
    }

    fn add_node(&mut self, x: f32, y: f32) {
        let candidate = Node { x, y };
        for (_, node) in self.nodes.iter() {
            if candidate == *node {
                return;
            };
        }
        self.nodes.insert(self.number_of_nodes, candidate);
        self.number_of_nodes += 1;
    }

    pub fn calulate_path_length(&self, path: &[usize]) -> f32 {
        let mut path_length = 0.0;
        for node_key in 1..path.len() {
            let current_node = match self.nodes.get(&(path[node_key - 1])) {
                Some(current_node) => current_node,
                None => panic!("Node does not exist"),
            };
            let next_node = match self.nodes.get(&(path[node_key])) {
                Some(next_node) => next_node,
                None => panic!("Node does not exist"),
            };
            path_length += Graph::get_edge_length(&current_node, &next_node);
        }
        path_length
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

    pub fn get_edge_length(node_1: &Node, node_2: &Node) -> f32 {
        let dx = node_1.x - node_2.x;
        let dy = node_1.y - node_2.y;
        (dx * dx + dy * dy).sqrt()
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
    fn calc_path_length() {
        let mut tmp_nodes = HashMap::new();
        tmp_nodes.insert(1, Node { x: 0.0, y: 1.0 });
        tmp_nodes.insert(2, Node { x: 1.0, y: 1.0 });
        tmp_nodes.insert(3, Node { x: 1.0, y: 2.0 });
        let test_graph = Graph {
            number_of_nodes: 3,
            nodes: tmp_nodes,
            edges_connected: vec![],
        };
        let path = vec![2, 3, 1];
        let path_length = test_graph.calulate_path_length(&path);
        assert_eq!(path_length, 1.0 + 2.0_f32.sqrt());
    }

    #[test]
    fn get_nearest_node() {
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
