;******************   Motorola DSP56300   ******************
;*** 	        memory configuration file		 ***
;***********************************************************
		

START		Fmain			;define entry point

; allocate sections in memory

RESERVE		p:$0..$3fff			; reserve int vecs
SECTION 	ex8_code	p:$4000		; allocate program

SECTION	ex8_data	l:			; allocate long data
						; for look up table
	
SECTION 	ex8_data	x:		; allocate data
SECTION 	ex8_data	y:		; allocate data
