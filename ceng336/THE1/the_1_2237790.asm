    #INCLUDE <p18f8722.inc>

    CONFIG OSC=HSPLL, FCMEN=OFF, IESO=OFF,PWRT=OFF,BOREN=OFF, WDT=OFF, MCLRE=ON, LPT1OSC=OFF, LVP=OFF, XINST=OFF, DEBUG=OFF   	
UDATA_ACS
  t1	res 1	; used in delay
  t2	res 1	; used in delay
  t3	res 1	; used in delay
	
    oper udata 0x20
    oper
    
    curr_port udata 0x21
    curr_port
    
    num1 udata 0x22
    num1
    
    num2 udata 0x23
    num2
    
    
    ORG 0x00
    goto START
	
	
JANITOR
	clrf LATA
	clrf LATB
	clrf LATC
	clrf LATD
	clrf LATE
	clrf num1
	clrf num2
	clrf curr_port
	clrf oper
	return
	

INIT
	clrf TRISE
	clrf TRISA
	clrf TRISB
	clrf TRISC
	clrf TRISD
	clrf LATA
	clrf LATE
	clrf LATB
	clrf LATC
	clrf LATD
	movlw 0x10
	movwf TRISA
	movlw 0x18
	movwf TRISE
	movlw 0xF
	movwf LATB
	movwf LATC
	movlw 0xFF
	movwf LATD
	clrf num1
	clrf num2
	clrf curr_port
	clrf oper
	return
	
	
DELAY	; Time Delay Routine with 3 nested loops
    MOVLW 82	; Copy desired value to W
    MOVWF t3	; Copy W into t3
    _loop3:
	MOVLW 0xA0  ; Copy desired value to W
	MOVWF t2    ; Copy W into t2
	_loop2:
	    MOVLW 0x9F	; Copy desired value to W
	    MOVWF t1	; Copy W into t1
	    _loop1:
		decfsz t1,F ; Decrement t1. If 0 Skip next instruction
		GOTO _loop1 ; ELSE Keep counting down
		decfsz t2,F ; Decrement t2. If 0 Skip next instruction
		GOTO _loop2 ; ELSE Keep counting down
		decfsz t3,F ; Decrement t3. If 0 Skip next instruction
		GOTO _loop3 ; ELSE Keep counting down
		return
		
		
begin_ra4
    btfsc PORTA,4 ;if ra4 zero skip
    goto wait_ra4
    goto begin_ra4
    
wait_ra4:
    btfsc PORTA,4 ; if ra4=0 it is released so skip, else wait for release
    goto wait_ra4
    movlw 0x1
    movwf oper ; oper = 1, add
    goto begin_check
    
begin_check:
    btfsc PORTA,4 ; ra4=0 -> skip
    goto ra4_press
    btfsc PORTE,3 ; re3=0 -> skip
    goto re3_press
    goto begin_check
      
ra4_press:
    btfsc PORTA,4 ; if ra4=0 skip
    goto ra4_press ; else released, do operns
    goto oper_change
    
oper_change:
    btfss oper,0 ; skip if 1
    goto opn_add
    goto opn_subs
    
opn_add:
    movlw 0x1
    movwf oper
    goto begin_check
    
opn_subs:
    movlw 0x0
    movwf oper
    goto begin_check
    
    
    
re3_press:
    btfsc PORTE,3
    goto re3_press ; if re3 != 0 wait for release, else next line
    goto change_port
    goto nums_check
    
nums_check:
    btfsc PORTE,3 ; ra4=0 -> skip
    goto re3_press
    btfsc PORTE,4 ; re3=0 -> skip
    goto re4_press
    goto nums_check
    
change_port:
    btfsc curr_port,1 ; if curr_port == xxx10 goto portD so fall down
    goto curr_portD
    incf curr_port ;else inc port
    goto nums_check
   
curr_portD:
    btfsc oper,0 ; if op=0 skip
    goto adder
    goto substracter
    
adder:
    movlw 0x0
    movf num2,0
    addwf num1
    goto adder_led
    
adder_led:
    movlw 0x0
    cpfseq num1 ; if num1 == 0 skip
    goto add_portD_led_onestep
    return
    
substracter:
    movf num2,0
    cpfsgt num1 ; skip if num1 < num2 ---fall down:::: num1-num2
    goto sub_cont
    subwf num1 ; result in num1
    goto subser_led
    
sub_cont:
    movf num1,0
    subwf num2 ; num2-num1
    movf num2,0
    movwf num1 ; result in num1, again
    goto subser_led
    
subser_led:
    movlw 0x0
    cpfseq num1
    goto sub_portD_led_onestep
    return
    
sub_portD_led_onestep:
    decf num1
    rlncf LATD
    incf LATD
    goto subser_led
    
add_portD_led_onestep:
    decf num1
    rlncf LATD
    incf LATD
    goto adder_led
    
re4_press:
    btfsc PORTE,4
    goto re4_press
    goto inc_num
    
inc_num:
    btfsc curr_port,0 ; if curr_port == 2(portC) skip
    goto inc_num1
    goto inc_num2
    
inc_num1:
    btfsc num1,2 ; if num1!=4 skip
    goto num1_reset
    incf num1
    goto portb_led
    
portb_led:
    rlncf LATB
    incf LATB
    goto nums_check
    
num1_reset:
    clrf num1
    clrf LATB
    goto nums_check
    
inc_num2:
    btfsc num2,2
    goto num2_reset
    incf num2
    goto portc_led
    
portc_led:
    rlncf LATC
    incf LATC
    goto nums_check
    
num2_reset:
    clrf num2
    clrf LATC
    goto nums_check
  
_MAIN:
    call begin_ra4
    call DELAY
    call JANITOR
    goto _MAIN
  	
START:
	call INIT
	call DELAY
	call JANITOR
	goto _MAIN
	
	
	
	
	END
