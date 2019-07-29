	.data
	.align	2
	.globl	class_nameTab
	.globl	Main_protObj
	.globl	Int_protObj
	.globl	String_protObj
	.globl	bool_const0
	.globl	bool_const1
	.globl	_int_tag
_int_tag:	.word	3	
	.globl	_bool_tag
_bool_tag:	.word	4	
	.globl	_string_tag
_string_tag:	.word	5	

### String constant objects ###

str_const0: 	.word	5	# Class Tag
	.word	6	# Object size
	.word	String_dispTab	# Dispatch table
	.word	int_const30	# String len (int obj)
	.ascii	"Main"
	.byte	0	# Null terminate
	.align	2
str_const1: 	.word	5	# Class Tag
	.word	5	# Object size
	.word	String_dispTab	# Dispatch table
	.word	int_const31	# String len (int obj)
	.ascii	"IO"
	.byte	0	# Null terminate
	.align	2
str_const2: 	.word	5	# Class Tag
	.word	6	# Object size
	.word	String_dispTab	# Dispatch table
	.word	int_const30	# String len (int obj)
	.ascii	"main"
	.byte	0	# Null terminate
	.align	2
str_const5: 	.word	5	# Class Tag
	.word	7	# Object size
	.word	String_dispTab	# Dispatch table
	.word	int_const32	# String len (int obj)
	.ascii	"out_string"
	.byte	0	# Null terminate
	.align	2
str_const6: 	.word	5	# Class Tag
	.word	9	# Object size
	.word	String_dispTab	# Dispatch table
	.word	int_const33	# String len (int obj)
	.ascii	"Enter an integer: "
	.byte	0	# Null terminate
	.align	2
str_const8: 	.word	5	# Class Tag
	.word	5	# Object size
	.word	String_dispTab	# Dispatch table
	.word	int_const34	# String len (int obj)
	.ascii	"Int"
	.byte	0	# Null terminate
	.align	2
str_const9: 	.word	5	# Class Tag
	.word	6	# Object size
	.word	String_dispTab	# Dispatch table
	.word	int_const35	# String len (int obj)
	.ascii	"in_int"
	.byte	0	# Null terminate
	.align	2
str_const10: 	.word	5	# Class Tag
	.word	6	# Object size
	.word	String_dispTab	# Dispatch table
	.word	int_const35	# String len (int obj)
	.ascii	"Read: "
	.byte	0	# Null terminate
	.align	2
str_const11: 	.word	5	# Class Tag
	.word	6	# Object size
	.word	String_dispTab	# Dispatch table
	.word	int_const36	# String len (int obj)
	.ascii	"out_int"
	.byte	0	# Null terminate
	.align	2
str_const12: 	.word	5	# Class Tag
	.word	5	# Object size
	.word	String_dispTab	# Dispatch table
	.word	int_const37	# String len (int obj)
	.ascii	"\n"
	.byte	0	# Null terminate
	.align	2
str_const13: 	.word	5	# Class Tag
	.word	6	# Object size
	.word	String_dispTab	# Dispatch table
	.word	int_const38	# String len (int obj)
	.ascii	"abort"
	.byte	0	# Null terminate
	.align	2
str_const14: 	.word	5	# Class Tag
	.word	6	# Object size
	.word	String_dispTab	# Dispatch table
	.word	int_const30	# String len (int obj)
	.ascii	"copy"
	.byte	0	# Null terminate
	.align	2
str_const15: 	.word	5	# Class Tag
	.word	7	# Object size
	.word	String_dispTab	# Dispatch table
	.word	int_const39	# String len (int obj)
	.ascii	"type_name"
	.byte	0	# Null terminate
	.align	2
str_const16: 	.word	5	# Class Tag
	.word	6	# Object size
	.word	String_dispTab	# Dispatch table
	.word	int_const36	# String len (int obj)
	.ascii	"isProto"
	.byte	0	# Null terminate
	.align	2
str_const17: 	.word	5	# Class Tag
	.word	6	# Object size
	.word	String_dispTab	# Dispatch table
	.word	int_const35	# String len (int obj)
	.ascii	"Object"
	.byte	0	# Null terminate
	.align	2
str_const18: 	.word	5	# Class Tag
	.word	6	# Object size
	.word	String_dispTab	# Dispatch table
	.word	int_const35	# String len (int obj)
	.ascii	"String"
	.byte	0	# Null terminate
	.align	2
str_const19: 	.word	5	# Class Tag
	.word	6	# Object size
	.word	String_dispTab	# Dispatch table
	.word	int_const30	# String len (int obj)
	.ascii	"Bool"
	.byte	0	# Null terminate
	.align	2
str_const24: 	.word	5	# Class Tag
	.word	7	# Object size
	.word	String_dispTab	# Dispatch table
	.word	int_const39	# String len (int obj)
	.ascii	"in_string"
	.byte	0	# Null terminate
	.align	2
str_const25: 	.word	5	# Class Tag
	.word	6	# Object size
	.word	String_dispTab	# Dispatch table
	.word	int_const35	# String len (int obj)
	.ascii	"length"
	.byte	0	# Null terminate
	.align	2
str_const26: 	.word	5	# Class Tag
	.word	6	# Object size
	.word	String_dispTab	# Dispatch table
	.word	int_const35	# String len (int obj)
	.ascii	"concat"
	.byte	0	# Null terminate
	.align	2
str_const27: 	.word	5	# Class Tag
	.word	6	# Object size
	.word	String_dispTab	# Dispatch table
	.word	int_const35	# String len (int obj)
	.ascii	"substr"
	.byte	0	# Null terminate
	.align	2
### Integer constant objects ###

int_const29: 	.word	3	# Class Tag
	.word	4	# Object size
	.word	Int_dispTab	# Dispatch table
	.word	0	
int_const30: 	.word	3	# Class Tag
	.word	4	# Object size
	.word	Int_dispTab	# Dispatch table
	.word	4	
int_const31: 	.word	3	# Class Tag
	.word	4	# Object size
	.word	Int_dispTab	# Dispatch table
	.word	2	
int_const32: 	.word	3	# Class Tag
	.word	4	# Object size
	.word	Int_dispTab	# Dispatch table
	.word	10	
int_const33: 	.word	3	# Class Tag
	.word	4	# Object size
	.word	Int_dispTab	# Dispatch table
	.word	18	
int_const34: 	.word	3	# Class Tag
	.word	4	# Object size
	.word	Int_dispTab	# Dispatch table
	.word	3	
int_const35: 	.word	3	# Class Tag
	.word	4	# Object size
	.word	Int_dispTab	# Dispatch table
	.word	6	
int_const36: 	.word	3	# Class Tag
	.word	4	# Object size
	.word	Int_dispTab	# Dispatch table
	.word	7	
int_const37: 	.word	3	# Class Tag
	.word	4	# Object size
	.word	Int_dispTab	# Dispatch table
	.word	1	
int_const38: 	.word	3	# Class Tag
	.word	4	# Object size
	.word	Int_dispTab	# Dispatch table
	.word	5	
int_const39: 	.word	3	# Class Tag
	.word	4	# Object size
	.word	Int_dispTab	# Dispatch table
	.word	9	

### Bool constant objects ###

bool_const0: 	.word	4	# Class Tag
	.word	4	# Object size
	.word	Bool_dispTab	# Dispatch table
	.word	0	# false
bool_const1: 	.word	4	# Class Tag
	.word	4	# Object size
	.word	Bool_dispTab	# Dispatch table
	.word	1	# true

###  Table of class names ###
class_nameTab:	.word	str_const17
	.word	str_const1
	.word	str_const0
	.word	str_const8
	.word	str_const19
	.word	str_const18

###  Table of protObjs and initcodes ###
class_objTab:	.word	Object_protObj
	.word	Object_init
	.word	IO_protObj
	.word	IO_init
	.word	Main_protObj
	.word	Main_init
	.word	Int_protObj
	.word	Int_init
	.word	Bool_protObj
	.word	Bool_init
	.word	String_protObj
	.word	String_init

#  Dispatch table for Class Object
Object_dispTab: 	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	Object.isProto

# Proto dispatch table for Class Object
Object_protoDispTab: 	.word	Object.abort.Proto
	.word	Object.type_name.Proto
	.word	Object.copy.Proto
	.word	Object.isProto.Proto

# Prototype object for Class Object
Object_protObj: 	.word	0	# Class Tag
	.word	3	# Object size
	.word	Object_protoDispTab	# Proto dispatch table

#  Dispatch table for Class IO
IO_dispTab: 	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	Object.isProto
	.word	IO.out_string
	.word	IO.out_int
	.word	IO.in_string
	.word	IO.in_int

# Proto dispatch table for Class IO
IO_protoDispTab: 	.word	Object.abort.Proto
	.word	Object.type_name.Proto
	.word	Object.copy.Proto
	.word	Object.isProto.Proto
	.word	IO.out_string.Proto
	.word	IO.out_int.Proto
	.word	IO.in_string.Proto
	.word	IO.in_int.Proto

# Prototype object for Class IO
IO_protObj: 	.word	1	# Class Tag
	.word	3	# Object size
	.word	IO_protoDispTab	# Proto dispatch table

#  Dispatch table for Class Main
Main_dispTab: 	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	Object.isProto
	.word	IO.out_string
	.word	IO.out_int
	.word	IO.in_string
	.word	IO.in_int
	.word	Main.main

# Proto dispatch table for Class Main
Main_protoDispTab: 	.word	Object.abort.Proto
	.word	Object.type_name.Proto
	.word	Object.copy.Proto
	.word	Object.isProto.Proto
	.word	IO.out_string.Proto
	.word	IO.out_int.Proto
	.word	IO.in_string.Proto
	.word	IO.in_int.Proto
	.word	Main.main.Proto

# Prototype object for Class Main
Main_protObj: 	.word	2	# Class Tag
	.word	3	# Object size
	.word	Main_protoDispTab	# Proto dispatch table

#  Dispatch table for Class Int
Int_protoDispTab:	# No proto
Int_dispTab: 	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	Object.isProto

# Prototype object for Class Int
Int_protObj: 	.word	3	# Class Tag
	.word	4	# Object size
	.word	Int_protoDispTab	# Proto dispatch table
	.word	0	# prim slot _val

#  Dispatch table for Class Bool
Bool_protoDispTab:	# No proto
Bool_dispTab: 	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	Object.isProto

# Prototype object for Class Bool
Bool_protObj: 	.word	4	# Class Tag
	.word	4	# Object size
	.word	Bool_protoDispTab	# Proto dispatch table
	.word	0	# prim slot _val

#  Dispatch table for Class String
String_protoDispTab:	# No proto
String_dispTab: 	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	Object.isProto
	.word	String.length
	.word	String.concat
	.word	String.substr

# Prototype object for Class String
String_protObj: 	.word	5	# Class Tag
	.word	5	# Object size
	.word	String_protoDispTab	# Proto dispatch table
	.word	Int_protObj	# Slot _val
	.word	0	# prim slot _str_field

	.globl	heap_start
heap_start:	.word 0


	.text
	.globl	Main_init
	.globl	Int_init
	.globl	String_init
	.globl	Main.main

	
Object_init:
	sw	$ra 0($sp)	
	addiu	$sp $sp -4	
	sw	$s0 0($sp)	
	addiu	$sp $sp -4	
	move	$s0 $a0	
	la	$t1 Object_dispTab
	sw	$t1 8($s0)	
	move	$a0 $s0	
	lw	$s0 4($sp)	
	addiu	$sp $sp 4	
	lw	$ra 4($sp)	
	addiu	$sp $sp 4	
	jr	$ra	

	
Object.abort.Proto:
	la	$a1 str_const13
	jal	_proto_abort

	
Object.type_name.Proto:
	la	$a1 str_const15
	jal	_proto_abort

	
Object.copy.Proto:
	la	$a1 str_const14
	jal	_proto_abort

	
Object.isProto.Proto:
	la	$a0 bool_const1
	jr	$ra	

	
IO_init:
	sw	$ra 0($sp)	
	addiu	$sp $sp -4	
	sw	$s0 0($sp)	
	addiu	$sp $sp -4	
	move	$s0 $a0	
	jal	Object_init
	la	$t1 IO_dispTab
	sw	$t1 8($s0)	
	move	$a0 $s0	
	lw	$s0 4($sp)	
	addiu	$sp $sp 4	
	lw	$ra 4($sp)	
	addiu	$sp $sp 4	
	jr	$ra	

	
IO.out_string.Proto:
	la	$a1 str_const5
	jal	_proto_abort

	
IO.out_int.Proto:
	la	$a1 str_const11
	jal	_proto_abort

	
IO.in_string.Proto:
	la	$a1 str_const24
	jal	_proto_abort

	
IO.in_int.Proto:
	la	$a1 str_const9
	jal	_proto_abort

	
Main_init:
	sw	$ra 0($sp)	
	addiu	$sp $sp -4	
	sw	$s0 0($sp)	
	addiu	$sp $sp -4	
	move	$s0 $a0	
	jal	IO_init
	la	$t1 Main_dispTab
	sw	$t1 8($s0)	
	move	$a0 $s0	
	lw	$s0 4($sp)	
	addiu	$sp $sp 4	
	lw	$ra 4($sp)	
	addiu	$sp $sp 4	
	jr	$ra	

	
Main.main:
	sw	$ra 0($sp)	
	addiu	$sp $sp -4	
	sw	$s0 0($sp)	
	addiu	$sp $sp -4	
	move	$s0 $a0	
	la	$a0 str_const6
	sw	$a0 0($sp)	
	addiu	$sp $sp -4	
	move	$a0 $s0	
	lw	$t1 8($a0)	
	lw	$t1 16($t1)	
	jalr	$t1	
	addiu	$sp $sp 4	
	move	$a0 $s0	
	lw	$t1 8($a0)	
	lw	$t1 28($t1)	
	jalr	$t1	
	sw	$a0 0($sp)	
	addiu	$sp $sp -4	
	la	$a0 str_const10
	sw	$a0 0($sp)	
	addiu	$sp $sp -4	
	move	$a0 $s0	
	lw	$t1 8($a0)	
	lw	$t1 16($t1)	
	jalr	$t1	
	addiu	$sp $sp 4	
	lw	$a0 4($sp)	
	sw	$a0 0($sp)	
	addiu	$sp $sp -4	
	move	$a0 $s0	
	lw	$t1 8($a0)	
	lw	$t1 20($t1)	
	jalr	$t1	
	addiu	$sp $sp 4	
	la	$a0 str_const12
	sw	$a0 0($sp)	
	addiu	$sp $sp -4	
	move	$a0 $s0	
	lw	$t1 8($a0)	
	lw	$t1 16($t1)	
	jalr	$t1	
	addiu	$sp $sp 4	
	lw	$zero 4($sp)	
	addiu	$sp $sp 4	
	lw	$s0 4($sp)	
	addiu	$sp $sp 4	
	lw	$ra 4($sp)	
	addiu	$sp $sp 4	
	jr	$ra	

	
Main.main.Proto:
	la	$a1 str_const2
	jal	_proto_abort

	
Int_init:
	sw	$ra 0($sp)	
	addiu	$sp $sp -4	
	sw	$s0 0($sp)	
	addiu	$sp $sp -4	
	move	$s0 $a0	
	jal	Object_init
	la	$t1 Int_dispTab
	sw	$t1 8($s0)	
	move	$a0 $s0	
	lw	$s0 4($sp)	
	addiu	$sp $sp 4	
	lw	$ra 4($sp)	
	addiu	$sp $sp 4	
	jr	$ra	

	
Bool_init:
	sw	$ra 0($sp)	
	addiu	$sp $sp -4	
	sw	$s0 0($sp)	
	addiu	$sp $sp -4	
	move	$s0 $a0	
	jal	Object_init
	la	$t1 Bool_dispTab
	sw	$t1 8($s0)	
	move	$a0 $s0	
	lw	$s0 4($sp)	
	addiu	$sp $sp 4	
	lw	$ra 4($sp)	
	addiu	$sp $sp 4	
	jr	$ra	

	
String_init:
	sw	$ra 0($sp)	
	addiu	$sp $sp -4	
	sw	$s0 0($sp)	
	addiu	$sp $sp -4	
	move	$s0 $a0	
	jal	Object_init
	la	$t1 String_dispTab
	sw	$t1 8($s0)	
	la	$a0 int_const29
	sw	$a0 12($s0)	
	move	$a0 $s0	
	lw	$s0 4($sp)	
	addiu	$sp $sp 4	
	lw	$ra 4($sp)	
	addiu	$sp $sp 4	
	jr	$ra	
