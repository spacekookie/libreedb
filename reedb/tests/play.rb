hash = { 'something' => 5, 'other' => 6 }

puts hash

hash.each { |key, val| hash.delete(key) if key == 'other' }

puts hash

# def user_code
# 	10.times do |x|
# 		puts 'I am user code'
# 		something_to_debounce
# 		sleep(1)
# 	end
# end
#
# class Debouncer
#
# 	attr_accessor :var
#
# 	def initialize
# 		@var = 0
# 	end
#
# 	def update
# 		@var += 1
# 	end
#
# 	def debouncer
# 		last = Time.new
# 		11.times do
# 			tmp = Time.new
# 			r_delta = tmp - last
# 			last =tmp
# 			puts "Debouncer no #{var}\nDelta: #{r_delta}"
# 			sleep(1.00001)
# 		end
# 	end
# end
#
# def something_to_debounce
# 	@bouncer.update
# end
#
# def init(options, &block)
# 	print options
#
# 	puts 'Ich bin Reedb setup code'
#
# 	@bouncer = Debouncer.new
#
# 	t2 = Thread.new { @bouncer.debouncer }
# 	t1 = Thread.new(&block)
#
# 	t2.join
# 	t1.join
#
# 	puts ' => Everything is dead'
# end
#
# init(0) { user_code }