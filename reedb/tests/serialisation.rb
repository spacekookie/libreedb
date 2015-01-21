require 'json'
require 'yaml'
require 'aes'

data = {}
data['header'] = {}
data['header']['header'] = 
data['header']['name'] = "Entry name"
data['header']['url'] = "www.entry.com"
data['header']['version'] = "1.1"

data['content'] = {}

data['content']['1.0'] = {}
data['content']['1.0']['username'] = 'spacekookie'
data['content']['1.0']['password'] = 'blume123'

data['content']['1.1'] = {}
data['content']['1.1']['reason'] = 'My reason to edit this entry was BEWBS!'
data['content']['1.1']['note'] = 'This is an example note'
data['content']['1.1']['password'] = '123brei'

@key = AES.key

rounds = 10

start_json = Time.now

rounds.times do |x|
	json_data = data.to_json
	encrypted_json = AES.encrypt(json_data, @key)
	File.open("json#{x}.ree", "wb").write(Base64.encode64("#{encrypted_json}")) # => Using ASCII Armour
end
end_json = Time.now
puts "JSON ==> #{((end_json - start_json)) * 1000} ms"

start_yaml = Time.now

rounds.times do |x|
	yaml_data = data.to_yaml
	encrypted_yaml = AES.encrypt(yaml_data, @key)
	File.open("yaml#{x}.ree", "wb").write(Base64.encode64("#{encrypted_yaml}")) # => Using ASCII Armour
end
end_yaml = Time.now
puts "YAML ==> #{((end_yaml - start_yaml)) * 1000} ms"

#
# JSON IS A CLEAR!!! Winner!
#
