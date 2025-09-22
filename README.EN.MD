Para la versión en español, click [aquí].<https://github.com/Saco-de-patatas/W1209-PY/blob/main/README.MD>

**Description:**

The following project involves modifying the W1209 thermostat to
implement some improvements that allow for advanced functions. Some of
these improvements are:

- 31 self-explanatory menus configurable in English or Spanish. (The
  original has 6 or 8, and they require a tutorial.)

- A timer with a setting period of up to 999 days.

- Visual/acoustic warning (alarm) (optional, but recommended)

- Two independent alarms for both heat and cold.

- Possibility to readjust the sensor\'s pullup resistor (R6 must be
  physically changed if necessary)

- Precise temperature control

- Two-digit resolution

- Converting Celsius and Fahrenheit scales (the eternal promise\...)

- Extended temperature ranges (from -273.15°C to 1,000,000,000°C). Now,
  nothing special needs to be done to get the thermostat working
  properly on a Pulltruder. High-temperature applications such as 3D
  printing.

- Up to 5 memories to make it work in different scenarios.

- Adjusting the working conditions (response speed, accuracy, sensor
  cable resistance in case of long distance)

- Even precise adjustment of the ADC in case of an extraneous voltage
  contribution in the sensing circuit (e.g. an electrochemical potential
  due to the joining of different metals).

- Allows you to turn the relay on and off by bypassing the thermostat.

- Possibility of configuring any NTC or PTC resistor, or even any
  component with NTC or PTC response, such as a diode or a mosfet, as a
  sensor.

The modification is based on the HW-557C variant or one with a similar
PCB layout. Modification is possible on other layouts, but it is
necessary to adapt the code to the pinout of the microcontroller used.
The HW-557C version uses the 8-bit, 8-cent FMD FT61E143 microcontroller.
The adaptation uses the 32-bit ARM M0+ microcontroller, model
PY32F002BW15, also priced at 8 cents. The Nuvoton M51blahblah and
STM8S003 versions can be upgraded to the PY32F002BF15 (notice the F),
since this MCU has the same pinout. However, I don\'t currently have any
thermostats with those MCUs, and the GPIO lines would need to be adapted
to the configuration of these micros.

To compile and/or flash the code on the MCU, you can do so using the
tutorial from the great developer IOSETTING that you can find in the
PY32F0 template folder on his github:

<https://github.com/IOsetting/py32f0-template>

You can also take a look at his very interesting blog:

<https://www.cnblogs.com/milton/p/18168592>

NOTE: The first time the thermostat code is flashed on our MCU
(PY32F002B), the SWD lines PA2 (SWC) and PB6 (SWIO) are used. However,
on the first execution of the code, SWIO becomes PA7 (this is a
configuration supported by the MCU), and this means that on the W1209
board, PA7 is the MCU line that is taken to the PROG pin header.
Furthermore, in order to access programming again, it is necessary that
as soon as the MCU is turned on, PA4 is at 0, that is, GND (this is
achieved on the W1209 module by simply holding the "+" button while the
power is connected). This way, the firmware enters programming mode and
enables PA2 and PA7 as SWC and SWIO respectively. Once in this mode, it
is not necessary to keep PA4 at zero, so you can simply release the
button once the display indicates programming mode.

Before going into the user manual for this upgrade, I recommend watching
this YouTube video starting at 40:15.

<https://www.youtube.com/watch?v=GLePONN3KA0&t=40m15s>

The video also details the entire modification process from the very
beginning, from flashing the firmware to turning the device on and
operating it.

<https://www.youtube.com/watch?v=GLePONN3KA0>

The operation of the W1209-PY firmware is based on the simplified
Steinhart-hart equation, specifically in the form:

$$T_{1} = \frac{\beta}{\ln{\frac{R_{1}}{R_{0}} + \frac{\beta}{T_{0}}}} = \ \frac{\beta}{\ln{R_{1} - \ln R_{0}} + \frac{\beta}{T_{0}}}$$

![](media/image1.png){width="7.644419291338583in"
height="6.270833333333333in"}The following diagram shows the variation
of the HW-557C version adapted to the improvement of the W1209-PY.

The W1209-PY works similarly to the original module. I\'ve tried to
maintain a certain degree of compatibility so that people who already
used the old module won\'t feel uncomfortable learning how to use it
again.

However, there are several methods for numerical editing. There is
simple editing, for some parameters that simply require holding down the
\"+\" or \"-\" button to reach the desired value. Holding down each of
these buttons separately causes the numbers to increase or decrease
increasingly rapidly. If you hold down one of them while holding down
the other, the value is reset to 0.

There are also some \"P\" menus that implement single-digit editing. For
large parameters, when it is not practical to modify a number by
scrolling up or down with the \"+\" and \"-\" buttons, single-digit
editing is used. This editing is accessed with a short press of \"+\" or
\"-\" if the menu is appropriate for this editing. A legend such as
\"1d0\" will then appear on the screen. The number on the left is the
position of the digit to be edited, and the number on the right is the
value of the number. If the value of the number can be negative, a \"-\"
sign will appear after the digit 9, indicating that the number can be
given a negative value. A short press of SET toggles between selecting
the digit and its value for editing/changing. Digits that have a decimal
point will have that decimal point in the \"d\" parameter (usually in
position 2 of the full number). A long press on SET memorizes the change
and returns to the "P" menu, but if the editing time is allowed to
expire, the editing is canceled and the value is not modified.

All \"P\" menus have a time delay after which the thermostat returns to
the main temperature screen, except for menus P18 and P19. In these
modes, the thermostat waits indefinitely for the user to snapshot the
sensor resistance value at that time, or to cancel the entry by changing
to \"no.\" Entry is performed with a short press of \"SET.\"

General instructions:

- On the temperature screen, a short press on \"SET\" enters the target
  temperature setting. This is indicated by the flashing display
  indicating the desired activation temperature.

- On the temperature display, a long press (more than two seconds) on
  the \"+\" button activates the relay regardless of the setting. A long
  press on the \"-\" button deactivates it. A short press on either of
  these two buttons stops the timer run timeout alarm (deactivates the
  timing function), or if the timer is running, a short press displays
  the remaining time.

- On the temperature screen, a long press on \"SET\" brings up the P
  menu (Program, Parameter, Pernambuco, whatever). Another long press,
  or waiting without touching anything for about 10 seconds, saves the
  changed settings and returns to the current temperature screen. A
  short press on the \"P\" menu brings up each menu option.

Below is a list of functions, programs, parameters, paraphernalia, etc.

<table>
<colgroup>
<col style="width: 19%" />
<col style="width: 22%" />
<col style="width: 58%" />
</colgroup>
<thead>
<tr class="header">
<th><p><strong>NUMBER of</strong></p>
<p><strong>program</strong></p></th>
<th><strong>Name</strong></th>
<th><strong>Function</strong></th>
</tr>
</thead>
<tbody>
<tr class="odd">
<td>P0</td>
<td>Functionality</td>
<td><p>Allows you to select three modes:</p>
<ul>
<li><p>Heater</p></li>
<li><p>Cooler</p></li>
<li><p>Window</p>
<ul>
<li><p>Active inside window</p></li>
<li><p>Active outside window</p></li>
</ul></li>
</ul>
<p>“Window” mode is a mode not found in the original, and allows the
device to activate or not in a range of temperatures within the window
configured in P1.</p></td>
</tr>
<tr class="even">
<td>P1</td>
<td>Margin</td>
<td>Configures the hysteresis or return temperature. For example, if the
target temperature is 25.00°C and the margin is 2.00°C, if in heater
mode, the thermostat deactivates when the temperature of 25°C is reached
and activates again when the temperature of 25-2 = 23°C is reached. This
value can be negative, in which case the margin is higher, that is, if
the target temperature is 25°C, it would deactivate at 27°C and activate
at 25°C (in short, the roles are reversed). If P0 is configured as a
window, then P1 represents the window width (the temperature range in
which the relay is active or inactive).</td>
</tr>
<tr class="odd">
<td>P2</td>
<td>Maximum Temp.</td>
<td>Maximum temperature limit that you can assign to the target or
desired temperature. This menu is included for compatibility, but it's
actually a completely useless option.</td>
</tr>
<tr class="even">
<td>P3</td>
<td>Minimum Temp.</td>
<td>The same, but with the minimum temperature. Whatever this
temperature, the thermostat internally sets the minimum temperature at 0
Kelvin, which means it cannot be set below -273.15°C.</td>
</tr>
<tr class="odd">
<td>P4</td>
<td>Offset</td>
<td>Number of degrees to correct in the sensor measurement. Since the
sensor measurement is not linear but logarithmic with respect to
temperature, adding a temperature value to adjust the accuracy is
completely useless, as you adjust the accuracy at one temperature and
undo it at the other temperatures. However, it is included for
compatibility.</td>
</tr>
<tr class="even">
<td>P5</td>
<td>Waiting time</td>
<td>In this option, you only configure whether you want the activation
or deactivation waiting time after a state change, in either seconds or
minutes. The separate times for each change are configured independently
in menus P21 and P22.</td>
</tr>
<tr class="odd">
<td>P6</td>
<td>Alarm 1</td>
<td>If alarm 1 is activated, the device can be configured to turn off
the relay and issue an optical-acoustic warning once the temperature is
exceeded. If the alarm temperature is higher than the target temperature
(plus the margin or window), the alarm will indicate "too hot." If the
alarm temperature is set below the target temperature (plus the margin
or window), it will indicate "too cold." If two alarms are activated,
the display will indicate the alarm whose temperature is furthest from
the target.</td>
</tr>
<tr class="even">
<td>P7</td>
<td>Alarm 2</td>
<td><p>Identical behavior to Alarm 1, both can be set to “very hot,”
“very cold,” or one hot and the other cold.</p>
<p>IMPORTANT NOTE: If any of the alarms are set "within" the configured
temperature margin, the device's response is unpredictable, and
sometimes it stops displaying the temperature measurement. This is
because it doesn't make sense for an alarm to be within the thermostat's
operating range. If this is the case, you can still access the menu and
disable the alarm or perform a factory reset.</p></td>
</tr>
<tr class="odd">
<td>P8</td>
<td><p>Select</p>
<p>Scale</p></td>
<td>In older models, some added function was supposed to convert from
the Fahrenheit scale to Celsius and vice versa. It never worked. This
time, it does, indicating temperatures in °F or °C when appropriate. All
temperatures are internally managed in Kelvin within the device, and
conversions to Celsius or Fahrenheit are only performed on the display,
whether for displaying or editing.</td>
</tr>
<tr class="even">
<td>P9</td>
<td>Default values</td>
<td>In this case, if selected, the current "preset," or working memory,
discards the current configuration and restores the default settings
stored in the code. It's also possible to erase the 5 preset memory
zones, so that if the device is turned off before exiting the menu, upon
restarting it will be as if it had never been turned on before.</td>
</tr>
<tr class="odd">
<td>P10</td>
<td>Timer</td>
<td>This function uses the accuracy of the MCU's internal clock (about
1%) to implement an RTC that counts down days, hours, minutes, and
seconds. If the option is enabled and a time greater than 0 seconds is
set, every 10 seconds the display will show a scrolling message with the
remaining time information. When the time expires, the relay deactivates
and an audible warning sounds. The timeout alarm is deactivated simply
by briefly pressing "+" or "-", in which case normal thermostat control
is restored.</td>
</tr>
<tr class="even">
<td>P11</td>
<td>Change target temperature by (sc)rolling</td>
<td><p>It is the way to edit the target or desired temperature to
achieve a precision of two decimal places.</p>
<p>This menu enables digit-by-digit editing.</p></td>
</tr>
<tr class="odd">
<td>P12</td>
<td>R_PullUp setting</td>
<td><p>When using an NTC, due to the nonlinear behavior of this element,
the greatest accuracy is obtained with NTC resistance values ​​close to
R_PullUP. If, for example, you intend to use a 10K NTC in the 85°C
range, at that temperature the sensor resistance is around 1kΩ. If you
change R_PullUp to 1kΩ (you have to physically replace the resistor;
simply changing the value in the configuration is not enough), the
temperature at 25°C will be less accurate, but at 85°C it will be much
more stable and precise.</p>
<p>This menu enables digit-by-digit editing.</p></td>
</tr>
<tr class="even">
<td>P13</td>
<td>R<sub>0</sub> adjustment</td>
<td><p>This value is specific to each thermistor and typically
corresponds to a T<sub>0</sub> setting of 25°C. It is the R<sub>0</sub>
value in the simplified Steinhart–Hart equation.</p>
<p>This menu enables digit-by-digit editing.</p></td>
</tr>
<tr class="odd">
<td>P14</td>
<td>T<sub>0</sub> adjustment</td>
<td><p>This value is specific to each thermistor and usually corresponds
to an R<sub>0</sub> setting given by the manufacturer at this
temperature. It is the T<sub>0</sub> value in the simplified
Steinhart–Hart equation.</p>
<p>This menu enables digit-by-digit editing.</p></td>
</tr>
<tr class="even">
<td>P15</td>
<td>Beta Adjustment</td>
<td><p>B or "beta" is the parameter of the Steinhart–Hart equation that
is usually derived from two resistance/temperature pairs called
(R<sub>0</sub>,T<sub>0</sub>) and (R<sub>1</sub>,T<sub>1</sub>). Given
Beta and one of the pairs, the other can be derived since they are
interchangeable. This parameter is usually defined by the manufacturer;
for example, the NTC 10K B3950 indicates that Beta is 3950. This menu
offers the option to update R1 using the value of T1 as a reference.</p>
<p>This menu enables digit-by-digit editing.</p></td>
</tr>
<tr class="odd">
<td>P16</td>
<td>R<sub>1</sub> adjustment</td>
<td><p>This value is specific to each thermistor and is typically
derived by the manufacturer from the (R<sub>0</sub>,T<sub>0</sub>) pair
and the Beta value, when determining a T<sub>1</sub> temperature of
85°C. It is the R<sub>1</sub> value in the simplified Steinhart–Hart
equation. By setting R<sub>1</sub> (or by capturing that value at
temperature T<sub>1</sub>), Beta can be parameterized.</p>
<p>This menu enables digit-by-digit editing.</p></td>
</tr>
<tr class="even">
<td>P17</td>
<td>T<sub>1</sub> adjustment</td>
<td><p>This value is specific to each thermistor and generally
corresponds to an R<sub>0</sub> setting given by the manufacturer. It is
the T<sub>1</sub> value in the simplified Steinhart–Hart equation.
Unlike T<sub>0</sub> and R<sub>0</sub>, which are provided by the
manufacturer or are considered default, T<sub>1</sub> and R<sub>1</sub>
are the "parameters of service" although this is merely a nomenclature.
Both pairs can be used by their own in conjunction with Beta to
calculate any temperature.</p>
<p>This menu enables digit-by-digit editing.</p></td>
</tr>
<tr class="odd">
<td>P18</td>
<td>R<sub>0</sub> Snapshot</td>
<td><p>This function is useful when we have an unknown sensor and want
to calibrate it. A temperature is set at T<sub>0</sub> (it doesn't have
to be 25°C), and when the temperature of the unknown sensor is reached
using an auxiliary thermometer, the sensor's resistance is captured to
use as R<sub>0</sub>. For example, a sensor temperature of T<sub>0</sub>
= 0°C can be used if the sensor is immersed in ice water.</p>
<p>This menu is not cancelled after a certain time has passed, to allow
time for the sensor to reach the calibration temperature.</p></td>
</tr>
<tr class="even">
<td>P19</td>
<td>R<sub>1</sub> Snapshot</td>
<td><p>This function is useful when we have an unknown sensor and want
to calibrate it. A temperature is set in T<sub>1</sub> (it doesn't have
to be 85°C), and when, with the help of an auxiliary thermometer, that
temperature is reached in the unknown sensor, the sensor's resistance is
captured and used as R<sub>1</sub>. For example, a sensor temperature of
100 degrees can be used if the sensor is immersed in boiling water.</p>
<p>Having defined the pairs (R<sub>0</sub>,T<sub>0</sub>) and
(R<sub>1</sub>,T<sub>1</sub>) you can derive Beta simply by accessing
menu P16 or P17.</p>
<p>This menu is not cancelled after a certain time has passed, to allow
time for the sensor to reach the calibration temperature.</p></td>
</tr>
<tr class="odd">
<td>P20</td>
<td><p>Cable Resistance</p>
<p>(in ohms)</p></td>
<td>If the sensor cable is too long, it may add a few ohms of resistance
that the thermostat will see as coming from the sensor, interfering with
the measurement. If this cable resistance is subtracted from the sensor
measurement, the sensor regains its accuracy. This parameter can only
have positive values.</td>
</tr>
<tr class="even">
<td>P21</td>
<td>Ignition delay</td>
<td>Specifies the number of seconds (or minutes, depending on P5) that
the relay must wait from the time it reaches the target temperature (or
its range) until the relay switches from off to on. Configurable up to
999 seconds or minutes.</td>
</tr>
<tr class="odd">
<td>P22</td>
<td>Delayed shutdown</td>
<td>Specifies the number of seconds (or minutes, according to P5) that
the relay must wait from when the target temperature (or its range) is
reached until the relay goes from being on to being off, configurable up
to 999 seconds or minutes.</td>
</tr>
<tr class="even">
<td>P23</td>
<td>Filter depth</td>
<td>The temperature is taken approximately 80 times per second, but
although the PY32F002B's ADC is 12-bit, the effective resolution, or
ENOB, is approximately 8 bits. This is due to the internal noise of the
MCU. The filter depth indicates approximately the number of bits the
filter will average to obtain a stable reading. The more bits, the
greater the stability, but the slower the thermostat will be. This value
allows for adjusting a compromise between response speed and
accuracy.</td>
</tr>
<tr class="odd">
<td>P24</td>
<td>ADC bit depth</td>
<td>The ADC's native bit depth is 12 bits, but oversampling allows for
more bits of resolution. This slows the thermostat's response, so this
value is a compromise between response speed and accuracy. The sum of
the filter and ADC bits can never exceed 31, otherwise an integer
overflow occurs.</td>
</tr>
<tr class="even">
<td>P25</td>
<td>ADC Offset</td>
<td>This parameter allows you to add an offset or bias to the value read
by the ADC. This makes sense to talk about adjustment here, since the
ADC reading is linear (although the resistance value isn't). Therefore,
if there is a constant voltage source in series with the sensor, this
setting allows you to correct it. The range is from -512 to +512.</td>
</tr>
<tr class="odd">
<td>P26</td>
<td>Activate rolling screen</td>
<td>In this configuration, you can set the main display, which normally
shows the sensor temperature, to scroll repeatedly through the entire
value. This way, you can display the temperature accuracy with a
two-digit resolution.</td>
</tr>
<tr class="even">
<td>P27</td>
<td>Activate Messages</td>
<td>The explanatory messages in each menu option can be disabled to make
the device more like the original, and also because they are no longer
necessary once the user has learned the settings they normally use. They
are enabled by default.</td>
</tr>
<tr class="odd">
<td>P28</td>
<td>Activate sound</td>
<td>The optical/audible alarm and timer notification can be disabled if
the timer requires silent operation. Note that if the optional circuitry
with the power LED and buzzer has not been implemented, this menu has no
effect.</td>
</tr>
<tr class="even">
<td>P29</td>
<td>Change active profile.</td>
<td>The device has five memory regions available for saving
configurations with each modification. When you exit the menu, any
modifications are saved in the active profile. If you change profiles
and the selected profile had previous data, you can choose between
loading that stored data and discarding the current one, or keeping the
current data and overwriting the data from the selected profile. If a
profile fails, it does not prevent the other profiles from functioning,
and you can attempt to recover it by deleting all profiles from P9.</td>
</tr>
<tr class="odd">
<td>P30</td>
<td>Change Language</td>
<td>This is perhaps the most obvious menu option. Due to memory
limitations, only two groups of messages could be stored, one in English
and another in Spanish. If PUYA designs a PY32F002BW-compatible
microcontroller with more Flash memory in a future, more timer functions
and more customizable languages ​​could be added. The restriction lies
more in the pinout of the current microcontroller than in the
possibility of changing the MCU to another model. (For example, the
CH32V203 series in TSSOP20 are very attractive because they have 256kB
of internal Flash memory.)</td>
</tr>
</tbody>
</table>
