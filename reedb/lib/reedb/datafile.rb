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

			# The header set is defined by the vault and specifies the fields that can
			# be specified in the header part of a file.
			construct_path(@name, @vault)
		end

		# New and improved data insetion function!
		# Data needs to have a certain format
		# {
		# 	'header': {
		# 		...
		# 	},
		# 	'body' {
		# 	...
		# 	}
		# }
		#
		# Malformed data will be ignored and reported in the vault logging.
		#
		# Writing into body is farely straight forward. A value needs a field and is
		# assigned a version automatically depending on the time in the cache cycle
		# of the file
		#
		# Writing to header is much more interesting.
		# There is a header_set defined in the parent vault that specifies what fields
		# are valid and what types they allow. For example, by default, there is a 'urls'
		# field that is a list of urls.
		#
		# WRITING THE SAME VALUE INTO A FIELD AS EXISTS BEFORE WILL RESET THAT VALUE!
		# That's how you can delete a field with this same function in the same step as
		# adding new information (I know...hacky. But clever)
		#
		# For further documentation on this function, please check with the Reedb wiki
		#
		# Params:
		# 			data => specifically formatted JSON data set (see docs)
		# 			mode => overwrite default file/ vault caching mode.
		#
		def insertv2(data, mode = :hard)
			sync if mode == :hard
			# Add option for :fast sync mode here at some point

			# First split up the data and check what's actually there
			to_head = data['header'] if data['header'] else false end
			to_body = data['body'] if data['body'] else false end

			# Returns an error code for malformed data
			return FILE_MALFORMED_DATA_ERROR unless to_body && to_head

			# Gets the time difference between edits.
			# If it's not big enough (5 seconds) will not increment version number
			current_time = DateTime.now.strftime('%Q').to_i
			old_time = @version.timestamp
			version_needs_changing = (current_time - old_time >= FILE_CACHE_TIME) && to_body

			# Actually updates the version if neccessary
			@version.update if version_needs_changing

			# Now make sure the version branch actually exists
			@dataset['body']["#{@version}"] = {} unless @dataset['body']["#{@version}"]

			# Update the latest version pointer in the dataset
			@dataset['header']['latest'] = "#{@version}" if version_needs_changing

			if to_head
				to_head.each do |key, value|
					@dataset['header']['name'] if key == 'name'
					
					# This means that the value can be stored
					if vault.header_set.include?(key)
						
						# If inserting into a header single field
						if vault.header_set[key] == 'single'
							if @dataset['header'][key]
								@dataset['header'][key] = nil
							else
								@dataset['header'][key] = value
							end

						# If inserting into a header list
						elsif vault.header_set[key] == 'list'
							if @dataset['header'][key].include?(value)
								@dataset['header'][key].delete(value)
							else
								@dataset['header'][key] << value
							end

						# If inserting into a header tree
						elsif vault.header_set[key] == 'tree'
							if @dataset['header'][key].include?(value)
								@dataset['header'][key].delete(value)
							else
								@dataset['header'][key][value[0]] = value[1]
							end
						end
					else
						# Report the incident
						VaultLogger.write("Tried to write invalid data to #{@name}", 'warn')
					end
				end
			end

			#  BOOORING! :C
			if to_body
				to_body.each do |field, value|
					@dataset['body']["#{@version}"]["#{field}"] = value
				end
			end
		end

		#
		#
		#

		def insert(data, mode = :hard)
			puts "[LEGACY MODE]: This function has been depreciated. Use 'insertv2' instead!"
			VaultLogger.write("[LEGACY MODE]: Using broken function. Aborting action!", 'error')
			return

			# => Updates the version of the file if neccesary
			# puts (@dataset['body'][@version] == {})

			# puts @dataset['body'][@version]
			curr_time = DateTime.now.strftime('%Q').to_i
			then_time = @version.timestamp
			# time_diff = @version.timestamp - curr_time
			time_dif = curr_time - then_time

			unless @dataset['body'][@version] == {}
				unless time_dif < 10000
					@version.update
				else
					# Does this really need an output??
					# puts "Not incrementing version because write cycle too short."
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
		#
		#
		def sync(mode = :hard)
			json_data = @dataset.to_json
			# puts json_data
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

		private

		def fill_file
			@dataset = {}
			@dataset['header'] = {}
			@dataset['header']['name'] = @name
			@dataset['header']['latest'] = @version

			# Adds the header fields that were specified by the parent vault here
			vault.header_set.each do |field, type|
				@dataset['header']["#{field}"] = nil if type == 'single'
				@dataset['header']["#{field}"] = [] if type == 'list'
				@dataset['header']["#{field}"] = {} if type == 'tree'
			end

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