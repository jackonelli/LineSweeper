use crate::graph;
use rand;
use rand::Rng;
use std::collections::HashSet;

pub struct AntSystem {
    graph: graph::Graph,
    number_of_ants: usize,
    evaporation: f32,
    alpha: f32,
    beta: f32,
    pheromone_levels: Vec<f32>,
}

impl AntSystem {
    fn init_pheromone_levels(&mut self) {}

    fn get_next_node(&self, current_node_key: usize, unvisited_nodes: &HashSet<usize>) -> usize {
        let mut transition_probabilities =
            self.calc_transition_probabilities(unvisited_nodes, current_node_key);
        self.roulette_wheel_selection(&mut transition_probabilities)
    }

    fn calc_transition_probabilities(
        &self,
        unvisited_nodes: &HashSet<usize>,
        current_node_key: usize,
    ) -> Vec<(usize, f32)> {
        let mut transition_probabilities = Vec::new();
        let mut total_probability = 0.0;
        transition_probabilities.reserve(unvisited_nodes.len());
        let current_node = self.graph.nodes.get(&current_node_key).unwrap();
        for node_key in unvisited_nodes.iter() {
            let node = self.graph.nodes.get(&node_key).unwrap();
            let tmp_prob = self.pheromone_levels
                [current_node_key * self.graph.number_of_nodes + node_key]
                .powf(self.alpha)
                * graph::Graph::get_edge_length(current_node, node).powf(-self.beta);
            transition_probabilities.push((*node_key, tmp_prob));
            total_probability += tmp_prob;
        }
        for prob in transition_probabilities.iter_mut() {
            prob.1 /= total_probability;
        }
        transition_probabilities.sort_by(|a, b| a.1.partial_cmp(&b.1).unwrap());
        transition_probabilities
    }

    fn roulette_wheel_selection(&self, transition_probabilities: &mut Vec<(usize, f32)>) -> usize {
        let mut rng = rand::thread_rng();
        let threshold = rng.gen_range(0.0, 1.0);
        let mut candidate = match transition_probabilities.pop() {
            Some(node) => node,
            None => panic!("Could not pop first prob from the stack."),
        };
        let mut cumulative_probability = candidate.1;
        while cumulative_probability < threshold {
            candidate = match transition_probabilities.pop() {
                Some(node) => node,
                None => panic!("Reached end of prob stack without exiting"),
            };
            cumulative_probability += candidate.1;
        }
        candidate.0
    }
}
