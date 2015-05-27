# ====================================================
# Copyright 2015 Random Robot Softworks (see @author)
# @author: Katharina Sabel | www.lonelyrobot.io
#
# Distributed under the GNU Lesser GPL Version 3
# (See accompanying LICENSE file or get a copy at
# 	https://www.gnu.org/licenses/lgpl.html)
# ====================================================

# Some errors code that are too generic to go into either of the categories.
# They just apply to EVERYTHING!

class ReedbError < StandardError
end

# General Error
class FunctionNotImplementedError < ReedbError
end

class UnknownOSError < ReedbError
end