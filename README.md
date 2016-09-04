# AR/CI

## Overview
Project Automatic and Reactive Collision Intervention (AR/CI ["arr-see"]) is a
remote-controlled vehicle with on-board collision avoidance and operator
assistance features that allow for semi-autonomous operation.

By design, AR/CI is intended to be "driven" by an operator via a remote device
for recreational fun, but AR/CI is also setup for self-preservation in the
event of reckless operation - using specific hardware and logic to prevent
damage to AR/CI as a result from negligent operation.

## Components
* **Vehicle Platform** - a "donor" vehicle chassis (axles, steering components,
gears, etc.) from an existing radio-controlled car
* **Movement** - a DC motor is used for propelling AR/CI forward and backward
* **Steering** - a servo is used to actuate the steering mechanisms in order to
set vehicle direction during forward/backward movement
* **Collision Prevention*** - distance sensors are used to provide AR/CI with a
"sense" of any obstacles in front or behind it
* **Control** - remote control of AR/CI is achieved wirelessly via an encrypted
and private WiFi access point broadcast by AR/CI
* **Controller** - a ATmega1284 is the primary microcontroller and gateway
between all of the components that enable AR/CI
* **Human-Machine Interaction** - a smartphone or any device with a web browser
(PC, tablet, etc.) is used as the input/command controller to operate AR/CI

**Collision Prevention was not fully implemented in the final design due to
time constraints, full implementation TBD*

## Operation
AR/CI is controlled via a virtual joystick^1 that is drawn onto a webpage hosted
on AR/CI's private network provided by the on-board WiFi Access Point (AP).
User control of AR/CI is performed by first searching for and then connecting
to the "ar-ciAP" access point with the pre-determined WPA2 passphrase. Once
connected, AR/CI's on-board DNS server routes ANY traffic through AR/CI's
LAN - ensuring that regardless of which web address the user tries to
access, he/she will automatically be redirected to AR/CI's control interface.
Alternatively, the user may manually point his/her web browser to the AR/CI
Gateway IP: 10.0.0.1.

The user interface is both touchscreen and mouse-friendly - a virtual joystick
that is drawn to the screen upon touching or clicking anywhere on the window
canvas. AR/CI moves relative to the direction that the joystick is pulled,
once drawn to the screen.

## Behind the Scenes
While AR/CI's construction may seem trivial from afar, there are actually
several systems working together "under the hood".

### ATmega1284
The ATmega1284 manages AR/CI's powertrain. Communication to the ATmega1284 is
done via USART on channel 0. The ATmega1284 expects a byte of data containing
the requested speed and steering angle for AR/CI. The most significant nibble
is split up to determine direction (forward or reverse) on bit 7, and a
steering angle position between 0-6 on bits 6, 5, 4. The least significant
nibble determines the speed (duty cycle) to drive the DC motor within a range
of 0-15.
