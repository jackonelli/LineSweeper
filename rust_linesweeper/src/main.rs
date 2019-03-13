use rust_linesweeper::graph;

fn main() {
    let filepath = String::from("../graph/rust_wahde.json");
    let graph = graph::Graph::new(filepath);
    println!(
        "Nearest neigbour path: {}",
        graph.length_nearest_neighbour_path(None)
    );
}
