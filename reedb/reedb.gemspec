# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'reedb/constants'

Gem::Specification.new do |spec|
  spec.name          = "reedb"
  spec.version       = Reedb::VERSION
  spec.authors       = ["Random Robot Softworks"]
  spec.email         = ["katharina.sabel@2rsoftworks.de"]
  spec.summary       = %q{Ruby filebased database using encryption from it's core. Used to power reepass}
  spec.description   = %q{Ruby database. Uses json and yaml files for data storage. Uses aes and twofish for encryption. Sha256 and Tiger2/192 for hashing. Encprytion from the core.}
  spec.homepage      = ""
  spec.license       = "MIT"

  spec.files         = `git ls-files -z`.split("\x0")
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]

  spec.add_development_dependency "bundler", "~> 1.7"
  spec.add_development_dependency "rake", "~> 10.0"
end
