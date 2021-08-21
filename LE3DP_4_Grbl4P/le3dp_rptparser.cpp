#include "le3dp_rptparser.h"

JoystickReportParser::JoystickReportParser(JoystickEvents *evt) :
	joyEvents(evt)
{}

void JoystickReportParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf)
{
	bool match = true;

	// Checking if there are changes in report since the method was last called
	for (uint8_t ii=0; ii<RPT_GAMEPAD_LEN; ii++) {
		if( buf[ii] != oldPad[ii] ) {
			match = false;
			break;
		}
  }
	// Calling Game Pad event handler
	if (!match && joyEvents) {
		joyEvents->OnGamePadChanged((const GamePadEventData*)buf);

		for (uint8_t ii=0; ii<RPT_GAMEPAD_LEN; ii++) oldPad[ii] = buf[ii];
	}
}

void JoystickEvents::OnGamePadChanged(const GamePadEventData *evt){
  boolean terse=true;
  //terse=false;
    
  if(terse){   
    String xNum,yNum,rNum,tNum,bNum;
    xNum=String(evt->x, HEX);
    yNum=String(evt->y, HEX);
    rNum=String(evt->twist, HEX); /* r for rotate */
    tNum=String(evt->slider, HEX);  /* t for throttle */
    
    /* pack the buttons_a, buttons_b, and hat into a single int
     *
     * buttons_a uses 8 bits
     *    A        1        Trigger
     *    A        2        Thumb Switch
     *    A        4        Near Button Left of Hat
     *    A        8        Near Button Right of Hat
     *    A       10        Far Button Left of Hat
     *    A       20        Far Button Right of Hat
     *    A       40        Lower Far Button on Base
     *    A       80        Upper Far Button on Base
     * buttons_b uses 4 bits
     *    B        1        Lower Mid-Distance Button on Base
     *    B        2        Upper Mid-Distance Button on Base
     *    B        4        Lower Near Button on Base
     *    B        8        Upper Near Button on Base
     * hat uses 4 bits
     *             8       hat centered
     *             0       hat Up
     *             1       hat UpperRight
     *             2       hat Right
     *             3       hat DownRight
     *             4       hat Down
     *             5       hat DownLeft
     *             6       hat Left
     *             7       hat UpperLeft
     *
     * buttons_a is the 8 LSB (Least Significant Bits)
     * buttons_b is the 4 bits above buttons_a
     * hat       is the 4 MSB
      */
    bNum=String(evt->buttons_a+(evt->buttons_b<<8)+(evt->hat<<12),HEX);

    /* will be outputting as hex, and uppercase looks more pretty */
    xNum.toUpperCase();
    yNum.toUpperCase();
    rNum.toUpperCase();
    tNum.toUpperCase();
    bNum.toUpperCase();    
    
    /* code legacy from before packing buttons_a, buttons_b, and the hat */
    //String hNum=String(evt->hat, HEX);
    //hNum.toUpperCase();
    //String ba  =String(evt->buttons_a,HEX);
    //String bb  =String(evt->buttons_b,HEX);
    //Serial.print("J0Y,"+xNum+","+yNum+","+rNum+","+tNum+","+hNum+","+bNum+","+ba+","+bb);
    
    Serial.print("J0Y,"+xNum+","+yNum+","+rNum+","+tNum+","+bNum);
    
  } else {
  	Serial.print("X: ");
  	PrintHex<uint16_t>(evt->x, 0x80);
  	Serial.print(" Y: ");
  	PrintHex<uint16_t>(evt->y, 0x80);
  	Serial.print(" Hat Switch: ");
  	PrintHex<uint8_t>(evt->hat, 0x80);
  	Serial.print(" Twist: ");
  	PrintHex<uint8_t>(evt->twist, 0x80);
  	Serial.print(" Slider: ");
  	PrintHex<uint8_t>(evt->slider, 0x80);
    Serial.print(" Buttons A: ");
  	PrintHex<uint8_t>(evt->buttons_a, 0x80);
  	Serial.print(" Buttons B: ");
  	PrintHex<uint8_t>(evt->buttons_b, 0x80);
  }
  Serial.println("");
}
