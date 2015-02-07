# ====================================================
# Copyright 2015 Random Robot Softworks (see @author)
# @author: Katharina Sabel | www.2rsoftworks.de
#
# Distributed under the GNU Lesser GPL Version 3
# (See accompanying LICENSE file or get a copy at
# 	https://www.gnu.org/licenses/lgpl.html)
# ====================================================

require_relative 'utilities'

module Reedb
	class Version

		attr_reader :host, :timestamp, :numeral

		def initialize
			@host = host
			@timestamp = Reedb::Utilities::parse_host
			@numeral = [1, 0]
		end

		def update
			@timestamp = Timestamp.new
			@host == Reedb::Utilities::parse_host ? incr_last : incr_first
		end

		def to_s
			return "#{@host}_#{@numeral}_#{@timestamp}"
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