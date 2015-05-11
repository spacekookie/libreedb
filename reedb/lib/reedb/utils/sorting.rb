# ====================================================
# Copyright 2015 Lonely Robot (see @author)
# @author: Katharina Sabel | www.2rsoftworks.de
#
# Distributed under the GNU Lesser GPL Version 3
# (See accompanying LICENSE file or get a copy at
#   https://www.gnu.org/licenses/lgpl.html)
# ====================================================

# Implements a rather neat and quick heapsort algorithm to sort values for database output.
#
class Array

	# Heapsorts a douplicate instance and leaves the original alone.
	#
	def heapsort
		self.dup.heapsort!
	end
 
	# Heapsorts that array instance.
	#
	def heapsort!
		# Heapify the array!
		((length - 2) / 2).downto(0) {|first| siftdown(first, length - 1)}
 
		(length - 1).downto(1) do |last|
			self[last], self[0] = self[0], self[last]
			siftdown(0, last - 1)
		end
		self
	end
 
	def siftdown(first, last)
		root = first
		loop do
			child = root * 2 + 1
			break if child > last
			if child + 1 <= last and self[child] < self[child + 1]
				child += 1
			end
			if self[root] < self[child]
				self[root], self[child] = self[child], self[root]
				root = child
			else
				break
			end
		end
	end
end