# ====================================================
# Copyright 2015 Random Robot Softworks (see @author)
# @author: Katharina Sabel | www.2rsoftworks.de
#
# Distributed under the GNU Lesser GPL Version 3
# (See accompanying LICENSE file or get a copy at
# 	https://www.gnu.org/licenses/lgpl.html)
# ====================================================

module Reedb

	class VaultConfig

		# Takes the containing vault as a parameter
		# 
		def initialize(master)
			@secure_cypher = ""
			@head = {}
			@body = {}
		end

	end

end