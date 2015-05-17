# ====================================================
# Copyright 2015 Lonely Robot (see @author)
# @author: Katharina Sabel | www.2rsoftworks.de
#
# Distributed under the GNU Lesser GPL Version 3
# (See accompanying LICENSE file or get a copy at
# 	https://www.gnu.org/licenses/lgpl.html)
# ====================================================

# This is a wrapper script that gets called by the reedb daemon to init the
# HTTP stack and Reedb modules.
# DO NOT USE THIS FILE WHEN USING REEDB AS A GEM DEPENDENCY!
# (unless you know what you're doing...)

# System requirements (HTTP stuff)
require 'optparse'
require 'sinatra'
require 'rack'

# Reedb requirements
require 'reedb'

# HTTP handler class that registers the functions
# for the vault interface
#
class ReedbHandler < Sinatra::Base

	# 	funct 	url 										descr
	#
	# 	 GET 	/vaults 									List of all vaults
	# 	 PUT 	/vaults 									Create a new vault.
	# 	 PUT 	/vaults/scope 								Scope a vault that already exists

	# 	 POST 	/vaults/*vault-id*/request_token			Auth for vault with ID
# [AUTH] POST 	/vaults/*vault-id*/headers					Return vault headers
# [AUTH] POST 	/vaults/*vault-id*/close					Close vault with ID

# [AUTH] POST 	/vaults/*vault-id*/files/*file-id*			Returns body of a file
# [AUTH] POST 	/vaults/*vault-id*/files/*file-id*/history	Returns history of a file (???)

# [AUTH] PUT 	/vaults/*vault-id*/files					Create file
# [AUTH] POST 	/vaults/*vault-id*/files/*file-id*			Update file contents
# [AUTH] POST 	/vaults/*vault-id*/files/*file-id*/remove	Removes a file

	configure :production, :development do
		enable :logging
	end

	def build_response(status_code, message, payload = nil)
		status status_code # Set http status code
		content_type 'application/json'
		response = { 'success' => (status_code >= 400 ? false : true),
					 'message' => message,
					 'payload' => payload }
		return response.to_json
	end

	# Returns a list of vaults scoped on the system
	get '/vaults' do
		payload = Reedb::Vault::available_vaults
		return build_response(200, "Currently scoped vaults", payload)
	end

	# Create a new vault on the system
	put '/vaults' do

		# If request was garbage
		unless request.content_type == 'application/json'
	   		return build_response(400, 'Data was malformed. Expects JSON!')
		end

		# Check if the JSON data
		data = nil
		begin
			data = JSON.parse(request.body.read)
		rescue
			return build_response(400, 'JSON data was malformed!')	
		end

		name = data["name"] if data["name"]
		path = data["path"] if data["path"]
		passphrase = data["passphrase"] if data["passphrase"]
		encryption = :auto # TODO: Handle this better!

		# This gets fired if not all neccesary information was provided.
		if name == nil && path == nil && passphrase == nil
			return build_response(400, 'Required data fields are missing from JSON data body!')	
		end

		# Now deal with the actual stuff
		token = nil

		# Catches ALL possible errors that can occur during this operation!
		begin
			token = Reedb::Vault::create_vault(name, path, passphrase, encryption)
		rescue InsecureUserPasswordError => e
			return build_response(400, e.message)

		rescue VaultExistsAtLocationError => e
			return build_response(409, e.message)

		rescue VaultWritePermissionsError,
			   VaultMissingConfigurationError,
			   VaultLoggerError,
			   BadCacheError,
			   EncryptionFailedError,
			   DecryptionFailedError => e

			# Bundled error return
			return build_response(500, e.message)
		end

		return build_response(201, "Vault was successfully crated at location", token)
	end

	# Scope a new vault on the system.
	put '/vaults/scope' do
		# If request was garbage
		unless request.content_type == 'application/json'
			return build_response(400, 'Data was malformed. Expects JSON!')
		end

		# Check if the JSON data
		data = nil
		begin
			data = JSON.parse(request.body.read)
		rescue
			return build_response(400, 'JSON data was malformed!')	
		end

		name = data["name"] if data["name"]
		path = data["path"] if data["path"]

		if name == nil && path == nil
			return build_response(400, 'Required data fields are missing from JSON data body!')	
		end

		begin
			Reedb::Vault::scope_vault(name, path)
		rescue VaultAlreadyScopedError, VaultDoesNotExistError => e
			return build_response(400, e.message)
		end

		# If everything went well
		return build_response(200, "Vault successfully scoped. It is now available to load.")
	end

	put '/vaults/unscope' do
		# If request was garbage
		unless request.content_type == 'application/json'
			return build_response(400, 'Data was malformed. Expects JSON!')
		end

		# Check if the JSON data
		data = nil
		begin
			data = JSON.parse(request.body.read)
		rescue
			return build_response(400, 'JSON data was malformed!')	
		end

		name = data["name"] if data["name"]
		path = data["path"] if data["path"]

		if name == nil && path == nil
			return build_response(400, 'Required data fields are missing from JSON data body!')	
		end

		begin
			Reedb::Vault::scope_vault(name, path)
		rescue VaultNotScopedError => e
			return build_response(400, e.message)
		end

		# If everything went well
		return build_response(200, "Vault successfully unscoped and will not show up in vault lists anymore.")
	end 

	#  Request a token for a vault
	post '/vaults/*/request_token' do
		vault_uuid = params[:splat][0]

		puts vault_uuid

		if vault_uuid == nil
			return build_response(400, 'Missing vault access id.')
		end

		# If request was garbage
		unless request.content_type == 'application/json'
			return build_response(400, 'Data was malformed. Expects JSON!')
		end

		# Check if the JSON data
		data = nil
		begin
			data = JSON.parse(request.body.read)
		rescue
			return build_response(400, 'JSON data was malformed!')	
		end

		passphrase = data["passphrase"] if data["passphrase"]
		permanent = false # TODO: Implement this!

		if passphrase == nil && permanent == nil
			return build_response(400, 'Required data fields are missing from JSON data body!')	
		end

		token = nil
		begin
			token = Reedb::Daemon::request_token(vault_uuid, passphrase)
		rescue VaultNotScopedError, VaultDoesNotExistError => e
			return build_response(404, e.message)

		rescue WrongUserPasswordError => e
			return build_response(401, e.message)			
		end

		return build_response(200, "Access successfully granted for vault", token)
	end

	post '/vaults/*/free_token' do
		vault_uuid = params[:splat][0]

		unless vault_uuid 
			return build_response(400, 'Missing vault access id.')
		end

		# If request was garbage
		unless request.content_type == 'application/json'
			return build_response(400, 'Data was malformed. Expects JSON!')
		end

		# Check if the JSON data
		data = nil
		begin
			data = JSON.parse(request.body.read)
		rescue
			return build_response(400, 'JSON data was malformed!')	
		end

		token = data["token"] if data["token"]

		if token == nil
			return build_response(400, 'Required data fields are missing from JSON data body!')	
		end

		begin
			Reedb::Daemon::free_token(token)
		rescue
			return build_response(400, 'Token unknown to Reedb. Can not free it!')
		end

		return build_response(200, "Token successfully freed")
	end

	# [AUTH] Close vault with id + token
	post '/vaults/*/close' do
		vault_uuid = params[:splat][0]

		unless vault_uuid 
			return build_response(400, 'Missing vault access id.')
		end

		# If request was garbage
		unless request.content_type == 'application/json'
			return build_response(400, 'Data was malformed. Expects JSON!')
		end

		# Check if the JSON data
		data = nil
		begin
			data = JSON.parse(request.body.read)
		rescue
			return build_response(400, 'JSON data was malformed!')	
		end

		token = data["token"] if data["token"]
		
		unless token
			return build_response(400, 'Required data fields are missing from JSON data body!')	
		end

		begin
			Reedb::Vault::close_vault(vault_uuid, token)
		
		rescue VaultNotAvailableError => e
			return build_response(404, e.message)

		rescue UnknownTokenError => e
			return build_response(401, e.message)

		rescue UnautherisedTokenError => e
			return build_response(403, e.message)

		rescue Exception => e
			return build_response(500, "An errror occured. #{e.message}")
		end

		return build_response(200, "Vault #{vault_uuid} successfully closed.")

	end

	# [AUTH] Request headers for a vault with token/ id
	post '/vaults/*/headers' do
		vault_uuid = params[:splat][0]

		if vault_uuid == nil 
			return build_response(400, 'Missing vault access id.')
		end

		# If request was garbage
		unless request.content_type == 'application/json'
			return build_response(400, 'Data was malformed. Expects JSON!')
		end

		# Check if the JSON data
		data = nil
		begin
			data = JSON.parse(request.body.read)
		rescue
			return build_response(400, 'JSON data was malformed!')	
		end

		token = data["token"] if data["token"]
		search = data["search"] if data["search"]

		if token == nil
			return build_response(400, 'Required data fields are missing from JSON data body!')	
		end

		headers = nil
		begin
			headers = Reedb::Vault::access_headers(vault_uuid, token, search)
		
		rescue VaultNotAvailableError => e
			return build_response(404, e.message)

		rescue UnknownTokenError => e
			return build_response(401, e.message)

		rescue UnautherisedTokenError => e
			return build_response(403, e.message)

		rescue Exception => e
			return build_response(500, "An errror occured. #{e.message}")
		end

		return build_response(200, "Headers for vault #{vault_uuid}", headers)
	end

	# [AUTH] Return body of a file
	post '/vaults/*/files/*' do
		vault_uuid = params[:splat][0]
		file_id = params[:splat][1]

		unless vault_uuid 
			return build_response(400, 'Missing vault access id.')
		end

		# If request was garbage
		unless request.content_type == 'application/json'
			return build_response(400, 'Data was malformed. Expects JSON!')
		end

		# Check if the JSON data
		data = nil
		begin
			data = JSON.parse(request.body.read)
		rescue
			return build_response(400, 'JSON data was malformed!')	
		end

		token = data["token"].delete!("\n") if data["token"]

		puts "#{token}\n"
		puts "#{Reedb::Config::Master::dump_config}\n"

		unless token
			return build_response(400, 'Required data fields are missing from JSON data body!')	
		end

		file = nil
		begin
			file = Reedb::Vault::access_file(vault_uuid, file_id, token, false)
		rescue FileNotFoundError => e
			return build_response(404, e.message)

		rescue VaultNotAvailableError => e
			return build_response(404, e.message)

		rescue UnknownTokenError => e
			return build_response(401, e.message)

		rescue UnautherisedTokenError => e
			return build_response(403, e.message)

		end
		return build_response(200, "File read without version history", file)
	end

	# [AUTH] Return history of a file
	post '/vaults/*/files/*/history' do
		vault_uuid = params[:splat][0]
		file_id = params[:splat][1]

		unless vault_uuid 
			return build_response(400, 'Missing vault access id.')
		end

		# If request was garbage
		unless request.content_type == 'application/json'
			return build_response(400, 'Data was malformed. Expects JSON!')
		end

		# Check if the JSON data
		data = nil
		begin
			data = JSON.parse(request.body.read)
		rescue
			return build_response(400, 'JSON data was malformed!')	
		end

		token = data["token"].delete!("\n") if data["token"]

		puts "#{token}\n"
		puts "#{Reedb::Config::Master::dump_config}\n"

		unless token
			return build_response(400, 'Required data fields are missing from JSON data body!')	
		end

		file = nil
		begin
			file = Reedb::Vault::access_file(vault_uuid, file_id, token, true)
		rescue FileNotFoundError => e
			return build_response(404, e.message)

		rescue VaultNotAvailableError => e
			return build_response(404, e.message)

		rescue UnknownTokenError => e
			return build_response(401, e.message)

		rescue UnautherisedTokenError => e
			return build_response(403, e.message)

		end
		return build_response(200, "File read with version history", file)
	end

	# [AUTH] Creates a new file with data
	put '/vaults/*/files' do
		vault_uuid = params[:splat][0]

		unless vault_uuid 
			return build_response(400, 'Missing vault access id.')
		end

		# If request was garbage
		unless request.content_type == 'application/json'
			return build_response(400, 'Data was malformed. Expects JSON!')
		end

		# Check if the JSON data
		data = nil
		begin
			data = JSON.parse(request.body.read)
		rescue
			return build_response(400, 'JSON data was malformed!')	
		end

		token = data["token"] if data["token"]
		name = data["name"] if data["name"]
		file_data = data["data"] if data["data"]

		unless token && name && file_data
			return build_response(400, 'Required data fields are missing from JSON data body!')	
		end

		headers = Reedb::Vault::access_headers(vault_uuid, token, nil)
		unless headers.include?(name)
			return build_response(400, "File already exists. Use update POST instead.")
		end

		response = nil
		begin
			response = Reedb::Vault::insert(vault_uuid, token, name, file_data)
		rescue VaultNotAvailableError => e
			return build_response(404, e.message)
			
		rescue UnknownTokenError => e
			return build_response(401, e.message)
			
		rescue UnautherisedTokenError => e
			return build_response(403, e.message)
			
		rescue FileBusyError => e
			return build_response(418, "Dont take this error code too seriously: #{e.message}")
		end

		return build_response(200, "File successfully created!")
	end

	# [AUTH] Update file contents
	post '/vaults/*/files/*' do
		vault_uuid = params[:splat][0]
		file_name = params[:splat][1]

		unless vault_uuid 
			return build_response(400, 'Missing vault access id.')
		end

		# If request was garbage
		unless request.content_type == 'application/json'
			return build_response(400, 'Data was malformed. Expects JSON!')
		end

		# Check if the JSON data
		data = nil
		begin
			data = JSON.parse(request.body.read)
		rescue
			return build_response(400, 'JSON data was malformed!')	
		end

		token = data["token"] if data["token"]
		file_data = data["data"] if data["data"]

		unless token && file_data
			return build_response(400, 'Required data fields are missing from JSON data body!')	
		end

		begin
			Reedb::Vault::insert(vault_uuid, token, file_name, file_data)

		rescue VaultNotAvailableError => e
			return build_response(404, e.message)
			
		rescue UnknownTokenError => e
			return build_response(401, e.message)
			
		rescue UnautherisedTokenError => e
			return build_response(403, e.message)
			
		rescue FileBusyError => e
			return build_response(418, "Dont take this error code too seriously: #{e.message}")
		end

		return build_response(200, "File successfully updated!")
	end

	# [AUTH] Removes a file
	post '/vaults/*/files/*/remove' do
		vault_uuid = params[:splat][0]
		file_name = params[:splat][1]

		unless vault_uuid 
			return build_response(400, 'Missing vault access id.')
		end

		# If request was garbage
		unless request.content_type == 'application/json'
			return build_response(400, 'Data was malformed. Expects JSON!')
		end

		# Check if the JSON data
		data = nil
		begin
			data = JSON.parse(request.body.read)
		rescue
			return build_response(400, 'JSON data was malformed!')	
		end

		token = data["token"] if data["token"]

		begin
			Reedb::Vault::remove(vault_uuid, token, file_name)
			
		rescue FileNotFoundError, VaultNotAvailableError => e
			return build_response(404, e.message)

		rescue UnknownTokenError => e
			return build_response(401, e.message)

		rescue UnautherisedTokenError => e
			return build_response(403, e.message)
		end

		return build_response(200, "File successfully deleted.")
	end
end

options = {}

# Setting default options
options[:pw_length] = 12
options[:verbose] = false
options[:daemon] = true
options[:port] = Reedb::NET_PORT
options[:os] = Reedb::Utilities::parse_os
options[:path] = Reedb::DEF_MASTER_PATH

#create parsers
opts = OptionParser.new
opts.on('-l', '--pw-length INTEGER') { |o| options[:pw_length] = o }
opts.on('-p', '--port INTEGER') { |o| options[:port] = o }
opts.on('-v', '--verbose') { options[:verbose] = true }
opts.on('-d', '--no-daemon') { options[:daemon] = false }
opts.on('-a', '--app-path STRING') { |o| options[:path] = o }
opts.parse! unless ARGV == []

# This creates the Reedb module and binds it to a variable to be interacted with in the future
Reedb::Core::init({:os => options[:os], :pw_length => options[:pw_length], 
	:daemon => options[:daemon], :verbose => options[:verbose], :path => options[:path]})
# Next up we start the HTTP server and that's that. We're up and running :)
Rack::Handler::WEBrick.run(ReedbHandler.new, {:Port => options[:port], :BindAddress => "localhost"})