require 'base64'

path = '/home/spacekookie/Desktop/file.txt'
data = 'wmkDuSa9OrV17aXZ0o0v+A==$bCZbwv8LccapjKxkuSADPxGBNA0nhw5wjXA+thUNer2ENrliYnY2A+aHhVdXJbli'

puts 'YES!'

File.write(path, Base64.encode64(data))

after = Base64.decode64(File.read(path))

puts data == after