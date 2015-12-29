extern crate hyper;

use hyper::Server;
use hyper::server::Request;
use hyper::server::Response;

fn hello(_: Request, res: Response) {
    res.send(b"You've reached Reedb customer service...There is no API currently available to take your request. Please wait...").unwrap();
}

fn main() {
    Server::http("0.0.0.0:13999").unwrap().handle(hello);
}