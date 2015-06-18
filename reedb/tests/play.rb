def someFunc
	10.times do |x|
		puts 'I am user code'
		sleep(1)
	end
end


def debouncer
	10.times do |x|
		puts "Debouncer no #{x}"
		sleep(1)
	end
end

def init(options, &block)
	puts 'Ich bin Reedb setup code'
	t2 = Thread.new { debouncer }
	t1 = Thread.new(&block)

	t2.join()
	t1.join()

	puts ' Everything is dead'
end

init(nil) { someFunc }