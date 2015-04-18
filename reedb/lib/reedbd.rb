#!/usr/bin/ruby

# ====================================================
# Copyright 2015 Lonely Robot (see @author)
# @author: Katharina Sabel | www.2rsoftworks.de
#
# Distributed under the GNU Lesser GPL Version 3
# (See accompanying LICENSE file or get a copy at
# 	https://www.gnu.org/licenses/lgpl.html)
# ====================================================


# Very simple wrapper script to start the Reedb daemon.
require 'optparse'
require 'daemons'

NAME = 'reedb'

if __FILE__ == $PROGRAM_NAME

	options = {}
	options[:pw_length] = 8
	options[:verbose] = false
	options[:daemon] = true
	options[:port] = 55736
	

	#create parsers
	opts = OptionParser.new()
	opts.banner = "Usage: #{NAME} [daemon options] -- [reedb options]\n\n\tAvailable [reedb options]:"

	opts.on('-l', '--pw-length INTEGER', "Define minimal passphrase length (Default: 12)") { |o| options[:pw_length] = o }
	opts.on('-p', '--port INTEGER', "Change the listener port. May break your setup!") { |o| options[:port] = o }

	opts.on('-v', '--verbose', "Enable verbose logging about the Reedb daemon.") { options[:verbose] = true }
	opts.on('-d', '--no-daemon', "Don't run Reedb as a background daemon. Log to STOUT instead of log file.") { options[:daemon] = false }

	# if ARGV == []
	# 	puts opts.help

	# 	puts "
 #  Available [daemon options]:
	# 	start                            Start an instance of the #{NAME} daemon
	# 	stop                             Stop all instances of the #{NAME} daemon
	# 	restart                          Stop all and restart a new instance of #{NAME} afterwards
	# 	status                           Show status (PID) of #{NAME} daemon instance

	# Common options:
 #    -?, --help                       Show this message
 #        --version                    Show #{NAME} version"

	# else
	# 	opts.parse!
	# end

	puts options
	Daemons.run(File.join(File.dirname("#{__FILE__}/reedb"), 'reedb.rb'), options)
end