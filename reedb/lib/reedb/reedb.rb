# ====================================================
# Copyright 2015 Lonely Robot (see @author)
# @author: Katharina Sabel | www.2rsoftworks.de
#
# Distributed under the GNU Lesser GPL Version 3
# (See accompanying LICENSE file or get a copy at
# 	https://www.gnu.org/licenses/lgpl.html)
# ====================================================

# Internal requirements
require_relative "utils/utilities"
require_relative "utils/logger"
require_relative "constants"
require_relative "reevault"

# System requirements
require 'optparse'
require 'json'

# Main Reedb module that handles the operation of vaults.
# This can be called from a gem dependency from another ruby app
# or used with the reedb daemon.
#
module Reedb
	class << self

		# Returns the platform/ architecture of the daemon and vault handler
		def archos() (return @@archos) end

		def daemon?() (return @@daemon) end

		def passlength() (return @@passlength) end

		def init(global_os, passlength = 8, d = true)
			@@daemon = d
			@@archos = global_os
			@@passlength = passlength

			@@vaults = {}

			# if global_os == :linux
			# 	parent_path = File.expand_path('~/.config/reedb/')
			# 	master_path = File.expand_path('/etc/reedb/')
			# 	log_path = File.expand_path('~/.config/reedb/logs')

			# else

			# 	# 
			# end
			# Reedb::DaemonLogger.setup("#{log_path}")

		end

		def active_vaults
			return @@vaults
		end

		# Default encryption type is 'AES'
		#
		def vault(name='default', path=nil, encrypt='auto_fill')
			@@vaults[name] = ReeVault.new(name, path, encrypt)
			return @@vaults[name]
		end
	end
end

# Entry hook for the daemon wrapper.
#
if File.basename($PROGRAM_NAME, '.rb') == "reedbd"

	# Setting default options
	options = {}
	options[:pw_length] = 12
	options[:verbose] = false
	options[:daemon] = true
	options[:port] = Reddb::NET_PORT

	#create parsers
	opts = OptionParser.new()
	#opts.banner = "Usage: #{NAME} [daemon options] -- [reedb options]\n\n\tAvailable [reedb options]:"

	opts.on('-l', '--pw-length INTEGER') { |o| options[:pw_length] = o }
	opts.on('-p', '--port INTEGER') { |o| options[:port] = o }
	opts.on('-v', '--verbose') { options[:verbose] = true }
	opts.on('-d', '--no-daemon') { options[:daemon] = false }
	opts.parse! unless ARGV == []

	puts options

	#Rack::Handler::WEBrick.run(ReedbHandler.new, {:Port => 55736, :BindAddress => "localhost"})
end

require 'sinatra'
require 'rack'


# HTTP handler class
#
class ReedbHandler < Sinatra::Base
	
	configure :production, :development do
		enable :logging
	end

	get '/vaults' do
		"List of vaults: 1 2 3"
	end

	get '/*/request_token' do
		vault = params['splat'][0]
	end

	get '/*/headers' do
		vault = params['splat'][0]
	end

	get '/*/*/body' do
		vault = params['splat'][0]
		file = params['splat'][1]
	  # params['password']
	end

	get '/*/*/history' do
		vault = params['splat'][0]
		file = params['splat'][1]
	end

	post '/insert' do
		# vault = params['splat'][0]
		# file = params['splat'][1]
		data = params['data']
		puts data
		return "SOMETHING AWESOME: #{data}"
	end
end

# puts "LALALA"

=begin
user_pw = "1234567890123"

Reedb.init(:unix, 12) # => defines OS and minimal password length on vault
path = File.expand_path('~/Desktop/reedb')

# Default encryption is set to 'aes'
begin
	Reedb.vault('default', "#{path}", :aes).load(user_pw)
rescue VaultDoesNotExistError, VaultExistsAtLocationError => e
	puts e.message
	puts "If you think this is a bug, please report it <3"
	# exit
end

data = {
	'header'=>{
		'url'=>'www.facebook.com',
		'category'=>'Web Login'
		},
	'body'=>{
		'password'=>'mega_secure_password',
		'username'=>'spacekookie'
	}
}

data2 = {
	'body'=>{
		'password'=>'less_secure',
	}
}=end


Reedb.active_vaults['default'].insert('facebook', data)

=begin
begin
	puts Reedb.active_vaults['default'].read_file('Peter pan')
rescue
	puts "This error was handled: could not read!"
end=end


=begin

begin
	Reedb.active_vaults['default'].remove_file('Peter pan')
rescue
	puts "This error was handled: could not remove!"
end
=end



# beg=begin

# in
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



=begin
puts Reedb.active_vaults['default'].read_file('facebook')
Reedb.active_vaults['default'].close
=end

# def setup_listener port
# 	@server = TCPServer.open(port)
# end

# loop {
#   client = server.accept
#   params = JSON.parse(client.gets)
#   Reedb::DaemonLogger.write(params)
# }