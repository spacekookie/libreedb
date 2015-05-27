# ====================================================
# Copyright 2015 Random Robot Softworks (see @author)
# @author: Katharina Sabel | www.2rsoftworks.de
#
# Distributed under the GNU Lesser GPL Version 3
# (See accompanying LICENSE file or get a copy at
# 	https://www.gnu.org/licenses/lgpl.html)
# ====================================================

# These have to do with tokens being old, bad or missing.

class DaemonError < StandardError
end

# These are scoping errors
class VaultAlreadyScopedError < DaemonError
end

class VaultNotScopedError < DaemonError
end

# Not knowing a vault
class VaultNotAvailableError < DaemonError
end

# Token errors
class UnknownTokenError < DaemonError
end

class UnautherisedTokenError < DaemonError
end

class MissingTokenError < DaemonError
end