use std::collections::HashMap;

use rust_linesweeper;

fn main() {
    let filepath = String::from("../graph/rust_wahde.json");
    //let filepath = String::from("../graph/nodes_test.json");
    let graph = rust_linesweeper::Graph::new(filepath);
}
