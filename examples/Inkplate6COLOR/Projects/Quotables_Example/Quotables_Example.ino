/*
    Quotables example for Soldered Inkplate 6COLOR
    For this example you will need only USB cable and Inkplate 6COLOR.
    Select "Inkplate 2(ESP32)" from Tools -> Board menu.
    Don't have "Inkplate 2(ESP32)" option? Follow our tutorial and add it:
    https://e-radionica.com/en/blog/add-inkplate-6-to-arduino-ide/

    This example shows you how to use simple API call without API key. Response
    from server is in JSON format, so that will be shown too how it is used. What happens
    here is basically ESP32 connects to WiFi and sends API call and server returns HTML 
    document containing one quote and some information about it, then using library ArduinoJSON
    we extract only quote from JSON data and show it on Inkplate 6COLOR. After displaying quote
    ESP32 goes to sleep and wakes up every 300 seconds to show new quote(you can change time interval).

    IMPORTANT:
    Make sure to change wifi credentials below
    Also have ArduinoJSON installed in your Arduino libraries, download here: https://arduinojson.org/
    You can deserialize JSON data easily using JSON assistant https://arduinojson.org/v6/assistant/

    Want to learn more about Inkplate? Visit www.inkplate.io
    Looking to get support? Write on our forums: http://forum.e-radionica.com/en/
    7 April 2022 by Soldered
*/

// Next 3 lines are a precaution, you can ignore those, and the example would also work without them
#ifndef ARDUINO_INKPLATECOLOR
#error "Wrong board selection for this example, please select Inkplate 6COLOR in the boards menu."
#endif

//---------- CHANGE HERE  -------------:

// Put in your ssid and password
char ssid[] = "";
char pass[] = "";

//----------------------------------

// Include Inkplate library to the sketch
#include "Inkplate.h"

// Include fonts used
#include "Fonts/exmouth_32pt7b.h"

// Our networking functions, declared in Network.cpp
#include "Network.h"

// create object with all networking functions
Network network;

// create display object
Inkplate display;

// Delay between API calls in seconds, 300 seconds is 5 minutes
#define DELAY_S 300

// Our functions declared below setup and loop
void drawAll();

char quote[128]; // Buffer to store quote
char author[64];

void setup()
{
    // Begin serial communitcation, sed for debugging
    Serial.begin(115200);

    // Initial display settings
    display.begin();
    display.setTextWrap(true); // Set text wrapping to true
    display.setTextColor(BLACK);

    // Our begin function
    network.begin();

    while (!network.getData(quote, author))
    {
        Serial.println("Retrying retriving data!");
        delay(1000);
    }

    // Our main drawing function
    drawAll();
    // Full refresh
    display.display();

    // Go to sleep before checking again
    // This is set in microseconds, so it needs to be
    // multiplied by million to get seconds
    esp_sleep_enable_timer_wakeup(1000000 * DELAY_S);
    (void)esp_deep_sleep_start();
}

void loop()
{
    // Never here
}

    

// Our main drawing function
void drawAll()
{
    uint8_t rows = strlen(quote) / 43, row = 0;
    display.setFont(&exmouth_32pt7b); // Set custom font
    display.setTextSize(1);
    display.setTextColor(random(0,7)); //Set text color to random color
    display.setCursor(48, display.height() / 2 - 24 * rows); //Place text in the middle
    uint16_t cnt = 0;
    while (quote[cnt] != '\0')
    {
        if (display.getCursorX() > display.width() - 90 && quote[cnt] == ' ')
        {
            row++;
            display.setCursor(48, display.height() / 2 - 24 * rows + row * 48);
        }
        display.print(quote[cnt]);
        cnt++;
    }
    uint16_t w,h;
    int16_t x,y;
    display.getTextBounds(author, 0, 0, &x, &y, &w, &h);
    display.setCursor(display.width() - w - 50, display.height() - 30); // Set cursor to fit author name in lower right corner
    display.print("-");
    display.println(author); // Print author
}
