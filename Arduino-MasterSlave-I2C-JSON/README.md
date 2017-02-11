# Arduino Master-Slave I2C and JSON Parsing
My contribution to a one-day hackathon during Summer 2016 on a 4-man team. The idea was to have a sort of embedded "dashboard" for credit card rewards information -- at a glance, the user would be able to see how many points/miles/cash they have earned for a particular credit card. As a part of the hackathon, the other team members built a web interface to hit [Capital One's DevExchange Rewards API](https://developer.capitalone.com/products/rewards/documentation/#_retrieve_rewards_accounts) where the user could select a credit card to display on the embeded dashboard. On my end, I developed this system of three Arduinos to receive rewards information in a JSON format (sent over USB serial), which would display it on different hardware components (LED's, bar graph, 7-Seg).

Because of the hardware that was available to us (ex. 7-Segs talking up more than half of an Arduino's digital pins), we needed multiple Arduinos to display the multiple pieces of information. To do this, one Arduino (directly connected over USB to the machine hosting the website) was designated the "master" and communicated to two other "slave" Arduinos over I2C to deliver their data to display.

Uses the Arduino's [SevSeg Library](http://playground.arduino.cc/Main/SevenSegmentLibrary) and the [ArduinoJson Library](https://github.com/bblanchon/ArduinoJson).

## Final "Product"
![Image of the final dashboard in use](https://github.com/NickCapurso/Embedded-Systems-Tinkering/blob/master/Arduino-MasterSlave-I2C-JSON/Setup.png "Image of the final dashboard in use")
Information being displayed (from left-to-right, using mock data):
* SevSeg - Rewards value (IIRC, it was in thousands).
* 3 Stacked LEDs - Rewards type (Miles / Points / Cash) (i.e. so the display reads "105,000 Points").
* Bar Graph - Just does a fill up & empty animation. Intended for it to be a "progress to next rewards milestone" display (which it was set up to do in the Sketch), but we found that there aren't really "milestones" that make sense for Capital One rewards.
* SevSeg - Last 3 of the credit card.
* 2 Stacked LEDs - Credit card type (MC / VISA) (i.e. so the display reads "Visa ending in x582).

