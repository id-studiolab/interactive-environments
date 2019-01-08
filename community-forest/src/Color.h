#include <math.h>

struct Lab
        {
            double l;
            double a;
            double b;
        };

struct COLOUR
        {
            double r;
            double g;
            double b;
        };

// double max(double a, double b) {
//     if(a > b) return a;
//     return b;
// }

// double min(double a, double b) {
//     if(a < b) return a;
//     return b;
// }
    
Lab RGB2Lab(COLOUR c1)
        {
            Lab c2;
            using namespace std;

            double var_R = c1.r;
            double var_G = c1.g;
            double var_B = c1.b;

            if (var_R > 0.04045) {
                var_R = pow(((var_R + 0.055) / 1.055), 2.4);
            } else {
                var_R = var_R / 12.92;
            }

            if (var_G > 0.04045) {
                var_G = pow(((var_G + 0.055) / 1.055), 2.4);
            } else {
                var_G = var_G / 12.92;
            }

            if (var_B > 0.04045) {
                var_B = pow(((var_B + 0.055) / 1.055), 2.4);
            } else {
                var_B = var_B / 12.92;
            }

            var_R = var_R * 100;
            var_G = var_G * 100;
            var_B = var_B * 100;

//Observer. = 2Â°, Illuminant = D65
            double X = var_R * 0.4124 + var_G * 0.3576 + var_B * 0.1805;
            double Y = var_R * 0.2126 + var_G * 0.7152 + var_B * 0.0722;
            double Z = var_R * 0.0193 + var_G * 0.1192 + var_B * 0.9505;

            double var_X = X / 95.047;
            double var_Y = Y / 100.000;
            double var_Z = Z / 108.883;

            if (var_X > 0.008856) {
                var_X = pow(var_X, (1.0 / 3));
            }
            else {
                var_X = (7.787 * var_X) + (16.0 / 116);
            }

            if (var_Y > 0.008856) {
                var_Y = pow(var_Y, (1.0 / 3));
            }
            else {
                var_Y = (7.787 * var_Y) + (16.0 / 116);
            }

            if (var_Z > 0.008856) {
                var_Z = pow(var_Z, (1.0 / 3));
            }
            else {
                var_Z = (7.787 * var_Z) + (16.0 / 116);
            }

            //clamping
            c2.l = max(0.0, (116.0 * var_Y) - 16);
            c2.a = max(-128.0, min(127.0, 500.0 * (var_X - var_Y)));
            c2.b = max(-128.0, min(127.0, 200.0 * (var_Y - var_Z)));

            return c2;
        }

COLOUR Lab2RGB(Lab c1) {
            COLOUR c2;
            using namespace std;

            double var_Y = (c1.l + 16) / 116.0;
            double var_X = (c1.a / 500.0) + var_Y;
            double var_Z = var_Y - (c1.b / 200);

            if (pow(var_Y, 3) > 0.008856) {
                var_Y = pow(var_Y, 3);
            } else {
                var_Y = (var_Y - 16.0 / 116) / 7.787;
            }

            if (pow(var_X, 3) > 0.008856) {
                var_X = pow(var_X, 3);
            } else {
                var_X = (var_X - 16.0 / 116) / 7.787;
            }

            if (pow(var_Z, 3) > 0.008856) {
                var_Z = pow(var_Z, 3);
            } else {
                var_Z = (var_Z - 16.0 / 116) / 7.787;
            }

            double X = var_X * 95.047;
            double Y = var_Y * 100.000;
            double Z = var_Z * 108.883;

            var_X = X / 100.0;
            var_Y = Y / 100.0;
            var_Z = Z / 100.0;

            double var_R = var_X * 3.2406 + var_Y * -1.5372 + var_Z * -0.4986;
            double var_G = var_X * -0.9689 + var_Y * 1.8758 + var_Z * 0.0415;
            double var_B = var_X * 0.0557 + var_Y * -0.2040 + var_Z * 1.0570;

            if (var_R > 0.0031308) {
                var_R = 1.055 * pow(var_R, (1 / 2.4)) - 0.055;
            } else {
                var_R = 12.92 * var_R;
            }

            if (var_G > 0.0031308) {
                var_G = 1.055 * pow(var_G, (1 / 2.4)) - 0.055;
            } else {
                var_G = 12.92 * var_G;
            }

            if (var_B > 0.0031308) {
                var_B = 1.055 * pow(var_B, (1 / 2.4)) - 0.055;
            } else {
                var_B = 12.92 * var_B;
            }

            // clamping
            c2.r = max(0.0, min(1.0, var_R));
            c2.g = max(0.0, min(1.0, var_G));
            c2.b = max(0.0, min(1.0, var_B));

            return (c2);
        }
