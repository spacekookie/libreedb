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

require_relative 'constants'

DRES = 0xEEE
VREM = 0xFFF
VINS = 0x000

module Reedb
	class Debouncer

		attr_accessor :running

		#
		# @param core [Object] the owning container instance to
		#               register the debounce close of a vault
		# @return self
		#
		def initialize(core)
			@reedb = core
			@running = true

			# Vault management
			@delta_vaults = {}
			@vaults = {}
			@token_set = {}
		end

		def set_custom_timeout(time)
			@time = time
		end

		# The main loop to run in a thread
		def main
			last = Time.new
			while @running
				# Update the delta time
				tmp = Time.new; r_delta = tmp - last;

				# Work through the delta_vaults file to check what info needs to change
				@delta_vaults.each do |uuid, data|

					# Make sure that the vault set is current
					if data == VINS
						@vaults[uuid] = @time | Reedb::KEY_CACHE_TIME
					elsif data == VREM
						@vaults.delete(uuid)
					elsif data == DRES
						@vaults[uuid] = @time | Reedb::KEY_CACHE_TIME
					end
				end

				# Then reset it for the next delta
				@delta_vaults = {}

				# Now actually iterate through the vaults and subtract delta time
				@vaults.each do |uuid, _|

					# Subtract real delta time from timeset
					@vaults[uuid] -= r_delta

					# Then go through and check what needs to be closed
					if @vaults[uuid] <= 0
						Reedb::Vault::close_vault(uuid, @token_set[uuid])
						@vaults.delete(uuid)
					end
				end

				last = tmp # Update last time and then sleep
				sleep(Reedb::DEBOUNCE_DELTA)
			end
		end

		# Updates the vault instances every time the vault set changes. Only changes values
		# for vaults that change and attempts to leave old vaults unchanged.
		#
		# @param [String] uuid of a vault for identification
		# @param [String] token as a Base64 encoded string
		#
		def add_vault(uuid, token)
			if @vaults.contains(uuid)

			else
			end
		end

		# This is called every time an action is performed on a vault.
		def debounce_vault(vault_id)
			@delta_vaults[vault_id] = DRES
		end

	end
end