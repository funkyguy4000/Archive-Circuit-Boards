/*
 * BRE.h
 *
 *  Created on: Dec 27, 2013
 *      Author: ShannonS
 */

#ifndef BRE_H_
#define BRE_H_

void configureClock(clock clk, port p, int speed){
    configure_clock_src(clk, p);
    configure_clock_rate(clk, speed, 1);
};

char recieveAck(port p){
   char AckVal;
   p :> AckVal;
   if(AckVal == 0){
       return 0;
   }
   else{
       return 1;
   }
}


#endif /* BRE_H_ */
