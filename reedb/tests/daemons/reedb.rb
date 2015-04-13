# this is my_daemons.rb
require 'daemons'
require_relative '../../lib/reedb/utils/logger'
require_relative '../../lib/reedb/utils/utilities'

puts "Output 1"

# Initialize the app while we're not a daemon
Reedb::DaemonLogger.setup("/home/spacekookie/Desktop")
# server = TCPServer.open(2000)

puts "Init done"


# Become a daemon
Daemons.daemonize

Reedb::DaemonLogger.write("Daemon created")

# The server loop
loop do
	# puts "Loop!"
 #  client = server.accept
 #  params = JSON.parse(client.gets)
  Reedb::DaemonLogger.write("Still alive")
  sleep(1)
end