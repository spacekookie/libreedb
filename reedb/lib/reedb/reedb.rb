# ====================================================
# Copyright 2015 Lonely Robot (see @author)
# @author: Katharina Sabel | www.2rsoftworks.de
#
# Distributed under the GNU Lesser GPL Version 3
# (See accompanying LICENSE file or get a copy at
# 	https://www.gnu.org/licenses/lgpl.html)
# ====================================================

# Internal requirements
require_relative "utils/utilities"
require_relative "utils/logger"
require_relative "constants"
require_relative "reevault"

# System requirements
require 'socket'
require 'json'

# Main module embedded as a system daemon.
# Handles new vaults, loading vaults and caching multiple vaults per
# name and/ or id.
#
module Reedb
	class << self

		# Returns the platform/ architecture of the daemon and vault handler
		def archos() (return @@archos) end

		def daemon?() (return @@daemon) end

		def passlength() (return @@passlength) end

		def init(global_os, passlength = 8, d = true)
			@@daemon = d
			@@archos = global_os
			@@passlength = passlength

			@@vaults = {}

			if global_os == :linux
				parent_path = File.expand_path('~/.config/reedb/')
				master_path = File.expand_path('/etc/reedb/')
				log_path = File.expand_path('~/.config/reedb/logs')

			else

				# 
			end
			Reedb::DaemonLogger.setup("#{log_path}")

		end

		def active_vaults
			return @@vaults
		end

		# Default encryption type is 'AES'
		#
		def vault(name='default', path=nil, encrypt='auto_fill')
			@@vaults[name] = ReeVault.new(name, path, encrypt)
			return @@vaults[name]
		end
	end
end

def setup_listener port
	@server = TCPServer.open(port)
end

loop {
  client = server.accept
  params = JSON.parse(client.gets)
  Reedb::DaemonLogger.write(params)
}