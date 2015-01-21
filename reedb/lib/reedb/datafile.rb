# ====================================================
# Copyright 2015 Random Robot Softworks (see @author)
# @author: Katharina Sabel | www.2rsoftworks.de
#
# Distributed under the GNU Lesser GPL Version 3
# (See accompanying LICENSE file or get a copy at
# 	https://www.gnu.org/licenses/lgpl.html)
# ====================================================

module Reedb
	class DataFile

		attr_reader :name

		attr_reader :path, :hash

		def initialize(name, parent, category, data)
			category = 'Unsorted' if category == nil
			data['note'] = 'Standard data entry' if data['note'] == nil

			# Then go to store the values.
			@name = name
			@hash = Reedb::SecurityUtils::sha256_hash("#{name}")
		end
	end
end