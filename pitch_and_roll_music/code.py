# A musical instrument that responds to pitch (change of notes) and roll (change of octaves)
#
# Created by Moriel Schottlender (mooeypoo) 2022
#
# License: GPLv3
import time
import math
import board
import digitalio
import pwmio
from adafruit_debouncer import Debouncer
from adafruit_lsm6ds.lsm6dsox import LSM6DSOX
import adafruit_rgbled
import notes
#from notes import getFreqFromFraction, getToneFromFraction

# Pin for the Red LED is connected to
RED_LED = board.D2
# Pin for the Green LED is connected to
GREEN_LED = board.D3
# Pin for the Blue LED is connected to
BLUE_LED = board.D4
# Pin for the buzzer
BUZZER_PIN = board.D12
# Pin for the button
BUTTON_PIN = board.D9

# Initialize RGBLED
led = adafruit_rgbled.RGBLED(RED_LED, BLUE_LED, GREEN_LED)

# Initialize gyroscope/accelerometer sensor
i2c = board.I2C()  # uses board.SCL and board.SDA
sensor = LSM6DSOX(i2c)

# Initialize button
btn = digitalio.DigitalInOut(BUTTON_PIN)
btn.switch_to_input(pull=digitalio.Pull.UP)
button = Debouncer(btn)

# Initialize buzzer
buzzer = pwmio.PWMOut(BUZZER_PIN, duty_cycle=0, frequency=440, variable_frequency=True)

# Translate the given value into a sensible RGB color value from 0 to 255
def angleToColor(angle):
    # The angles we get are always 0 to 90
    # but we set the negative values to be from 90 to 180
    angle = getNormalizedAngle(angle)

    colr = round(255 * angle / 180)

    # Make sure we return legal values
    if colr > 255:
        return 255
    else:
        return abs(colr)

def getNormalizedAngle(angle):
    # The angles we get are 0 (flat) to 90 (upright) or -90 (downright)
    # For the pitches to work, we need to set 0 as the center of the tone range
    # and then -90 as our base and +90 as our max
    # Meaning, it would be easier if we pretend -90 is our 0 and +90 is our 180
    # so we can calculate the fractional position of the angle
    # and use that fractional position to have the same position for the tone within the range
    return 180 - round(90 + angle)

# Translate an angle to a sensible tone
# For the moment, define "sensible" as the range between c3 to c7
def angleToTone(pitch_angle, roll_angle):
    pitch_angle = getNormalizedAngle(pitch_angle)
    roll_angle = getNormalizedAngle(roll_angle)
    # Now we have an actual angle from 0 to 180
    # Assuming 0 = tones.C3 and 180 = tones.C7
    # What fraction of 180 that is our given angle?
    # That would be the fraction we pick a tone from the range
    pitch_fraction = pitch_angle / 180
    roll_fraction = roll_angle / 180

    # We'll use the 'roll' angle to control the octave
    # We have in `notes` the rance of octave C1 to C8
    # which means we have 8-1 = 7 possible numbers
    # But we "cancel out" the top one, since this calculation
    # always gets us the 'low-to-high' range (we add +1 to the end-tone)
    octave = round(roll_fraction * 7)
    tone_start = "C" + str(octave)
    tone_end = "C" + str(octave + 1)
    # return notes.getFreqFromFraction(fraction, tone_start, tone_end) # Continuous
    return notes.getToneFromFraction(pitch_fraction, tone_start, tone_end) # Note by note ladder

# Calculate pitch and roll
# See https://engineering.stackexchange.com/a/3350
def getPitchAndRoll(accArray):
    ax = (accArray[0] * 3.9)
    ay = (accArray[1] * 3.9)
    az = (accArray[2] * 3.9)
    pitch = 180 * math.atan(ax/math.sqrt(ay*ay + az*az)) / math.pi;
    roll = 180 * math.atan(ay/math.sqrt(ax*ax + az*az)) / math.pi;
    return (pitch, roll)

system_active = False
while True:
    button.update()
    if button.fell:
        # Activate or deactivate
        system_active = not system_active

    if system_active:
        (pitch, roll) = getPitchAndRoll(sensor.acceleration)

        # LED color
        led.color = (angleToColor(pitch), angleToColor(roll), 255 - angleToColor(pitch))

        # Buzzer pitch & volume
        buzzer.frequency = round(angleToTone(pitch, roll))
        buzzer.duty_cycle = 65535 // 2 # 50%


    else:
        # LED off
        led.color = (0, 0, 0)
        # buzzer off
        buzzer.duty_cycle = 0
