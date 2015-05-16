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
require_relative '../reedb'

# HTTP handler class that registers the functions
# for the vault interface
#
class ReedbHandler < Sinatra::Base

	#
	# funct 		url 									descr
	#
	#CHECK#  GET 			/vaults 									List of all vaults
	#  GET 			/vaults/*vault-id*							Vault with ID

	#  GET 			/vaults/*vault-id*/files/*file-id*/body		Returns body of a file
	#  GET 			/vaults/*vault-id*/files/*file-id*/history	Returns history of a file (???)

	#  PUT 			/vaults/*vault-id*/files/					Create file
	#  POST 		/vaults/*vault-id*/files/*file-id*			Update file contents

	#  PUT 			/vaults										Create new vault
# DELETE 		/vaults/*vault-id*							Deletes a vault
	#

	configure :production, :development do
		enable :logging
	end

	# Require authentication to access basic  vault functionality.
	get '/vaults' do
		return Reedb::available_vaults.to_json
	end

	put '/new_vault' do
		# title = params['title']
		return "#{params}"
		#name = params['name']
		#path = params['path']
		#passprase = params['passphrase']
		#encryption = params['encryption']
		return "Hello World"
		#return "#{name}, #{path}, #{passphrase}, #{encryption}"
		# Reedb::create_vault(name, path, passphrase, encryption = :auto)
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

options = {}

# Setting default options
options[:pw_length] = 12
options[:verbose] = false
options[:daemon] = true
options[:port] = Reedb::NET_PORT
options[:os] = Reedb::Utilities::parse_os
options[:path] = Reedb::MASTER_PATH

#create parsers
opts = OptionParser.new
opts.on('-l', '--pw-length INTEGER') { |o| options[:pw_length] = o }
opts.on('-p', '--port INTEGER') { |o| options[:port] = o }
opts.on('-v', '--verbose') { options[:verbose] = true }
opts.on('-d', '--no-daemon') { options[:daemon] = false }
opts.on('-a', '--app-path STRING') { |o| options[:path] = o }
opts.parse! unless ARGV == []

# This creates the Reedb module and binds it to a variable to be interacted with in the future
Reedb::init({:os => options[:os], :pw_length => options[:pw_length], 
	:daemon => options[:daemon], :verbose => options[:verbose], :path => options[:path]})
# Next up we start the HTTP server and that's that. We're up and running :)
Rack::Handler::WEBrick.run(ReedbHandler.new, {:Port => options[:port], :BindAddress => "localhost"})