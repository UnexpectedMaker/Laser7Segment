# Laser7Segment Display Library
An API that lets you display numbers and letters on Laser7Segment RGB LED displays that are available from [tindie](https://www.tindie.com/products/seonr/laser-segment-display/)

You can find the Open Source hardware PCB design and Gerber files here too as well as the DXF laser cut design files

## Installing and Use
The downloaded code should be added to your arduino IDE libraries folder, and the library requires the [Adafruit NeoPixel library](https://github.com/adafruit/Adafruit_NeoPixel) for it to function.

To include the Laser7Segment library in your project:

    #include <Laser7Segment.h>

You can then initialise the display with the following line that includes the number of Laser7Segment digits and the GPIO to control them:

    Laser7Segment disp( 5, 4 );

You then start the display with the bebin method, passing the brightness:

    disp.Begin(80);

The you simply pass the display the String you would like displayed and a color:

    disp.DisplayText( "1234", disp.Color( 255,0,0) );

Enjoy!

# Support Unexpected Maker

I love designing, making and releasing our projects as open source. I do it because I believe it’s important to share knowledge and give back to the community, like many have done before me. It helps us all learn and grow.

That said, a lot of time, effort and finances have gone into designing and releasing these files, so please consider supporting me by buy some of my TiinyPICO products:

https://tinypico.com/shop

Or by buying one of our products on tindie:

https://www.tindie.com/stores/seonr/

Or by becoming a Patron:

https://www.patreon.com/unexpectedmaker


# Unexpected Maker
http://youtube.com/c/unexpectedmaker

http://twitter.com/unexpectedmaker

https://www.facebook.com/unexpectedmaker/

https://www.instagram.com/unexpectedmaker/

