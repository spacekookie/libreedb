# ====================================================
# Copyright 2015 Lonely Robot (see @author)
# @author: Katharina Sabel | www.2rsoftworks.de
#
# Distributed under the GNU Lesser GPL Version 3
# (See accompanying LICENSE file or get a copy at
# 	https://www.gnu.org/licenses/lgpl.html)
# ====================================================

require 'socket'
require 'timeout'


module Reedb

	class Timestamp
		attr_accessor :utc

		def initialize
			@utc = Time.now.getutc
		end

		def < x
			return (self.utc < x.utc)
		end

		def > x
			return (self.utc > x.utc)
		end

		def == x
			return (self.utc == x.utc)
		end

		def to_s
			return "#{@utc}"
		end
	end

	class Utilities

		# Takes version string and increments it by 1. Does patch-minor and minor-major steps
		# TODO: Replace this with a RegEx to split the version string at the dots, and analise it's sub-elements.
		def self.increment_version(version)
			# <b>DEPRECATED:</b> Please use <tt>Version class</tt> instead.
			warn "[DEPRECIATED] `increment_version` is depreciated. Please use Version class instead."

			if version?(version) # Checks if it actually is version
				i = version[2].to_i

				tmp = version

				if i < 9
					i += 1
					tmp = version[0] + "." + i.to_s

				elsif i >= 9
					i = version[0].to_i
					i += 1
					tmp = i.to_s + "." + "0".to_s
				end

				return tmp
			end
		end

		# Normalises name to have the first letter be capital
		def self.capitalise_first(string)
			string[0].upcase + string.slice(1..-1)
		end

		#
		# Returns if the provided string is actually a version string.
		#  
		#
		# => I love you Ashley, stop looking in my source :')
		# TODO: Use RegEx to scan through ENTIRE string, not just mayor and minor version
		def self.version?(version)
			def is_i?(i)
				i.to_i.to_s == i
			end

			is_i?(version[0]) and is_i?(version[2]) ? true : false
		end

		# Fix the actual inputs (aka test on virtual machines)
		def self.parse_os
			platform = RUBY_PLATFORM
			if platform.end_with?('linux')
				return :linux
			elsif platform.end_with?('Windows')
				return :win
			elsif platform.end_with?('Mac OS X')
				return :osx
			end
		end

		def self.parse_host
			return "#{Socket.gethostname}"
		end

		# Returns user currently logged in. Not sure for what that will be used but hey!
		#
		def self.parse_user
			if parse_os == :win
				return ENV['USERNAME']
			else
				return ENV['USER']
			end
		end

		def self.check_port(port)
			begin
				Timeout::timeout(1) do
					begin
						s = TCPSocket.new('127.0.0.1', port)
						s.close
						return true
					rescue Errno::ECONNREFUSED, Errno::EHOSTUNREACH
						return false
					end
				end
			rescue Timeout::Error
				return false
			end
		end

		def self.get_time(only_date = false)
			time = Time.now
			val_h = "#{time.year}-#{'%02d' % time.month}-#{'%02d' % time.day}"
			val_t = "#{time.hour}:#{'%02d' % time.min}:#{'%02d' % time.sec}"

			# => TODO: Make this more Ruby-Like
			if only_date
				return "#{val_h}"
			else
				return "[#{val_h} #{val_t}]"
			end
		end
	end
end