module Reedb

	module Config
		module Master
			class << self
				def config
					puts "Master Config"
				end
			end
		end

		module Vault
			class << self
				def config
					puts "Vault Config"
				end
			end
		end
	end
end


Reedb::Config::Master::config
Reedb::Config::Vault::config