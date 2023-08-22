# Definition of the "object format"
## Sketch:
```
LINK  
nr_segs nr_syms nr_rels  
-- segments --  
-- symbols --  
-- rels --  
-- data --  
```

# Description
The first line is the ‘‘magic number,’’ the word LINK.  

The second line contains at least three decimal numbers, the number of  
segments in the file, the number of symbol table entries, and the number of  
relocation entries. There may be other information after the three numbers  
for extended versions of the linker. If there are no symbols or relocations,  
the respective number is zero.

## Segment definition
Each segment definition contains the segment name, the address where the  
segment logically starts, the length of the segment in bytes, and a  
string of code letters describing the segment. Code letters include R for  
readable, W for writable, and P for present in the object file.  
(Other letters may be present as well.) A typical set of segments for an  
a.out like file would be:

```
.text 0x1000 0x2500 RP
.data 0x4000 0xC00 RWP
.bss 0x5000 0x1900 RW
```

Segments are numbered in the order their definitions appear, with the first  
segment being number 1.

# Symbol table
Next comes the symbol table. Each entry is of the form:

```
name value seg type
```

The name is the symbol name. The value is the hex value of the symbol.  
Seg is the segment number relative to which the segment is defined, or 0  
for absolute or undefined symbols. The type is a string of letters including  
D for defined or U for undefined. Symbols are also numbered in the order  
they’re listed, starting at 1.  

# Relocations
Have the form:

```
loc seg ref type ...
```

Loc is the location to be relocated, seg is the segment within which the  
location is found, ref is the segment or symbol number to be relocated there,  
and type is an architecture-dependent relocation type. Common types are:  
- A4 for a four-byte absolute address.
- R4 for a four-byte relative address.

Some relocation types may have extra fields after the type.  
Following the relocations comes the object data. The data for each segment  
is a single long hex string followed by a newline. Each pair of hex digits  
represents one byte. The segment data strings are in the same order as the  
segment table, and there must be segment data for each ‘‘present’’ segment.  
The length of the hex string is determined by the the defined length of the  
segment; if the segment is 100 bytes long, the line of segment data is 200  
characters, not counting the newline at the end.