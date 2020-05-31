/*
    GROUP 9
    Alperen Oguz Cakmak 2237162
    Deniz Polat 2237790
    Deniz Turan Caglarca 2237154
    
    STRUCTURE DETAILS
    
    The structure is based on 5 different interrupts, RB4 button interrupt, A/D conversion interrupt and 3 timer interrupts (Timer0, 1 and 2).
    
    Main loop checks the flags which could be set in isr in order to take action. We avoided calling functions in our isr so we used flags and did corresponding calculations and updates in the main loop.
    
    In our isr we checked different interrupt flags in order to set corresponding flags.
    
    Timer0 is interrupted every 50ms in order to start a new A/D conversion as we are asked to. Since the timer0 interrupt is called at the very beginning of our code, A/D conversion starts at the second timer0 interrupt. 
    Timer1 is interrupted also every 50ms but it has a software level postscale which is written by us to measure 5s and 500ms time interval. While in guessing phase, postscaler counts up to 100 and then sets the game end flag. After that while game end flag is set our code measures 500ms using game_end_postscale in order to blink at the correct times.
    Timer2 interrupt is used to set tmr2interrupt flag and ensures that the button push is debounced at pressing and releasing moments.
    
    A/D conversion interrupt is only used for sampling the converted value and light up the 7-segment display. It sets the sample_value flag so that in the main loop we can do the corresponding calculations and updates
    RB4 interrupt is used to detect rb4 presses, sets the portb_read flag and take the current value in order to check if it is correct guess or not.
    
    
    
*/

#pragma config OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

#include <xc.h>
#include "breakpoints.h"
/* Initialize variables */
unsigned int rb4pressed = 0; /* a flag indicating rb4 button is pressed */
unsigned int adcWhenRb4Pressed = 0; /* holds sample adc result at the time when rb4 is pressed */
unsigned int game_end_flag = 0; /* a flag indicating 5secs has passed or a correct guess is made */
unsigned int game_end_postscale = 0; /* a softwared postscaler to count 500ms for blink for last 2 secs */
unsigned int timer1_postscale = 0; /* a postscaler to hold 5secs with timer1 */
unsigned int sample_value = 0; /* a flag showing A/D conversion has happened, used per 50ms */
unsigned int blink_number = 0;  /* this flag is used in the last 2 seconds. special_number is shown or ports are closed depending upon this flag's being odd or even */
unsigned int blink_flag = 5; /* a flag to call blink() function not always but per 500ms, initialized as 5 in order to compare with 0,1,2,3 and not to conflict */
unsigned int init_flag = 1; /* indicates that game needs to be configured with initial configuration, initialized as 1 and once configured, reseted until game restarts again. */
unsigned int restart_flag = 0; /* this flag is 1 when game is over and a new game begins */
unsigned int tmr2interrupt = 0; /* shows that timer2 has interrupted */
unsigned int portb_read = 0; /* holds the value of PORTBbits.RB4 to use as a flag */
unsigned int adcInt = 0; /* samples adc_value. used to convert the adc_value not in isr but in the main loop */
unsigned int close_rb4 = 0; /* used to handle debouncing of rb4 */

void display(int number){
/* Function for displaying and blinking digit on 7 Segment Display 
    Always turn RH3 on, if we want to close display number 10 is used*/
    PORTHbits.RH3 = 1;
    LATHbits.LH3 = 1;
	if(number == 10){
    	LATJ = 0;
        PORTJ = 0;
    }else{
        if(number == 0){
        	LATJ = 0b00111111;
            PORTJ = 0b00111111;
        }else if(number == 1){
        	LATJ = 0b00000110;
            PORTJ = 0b00000110;
        }else if(number == 2){
        	LATJ = 0b01011011;
            PORTJ = 0b01011011;
        }else if(number == 3){
        	LATJ = 0b01001111;
            PORTJ = 0b01001111;
        }else if(number == 4){
        	LATJ = 0b01100110;
            PORTJ = 0b01100110;
        }else if(number == 5){
        	LATJ = 0b01101101;
            PORTJ = 0b01101101;
        }else if(number == 6){
        	LATJ = 0b00111111;
            PORTJ = 0b00111111;
        }else if(number == 7){
        	LATJ = 0b00000111;
            PORTJ = 0b00000111;
        }else if(number == 8){
        	LATJ = 0b01111111;
            PORTJ = 0b01111111;
        }else if(number == 9){
        	LATJ = 0b01101111;
            PORTJ = 0b01101111;
        }
    }
    return;
}


int mapADCToDigit(int binary) {     /* this function takes pure value coming from A/D conversion and converts to a digit [0..9] */
    int digitMappedVal = (binary - 1) / 102;
    if(digitMappedVal == 10) digitMappedVal--;  /* last portion is 4 bits more (1024 is not dividable to 10) so if the value is equal to 1020+ make it digit 9 */
    return digitMappedVal;
}


void blink() {  /* this function is called when the game has ended for two seconds, blinking with a 500ms period, wrt a flag blink_number */
    if(blink_number % 2) { /* if blink_number is odd, turn off */
        display(10);        /* 10 is just a flag to display function to turn off */
        latjh_update_complete();
    }
        else { /* blink number is even, so show the special number  */
        display(((int)special_number()));
        latjh_update_complete();
    }

    return;
}


void init(){ /* initialize variables, ports */
    /* Global variables reseted */
    rb4pressed = 0;
    game_end_flag = 0;
    game_end_postscale = 0;
    timer1_postscale = 0;
    sample_value = 0;
    blink_number = 0;
    restart_flag = 0;
    blink_flag = 5;
    close_rb4 = 0;
    adcWhenRb4Pressed = 0;
    tmr2interrupt = 0;
    portb_read = 0;
    adcInt = 0;
    
    LATB = 0x00;    /* Clear the ports */
    LATC = 0x00;
    LATD = 0x00;
    LATE = 0x00;
    LATH = 0x00;
    LATJ = 0x00;

    TRISB = 0x10;   /* Initialize RB4 as input and others as output. */
    TRISC = 0x00;
    TRISD = 0x00;
    TRISE = 0x00;
    TRISH = 0x10;   /* Initialize AN12 as input */
    TRISJ = 0x00;
    
    return;
}

void init_interrupt() { /* initialize timers and A/D converter */
	INTCON = 0b01101000; /* timer0 and rb port interrupt enabled */
    PIE1 = 0b01000001; /* A/D conversion and timer1 interrupt enabled */
    
    T0CON = 0x82; /* Timer0 initialize */
    TMR0L = 0xDC;   /*Timer0 initial values are set.*/    
    TMR0H = 0x0B;
    
    ADCON0 = 0x30;  /* A/D conversion registers initialized. */
    ADCON1 = 0x00;
    ADCON2 = 0xA2;

    TMR1L = 0xDC;   /*Timer1 initial values are set.*/
    TMR1H = 0x0B;
    
    TMR2 = 0x00;    /* Timer2 initial value is set. */
    
    T1CON = 0xF9;   /* Timer1 and Timer2 initialize */
    T2CON = 0b01111011;
    
    TMR2IE = 1;     /* Timer2 interrupt enabled. */
    RBIE = 1;       /* RB port interrupt enabled. */
    ADON = 1;       /* A/D conversion is enabled. */
    
    return;
}

void DisplayArrow(int isDownArrow) {  /* Function takes a flag as parameter, which indicates the arrow's direction. */
    if(isDownArrow) {       /* Make a down arrow on ports c-e [0-3] */
        PORTC = 0b00000100;  
        LATC = 0b00000100;
        PORTE = 0b00000100;
        LATE = 0b00000100;
    }
    else {      /* Make an up arrow */
        PORTC = 0b00000010;
        LATC = 0b00000010;
        PORTE = 0b00000010;
        LATE = 0b00000010;
    }
    PORTD = 0b00001111; /* port d is just a straight line for both arrows */
    LATD = 0b00001111;
    latcde_update_complete();
    return;
}

void RB4Pressed() { /* a function comparing the user's guess with special_number */
    adcWhenRb4Pressed = mapADCToDigit(adcInt);  /* take the value of adc when rb4 is pressed */
    rb4_handled();
    rb4pressed = 0;     /* reset flag */
    int to_be_guessed = ((int)special_number());
    
    if(adcWhenRb4Pressed == to_be_guessed){     /* if the value we read from potentiometer is equal to special_number */
        correct_guess();
        game_end_flag = 1;      /* set game over flag */
        RBIE = 0;       /* game is over so we do not need timer0 or rb4 inputs/interrupts anymore */
        T0IE = 0;
        
        /* Turn off PORTCDE arrow  */
        PORTC = 0;
        LATC = 0;
        PORTE = 0;
        LATE = 0;
        PORTD = 0;
        LATD = 0;
        latcde_update_complete();
        
        game_end_postscale = 0;     /* reset game end postscaler */
        TMR1IF = 1;     /* Timer1 interrupt flag is set to 1 in order to call timer1 isr immediately after successfull guess. This step is required for our solution. */
    } 
    else if(adcWhenRb4Pressed < to_be_guessed) DisplayArrow(0);     /* special_number is greater than user's guess, so we need an up arrow */
    else DisplayArrow(1);   /* special_number is less than user's guess, so we need a down arrow */
    return;
}

void __interrupt() isr(){ /* INTERRUPT SERVICE ROUTINE */
	if (TMR0IF == 1) {  /* Interrupt every 50ms to start A/D conversion again. */
    	TMR0IF = 0;	/* Reset Timer0 interrupt flag */
        TMR0L = 0xDC;	/* Set initial timer0 value as 3036 again */
    	TMR0H = 0x0B;
        
        if(init_flag == 0) {            
            GODONE = 1;
        }
    }
    
    if(ADIF == 1) { /* Interrupt when A/D conversion is finished to sample converted value */
        ADIF = 0;   /* A/D interrupt flag cleared. */
        adc_value = ADRESL + (ADRESH * 256); /* Sample the result of A/D conversion */
        adc_complete();            
        sample_value = 1; /* Set the flag to sample the value at main loop */

    }
	
  	if (TMR1IF == 1) {  /* Interrupt every 50ms but only action at every 500ms */
  	    TMR1IF = 0;		/* Reset Timer1 interrupt flag */
        TMR1L = 0xDC;	/* Set initial timer1 value as 3036 again. */
    	TMR1H = 0x0B;
    	timer1_postscale++;
    	game_end_postscale++;
    		
        if(timer1_postscale == 100){ /* 5 seconds has passed */
    	    game_over();
    	    game_end_flag = 1;      /* The flag that indicated endgame is set */
            game_end_postscale = 1;
            
            /* Turn off PORTCDE arrow  */
            PORTC = 0;
            LATC = 0;
            PORTE = 0;
            LATE = 0;
            PORTD = 0;
            LATD = 0;
            latcde_update_complete();
    	}
        
    	if(game_end_flag == 1){ /* If system is in end game state. */
    	    if(game_end_postscale == 11){ /* 500 ms has passed at the end game. */
                hs_passed();
                game_end_postscale = 1;
    	        blink_number++; /* blink_number indicates how many cycles has passed in end game phase to decide 7-segment on and off */
                if(blink_number == 4){ /* Blinked 4 times, restart game */ 
                    restart_flag = 1;
                    game_end_flag = 0;
                }
    	    } 
    	}
        else if(timer1_postscale % 10 == 0) hs_passed(); /* calls hs_passed breakpoint function in every 500 ms when the game is not over */
    }
  	
    if (RBIF == 1){  /*On Rb4 interrupt read Rb4 and start timer2 */
        portb_read = PORTBbits.RB4;
        RBIF = 0;
        TMR2ON = 1;
        TMR2 = 0x00;
        TMR2IE = 1;
        adcInt = adc_value; /* get adc_value at time of rb4 press for later use */
    }

    if (TMR2IF == 1){ /* Timer2 interrupt at 6.5ms if Rb4 dosn't change*/
        TMR2IF = 0;
        
        /* Only call rb4_press if the correct state is achieved 
            close_rb4 is 1 in pressed state and 0 in unpressed state */
        if (portb_read == 1 && close_rb4 == 0){
            tmr2interrupt = 1;
			close_rb4 = 1;
			TMR2ON = 0;
        }else if(portb_read == 0 && close_rb4 == 1){
            TMR2ON = 0;
			close_rb4 = 0;
        }
    }
}

void main(void) {
    while(1){
	    if (init_flag) {  /* init functions are called only if init_flag is set. */
            init_interrupt();          
            init();
            GIE = 1; /* Enable all interrupts */
            init_complete();
            TMR0IF = 1; /* Timer0 interrupt flag is set to 1 in order to call timer0 isr immediately. This step is required for our solution. */
            init_flag = 0;   /* flag is cleared so that we never restart the game unless it is finished. */
	    } 
	    
	    if(tmr2interrupt) {/* Call rbp4 pressed if tmr2 interrupt flag is set*/
	        RB4Pressed();
	        tmr2interrupt = 0;
	    }
	   
	    if (sample_value) { /* If sample_value flag is set, converted result is written to the 7-segment display.*/
	        display(mapADCToDigit(adc_value));
	        latjh_update_complete();
	        sample_value = 0;   /* flag is reset */
	    }
	   
	    if(game_end_flag){ 
            if(blink_flag != blink_number) {    /* blink flag changes per 500ms in timer1 interrupt. if it is different than blink_number, it means 500ms has passed and special_number should blink. */
                blink();
                blink_flag = blink_number;  /* so the function will not enter here until blink_flag is changed again */
            }
	    }
	   
	    if(restart_flag){ /* If restart flag is set we call restart() and set init_flag to start over. */
	        restart();
	        init_flag = 1;
	    }
    }
	return;
}