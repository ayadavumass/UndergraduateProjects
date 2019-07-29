	.data
	.align	2
	.globl	class_nameTab
	.globl	Main_protObj
	.globl	Int_protObj
	.globl	String_protObj
	.globl	bool_const0
	.globl	bool_const1
	.globl	_int_tag
	.globl	_bool_tag
	.globl	_string_tag
_int_tag:
	.word	2
_bool_tag:
	.word	3
_string_tag:
	.word	4
	.globl	_MemMgr_INITIALIZER
_MemMgr_INITIALIZER:
	.word	_NoGC_Init
	.globl	_MemMgr_COLLECTOR
_MemMgr_COLLECTOR:
	.word	_NoGC_Collect
	.globl	_MemMgr_TEST
_MemMgr_TEST:
	.word	0
	.word	-1
str_const25:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const0
	.byte	0	
	.align	2
	.word	-1
str_const24:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const5
	.ascii	"Main"
	.byte	0	
	.align	2
	.word	-1
str_const23:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const2
	.ascii	"Q"
	.byte	0	
	.align	2
	.word	-1
str_const22:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const2
	.ascii	"P"
	.byte	0	
	.align	2
	.word	-1
str_const21:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const2
	.ascii	"O"
	.byte	0	
	.align	2
	.word	-1
str_const20:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const2
	.ascii	"N"
	.byte	0	
	.align	2
	.word	-1
str_const19:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const2
	.ascii	"M"
	.byte	0	
	.align	2
	.word	-1
str_const18:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const2
	.ascii	"L"
	.byte	0	
	.align	2
	.word	-1
str_const17:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const2
	.ascii	"K"
	.byte	0	
	.align	2
	.word	-1
str_const16:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const2
	.ascii	"J"
	.byte	0	
	.align	2
	.word	-1
str_const15:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const2
	.ascii	"I"
	.byte	0	
	.align	2
	.word	-1
str_const14:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const2
	.ascii	"H"
	.byte	0	
	.align	2
	.word	-1
str_const13:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const2
	.ascii	"G"
	.byte	0	
	.align	2
	.word	-1
str_const12:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const2
	.ascii	"F"
	.byte	0	
	.align	2
	.word	-1
str_const11:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const2
	.ascii	"E"
	.byte	0	
	.align	2
	.word	-1
str_const10:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const2
	.ascii	"D"
	.byte	0	
	.align	2
	.word	-1
str_const9:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const2
	.ascii	"C"
	.byte	0	
	.align	2
	.word	-1
str_const8:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const2
	.ascii	"B"
	.byte	0	
	.align	2
	.word	-1
str_const7:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const2
	.ascii	"A"
	.byte	0	
	.align	2
	.word	-1
str_const6:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const7
	.ascii	"String"
	.byte	0	
	.align	2
	.word	-1
str_const5:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const5
	.ascii	"Bool"
	.byte	0	
	.align	2
	.word	-1
str_const4:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const4
	.ascii	"Int"
	.byte	0	
	.align	2
	.word	-1
str_const3:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const3
	.ascii	"IO"
	.byte	0	
	.align	2
	.word	-1
str_const2:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const7
	.ascii	"Object"
	.byte	0	
	.align	2
	.word	-1
str_const1:
	.word	4
	.word	8
	.word	String_dispTab
	.word	int_const14
	.ascii	"<basic class>"
	.byte	0	
	.align	2
	.word	-1
str_const0:
	.word	4
	.word	11
	.word	String_dispTab
	.word	int_const19
	.ascii	"./BASE_cgtest/caseloop.cl"
	.byte	0	
	.align	2
	.word	-1
int_const19:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	25
	.word	-1
int_const18:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	17
	.word	-1
int_const17:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	16
	.word	-1
int_const16:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	15
	.word	-1
int_const15:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	14
	.word	-1
int_const14:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	13
	.word	-1
int_const13:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	12
	.word	-1
int_const12:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	11
	.word	-1
int_const11:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	10
	.word	-1
int_const10:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	9
	.word	-1
int_const9:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	8
	.word	-1
int_const8:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	7
	.word	-1
int_const7:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	6
	.word	-1
int_const6:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	5
	.word	-1
int_const5:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	4
	.word	-1
int_const4:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	3
	.word	-1
int_const3:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	2
	.word	-1
int_const2:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	1
	.word	-1
int_const1:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	1000
	.word	-1
int_const0:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	0
	.word	-1
bool_const0:
	.word	3
	.word	4
	.word	Bool_dispTab
	.word	0
	.word	-1
bool_const1:
	.word	3
	.word	4
	.word	Bool_dispTab
	.word	1
class_nameTab:
	.word	str_const2
	.word	str_const3
	.word	str_const4
	.word	str_const5
	.word	str_const6
	.word	str_const7
	.word	str_const8
	.word	str_const9
	.word	str_const10
	.word	str_const11
	.word	str_const12
	.word	str_const13
	.word	str_const14
	.word	str_const15
	.word	str_const16
	.word	str_const17
	.word	str_const18
	.word	str_const19
	.word	str_const20
	.word	str_const21
	.word	str_const22
	.word	str_const23
	.word	str_const24
class_objTab:
	.word	Object_protObj
	.word	Object_init
	.word	IO_protObj
	.word	IO_init
	.word	Int_protObj
	.word	Int_init
	.word	Bool_protObj
	.word	Bool_init
	.word	String_protObj
	.word	String_init
	.word	A_protObj
	.word	A_init
	.word	B_protObj
	.word	B_init
	.word	C_protObj
	.word	C_init
	.word	D_protObj
	.word	D_init
	.word	E_protObj
	.word	E_init
	.word	F_protObj
	.word	F_init
	.word	G_protObj
	.word	G_init
	.word	H_protObj
	.word	H_init
	.word	I_protObj
	.word	I_init
	.word	J_protObj
	.word	J_init
	.word	K_protObj
	.word	K_init
	.word	L_protObj
	.word	L_init
	.word	M_protObj
	.word	M_init
	.word	N_protObj
	.word	N_init
	.word	O_protObj
	.word	O_init
	.word	P_protObj
	.word	P_init
	.word	Q_protObj
	.word	Q_init
	.word	Main_protObj
	.word	Main_init
Object_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
Main_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	Main.main
A_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
B_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
C_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
D_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
E_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
F_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
G_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
H_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
I_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
J_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
K_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
L_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
M_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
N_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
O_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
P_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
Q_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
String_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	String.length
	.word	String.concat
	.word	String.substr
Bool_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
Int_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
IO_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	IO.out_string
	.word	IO.out_int
	.word	IO.in_string
	.word	IO.in_int
	.word	-1
Object_protObj:
	.word	0
	.word	3
	.word	Object_dispTab
	.word	-1
Main_protObj:
	.word	22
	.word	3
	.word	Main_dispTab
	.word	-1
A_protObj:
	.word	5
	.word	3
	.word	A_dispTab
	.word	-1
B_protObj:
	.word	6
	.word	3
	.word	B_dispTab
	.word	-1
C_protObj:
	.word	7
	.word	3
	.word	C_dispTab
	.word	-1
D_protObj:
	.word	8
	.word	3
	.word	D_dispTab
	.word	-1
E_protObj:
	.word	9
	.word	3
	.word	E_dispTab
	.word	-1
F_protObj:
	.word	10
	.word	3
	.word	F_dispTab
	.word	-1
G_protObj:
	.word	11
	.word	3
	.word	G_dispTab
	.word	-1
H_protObj:
	.word	12
	.word	3
	.word	H_dispTab
	.word	-1
I_protObj:
	.word	13
	.word	3
	.word	I_dispTab
	.word	-1
J_protObj:
	.word	14
	.word	3
	.word	J_dispTab
	.word	-1
K_protObj:
	.word	15
	.word	3
	.word	K_dispTab
	.word	-1
L_protObj:
	.word	16
	.word	3
	.word	L_dispTab
	.word	-1
M_protObj:
	.word	17
	.word	3
	.word	M_dispTab
	.word	-1
N_protObj:
	.word	18
	.word	3
	.word	N_dispTab
	.word	-1
O_protObj:
	.word	19
	.word	3
	.word	O_dispTab
	.word	-1
P_protObj:
	.word	20
	.word	3
	.word	P_dispTab
	.word	-1
Q_protObj:
	.word	21
	.word	3
	.word	Q_dispTab
	.word	-1
String_protObj:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const0
	.word	0
	.word	-1
Bool_protObj:
	.word	3
	.word	4
	.word	Bool_dispTab
	.word	0
	.word	-1
Int_protObj:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	0
	.word	-1
IO_protObj:
	.word	1
	.word	3
	.word	IO_dispTab
	.globl	heap_start
heap_start:
	.word	0
	.text
	.globl	Main_init
	.globl	Int_init
	.globl	String_init
	.globl	Bool_init
	.globl	Main.main
Object_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
Main_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	Object_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
A_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	Object_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
B_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	A_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
C_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	B_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
D_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	C_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
E_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	D_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
F_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	E_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
G_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	F_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
H_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	G_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
I_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	H_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
J_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	I_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
K_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	J_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
L_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	K_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
M_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	L_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
N_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	M_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
O_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	N_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
P_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	O_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
Q_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	P_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
String_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	Object_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
Bool_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	Object_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
Int_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	Object_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
IO_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	Object_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
Main.main:
	addiu	$sp $sp -32
	sw	$fp 32($sp)
	sw	$s0 28($sp)
	sw	$ra 24($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	la	$s1 int_const0
	la	$a0 A_protObj
	jal	Object.copy
	sw	$s1 0($fp)
	sw	$s2 4($fp)
	sw	$s3 8($fp)
	sw	$s4 12($fp)
	sw	$s5 16($fp)
	jal	A_init
	lw	$s1 0($fp)
	lw	$s2 4($fp)
	lw	$s3 8($fp)
	lw	$s4 12($fp)
	lw	$s5 16($fp)
	move	$s2 $a0
	la	$a0 Q_protObj
	jal	Object.copy
	sw	$s1 0($fp)
	sw	$s2 4($fp)
	sw	$s3 8($fp)
	sw	$s4 12($fp)
	sw	$s5 16($fp)
	jal	Q_init
	lw	$s1 0($fp)
	lw	$s2 4($fp)
	lw	$s3 8($fp)
	lw	$s4 12($fp)
	lw	$s5 16($fp)
	move	$s3 $a0
	la	$s4 int_const0
label0:
	move	$s5 $s1
	la	$a0 int_const1
	lw	$t1 12($s5)
	lw	$t2 12($a0)
	la	$a0 bool_const1
	blt	$t1 $t2 label2
	la	$a0 bool_const0
label2:
	lw	$t1 12($a0)
	beq	$t1 $zero label1
	bne	$s3 $zero label4
	la	$a0 str_const0
	li	$t1 49
	jal	_case_abort2
label4:
	lw	$t2 0($s3)
	blt	$t2 21 label5
	bgt	$t2 21 label5
	move	$s5 $s3
	la	$a0 int_const18
	b	label3
label5:
	blt	$t2 20 label6
	bgt	$t2 21 label6
	move	$s5 $s3
	la	$a0 int_const17
	b	label3
label6:
	blt	$t2 19 label7
	bgt	$t2 21 label7
	move	$s5 $s3
	la	$a0 int_const16
	b	label3
label7:
	blt	$t2 18 label8
	bgt	$t2 21 label8
	move	$s5 $s3
	la	$a0 int_const15
	b	label3
label8:
	blt	$t2 17 label9
	bgt	$t2 21 label9
	move	$s5 $s3
	la	$a0 int_const14
	b	label3
label9:
	blt	$t2 16 label10
	bgt	$t2 21 label10
	move	$s5 $s3
	la	$a0 int_const13
	b	label3
label10:
	blt	$t2 15 label11
	bgt	$t2 21 label11
	move	$s5 $s3
	la	$a0 int_const12
	b	label3
label11:
	blt	$t2 14 label12
	bgt	$t2 21 label12
	move	$s5 $s3
	la	$a0 int_const11
	b	label3
label12:
	blt	$t2 13 label13
	bgt	$t2 21 label13
	move	$s5 $s3
	la	$a0 int_const10
	b	label3
label13:
	blt	$t2 12 label14
	bgt	$t2 21 label14
	move	$s5 $s3
	la	$a0 int_const9
	b	label3
label14:
	blt	$t2 11 label15
	bgt	$t2 21 label15
	move	$s5 $s3
	la	$a0 int_const8
	b	label3
label15:
	blt	$t2 10 label16
	bgt	$t2 21 label16
	move	$s5 $s3
	la	$a0 int_const7
	b	label3
label16:
	blt	$t2 9 label17
	bgt	$t2 21 label17
	move	$s5 $s3
	la	$a0 int_const6
	b	label3
label17:
	blt	$t2 8 label18
	bgt	$t2 21 label18
	move	$s5 $s3
	la	$a0 int_const5
	b	label3
label18:
	blt	$t2 7 label19
	bgt	$t2 21 label19
	move	$s5 $s3
	la	$a0 int_const4
	b	label3
label19:
	blt	$t2 6 label20
	bgt	$t2 21 label20
	move	$s5 $s3
	la	$a0 int_const3
	b	label3
label20:
	blt	$t2 5 label21
	bgt	$t2 21 label21
	move	$s5 $s3
	la	$a0 int_const2
	b	label3
label21:
	jal	_case_abort
label3:
	move	$s4 $a0
	move	$s5 $s1
	la	$a0 int_const2
	jal	Object.copy
	lw	$t2 12($a0)
	lw	$t1 12($s5)
	add	$t1 $t1 $t2
	sw	$t1 12($a0)
	move	$s1 $a0
	b	label0
label1:
	move	$a0 $zero
	move	$s5 $s4
	la	$t2 int_const18
	move	$t1 $s5
	la	$a0 bool_const1
	beq	$t1 $t2 label25
	la	$a1 bool_const0
	jal	equality_test
label25:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label24
	la	$a0 bool_const0
label24:
	lw	$t1 12($a0)
	beqz	$t1 label22
	move	$a0 $s0
	sw	$s1 0($fp)
	sw	$s2 4($fp)
	sw	$s3 8($fp)
	sw	$s4 12($fp)
	sw	$s5 16($fp)
	bne	$a0 $zero label26
	la	$a0 str_const0
	li	$t1 52
	jal	_dispatch_abort
label26:
	lw	$t1 8($a0)
	lw	$t1 0($t1)
	jalr		$t1
	lw	$s1 0($fp)
	lw	$s2 4($fp)
	lw	$s3 8($fp)
	lw	$s4 12($fp)
	lw	$s5 16($fp)
	b	label23
label22:
	la	$a0 int_const0
label23:
	la	$s1 int_const0
label27:
	move	$s5 $s1
	la	$a0 int_const1
	lw	$t1 12($s5)
	lw	$t2 12($a0)
	la	$a0 bool_const1
	blt	$t1 $t2 label29
	la	$a0 bool_const0
label29:
	lw	$t1 12($a0)
	beq	$t1 $zero label28
	bne	$s2 $zero label31
	la	$a0 str_const0
	li	$t1 75
	jal	_case_abort2
label31:
	lw	$t2 0($s2)
	blt	$t2 21 label32
	bgt	$t2 21 label32
	move	$s5 $s2
	la	$a0 int_const18
	b	label30
label32:
	blt	$t2 20 label33
	bgt	$t2 21 label33
	move	$s5 $s2
	la	$a0 int_const17
	b	label30
label33:
	blt	$t2 19 label34
	bgt	$t2 21 label34
	move	$s5 $s2
	la	$a0 int_const16
	b	label30
label34:
	blt	$t2 18 label35
	bgt	$t2 21 label35
	move	$s5 $s2
	la	$a0 int_const15
	b	label30
label35:
	blt	$t2 17 label36
	bgt	$t2 21 label36
	move	$s5 $s2
	la	$a0 int_const14
	b	label30
label36:
	blt	$t2 16 label37
	bgt	$t2 21 label37
	move	$s5 $s2
	la	$a0 int_const13
	b	label30
label37:
	blt	$t2 15 label38
	bgt	$t2 21 label38
	move	$s5 $s2
	la	$a0 int_const12
	b	label30
label38:
	blt	$t2 14 label39
	bgt	$t2 21 label39
	move	$s5 $s2
	la	$a0 int_const11
	b	label30
label39:
	blt	$t2 13 label40
	bgt	$t2 21 label40
	move	$s5 $s2
	la	$a0 int_const10
	b	label30
label40:
	blt	$t2 12 label41
	bgt	$t2 21 label41
	move	$s5 $s2
	la	$a0 int_const9
	b	label30
label41:
	blt	$t2 11 label42
	bgt	$t2 21 label42
	move	$s5 $s2
	la	$a0 int_const8
	b	label30
label42:
	blt	$t2 10 label43
	bgt	$t2 21 label43
	move	$s5 $s2
	la	$a0 int_const7
	b	label30
label43:
	blt	$t2 9 label44
	bgt	$t2 21 label44
	move	$s5 $s2
	la	$a0 int_const6
	b	label30
label44:
	blt	$t2 8 label45
	bgt	$t2 21 label45
	move	$s5 $s2
	la	$a0 int_const5
	b	label30
label45:
	blt	$t2 7 label46
	bgt	$t2 21 label46
	move	$s5 $s2
	la	$a0 int_const4
	b	label30
label46:
	blt	$t2 6 label47
	bgt	$t2 21 label47
	move	$s5 $s2
	la	$a0 int_const3
	b	label30
label47:
	blt	$t2 5 label48
	bgt	$t2 21 label48
	move	$s5 $s2
	la	$a0 int_const2
	b	label30
label48:
	jal	_case_abort
label30:
	move	$s4 $a0
	move	$s5 $s1
	la	$a0 int_const2
	jal	Object.copy
	lw	$t2 12($a0)
	lw	$t1 12($s5)
	add	$t1 $t1 $t2
	sw	$t1 12($a0)
	move	$s1 $a0
	b	label27
label28:
	move	$a0 $zero
	move	$s5 $s4
	la	$t2 int_const2
	move	$t1 $s5
	la	$a0 bool_const1
	beq	$t1 $t2 label52
	la	$a1 bool_const0
	jal	equality_test
label52:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label51
	la	$a0 bool_const0
label51:
	lw	$t1 12($a0)
	beqz	$t1 label49
	move	$a0 $s0
	sw	$s1 0($fp)
	sw	$s2 4($fp)
	sw	$s3 8($fp)
	sw	$s4 12($fp)
	sw	$s5 16($fp)
	bne	$a0 $zero label53
	la	$a0 str_const0
	li	$t1 78
	jal	_dispatch_abort
label53:
	lw	$t1 8($a0)
	lw	$t1 0($t1)
	jalr		$t1
	lw	$s1 0($fp)
	lw	$s2 4($fp)
	lw	$s3 8($fp)
	lw	$s4 12($fp)
	lw	$s5 16($fp)
	b	label50
label49:
	la	$a0 int_const0
label50:
	lw	$fp 32($sp)
	lw	$s0 28($sp)
	lw	$ra 24($sp)
	addiu	$sp $sp 32
	jr	$ra	
