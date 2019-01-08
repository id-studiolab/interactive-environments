void setLED(int strip, int amount, int brightness, double hue);
void sendLED(int strip, double brightnessIncrease, double hueIncrease, double nextHue);
double getNewHue(double current);
void setHue();
void connect();
void parseTime(const char *datetime);
int getAmountOfLeds(int strip);
void onMessage(String &topic, String &payload);

double calculateIncrease(double a, double b, double updateInterval, double time, bool round)
{
    return updateInterval * ((round ? 2.0 : 1.0) * abs(a - b)) / time;
}
double calculateHsvIncrease(double a, double b, double updateInterval, double totalTime)
{
    double diff = b - a;
    return (diff + ((abs(diff) > 180) ? ((diff < 0) ? 360 : -360) : 0)) * updateInterval / totalTime;
}