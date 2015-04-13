
# user_pw = "1234567890123"

# Reedb.init(:unix, 12) # => defines OS and minimal password length on vault
# path = File.expand_path('~/Desktop/reedb')

# # Default encryption is set to 'aes'
# begin
# 	Reedb.vault('default', "#{path}", :aes).load(user_pw)
# rescue VaultDoesNotExistError, VaultExistsAtLocationError => e
# 	puts e.message
# 	puts "If you think this is a bug, please report it <3"
# 	# exit
# end

# data = {
# 	'body'=>{
# 		'password'=>'mega_secure_password',
# 		'username'=>'spacekookie'
# 	}
# }

# data2 = {
# 	'body'=>{
# 		'password'=>'less_secure',
# 	}
# }

# Reedb.active_vaults['default'].insert('Peter pan', data)

# begin
# 	puts Reedb.active_vaults['default'].read_file('Peter pan')
# rescue
# 	puts "This error was handled: could not read!"
# end


# begin
# 	Reedb.active_vaults['default'].remove_file('Peter pan')
# rescue
# 	puts "This error was handled: could not remove!"
# end



# begin
# 	Reedb.vault(name='default', "#{path}", :aes).secure_config(true).create(user_pw)

# 	sample_data = {}
# 	sample_data['header'] = {}
# 	sample_data['header']['name'] = "Sample"
# 	sample_data['header']['category'] = "Unsorted"
# 	sample_data['body'] = {}
# 	sample_data['body']['username'] = "spacekookie"
# 	sample_data['body']['password'] = 'the_flying_unicorn_shits_rainbows'

# 	Reedb.active_vaults['default'].insert('Sample', sample_data)

# rescue
# 	puts "Error occured opening your vault. Does it exist?"
# end

# puts Reedb.active_vaults['default'].read_file('Peter Pan')
# Reedb.active_vaults['default'].close