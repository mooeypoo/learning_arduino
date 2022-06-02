/**
 * Defines a controller for the buzzer
 * @author Moriel Schottlender 2022
 */
class MusicPlayer {
    private:
        int beat = 1000; // Default 1 second
        int pinBuzzer;

    public:
        MusicPlayer(int pinNumber) {
            this->pinBuzzer = pinNumber;
        }
        
        void playTune(double musicarr[][2], int numOfRows) {
            for (int i = 0; i < numOfRows; i++) {
                this->playNote(musicarr[i][0], musicarr[i][1], true);
            }
        }

        void playNote(double note, double beat_multiplier = 1.0, boolean withDelay = false) {
            int length_of_tone = round(this->beat * beat_multiplier);

            if (note != 0) {
              // Output a tone
              tone(this->pinBuzzer, note, length_of_tone);
            }
            if (withDelay) {
              delay(length_of_tone);
            }
        }

        void setBeat(int beatLen) {
            this->beat = beatLen;
        }

        int getBeat() {
            return this->beat;
        }
};
