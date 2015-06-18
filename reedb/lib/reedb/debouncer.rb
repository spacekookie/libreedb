# ====================================================
# Copyright 2015 Lonely Robot (see @author)
# @author: Katharina Sabel | www.lonelyrobot.io
#
# Distributed under the GNU Lesser GPL Version 3
# (See accompanying LICENSE file or get a copy at
# 	https://www.gnu.org/licenses/lgpl.html)
# ====================================================

'' '
THis file contains the debounce handler for the daemon.
That means that it is handed vault information and woken up every time there is an action
on a specified vault which makes it "debounce" back to the start of the counter.
' ''

module Reedb
	class Debouncer

		attr_accessor :running

		#
		# @param core [Object] the owning container instance to
		#               register the debounce close of a vault
		# @param vault_list [Hash] Collection of vaults
		#
		# @return self
		#
		def initialize(core, vault_list)
			@reedb = core
			@running = true
			update_vaults(vault_list)
		end

		# The main loop to run in a thread
		def main
			last = Time.new
			while @running

				sleep(Reedb::DEBOUNCE_DELTA)
			end
		end

		# Updates the vault instances every time the vault set changes. Only changes values
		# for vaults that change and attempts to leave old vaults unchanged.
		#
		# @param [List] vault_list of vault UUIDs that get assigned idle times.
		#
		def update_vaults(vault_list)
			@vaults = vault_list
		end

		# This is called every time an action is performed on a vault.
		def debounce_vault(vault_id)

		end

	end
end