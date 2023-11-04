[ ] 	a generic list implementation is needed
	[ ] change the memcpy thing. the list must be a list of
	pointers and just store copies of the pointers there. 
	memcpy might be slow and anyway it is not needed.

[ ] 	create a better way to do debugging

[ ] 	in linker.c/linker_save calculate header size and write it after
	magic number.

[ ] 	Do additional error handling for the parse stages.
	Things like size checking and whatever.
	also free unused memory.
	the .bss does not have any data and needs to be
	allocated by the linker, i will not really check for 
	junk in the input file since the file could be linked
	like this. but it's still a candidate for error checking.

[ ] 	after most stuff is written, test performance and then try
	to remove some indirection levels and test again to see	
	if it provides a big improvement.