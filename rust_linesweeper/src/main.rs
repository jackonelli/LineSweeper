use std::collections::HashMap;

use rust_linesweeper;

fn main() {
    let filepath = String::from("../graph/wahde_rust.json");
    let graph = rust_linesweeper::Graph::new(filepath);
}

