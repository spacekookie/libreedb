require_relative "reedb/constants"
require_relative "reedb/reevault"

module Reedb
	def self.vault(name='auto_fill', path)
		return ReeVault.new(name, path)
	end

	def self.test()
		puts "Test"
	end
end

Reedb.vault(name='cool_name', path='~/desktop/').create()
Reedb.vault(path='~/desktop/cool_name').load('secure password')