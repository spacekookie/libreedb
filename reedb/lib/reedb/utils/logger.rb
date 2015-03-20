# ====================================================
# Copyright 2015 Random Robot Softworks (see @author)
# @author: Katharina Sabel | www.2rsoftworks.de
#
# Distributed under the GNU Lesser GPL Version 2.1
# (See accompanying LICENSE file or get a copy at
# 	https://www.gnu.org/licenses/lgpl.html)
# ====================================================

require 'logger'

module Reedb

	# A logger that gets bound to a vault and logs vault activity in the
	# vaults logs/ directory.
	# It's using the Ruby internal logging library to split up logs into date blocks.
	# (Each day is one log file).
	# 
	class VaultLogger

		@@logger = nil

		# Sets up a logger on a vault and loads existing logs if they exist.
		# Logs are limited not in size but only by dates. Vault logs don't contain whitespaces.
		#
		def self.setup(path)
			log_path = "#{path}/logs/#{Reedb::Utilities.get_time(true)}.log"
			@@logger = Logger.new("#{log_path}")
		end


		# Writes something to the current vault log (under #{@path}/logs/)
		# Possible parameters are:
		# 'debug':: Throwing every operation at the log.
		# 'info':: Default logging level for most situations and events.
		# 'warn':: Logs a warning. Should be event that won't impact stability.
		# 'error':: Logs an error. Should be recoverable event.
		# 'fatal':: Logs a fatal crash. Should make the Reepass daemon crash!
		def self.write(message, level = "")
			if level == "warn"
				@@logger.warn(message)

			elsif level == "debug"
				@@logger.debug(message)

			elsif level == "error"
				@@logger.error(message)

			elsif level == "fatal"
				@@logger.fatal(message)
				
			else
				@@logger.info(message)
			end
		end

	end
end