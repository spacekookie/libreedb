# ====================================================
# Copyright 2015 Lonely Robot (see @author)
# @author: Katharina Sabel | www.2rsoftworks.de
#
# Distributed under the GNU Lesser GPL Version 3
# (See accompanying LICENSE file or get a copy at
# 	https://www.gnu.org/licenses/lgpl.html)
# ====================================================

require_relative 'utils/version'
require_relative 'security/encryption'

module Reedb
	class DataFile

		# Name of the datafile in clear.
		# To get the hashed variant use the path getter.
		#
		attr_reader :name

		# Path and hash of the file on the system. Used when caching files in a vault
		#
		attr_reader :path

		# The latest version of this data file. Contains a version numeral and
		# a last-changed timestamp to provide better history for 
		# multi-computer setups.
		#
		attr_reader :version

		# attr_reader :dataset

		# Default constructor for a datafile.
		#
		def initialize(name, vault, old_file = nil)
			if old_file
				@dataset = old_file
				@name = @dataset['header']['name']
				@version = Version.new(@dataset['header']['latest'])
				@vault = vault
			else
				@version = Version.new
				@vault = vault
				@name = name
				fill_file
			end
			construct_path(@name, @vault)
		end

		def insert(data, mode = :hard)
			# => Updates the version of the file if neccesary
			# puts (@dataset['body'][@version] == {})

			puts @dataset['body'][@version]
			curr_time = DateTime.now.strftime('%Q').to_i
			then_time = @version.timestamp
			# time_diff = @version.timestamp - curr_time
			time_dif = curr_time - then_time

			unless @dataset['body'][@version] == {}
				unless time_dif < 10000
					@version.update
				else
					puts "Not incrementing version because write cycle too short."
				end
			end

			data.each do |master, section|
				if master == 'body'
					@dataset[master]["#{@version}"] = {} unless @dataset[master]["#{@version}"]
				end

				section.each do |key, value|
					if master == 'header'
						@dataset[master][key] = value
						next
					end

					@dataset[master]["#{@version}"][key] = value
					@dataset['header']['latest'] = @version
				end
			end
			sync if mode == :hard
		end

		def cache mode
			return @dataset['header'] if mode == :header
			return @dataset if mode == :full
		end

		# Has two modes. In soft-mode it schedules a sync with the file system.
		# In hard-mode (default) it force syncs itself with the filesystem by itself after every insert.
		#
		def sync
			json_data = @dataset.to_json
			#puts json_data
			crypt_json = @vault.crypt.encrypt(json_data)

			tmp = File.open("#{@path}", "wb+")
			tmp.write(Base64.encode64("#{crypt_json}"))
			tmp.close
			VaultLogger.write("File #{@name} has been synced", 'debug')
		end

		# Closes the file on the file system and dumps the header 
		# reference from the containing vault.
		# Usually only called on vault close & lock.
		#
		def close
			sync
			remove_instance_variable(:@dataset)
			VaultLogger.write("Closing file #{name}", 'debug')
		end

		def to_s
			@dataset
		end

		private

		def fill_file
			@dataset = {}
			@dataset['header'] = {}
			@dataset['header']['name'] = @name
			@dataset['header']['latest'] = @version
			@dataset['body'] = {}
			# @version.update
			@dataset['body']["#{@version}"] = {}
			# @dataset['body']["#{@version}"]['name'] = @name
		end

		def construct_path(name, vault)
			@path = "#{vault.path}/data/#{SecurityUtils::tiger_hash(@name)}.ree"
		end
	end
end