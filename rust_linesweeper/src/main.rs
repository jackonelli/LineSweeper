use rust_linesweeper::ant_system;
use rust_linesweeper::graph;

fn main() {
    let filepath = String::from("../graph/rust_wahde.json");
    let graph = graph::Graph::new(&filepath);
    let ant_system = ant_system::AntSystem::new(graph, 5, 0.1, 1.0, 1.0);
}
