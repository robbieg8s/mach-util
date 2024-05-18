mach:

test: mach
	- ./mach
	./mach absolute_time
	./mach approximate_time
	./mach continuous_approximate_time
	./mach continuous_time
	./mach timebase_info
