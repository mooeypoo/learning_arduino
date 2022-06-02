/**
 * Defines a controller for the RGB LED
 * @author Moriel Schottlender 2022
 */
class RGBLED {
    private:
        int pinRed;
        int pinBlue;
        int pinGreen;

    public:
        int C_RED[3] = {255, 0, 0};
        int C_PINK[3] = {255, 0, 220};
        int C_PURPLE[3] = {125, 0, 255};
        int C_BLUE[3] = {0, 255, 255};
        int C_GREEN[3] = {0, 255, 0};
        int C_YELLOW[3] = {255, 255, 0};
        int C_ORANGE[3] = {255, 150, 0};
        int C_LIGHTRED[3] = {255, 125, 125};
        int C_WHITE[3] = {255, 255, 255};
        int* C_ALL[8] = {
          C_RED, C_PINK, C_PURPLE, C_BLUE, C_GREEN, C_YELLOW, C_ORANGE, C_LIGHTRED
        };

        RGBLED(int pinRedNum, int pinGreenNum, int pinBlueNum) {
            this->pinRed = pinRedNum;
            this->pinGreen = pinGreenNum;
            this->pinBlue = pinBlueNum;
        }

        void setup() { 
            // Set up LED
            pinMode(this->pinRed, OUTPUT);
            pinMode(this->pinGreen, OUTPUT);
            pinMode(this->pinBlue, OUTPUT);
            digitalWrite(this->pinRed, LOW);
            digitalWrite(this->pinGreen, LOW);
            digitalWrite(this->pinBlue, LOW);
        }

        void on(int rgb[3]) {
            analogWrite(this->pinRed, rgb[0]);
            analogWrite(this->pinGreen, rgb[1]);
            analogWrite(this->pinBlue, rgb[3]);
        }

        void off() {
            analogWrite(this->pinRed, 0);
            analogWrite(this->pinGreen, 0);
            analogWrite(this->pinBlue, 0);
        }

        void flash(int rgb[3], int light_duration) {
          this->on(rgb);
          delay(light_duration);
          this->off();
        }
};
