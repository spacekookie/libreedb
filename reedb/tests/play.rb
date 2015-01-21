require 'aes'

k = AES.key
data = AES.encrypt(k, "this_is_my_mega_secure_penis_i_mean_password_yo_bitch")
File.open("readme", "wb").write(Base64.encode64(data))