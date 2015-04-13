require 'socket'
require 'json'

host = 'localhost'
port = 2000

s = TCPSocket.open(host, port)

data = {'body'=>{"who's the boss?"=>"ME BITCHES!!!"}}

request = data.to_json
s.print(request)

s.close