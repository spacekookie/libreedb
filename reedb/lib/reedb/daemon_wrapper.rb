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
require 'sinatra'
require 'rack'

# Reedb requirements
require_relative 'reedb'

# Registers global options handle for the Reedb module
@@options = {}
@@reedb = nil

# HTTP handler class that registers the functions
# for the vault interface
#
class ReedbHandler < Sinatra::Base
	
	configure :production, :development do
		enable :logging
	end

	# Require authentication to access basic  vault functionality.
	get '/vaults' do
	end

	post '/new_vault' do

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

## ENTRY HOOK FOR THE DAEMON
if __FILE__ == $PROGRAM_NAME

	# Setting default options
	@@options[:pw_length] = 12
	@@options[:verbose] = false
	@@options[:daemon] = true
	@@options[:port] = Reedb::NET_PORT
	@@options[:os] = Reedb::parse_os


	#create parsers
	opts = OptionParser.new()
	opts.on('-l', '--pw-length INTEGER') { |o| @@options[:pw_length] = o }
	opts.on('-p', '--port INTEGER') { |o| @@options[:port] = o }
	opts.on('-v', '--verbose') { @@options[:verbose] = true }
	opts.on('-d', '--no-daemon') { @@options[:daemon] = false }

	opts.parse! unless ARGV == []
	
	# This creates the Reedb module and binds it to a variable to be interacted with in the future
	@@reedb = Reedb::init({:os => @@options[:os], :pw_length => @@options[:pw_length], 
		:daemon => @@options[:daemon], :verbose => @@options[:verbose]})

	# Next up we start the HTTP server and that's that. We're up and running :)
	Rack::Handler::WEBrick.run(ReedbHandler.new, {:Port => @@options[:port], :BindAddress => "localhost"})
end

