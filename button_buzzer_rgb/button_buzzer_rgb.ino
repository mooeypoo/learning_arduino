/**
 * Arduino sketch - RGB LED + PASSIVE BUZZER + BUTTON
 * @author Moriel Schottlender 2022
 */
#include <math.h>
#include <pitches.h>
#include "RGBLED.h"
#include "MusicPlayer.h"

// Define Pins for LED
#define PIN_BLUE 3
#define PIN_GREEN 5
#define PIN_RED 6
// Define Pin for passive buzzer
#define PIN_PASSIVE_BUZZER 8
// Define pin for the button
#define PIN_BUTTON 2

/* MUSIC SHEET */

// Define the music - Jagerlust
double music_jagerlust[][2] = {
  {NOTE_G5, 1}, {NOTE_E5, 1}, {NOTE_E5, 2},
  {NOTE_F5, 1}, {NOTE_D5, 1}, {NOTE_D5, 2},
  {NOTE_C5, 1}, {NOTE_D5, 1}, {NOTE_E5, 1}, {NOTE_F5, 1}, {NOTE_G5, 1}, {NOTE_G5, 1}, {NOTE_G5, 2},
  {NOTE_G5, 1}, {NOTE_E5, 1}, {NOTE_E5, 2},
  {NOTE_F5, 1}, {NOTE_D5, 1}, {NOTE_D5, 2},
  {NOTE_C5, 1}, {NOTE_E5, 1}, {NOTE_G5, 1}, {NOTE_G5, 1}, {NOTE_C5, 2},
  {NOTE_C6, 0.5}
};
// Define the music - Hallelujah
// https://musescore.com/user/21965011/scores/4217351
double music_hallelujah[][2] = {
  {NOTE_E5, 0.5}, {NOTE_G5, 1}, {NOTE_G5, 0.5}, {NOTE_G5, 1}, {NOTE_G5, 0.5},
  {NOTE_A5, 0.5}, {NOTE_A5, 0.5}, {NOTE_A5, 1.5}, {NOTE_E5, 0.5}, {NOTE_G5, 1}, {NOTE_G5, 0.5}, {NOTE_G5, 1},
  {NOTE_G5, 0.5},
  {NOTE_A5, 0.5}, {NOTE_A5, 0.5}, {NOTE_A5, 1.5}, {NOTE_G5, 0.5}, {NOTE_A5, 1}, {NOTE_A5, 1}, {NOTE_A5, 0.5}, {NOTE_A5, 0.5},
  {NOTE_A5, 1}, {NOTE_G5, 0.5}, {NOTE_G5, 1}, {NOTE_F5, 0.5}, {NOTE_G5, 1.5}, {NOTE_G5, 1.5}
};
// Define the music - Yakety Sax - Benny Hill Theme
// https://musescore.com/user/152066/scores/152784
double music_yaketisax[][2] = {
  {NOTE_D6, 2}, {NOTE_G6, 1}, {NOTE_G6, 1},
  {NOTE_E6, 0.5}, {NOTE_D6, 0.5}, {NOTE_B5, 0.5}, {NOTE_G5, 0.5},
  {NOTE_D6, 1}, {NOTE_E6, 1}, {NOTE_D6, 0.5}, {NOTE_B5, 0.5}, {NOTE_A5, 1}
};

/* FUNCTIONALITY */

// Instantiate managers
RGBLED led = RGBLED(PIN_RED, PIN_GREEN, PIN_BLUE);
MusicPlayer player = MusicPlayer(PIN_PASSIVE_BUZZER);
boolean isItPlaying;
int numOfNotes;

void setup() {
  // Set up the button
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  // Set up led (the buzzer doesn't need pinMode setup)
  led.setup();

  // Set up the music
  numOfNotes = sizeof(music_hallelujah)/sizeof(music_hallelujah[0]);
  player.setBeat(600);

  isItPlaying = false;
}

void loop() {
  // Listen to button press
  // But only respond if the music isn't already playing
  if (digitalRead(PIN_BUTTON) == LOW && isItPlaying == false) {
    isItPlaying = true;
    // Go over the notes
    // Play each note but also turn the RGB LED on with a random color
    for (int i = 0; i < numOfNotes; i++) {
      double delay_length = player.getBeat() * music_hallelujah[i][1];
  
      // Random color
      int randIndex = rand() % 8;
  
      if (music_hallelujah[i][0] != 0) {
        player.playNote(music_hallelujah[i][0], music_hallelujah[i][1]);
        led.on(led.C_ALL[randIndex]);
      }
      delay(delay_length);
      led.off();
    }
    isItPlaying = false;
  }
}
