[ ] 	Keep track of .text, .data, and .bss segments 
	as separate pointers inside linker struct.

[ ] 	in linker.c/linker_save calculate header size and write it after
	magic number.

[ ] 	Do additional error handling for the parse stages.
	Things like size checking and whatever.
	also free unused memory.
	the .bss does not have any data and needs to be
	allocated by the linker, i will not really check for 
	junk in the input file since the file could be linked
	like this. but it's still a candidate for error checking.