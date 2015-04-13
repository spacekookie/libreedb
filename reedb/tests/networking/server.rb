require 'socket'
require 'json'
require 'daemons'

server = TCPServer.open(2000)

Daemons.daemonize

loop {
  client = server.accept
  params = JSON.parse(client.gets)
  puts params
  # raise params.inspect
}