set -e  # exit on error.
set -x  # echo commands.

echo "##################### PREPARING ENVIRONMENT BECAUSE FUCK TRAVIS! ######################"

# - libgcrypt-dev   # C crypto - secure RAM
# - libcryptopp-dev # C++ crypto - ciphers & keygen

# Installs the required version of libgpgerror for libgcrypt
wget --no-check-certificate https://gnupg.org/ftp/gcrypt/libgpg-error/libgpg-error-1.22.tar.bz2 -O gpgerror.tar.bz2
mkdir _my_gpgerror && tar xf gpgerror.tar.bz2 -C _my_gpgerror --strip 1
cd _my_gpgerror && ./configure && make && sudo make install


# Installs the required version of libgcrypt for the build
wget --no-check-certificate https://gnupg.org/ftp/gcrypt/libgcrypt/libgcrypt-1.7.0.tar.bz2 -O libgcrypt.tar.bz2
mkdir _my_gcrypt && tar xf libgcrypt.tar.bz2 -C _my_gcrypt --strip 1
cd _my_gcrypt && ./configure && make && sudo make install

# Installs the required version of libcrypto++ for the build
wget --no-check-certificate https://www.cryptopp.com/cryptopp563.zip -O cryptopp.zip
mkdir ./_my_cryptopp && unzip cryptopp.zip -d ./_my_cryptopp
cd ./_my_cryptopp && make && sudo make install

echo "########### LIBRARY PREPERATION DONE ###########"
