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
    shortest_path: Vec<usize>,
    min_path_length: f32,
    total_iterations: usize,
}

impl AntSystem {
    pub fn new(
        graph: graph::Graph,
        number_of_ants: usize,
        evaporation: f32,
        alpha: f32,
        beta: f32,
    ) -> AntSystem {
        let mut ant_system = AntSystem {
            graph,
            number_of_ants,
            evaporation,
            alpha,
            beta,
            pheromone_levels: vec![0.0],
            min_path_length: std::f32::INFINITY,
            shortest_path: vec![0],
            total_iterations: 0,
        };
        ant_system.init_pheromone_levels();
        ant_system
    }

    fn init_pheromone_levels(&mut self) {
        let initial_level =
            self.number_of_ants as f32 / self.graph.length_nearest_neighbour_path(None);
        self.pheromone_levels =
            vec![initial_level; self.graph.number_of_nodes * self.graph.number_of_nodes];
    }

    pub fn improve_best_path(&mut self, max_number_of_iterations: usize) {
        let mut local_iteration = 0;
        let previous_shortest_path_length = self.min_path_length;
        while self.min_path_length > previous_shortest_path_length
            && local_iteration <= max_number_of_iterations
        {
            local_iteration += 1;
            let mut delta_pheromone_levels = vec![0.0; self.pheromone_levels.len()];
            for k_ant in 0..self.number_of_ants {
                let path = self.generate_path();
                let path_length = self.graph.calulate_path_length(&path);
                self.update_delta_pheromone_levels(&mut delta_pheromone_levels, &path, path_length);
                if path_length < self.min_path_length {
                    self.min_path_length = path_length;
                    self.shortest_path = path;
                }
            }
            self.update_pheromone_leves(&delta_pheromone_levels);
        }
    }

    fn update_delta_pheromone_levels(
        &self,
        delta_pheromone_levels: &mut [f32],
        path: &[usize],
        path_length: f32,
    ) {
        let number_of_nodes_in_path = path.len();
        for i in 0..number_of_nodes_in_path - 1 {
            let source = path[i];
            let target = path[i + 1];
            delta_pheromone_levels[source * self.graph.number_of_nodes + target] +=
                1.0 / path_length;
        }
    }

    fn generate_path(&self) -> Vec<usize> {
        vec![0]
    }

    fn update_pheromone_leves(&mut self, delta_pheromone_levels: &[f32]) {}

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
