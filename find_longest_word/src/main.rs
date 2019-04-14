mod single;
mod multi;

fn main() {
    println!("Single threaded solutions:");
    single::Single();
    println!("Multi threaded solutions");
    multi::Multi();
}
