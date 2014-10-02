This demo shows HID mouse function using USB ROM API.

The code is located in 3 files:
1. main.c - initialization and button handling.
2. hid_usbdesc.c - USB descriptors (device, configuration, string and others).
3. usbd_hid.c - USB callback functions (3 callbacks are used: on set configuration, on IN endpoint request and on get HID report).

The program algorithm can be described as follows:
1. Main initializes USB and enables pull-up to signal host about device connection.
2. Host sends SET_CONFIGURATION standard request.
3. When this request is received by the device (configuration event), device sends mouse report to the host.
4. When report is sent (IN event), device sends it again. So, mouse report is transmitted continuously.
5. Main loop is scanning buttons.
   When no buttons are pressed, X and Y displacement  values of the report are zero. When some buttons are pressed X and/or Y values equals to +1 or -1. Since mouse report is transmitted continuously, it is transmitted with zero values when no buttons are pressed and with non-zero when some button is pressed.
