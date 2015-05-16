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
require 'reedb'

NAME = 'reedb'

if __FILE__ == $PROGRAM_NAME

	if ARGV.include? '--version'
		puts Reedb::VERSION
		exit
	end

	if ARGV == []
		puts "[ERROR]: Invalid application arguments!"
		puts "Usage: reedb [daemon options] -- [reedb options]

  Available [reedb options]:
     -l, --pw-length INTEGER     Define minimal passphrase length (Default: 12)
     -p, --port INTEGER          Change the listener port. May break your setup!
     -a, --app-path STRING      Change the path for the reedb config files/ logs. (default in ~)
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
		:app_name   => "reedb",
		:backtrace  => true,
		# :monitor    => true,
		# :ontop      => true
	}

	spec = Gem::Specification.find_by_name("reedb")
	(puts "Error: reedb not installed!" ; return nil) unless spec
	path = (spec.gem_dir) + "/lib/reedb"

	Daemons.run(File.join(path, 'daemon_wrapper.rb'), options)
end