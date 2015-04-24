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
require 'daemons'
require_relative 'reedb/constants.rb'

NAME = 'reedb'

if __FILE__ == $PROGRAM_NAME	

	if ARGV.include? '--version'
		puts Reedb::VERSION
		exit
	end

	if ARGV == []
		# First prints out some help stuff
		puts "Usage: reedb [daemon options] -- [reedb options]

  Available [reedb options]:
     -l, --pw-length INTEGER     Define minimal passphrase length (Default: 12)
     -p, --port INTEGER          Change the listener port. May break your setup!
     -v, --verbose               Enable verbose logging about the Reedb daemon.
     -d, --no-daemon             Don't run Reedb as a background daemon. Log to STOUT instead of log file.

  Available [daemon options]:
     start                       Start an instance of the #{NAME} daemon
     stop                        Stop all instances of the #{NAME} daemon
     restart                     Stop all and restart a new instance of #{NAME} afterwards
     status                      Show status (PID) of #{NAME} daemon instance

  Common options:
         --version               Show #{NAME} version"

		# Then exits the application
		exit
	end

	options = {
		# :app_name   => "myproc",
		:backtrace  => true,
		# :monitor    => true,
		# :ontop      => true
	}

	# If it went through it then runs the daemon with the desired options
	path = File.join(File.dirname(__FILE__), 'reedb')
	Daemons.run(File.join(path, 'reedb.rb'), options)

end