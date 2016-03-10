;****************************************************************
; FILENAME : ex8_func.asm
;
; ROUTINES : sig_pow
;****************************************************************
	TITLE	'EX8 POW'
;***********************************************************
;Input:  r0 input pointer for real signal
;	 r4 input pointer for imag signal
;	 n2 length of input fields in words
;Output: b	signal power
;	 r1	address of window (1st sample)
;	 y0	power class
;***********************************************************

;****** Definition of Constants **********
LEVS	equ	16				; num of power levels
;****** Definition of Table **********
	SECTION	ex8_data			; data section starts here
	ORG	L:
	xdef	POW_CL
POW_CL	dc	$0				; look up table			1
	dc	$a				; for power			2
	dc	$64				; classification		3
	dc	$3e8				;				4
	dc	$2710				;				5
	dc	$186a0				;				6
	dc	$f4240				;				7
	dc	$989680				;				8
	dc	$5f5e100			;				9
	dc	$3b9aca00			;				10
	dc	$2540be400			;				11
	dc	$174876e800			;				12
	dc	$e8d4a51000			;				13
	dc	$9184e72a000			;				14
	dc	$5af310a4000			;				15
	dc	$7ffffffffff			; maximum			16
	ENDSEC
;******	PROGRAM:   **********************	
	SECTION ex8_code			; declare a program section,..
	xdef	sig_pow				; refer to main file
	xref	POW_CL
	xref  ex8_start 
	ORG	p:				; to be located in p RAM
						; (see ex8.ctl for allocation)
sig_pow
	clr	a				; init a (power store)
	move	n2,r2				; store number of inputs (used?)
	move	n2,b				; get number of inputs
	move	r0,r1				; copy real pointer
	sub	#2,b				; reduce to loop count
						; (N-M=1) times, see documentation
	move	r4,r5				; copy imag pointer
	move			x:(r0)+,x0	; 	    and get 1.real
	move 	b,n2				; store loop count back
	mac	x0,x0,a		y:(r4)+,y0	; mac 1.real^2, get 1. imag 
	mac	y0,y0,a		x:(r0)+,x0	; mac 1.imag^2, get 2. real
	mac	x0,x0,a		y:(r4)+,y0	; mac 2.real^2, get 2. imag 
	mac	y0,y0,a		x:(r0)+,x0	; mac 2.imag^2, get 3. real
	clr	b				; init b (maximum store)

	do	n2,get_pow			; start main loop	
	mac	x0,x0,a		y:(r4)+,y0	; mac real^2, get 3. (++) imag 
	mac	y0,y0,a		x:(r1)+,x0	; mac imag^2, get -1.(++) real
	max	a,b				; if maximum, store in b
	tcc	r1,r6				; store pos, if max update
	mac	-x0,x0,a	y:(r5)+,y0	; mac -real^2, get -1.(++) imag 
	mac	-y0,y0,a	x:(r0)+,x0	; mac -imag^2, get 4. (++) real
get_pow						; maximum power now in b..
	move	#POW_CL,r0			; init pointer for table
	move	r6,a0				; get address of maximum + 1
	dec	a				; = maximum 		
	move			x:(r0)+,x0	; get first threshold
	move	a0,r1				; store max address
do_sub	do	#LEVS,pow_class			; 
	move	b0,a0				; get power for threshold compare
	move	b1,a1				;  "
	sub	x,a		l:(r0)+,x	; compare to threshold
	nop					; break latency
	brkle					; if threshold reached, exit loop
	move	r0,a				; else get new input address.. 
	sub	#(POW_CL+2),a			; .. and calc power step..	
	move	a,y0				; .. from this number
pow_class	
  jsr ex8_start
	rts					; end of routine
	ENDSEC					; end of code