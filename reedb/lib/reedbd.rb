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

if __FILE__ == $PROGRAM_NAME

	options = {}

	#create parsers
	opts = OptionParser.new()
	opts.banner = "Usage: reedb [daemon options] -- [reedb options]\n\n\tAvailable [reedb options]:"

	opts.on('-l', '--pw-length INTEGER', "Define minimal passphrase length (Default: 12)") { |o| options[:pw_length] = o }
	opts.on('-p', '--port INTEGER', "Change the listener port. May break your setup!") { |o| options[:port] = o }

	opts.on('-v', '--verbose', "Enable verbose logging about the Reedb daemon.") { options[:verbose] = false }
	opts.on('-d', '--daemon', "Run Reedb as a background daemon. Log to file instead of STOUT.") { |o| options[:daemon] = o }

	if ARGV == []
		puts opts.help

		puts '
  Available [daemon options]:
	  start         Start an instance of the reedb daemon
	  stop          Stop all instances of the reedb daemon
	  restart       Stop all and restart a new instance of reedb afterwards
	  status        Show status (PID) of reedb daemon instance

Common options:
    -?, --help                       Show this message
        --version                    Show reedb version'


	else
		puts "Something else"
	end

	

end

# opts.parse!

# 

# 	# Define the options and populate with defaults.
# 	options = {}
# 	# default_options(options)

# 	# Populate the options parser with the system and then parse the arguments.
# 	op = OptionParser.new do |opt|
# 	end


# 	# Now do the actual stuff that needs to get done.
# 	# op.help

# 	# Reedb.init(options['platform'], options['pw_length'])

# end

# 