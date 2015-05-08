# ====================================================
# Copyright 2015 Lonely Robot (see @author)
# @author: Katharina Sabel | www.2rsoftworks.de
#
# Distributed under the GNU Lesser GPL Version 3
# (See accompanying LICENSE file or get a copy at
# 	https://www.gnu.org/licenses/lgpl.html)
# ====================================================

require_relative 'utilities'

module Reedb
	class Version1
		attr_reader :timestamp, :revision

		def initialize(existing = nil)

			# If a version is created from an existing pattern
			if existing
				data = existing.split('::')
				@revision = data[0].to_i
				@timestamp = data[1].to_i
				return self
			end

			# This creates a new version
			@revision = 1
			@timestamp = DateTime.now.strftime('%Q')
		end

		def update
			@revision += 1
			@timestamp = DateTime.now.strftime('%Q')
		end

		# This formatting is actually used in the dataset
		def to_s
			return "R#{@revision}::#{@timestamp}"
		end
	end

	# Has been depreciated in version 0.9.9 (of Reedb, not the class)
	# Too specific and broad usecase. Was replaced with the new Version
	# DO NOT USE!
	#
	class Version_spec

		attr_reader :host, :timestamp, :numeral

		def initialize(existing = nil)
			puts "DO NOT USE! Use 'Reedb::Version' INSTEAD!"
			if existing
				data = existing.split('_')
				@host = data[0]
				
				# Hack to make the version numeral work.
				@numeral = data[1].split('.')
				@numeral[0] = @numeral[0].to_i
				@numeral[1] = @numeral[1].to_i

				@timestamp = data[2]
				@timestamp = @timestamp.to_i
				return self
			end
			@host = Reedb::Utilities::parse_host
			@timestamp = DateTime.now.strftime('%Q')
			@numeral = [0, 0]
			@timestamp = @timestamp.to_i
		end

		def update
			@timestamp = DateTime.now.strftime('%Q')
			@host == Reedb::Utilities::parse_host ? incr_last : incr_first
			@host = Reedb::Utilities::parse_host
		end

		# def greater(other)
		# 	if @timestamp > other.timestamp
		# 		puts "This is greater"
		# 	else
		# 		puts "Other is greater"
		# 	end
		# end

		def to_s
			return [@host, @numeral.join('.'), @timestamp].join('_')
		end

		private

		def incr_first
			@numeral[0] += 1
			@numeral[-1] = 0
		end

		def incr_last
			@numeral[-1] += 1
		end
	end
end