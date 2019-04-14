mod single;
mod multi;

fn main() {
    println!("Single threaded solutions:");
    println!("--------------------------");
    single::single();
    println!("");
    println!("Multi threaded solutions");
    println!("--------------------------");
    multi::multi();
    println!("");
}
