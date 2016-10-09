#!/bin/bash

######################################################################
#
# (c) 2016 - This file is part of libreedb and thus licenced under
#               the LGPL v3.0 or (by your choice) later.
#
# This is a build script for libreedb and reedbd + extentions. It
#   will download dependencies and build the database library and
#   corresponsing daemon to manage extentions and sessions.
#
# Please feel free to use this script when creating packages for
#   either libreedb or reedbd.
#
# If you notice that distribution or os support is lacking, please
#   consider providing additional support via pull request
#
# Authors:
#     Katharina 'spacekookie' Sabel <sabel.katharina@gmail.com>
#
######################################################################

# Set some variables and modes
set -x
WIN="win"
OSX="osx"
LINUX="linux"

# Get the OS this script is run on. 
# Returns either 'WIN', 'OSX', 'LINUX', 'BSD'
#
function get_os {
}

# Can recognise different distributions.
# Returns either 'FEDORA', 'SUSE', 'DEBIAN', 'ARCH'.
#
function get_distribution {

}

if [[ get_os -eq "" ]]; then
  #statements
fi