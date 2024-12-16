# Building a NetLog MK1
To build a NetLog MK1, you need the following tools and parts:

| Tools                 |
|-----------------------|
| Soldering iron        |
| Flushcutters          |
| 3D printer            |
| Wire strippers        |
| 2.5mm hex screwdriver |

| Parts                          |
|--------------------------------|
| LOLIN S3 Pro dev board         |
| 4x M3 threaded inserts         |
| 4x M3 hex socket cap screws    |
| GY-NEO6MV2 GPS module          |
| 28 x 28 x ~9mm uFL GPS antenna |
| JST SH 4-pin connector         |
| 4x JST SH cables               |

## Step 1: Building the case
Print `case.stl` with PLA or PET(G). Next, insert four M3 threaded inserts into the screw holes.
![Inserting the inserts](../pictures/insert.jpg)

When you're done, the case should look something like this:
![Finished case](../pictures/case.jpg)

## Step 2: Preparing the GPS module
Next, we'll connect a JST SH (1.0mm) 4-pin cable to the GPS module.

I recommend getting a set that has pre-crimped cables, as manually crimping these connectors can be difficult.
![JST connector with cables](../pictures/jst_conn.jpg)

Insert the cables into the connector and cut the ends so they are about
5cm long. Strip the ends of the cables, then solder them to the GPS module with the pinout shown in the image below:
![Connector pinout](../pictures/gps_wiring.png)

This is what the result should look like:
![Result](../pictures/solder_gps.jpg)

For the GPS module to fit in the case, you will need to trim the solder blobs on the bottom of the GPS module:
![Flush solder blobs](../pictures/trim_gps.jpg)

## Step 3: Printing the cover
Print `lid_text.stl` or `lid_logo.stl` with **clear** PLA or PET(G). This is so that the LEDs can shine through the cover.
![Cover](../pictures/lid.jpg)
Those weird towers on the cover hold the components in place.

> `lid_text.stl` reads _NetLog MK1_ on the bottom of the cover, whereas `lid_logo.stl` has the the NetLog MK1 logo (found in `logo.svg`) in the
    bottom right corner. I'll use `lid_text.stl` for the tutorial.

## Step 4: Assembling the device
Now we have the case, cover, and components ready for assembly.
![Components](../pictures/components.jpg)

Put the components in the case like this:
![Components in case](../pictures/assembled_unconnected.jpg)

> Note: Due to tight tolerances, getting the components in their slots may take some force.

Next, connect the JST cable of the GPS module into the S3 Pro's I2C port, and the GPS antenna's uFL connector to the GPS module.
![Case with cables connected](../pictures/case_finished.jpg)

> While the 4-pin JST connector on the S3 Pro is intended for I2C,
    the versatility of the ESP32's GPIO pins allows us to use it for
    UART as well.

> Before attaching the cover, it may be a good idea to [flash the firmware.](flashing_tutorial.md)

Now, we can screw the cover onto the case:
![Screwing cover onto case](../pictures/screw_lid.jpg)

With this, your NetLog MK1 is finished!
In case you haven't already, you now just have to [flash the firmware.](flashing_tutorial.md)
