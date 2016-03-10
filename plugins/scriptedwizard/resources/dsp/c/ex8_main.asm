;****************************************************************
; FILENAME : ex8_main.asm
;
; ROUTINES : main program for ex8
;****************************************************************
	TITLE	'EX8 MAIN'

;****** Definition of Constants **********
ABC	equ	300				; number of input words
FAC	equ	0.0001				; factor for signal amplitude

;****** Generation of Input data *********
	SECTION	ex8_data
	ORG	x:				; put data into x mem
	xdef	DATA1X				; make label visible and 
DATA1X	DUPF	CC,0,ABC/2-1,1			; now do ABC times..
	dc	\CC*FAC*@SIN(\CC*0.01)		; the allocation..
	ENDM					; of test data
	DUPF	CC,ABC/2,0,-1			; increasing
	dc	\CC*FAC*@SIN(\CC*0.01)		; and decreasing..
	ENDM					; amplitude
	ORG	y:				; put data into x mem
	xdef	DATA1Y				; make label visible and 
DATA1Y	DUPF	CC,0,ABC/2-1,1			; now do ABC times..
	dc	\CC*FAC*@COS(\CC*0.01+12)	; the allocation..
	ENDM					; of test data
	DUPF	CC,ABC/2,0,-1			; increasing
	dc	\CC*FAC*@COS(\CC*0.01+12)	; and decreasing..
	ENDM					; amplitude
		ENDSEC				; end of data allocation

;******	MAIN PROGRAM:   *******************	
	SECTION ex8_code			; declare a program section,..

	global ex8_start
	ORG	p:				; to be located in p RAM
	xref	DATA1X				; see ex8.ctl for allocation
	xref	DATA1Y				; see ex8.ctl for allocation
ex8_start
	move	#DATA1X,r0			; init base pointer to real start
	move	#DATA1Y,r4			; init base pointer to imag start
	move 	#ABC,n2				; get blocksize (= number of figures)
	jsr 	sig_pow				; jump to function now
end_main
	bra	end_main			; execution's termination
	ENDSEC					; end of code
