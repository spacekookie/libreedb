# coding: utf-8
lib = File.expand_path('./lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require_relative 'lib/reedb/constants'

Gem::Specification.new do |spec|
  spec.name          = "reedb"
  spec.version       = Reedb::VERSION
  spec.authors       = ["Lonely Robot"]
  spec.email         = ["sabel.katharina@gmail.com"]
  spec.summary       = %q{Ruby storage containers/ database that uses completely encrpypted files for maximum security.}
  spec.description   = %q{Ruby storage containers/ database. Uses Rinjdael, Twofish, ... to keep your data safe. Made for native and local application development. Spawns a system daemon that manages vaults that applications can talk to.}
  spec.homepage      = "https://github.com/tr-lonelyrobot/reedb/wiki"
  spec.license       = "LGPL v3.0"

  spec.files         = `git ls-files -z`.split("\x0")
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]

  spec.add_development_dependency "bundler", "~> 1.7"
  spec.add_development_dependency "rake", "~> 10.0"

  spec.add_dependency 'aes', '~> 0.5'
  spec.add_dependency 'twofish', '~> 1.0'
  spec.add_dependency 'digest-tiger', '~> 1.0'
  spec.add_dependency 'json', '~> 1.8'
  spec.add_dependency 'daemons', '~> 1.2'
  spec.add_dependency 'sinatra', '~> 1.4'
  spec.add_dependency 'hashids', '~> 1.0'
end
