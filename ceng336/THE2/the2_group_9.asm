;Alperen Oguz Cakmak 2237162
;Deniz Polat 2237790
;Deniz Turan Caglarca 2237154
;THE2_v2 on simulation environment
;We did not test on board.
#INCLUDE <p18f8722.inc>

    CONFIG OSC=HSPLL, FCMEN=OFF, IESO=OFF,PWRT=OFF,BOREN=OFF, WDT=OFF, MCLRE=ON, LPT1OSC=OFF, LVP=OFF, XINST=OFF, DEBUG=OFF
    errorlevel -207
;variables 
var1 udata 0x20
var1

shift_buttons_enable udata 0x21
shift_buttons_enable

tmr1_hi udata 0x22
tmr1_hi

tmr1_lo udata 0x23
tmr1_lo

new_ball_index udata 0x24
new_ball_index

score udata 0x25
score

level udata 0x26
level

shift_amount udata 0X27 ; level güncelleyen bunu da güncellesin (level1 -> 1, level2 -> 3, level3 -> 5)
shift_amount

shift_flag udata 0x28
shift_flag

bar_pos udata 0x29		;ilk 4 bit bar? represent ediyor -> 00001100 -> bar AB pozisyonunda
bar_pos

hp_amount udata 0x2A
hp_amount

ball_count udata 0x2B
ball_count
 
temp_shift udata 0x2C
temp_shift
 
post_scaler udata 0x2D
post_scaler
 
temp_hp udata 0x2E
temp_hp
;;;;;;
    ORG 0x00
    goto START 
    ORG 0x08
    goto timer0_interrupt

TIMER_INIT
    movlw B'11000111'
    movwf T0CON
    movlw B'00000110'
    movwf TMR0L
    movlw B'11001001'
    movwf T1CON
    return
    
INTERRUPT_INIT
    bcf RCON, 0
    movlw B'00100000'
    movwf INTCON
    return

INIT
    clrf TRISA
    clrf TRISB
    clrf TRISC
    clrf TRISD
    clrf TRISG
    clrf TRISH
    clrf TRISJ

    clrf LATA
    clrf LATB
    clrf LATC
    clrf LATD
    clrf LATG
    clrf LATH
    clrf LATJ

    movlw 0xF
    movwf ADCON1
    
    movlw B'00000000'	;0 vermek istediklerinizi buraya yazabilirsiniz
    movwf var1
    movwf new_ball_index
    movwf shift_flag
    movwf ball_count
    movwf score
    movwf TRISA
    movwf TRISB
    movwf TRISC
    movwf TRISD
    movwf TRISJ
    
    movlw B'00001101'
    movwf TRISG
    
    movlw B'00001001'
    movwf TRISH

    movlw B'00100000'
    movwf LATB
    movwf LATA
    
    movlw B'00000001'	;Set display level 1
    movwf LATH
    movwf shift_amount
    movlw B'00000110'
    movwf LATJ
    
    
    movlw B'00001000'	;Set display health 5
    movwf LATH
    movlw B'01101101'
    movwf LATJ
    
    movlw B'00000101'
    movwf hp_amount
    movwf temp_hp
    
    movlw B'00001100'
    movwf bar_pos
    
    movlw B'00000001' 	;1 vermek istediklerinizi buraya yazabilirsiniz
    movwf shift_buttons_enable
    movwf level

    movlw 0xB2
    movwf post_scaler

    return

SHIFT_BUTTON_TASK		;handles RG2 and RG3 button presses, shifts bar, increments score if new place of bar catches a ball
    btfsc PORTG, 2		;if RG2 pressed move bar to right
    goto SHIFT_RIGHT
    btfsc PORTG, 3		;if RG3 pressed move bar to left
    goto SHIFT_LEFT
    bsf shift_buttons_enable, 0
    return

    SHIFT_RIGHT: 	;shift bar from anywhere to one block right if possible
	btfss shift_buttons_enable, 0
	goto finish_shift_right
	btfss LATA, 5
	goto check_b
    shift_ab_to_bc:
	btfsc LATC, 5
	INCF score
	bsf LATC, 5
	bcf LATA, 5
	movlw B'00000110'
	movwf bar_pos
	goto finish_shift_right
    check_b:
	btfss LATB, 5
	goto finish_shift_right
    shift_bc_to_cd:
	btfsc LATD, 5
	INCF score
	bsf LATD, 5
	bcf LATB, 5
	movlw B'00000011'
	movwf bar_pos
    finish_shift_right:
	bcf shift_buttons_enable, 0
	btfss PORTG, 2
	bsf shift_buttons_enable, 0
	return

    SHIFT_LEFT:		;shift bar from anywhere to one block left if possible
	btfss shift_buttons_enable, 0
	goto finish_shift_left
	btfss LATD, 5
	goto check_c
    shift_cd_to_bc:
	btfsc LATB, 5
	INCF score
	bsf LATB, 5
	bcf LATD, 5
	movlw B'00000110'
	movwf bar_pos
	goto finish_shift_left
    check_c:
	btfss LATC, 5
	goto finish_shift_left
    shift_bc_to_ab:
	btfsc LATA, 5
	INCF score
	bsf LATA, 5
	bcf LATC, 5
	movlw B'00001100'
	movwf bar_pos
    finish_shift_left:	
	bcf shift_buttons_enable, 0
	btfss PORTG, 3
	bsf shift_buttons_enable, 0
	return
        
HEALTH_DECREASE_DISPLAY
    
    movf temp_hp, 0
    cpfseq hp_amount
    goto led_not_5
    return
    
led_not_5:
    movf hp_amount, 0
    movwf temp_hp
    movlw B'00000100'
    cpfseq hp_amount
    goto led_not_4
    goto decrease_from_5_to_4
led_not_4:
    movlw B'00000011'
    cpfseq hp_amount
    goto led_not_3
    goto decrease_from_4_to_3
led_not_3:
    movlw B'00000010'
    cpfseq hp_amount
    goto decrease_from_2_to_1
    goto decrease_from_3_to_2
    
decrease_from_5_to_4:
    movlw B'00001000'	;Set display health 4
    movwf LATH
    movlw B'01100110'
    movwf LATJ
    return
    
    decrease_from_4_to_3:
    movlw B'00001000'	;Set display health 3
    movwf LATH
    movlw B'01001111'
    movwf LATJ
    return
    
    decrease_from_3_to_2:
    movlw B'00001000'	;Set display health 2
    movwf LATH
    movlw B'01011011'
    movwf LATJ
    return
    
    decrease_from_2_to_1:
    movlw B'00001000'	;Set display health 1
    movwf LATH
    movlw B'00000110'
    movwf LATJ
    return
        
LEVEL_UP_DISPLAY
    movlw B'00000011'
    cpfseq level
    goto increase_from_1_to_2
    goto increase_from_2_to_3
    
    increase_from_1_to_2:
	movlw B'00000001'	;Set display level 2
	movwf LATH
	movlw B'01011011'
	movwf LATJ
	return
      
    increase_from_2_to_3:
    	movlw B'00000001'	;Set display level 3
    	movwf LATH
    	movlw B'01001111'
    	movwf LATJ
        return

CHECK_LEVEL
    movlw B'00000101'
    cpfseq ball_count ;Check if 5 balls have been generated
    goto check_15
    goto display_2

display_2:
    INCF level
    movlw B'00000011'
    movwf shift_amount
    call LEVEL_UP_DISPLAY
    return

check_15:
    movlw B'00001111'
    cpfseq ball_count ;Check if 15 balls have been generated
    return
    INCF level
    movlw B'00000101'
    movwf shift_amount
    call LEVEL_UP_DISPLAY
    return
;;;;;;;;; burdan yukars? fonksiyonlar a??as? labellar ?eklinde olsun

timer0_interrupt:
    btfss INTCON, 2  ; is tmr0 flag set
    retfie FAST
    bcf INTCON, 2 ; clr tmr0 flag so not stuck in that interrupt
    movlw 0xFF
    cpfseq post_scaler
    goto increment_postscaler
    goto reset_postscaler
  
increment_postscaler:
    INCF post_scaler
    retfie FAST

reset_postscaler:
    btfss level, 1
    goto level1_postscale
    btfss level, 0
    goto level2_postscale
    goto set_postscale_level3
    
level1_postscale:
    movlw 0x03
    cpfsgt ball_count
    goto set_postscale_level1
    goto set_postscale_level2
    
level2_postscale:
    movlw 0x0D
    cpfsgt ball_count
    goto set_postscale_level2
    goto set_postscale_level3
    
set_postscale_level1:
    movlw 0xB3
    movwf post_scaler
    goto interrupt_start

set_postscale_level2:
    movlw 0xC3
    movwf post_scaler
    goto interrupt_start

set_postscale_level3:
    movlw 0xCB
    movwf post_scaler
    goto interrupt_start

interrupt_start:
    btfsc bar_pos, 3
    goto check_score_porta 
    btfsc LATA, 5
    decf hp_amount
    goto checkfor_portb
check_score_porta:
    btfsc LATA, 4
    INCF score
    goto checkfor_portb
    
checkfor_portb:
    btfsc bar_pos, 2
    goto check_score_portb
    btfsc LATB, 5
    decf hp_amount
    goto checkfor_portc
check_score_portb:
    btfsc LATB, 4
    INCF score
    goto checkfor_portc
        
checkfor_portc:
    btfsc bar_pos, 1
    goto check_score_portc
    btfsc LATC, 5
    decf hp_amount
    goto checkfor_portd
check_score_portc:
    btfsc LATC, 4
    INCF score
    goto checkfor_portd
     
checkfor_portd:
    btfsc bar_pos, 0
    goto check_score_portd
    btfsc LATD, 5
    decf hp_amount
    goto move_balls
check_score_portd:
    btfsc LATC, 4
    INCF score
    goto move_balls
     
move_balls:  ;;  move balls 1 unit down
    movf LATA, 0  
    andlw B'00011111' ;; take ra0..ra4
    movwf var1
    rlncf var1
    btfsc bar_pos, 3  ;; if bar is on ra5
    bsf var1, 5  ;; light it, else skip
    movf var1, 0
    movwf LATA
    movf LATB, 0   ;; for portB
    andlw B'00011111'
    movwf var1
    rlncf var1
    btfsc bar_pos, 2
    bsf var1, 5
    movf var1, 0
    movwf LATB    
    movf LATC, 0  ;; for PORTC
    andlw B'00011111'
    movwf var1
    rlncf var1
    btfsc bar_pos, 1 
    bsf var1, 5
    movf var1, 0
    movwf LATC
    movf LATD, 0  ;; for PORTD
    andlw B'00011111'
    movwf var1
    rlncf var1
    btfsc bar_pos, 0
    bsf var1, 5 
    movf var1, 0
    movwf LATD
    call CHECK_LEVEL
    call HEALTH_DECREASE_DISPLAY
    movlw d'29'
    cpfsgt ball_count
    goto random_ball_generate
    INCF ball_count
    retfie FAST

;;;; generation of new ball starts here ----------------
random_ball_generate:
    INCF ball_count
    movf tmr1_lo, 0
    movwf new_ball_index
    movlw B'00000111' ; take rightmost 3 bit
    andwf new_ball_index, 1
    btfsc new_ball_index, 2
    bcf new_ball_index, 2 ; modulo operation if number geq 4
    btfsc new_ball_index, 1 ;if bit1 set, num is 2 or 3
    goto rc_or_rd
    btfsc new_ball_index, 0
    goto generate_at_rb
    goto generate_at_ra

rc_or_rd:
    btfsc new_ball_index, 0
    goto generate_at_rd; new_ball_index == 1, generate ball on rb0
    goto generate_at_rc

generate_at_ra:
    bsf LATA, 0
    goto update_tmr1

generate_at_rb:
    bsf LATB, 0
    goto update_tmr1

generate_at_rc:
    bsf LATC, 0
    goto update_tmr1

generate_at_rd:
    bsf LATD, 0
    goto update_tmr1

update_tmr1:
    movf shift_amount, 0
    movwf temp_shift
update_tmr1_continue:    
    movlw 0x0
    cpfseq temp_shift
    goto exchange_bits
    retfie FAST ;; interrupt burda bitiyor bence, yeni top geldi daha nabak

exchange_bits:   ;; shift right for 16 bits starts here
    movf tmr1_lo, 0
    andlw B'00000001'
    movwf shift_flag
    btfss tmr1_hi, 0
    goto change_tmr1_lo_to_0
    bsf tmr1_lo, 0
    goto exchange_tmr1
    
change_tmr1_lo_to_0:
    bcf tmr1_lo, 0
    goto exchange_tmr1
    
exchange_tmr1:
    btfss shift_flag, 0
    goto change_tmr1_hi_to_0
    bsf tmr1_hi, 0
    goto onebit_shr_tmr1
    
change_tmr1_hi_to_0:
    bcf tmr1_hi, 0
    goto onebit_shr_tmr1
    
onebit_shr_tmr1:
    decf temp_shift
    movf tmr1_lo, 0
    rrncf tmr1_lo
    rrncf tmr1_hi
    goto update_tmr1_continue  ;; shift right 16 bits ends here

;;;; generation of new ball ends here ---------------------------------

START:
    call INIT
    call INTERRUPT_INIT
    call TIMER_INIT
    ;bsf INTCON, 7
RG0_PRESS:
    btfss PORTG, 0		;see if RG0 pressed
    goto RG0_PRESS 
    movf TMR1H, 0 ; for random ball generation, val of tmr1 when rg0 is pressed
    movwf tmr1_hi
    movf TMR1L, 0
    movwf tmr1_lo ;;;;; use the val of Timer1 interrupt at the time of RG0 button is pressed at the beginning of the game	
RG0_RELEASE:
    btfsc PORTG, 0		;see if RG0 released
    goto RG0_RELEASE
    bsf INTCON, 7
    goto MAIN_LOOP		;go to main loop if pressed and released
    
MAIN_LOOP:
    call SHIFT_BUTTON_TASK
    movlw B'00100100'
    cpfslt ball_count 	;Check if ball_count is 35
    goto START
    movlw B'00000000'	
    cpfseq hp_amount	;Check if hp is 0
    goto MAIN_LOOP
    goto START
    END
