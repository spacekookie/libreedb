# ====================================================
# Copyright 2015 Lonely Robot (see @author)
# @author: Katharina Sabel | www.2rsoftworks.de
#
# Distributed under the GNU Lesser GPL Version 3
# (See accompanying LICENSE file or get a copy at
# 	https://www.gnu.org/licenses/lgpl.html)
# ====================================================

module Reedb

	# Handler class to map vaults in the reedb api to a set
	class MetaVault
		attr_accessor :name, :path, :size, :uuid

		def initialize(name, path, size, uuid)
			@name = name
			@path = path
			@size = size
			@uuid = uuid
		end

		# Used to compare vaults
		def includes?(name) return (@name == name) end
		def includes?(name, path) return (@name == name && @path == path) end
		def to_s() return "#{@name}@#{@path}, size: #{@size}" end
	end
end