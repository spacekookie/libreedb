# ====================================================
# Copyright 2015 Random Robot Softworks (see @author)
# @author: Katharina Sabel | www.2rsoftworks.de
#
# Distributed under the GNU Lesser GPL Version 3
# (See accompanying LICENSE file or get a copy at
# 	https://www.gnu.org/licenses/lgpl.html)
# ====================================================

require_relative 'utils/version'

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

		# Default constructor for a datafile.
		#
		def initialize(name, vault)
			@version = Version.new()
			@vault = vault
			@name = name
			construct_path(@name, @vault)
			fill_file
		end


		def insert(f, master = :body)
			@version.update # => Updates the version of the file
			@dataset['body']["#{version}"]['note'] = f if f.is_a?(String)

			f.each do |parent, child|
				if master == :header
					@dataset['header']["#{version}"] = {}
					@dataset['header']["#{version}"]["#{parent}"] = "#{child}"
				elsif master == :body
					@dataset['body']["#{version}"] = {}
					@dataset['body']["#{version}"]["#{parent}"] = "#{child}"
				else
					puts "Error: Unknown master type."
				end
			end
		end

		# Has two modes. In soft-mode it schedules a sync with the file system.
		# In hard-mode (default) it force syncs itself with the filesystem by itself
		#
		def sync mode
			@mode = :hard if mode == nil

		end

		# Closes the file on the file system and dumps the header reference from the
		# containing vault.
		#
		def close

		end

		private

		def fill_file
			@dataset = {}
			@dataset['header'] = {}
			@dataset['body'] = {}
			@dataset['header']['name'] = @name
		end

		def construct_path(name, vault)
			base_path = Reedb::Utilities::append_to_path("#{vault.path}", "data")
			@path = "#{Reedb::Utilities::append_to_path(base_path, name)}"
		end
	end
end