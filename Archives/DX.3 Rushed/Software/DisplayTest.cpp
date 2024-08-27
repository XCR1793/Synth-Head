#include <Arduino.h>
#include <ImageTest.h>
#include <Display.h>
#include <vector>

HUB75 Panels;
Compose compose;
Shader shader;

#define RX 16
#define TX 17
HardwareSerial fromSensors(2);

// uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b) {
//     // Convert RGB values (0-99) to RGB565 format
//     uint8_t r5 = (r * 31) / 99;    // 5-bit red component
//     uint8_t g6 = (g * 63) / 99;    // 6-bit green component
//     uint8_t b5 = (b * 31) / 99;    // 5-bit blue component
    
//     // Combine components into 16-bit RGB565 value
//     return (r5 << 11) | (g6 << 5) | b5;
// }

// std::vector<uint16_t> rgb565_array(100);

void setup(){
    Panels.PanelCount(2);
    Panels.Buffer(5);
    Panels.SetPins(26, 27, 25, 12, 5, 14, 2, 4, 19, 18, 13, 21, 23, 22);
    Panels.Initialise();
    Panels.Clear();
    Panels.drive_HUB75();

    Serial.begin(115200);
    fromSensors.begin(1500000, SERIAL_8N1, RX, TX);

    // for (int i = 0; i < 100; ++i) {
    //     uint16_t rgb_value = rgb565(i, i, i);
    //     rgb565_array[i] = rgb_value;
    //     Serial.print(rgb_value);
    //     Serial.print(" ");
    // }
    // Serial.println("");
}

double AccelX, AccelY, AccelZ, GyroX, GyroY, GyroZ, Brightness1, Brightness2;
uint64_t CommandStatus;

int roundNumber(double num) {
    int integerPart = static_cast<int>(num);  // Integer part of the number
    double fractionalPart = num - integerPart; // Fractional part of the number

    if (fractionalPart >= 0.5)
        return integerPart + 1;
    else if (fractionalPart <= -0.5)
        return integerPart - 1;
    else
        return integerPart;
}

int roundUp(double num) {
    int integerPart = static_cast<int>(num);
    if (num > 0 && num != integerPart) {
        return integerPart + 1;
    }
    return integerPart;
}

int roundDown(double num) {
    int integerPart = static_cast<int>(num);
    if (num < 0 && num != integerPart) {
        return integerPart - 1;
    }
    return integerPart;
}

double lerp(double start, double end, double fromStart, double fromEnd, double value){return start + (value - fromStart) / (fromEnd - fromStart) * (end - start);}

// void calculateRatios(double start, double end, double v, double& ratioToStart, double& ratioToEnd) {
//     // Calculate distances
//     double distanceFromStart = v - start;
//     double distanceFromEnd = end - v;

//     // Calculate total span
//     double totalSpan = end - start;

//     // Calculate ratios
//     ratioToStart = distanceFromStart / totalSpan;
//     ratioToEnd = distanceFromEnd / totalSpan;
// }

void calculateRatios(double start, double end, double v, double& ratioToStart, double& ratioToEnd) {
    // Calculate distances
    double distanceFromStart = v - start;
    double distanceFromEnd = end - v;

    // Avoid division by zero
    if (start == end) {
        ratioToStart = 0.5;  // Handle the case where start == end
        ratioToEnd = 0.5;
    } else {
        // Calculate total span
        double totalSpan = end - start;

        // Calculate ratios
        ratioToStart = distanceFromStart / totalSpan;
        ratioToEnd = distanceFromEnd / totalSpan;
    }
}

void extractRGB(uint32_t color565, uint32_t &red, uint32_t &green, uint32_t &blue) {
    // Extracting each component using bit masks and shifts
    red = (color565 >> 11) & 0x1F;    // 5 bits for red
    green = (color565 >> 5) & 0x3F;   // 6 bits for green
    blue = color565 & 0x1F;           // 5 bits for blue
}

uint32_t packRGB(uint32_t red, uint32_t green, uint32_t blue) {
    // Ensure the values are within the valid range for RGB565 format
    red = red & 0x1F;       // 5 bits for red (0-31)
    green = green & 0x3F;   // 6 bits for green (0-63)
    blue = blue & 0x1F;     // 5 bits for blue (0-31)

    // Pack the RGB values into a single 16-bit integer (565 format)
    int color565 = (red << 11) | (green << 5) | blue;

    return color565;
}

void bilinear_interpolation(int baselayer[], int basewidth, int spriteArray[], int spritewidth, double offsetx, double offsety){
    for(int i = 0; i < 312; i++){
        double lastx = i, lasty = 0;
        uint32_t R,G,B;
        extractRGB(spriteArray[i], R, G, B);
        // Serial.print(spriteArray[i]);
        // Serial.print(" ");
        while(spritewidth <= lastx){
            lastx -= spritewidth;
            lasty ++;
        }
        // if(spritewidth==lastx+1){Serial.println("");}
        lastx += offsetx;
        lasty += offsety;
        // Serial.print(lastx);
        // Serial.print(",");
        // Serial.print(lasty);
        // Serial.print(" ");
        double AAx, AAy, ABx, ABy;
        calculateRatios(roundDown(lastx), roundUp(lastx), lastx, AAx, ABx);
        calculateRatios(roundDown(lastx), roundDown(lasty), lasty, AAy, ABy);

        double Q1 = (ABx - lastx) * (ABy - lasty);
        double Q2 = (lastx - AAx) * (ABy - lasty);
        double Q3 = (lastx - AAx) * (lasty - AAy);
        double Q4 = (ABx - lastx) * (lasty - AAy);
        // Serial.print(Q1); Serial.print(" ");
        // Serial.print(Q2); Serial.print(" ");
        // Serial.print(Q3); Serial.print(" ");
        // Serial.print(Q4); Serial.print(" ");
        // Serial.println("");

        if (isnan(Q1) || isnan(Q2) || isnan(Q3) || isnan(Q4)) {
            // Handle NaN values here, possibly by skipping this iteration or setting default values
            continue;  // For example, skip this iteration
        }

        // Accumulate colors
        uint32_t color = 0; // Initialize color accumulator

        color += packRGB(R * Q1, G * Q1, B * Q1);
        color += packRGB(R * Q2, G * Q2, B * Q2);
        color += packRGB(R * Q3, G * Q3, B * Q3);
        color += packRGB(R * Q4, G * Q4, B * Q4);

        // Draw interpolated pixel
        Panels.DrawPixel(roundDown(lastx), roundDown(lasty), color);

    }
    // Serial.println("");
}

// Simple swap function for integers
void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

#include <vector>

// Function to determine maximum and minimum y-coordinate of the polygon
void getMinMaxY(const int (*points)[2], int num_points, int& ymin, int& ymax) {
    ymin = ymax = points[0][1];
    for (int i = 1; i < num_points; ++i) {
        if (points[i][1] < ymin) {
            ymin = points[i][1];
        }
        if (points[i][1] > ymax) {
            ymax = points[i][1];
        }
    }
}

// Function to set a pixel in the canvas (1D array) with RGB565 color
void setPixel(uint16_t* canvas, int width, int x, int y, uint16_t rgb565_color) {
    // Ensure coordinates are within bounds
    if (x < 0 || x >= width || y < 0 || y >= width) {
        return;
    }
    
    // Calculate index in 1D array
    int index = y * width + x;
    
    // Set pixel color in canvas
    canvas[index] = rgb565_color;
}

// Function to draw a horizontal line in the canvas (1D array) with RGB565 color
void drawHorizontalLine(uint16_t* canvas, int width, int x1, int x2, int y, uint16_t rgb565_color) {
    for (int x = x1; x <= x2; ++x) {
        setPixel(canvas, width, x, y, rgb565_color);
    }
}

// Function to calculate the fractional part of a float value
float fractional_part(float x) {
    return x - floor(x);
}

// Function to fill a polygon using scanlines
void fillPolygon(uint16_t* canvas, int width, int height, const int (*points)[2], int num_points, uint16_t rgb565_color) {
    int ymin, ymax;
    getMinMaxY(points, num_points, ymin, ymax);
    
    // Iterate through each scanline within the polygon's bounding box
    for (int y = ymin; y <= ymax; ++y) {
        // List to store intersections of current scanline with polygon edges
        std::vector<int> intersections;
        
        // Find intersections of current scanline with polygon edges
        for (int i = 0; i < num_points; ++i) {
            int x1 = points[i][0];
            int y1 = points[i][1];
            int x2 = points[(i + 1) % num_points][0];
            int y2 = points[(i + 1) % num_points][1];
            
            // Ensure the edge is not horizontal
            if ((y1 <= y && y2 > y) || (y2 <= y && y1 > y)) {
                // Calculate intersection point x-coordinate using linear interpolation
                float x_inter = static_cast<float>(x1 + (y - y1) / static_cast<float>(y2 - y1) * (x2 - x1));
                intersections.push_back(static_cast<int>(x_inter));
            }
        }
        
        // Sort intersections from left to right
        std::sort(intersections.begin(), intersections.end());
        
        // Fill pixels between pairs of intersections
        for (size_t i = 0; i < intersections.size(); i += 2) {
            int x_start = intersections[i];
            int x_end = intersections[i + 1];
            
            // Draw horizontal line between x_start and x_end on current scanline y
            for (int x = x_start; x <= x_end; ++x) {
                setPixel(canvas, width, x, y, rgb565_color);
            }
        }
    }
}

void rasterizeShape(uint16_t* canvas, int width, int height, const int (*points)[2], int num_points, bool fill_shape, uint16_t rgb565_color) {
    // Initialize canvas with white background (optional)
    for (int i = 0; i < width * height; ++i) {
        canvas[i] = 0x0000;  // White color in RGB565
    }
    
    // Iterate through each edge of the shape
    for (int i = 0; i < num_points; ++i) {
        // Get current and next point (considering the shape is closed)
        int x0 = points[i][0];
        int y0 = points[i][1];
        int x1 = points[(i + 1) % num_points][0];
        int y1 = points[(i + 1) % num_points][1];
        
        // Draw antialiased line segment using Wu's algorithm
        // Bresenham's line algorithm variables
        bool steep = abs(y1 - y0) > abs(x1 - x0);
        if (steep) {
            swap(x0, y0);
            swap(x1, y1);
        }
        if (x0 > x1) {
            swap(x0, x1);
            swap(y0, y1);
        }
        
        int dx = x1 - x0;
        int dy = y1 - y0;
        float gradient = static_cast<float>(dy) / dx;
        
        // Handle first endpoint
        int xend = round(x0);
        float yend = y0 + gradient * (xend - x0);
        float xgap = 1 - (x0 + 0.5f - floor(x0 + 0.5f));
        int xpxl1 = xend; // Integer part of x
        int ypxl1 = floor(yend);
        
        if (steep) {
            setPixel(canvas, width, ypxl1, xpxl1, rgb565_color);
            setPixel(canvas, width, ypxl1 + 1, xpxl1, rgb565_color);
        } else {
            setPixel(canvas, width, xpxl1, ypxl1, rgb565_color);
            setPixel(canvas, width, xpxl1, ypxl1 + 1, rgb565_color);
        }
        
        float intery = yend + gradient; // Start y (integer) + gradient for the next pixel
        
        // Handle second endpoint
        xend = round(x1);
        yend = y1 + gradient * (xend - x1);
        xgap = (x1 + 0.5f - floor(x1 + 0.5f));
        int xpxl2 = xend; // Integer part of x
        int ypxl2 = floor(yend);
        
        if (steep) {
            setPixel(canvas, width, ypxl2, xpxl2, rgb565_color);
            setPixel(canvas, width, ypxl2 + 1, xpxl2, rgb565_color);
        } else {
            setPixel(canvas, width, xpxl2, ypxl2, rgb565_color);
            setPixel(canvas, width, xpxl2, ypxl2 + 1, rgb565_color);
        }
        
        // Main loop between endpoints
        for (int x = xpxl1 + 1; x <= xpxl2 - 1; ++x) {
            float weight = steep ? 1 - (intery - floor(intery)) : 1 - (intery - floor(intery));
            
            if (fill_shape) {
                // Determine which pixels to fill
                int ypos = floor(intery);
                setPixel(canvas, width, steep ? ypos : x, steep ? x : ypos, rgb565_color);
                setPixel(canvas, width, steep ? ypos + 1 : x, steep ? x : ypos + 1, rgb565_color);
            } else {
                // Draw antialiased line
                setPixel(canvas, width, steep ? floor(intery) : x, steep ? x : floor(intery), rgb565_color);
                setPixel(canvas, width, steep ? floor(intery) + 1 : x, steep ? x : floor(intery) + 1, rgb565_color);
            }
            
            intery += gradient;
        }
    }
    
    // If fill_shape is true, fill the shape using fillPolygon
    if (fill_shape) {
        fillPolygon(canvas, width, height, points, num_points, rgb565_color);
    }
}

// Function to rotate points around a specified center
void rotatePoints(int points[][2], int num_points, int cx, int cy, double theta) {
    // Convert angle to radians
    double cos_theta = cos(theta);
    double sin_theta = sin(theta);

    // Rotate each point around (cx, cy)
    for (int i = 0; i < num_points; ++i) {
        // Translate point so that (cx, cy) is at the origin
        double translated_x = points[i][0] - cx;
        double translated_y = points[i][1] - cy;

        // Perform rotation
        double rotated_x = translated_x * cos_theta - translated_y * sin_theta;
        double rotated_y = translated_x * sin_theta + translated_y * cos_theta;

        // Translate back and update the points array, rounding to the nearest integer
        points[i][0] = round(rotated_x + cx);
        points[i][1] = round(rotated_y + cy);
    }
}

double degreesToRadians(double degrees) {
    return degrees * (M_PI / 180.0);
}

double average(std::vector<double> dataset){
    double cumilative = 0;
    if(!dataset.empty()){
        for(auto data: dataset){
            cumilative += data;
        }
        return cumilative/dataset.size();
    }
    return 0;
}

// Function to retrieve bits from a uint64_t integer
uint64_t retrieveBits(uint64_t originalValue, uint8_t startBit, uint8_t endBit) {
    // Calculate the bit length of the range
    uint8_t bitLength = endBit - startBit + 1;

    // Create a mask for the bit length
    uint64_t mask = ((1ULL << bitLength) - 1) << startBit;

    // Extract the value from the originalValue using the mask
    uint64_t retrievedValue = (originalValue & mask) >> startBit;

    return retrievedValue;
}

// void rgb565_to_rgb(uint16_t rgb565, uint8_t* r, uint8_t* g, uint8_t* b) {
//     *r = ((rgb565 >> 11) & 0x1F) * 99 / 31;
//     *g = ((rgb565 >> 5) & 0x3F) * 99 / 63;
//     *b = (rgb565 & 0x1F) * 99 / 31;
// }

// // Process the pixel array with a selected LUT index
// void process_pixels(int* pixel_array, size_t size, const std::vector<uint16_t>& lut, size_t lut_index) {
//     for (size_t i = 0; i < size; i++) {
//         uint8_t r, g, b;
//         rgb565_to_rgb(static_cast<uint16_t>(pixel_array[i]), &r, &g, &b);
        
//         uint8_t lut_r, lut_g, lut_b;
//         rgb565_to_rgb(lut[lut_index], &lut_r, &lut_g, &lut_b);

//         r = (r > lut_r) ? r - lut_r : 0;
//         g = (g > lut_g) ? g - lut_g : 0;
//         b = (b > lut_b) ? b - lut_b : 0;

//         pixel_array[i] = static_cast<int>(rgb565(r, g, b));
//     }
// }

uint16_t hsvToRgb565(float h, float s, float v) {
    float r = 0, g = 0, b = 0;

    int i = static_cast<int>(h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);

    switch (i % 6) {
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }

    uint8_t r8 = static_cast<uint8_t>(r * 31);  // 5 bits for red
    uint8_t g8 = static_cast<uint8_t>(g * 63);  // 6 bits for green
    uint8_t b8 = static_cast<uint8_t>(b * 31);  // 5 bits for blue

    return (r8 << 11) | (g8 << 5) | b8;
}

void rgbToHsv(float r, float g, float b, float &h, float &s, float &v) {
    float max = std::max({r, g, b});
    float min = std::min({r, g, b});
    v = max;

    float delta = max - min;
    if (max == 0) {
        s = 0;
        h = 0; // Undefined, maybe nan?
    } else {
        s = delta / max;

        if (r == max) {
            h = (g - b) / delta;
        } else if (g == max) {
            h = 2 + (b - r) / delta;
        } else {
            h = 4 + (r - g) / delta;
        }

        h /= 6;
        if (h < 0) {
            h += 1;
        }
    }
}

void printBits(double value) {
  // Use a union to reinterpret the double's memory as bytes
  union {
    double d;
    byte b[sizeof(double)];
  } u;

  u.d = value;

  // Print each byte in the union as binary
  for (int i = sizeof(double) - 1; i >= 0; i--) {
    for (int j = 7; j >= 0; j--) {
      Serial.print((u.b[i] >> j) & 1);
    }
    Serial.print(" ");
  }
  Serial.println();
}

std::vector<double> AX, AY, AZ, GX, GY, GZ;
double colourcycle = 0, colourvelocity = 0;
long int cyclecounter = 0;

uint8_t AnimationID = 0;

void loop() {
    static unsigned long startTime = 0;
    static unsigned long endTime = 0;
    static unsigned long totalTime = 0;
    static int numLoops = 0;
    static int numIterations = 0;

    // Start measuring time
    startTime = millis();

    // Your existing code here
    if (fromSensors.available()) {
        sscanf(fromSensors.readStringUntil('\n').c_str(), "%lf %lf %lf %lf %lf %lf %lf %lf %llu", &AccelX, &AccelY, &AccelZ, &GyroX, &GyroY, &GyroZ, &Brightness1, &Brightness2, &CommandStatus);
        AnimationID = retrieveBits(CommandStatus, 24, 31);
        // Serial.print("AccelX: "); Serial.print(AccelX); Serial.print(" ");
        // Serial.print("AccelY: "); Serial.print(AccelY); Serial.print(" ");
        // Serial.print("AccelZ: "); Serial.print(AccelZ); Serial.print(" ");
        // Serial.print("GyroX: "); Serial.print(GyroX*20); Serial.print(" ");
        // Serial.print("GyroY: "); Serial.print(GyroY*20); Serial.print(" ");
        // Serial.print("GyroZ: "); Serial.print(GyroZ*20); Serial.print(" ");
        // Serial.print("Bright1: "); Serial.print(Brightness1); Serial.print(" ");
        // Serial.print("Bright2: "); Serial.print(Brightness2); Serial.print(" ");
        // Serial.print("CommandStatus: "); Serial.print(CommandStatus); Serial.print(" ");
        // Serial.println("");

        // CommandStatus = 12349078;
        // printBits(CommandStatus);
        Serial.println(CommandStatus);
    }

    uint16_t rgb565_color = 0xfbe4;
    // float h,s,v;
    // rgbToHsv(1, 0.5, 0.05, h, s, v);
    // Serial.print(h);
    // Serial.print(" ");
    // Serial.print(s);
    // Serial.print(" ");
    // Serial.print(v);make it
    // Serial.println("");
    // uint16_t rgb565_color = hsvToRgb565(0.08,0.95,(1000-Brightness1*5)/1000);

    #define WIDTH 30
    #define HEIGHT 30

    uint16_t canvas[WIDTH * HEIGHT]; // Static allocation of canvas array

    if(!AX.empty()){if(AX.size()>5){AX.erase(AX.begin());}else{AX.push_back(AccelX);}}else{AX.push_back(0);}
    if(!AY.empty()){if(AY.size()>5){AY.erase(AY.begin());}else{AY.push_back(AccelY);}}else{AY.push_back(0);}
    if(!AZ.empty()){if(AZ.size()>5){AZ.erase(AZ.begin());}else{AZ.push_back(AccelZ);}}else{AZ.push_back(0);}
    if(!GX.empty()){if(GX.size()>5){GX.erase(GX.begin());}else{GX.push_back(GyroX );}}else{GX.push_back(0);}
    if(!GY.empty()){if(GY.size()>5){GY.erase(GY.begin());}else{GY.push_back(GyroY );}}else{GY.push_back(0);}
    if(!GZ.empty()){if(GZ.size()>5){GZ.erase(GZ.begin());}else{GZ.push_back(GyroZ );}}else{GZ.push_back(0);}


    // for(auto print: AX){
    //     Serial.print(print);
    //     Serial.print(" ");
    // }
    // Serial.println("");

    double ax_value = average(AX);
    double ay_value = average(AY);
    double az_value = average(AZ);
    double gx_value = average(GX);
    double gy_value = average(GY);
    double gz_value = average(GZ);


    switch (AnimationID) {
        case 0: {
            if(Brightness1 > 30) {
                int points[][2] = {{6, 8}, {14, 8}, {20, 11}, {26, 17}, {27, 19}, {28, 22}, {23, 22}, {21, 19}, {19, 17}, {17, 17}, {16, 19}, {18, 22}, {7, 22}, {4, 20}, {2, 17}, {2, 12}};
                int num_points = sizeof(points) / sizeof(points[0]);
                rotatePoints(points, num_points, 15, 15, degreesToRadians(-gx_value * 8));
                rasterizeShape(canvas, WIDTH, HEIGHT, points, num_points, true, rgb565_color);
                compose.BlendtoLayer(Panels.Array, 128, 32, canvas, WIDTH, HEIGHT, 14 + (gy_value * 5) - (az_value * 2), 1 + round(-gz_value * 3) + round(ay_value), 0);
            } else {
                int points[][2] = {{4, 12}, {8, 10}, {13, 9}, {17, 9}, {22, 11}, {24, 13}, {26, 16}};
                int num_points = sizeof(points) / sizeof(points[0]);
                rotatePoints(points, num_points, 15, 15, degreesToRadians(-gx_value * 8));
                rasterizeShape(canvas, WIDTH, HEIGHT, points, num_points, true, rgb565_color);
                compose.BlendtoLayer(Panels.Array, 128, 32, canvas, WIDTH, HEIGHT, 14 + (gy_value * 5) - (az_value * 2), 1 + round(-gz_value * 3) + round(ay_value), 0);
            }
            if(Brightness2 > 30) {
                int points2[][2] = {{24, 8}, {27, 12}, {27, 17}, {26, 20}, {23, 22}, {12, 22}, {15, 19}, {13, 17}, {11, 17}, {9, 19}, {7, 22}, {2, 22}, {3, 19}, {4, 17}, {10, 11}, {16, 8}};
                int num_points2 = sizeof(points2) / sizeof(points2[0]);
                rotatePoints(points2, num_points2, 15, 15, degreesToRadians(gx_value * 8));
                rasterizeShape(canvas, WIDTH, HEIGHT, points2, num_points2, true, rgb565_color);
                compose.BlendtoLayer(Panels.Array, 128, 32, canvas, WIDTH, HEIGHT, 80 + (gy_value * 5) + (az_value * 2), 1 + round(gz_value * 3) + round(ay_value), 0);
            } else {
                int points2[][2] = {{4, 16}, {6, 13}, {8, 11}, {13, 9}, {17, 9}, {23, 10}, {27, 13}};
                int num_points2 = sizeof(points2) / sizeof(points2[0]);
                rotatePoints(points2, num_points2, 15, 15, degreesToRadians(gx_value * 8));
                rasterizeShape(canvas, WIDTH, HEIGHT, points2, num_points2, true, rgb565_color);
                compose.BlendtoLayer(Panels.Array, 128, 32, canvas, WIDTH, HEIGHT, 80 + (gy_value * 5) + (az_value * 2), 1 + round(gz_value * 3) + round(ay_value), 0);
            }
            break;
        }
        case 1: {
            if(Brightness1 > 30) {
                int points[][2] = {{15, 24}, {7, 14}, {6, 10}, {10, 6}, {14, 8}, {15, 11}, {16, 8}, {20, 6}, {24, 10}, {23, 14}};
                int num_points = sizeof(points) / sizeof(points[0]);
                rotatePoints(points, num_points, 15, 15, degreesToRadians(-gx_value * 8));
                rasterizeShape(canvas, WIDTH, HEIGHT, points, num_points, true, rgb565_color);
                compose.BlendtoLayer(Panels.Array, 128, 32, canvas, WIDTH, HEIGHT, 14 + (gy_value * 5) - (az_value * 2), 1 + round(-gz_value * 3) + round(ay_value), 0);
            } else {
                int points[][2] = {{4, 12}, {8, 10}, {13, 9}, {17, 9}, {22, 11}, {24, 13}, {26, 16}};
                int num_points = sizeof(points) / sizeof(points[0]);
                rotatePoints(points, num_points, 15, 15, degreesToRadians(-gx_value * 8));
                rasterizeShape(canvas, WIDTH, HEIGHT, points, num_points, true, rgb565_color);
                compose.BlendtoLayer(Panels.Array, 128, 32, canvas, WIDTH, HEIGHT, 14 + (gy_value * 5) - (az_value * 2), 1 + round(-gz_value * 3) + round(ay_value), 0);
            }
            if(Brightness2 > 30) {
                int points2[][2] = {{15, 24}, {7, 14}, {6, 10}, {10, 6}, {14, 8}, {15, 11}, {16, 8}, {20, 6}, {24, 10}, {23, 14}};
                int num_points2 = sizeof(points2) / sizeof(points2[0]);
                rotatePoints(points2, num_points2, 15, 15, degreesToRadians(gx_value * 8));
                rasterizeShape(canvas, WIDTH, HEIGHT, points2, num_points2, true, rgb565_color);
                compose.BlendtoLayer(Panels.Array, 128, 32, canvas, WIDTH, HEIGHT, 80 + (gy_value * 5) + (az_value * 2), 1 + round(gz_value * 3) + round(ay_value), 0);
            } else {
                int points2[][2] = {{4, 16}, {6, 13}, {8, 11}, {13, 9}, {17, 9}, {23, 10}, {27, 13}};
                int num_points2 = sizeof(points2) / sizeof(points2[0]);
                rotatePoints(points2, num_points2, 15, 15, degreesToRadians(gx_value * 8));
                rasterizeShape(canvas, WIDTH, HEIGHT, points2, num_points2, true, rgb565_color);
                compose.BlendtoLayer(Panels.Array, 128, 32, canvas, WIDTH, HEIGHT, 80 + (gy_value * 5) + (az_value * 2), 1 + round(gz_value * 3) + round(ay_value), 0);
            }

            break;
        }
        case 3: {
            Panels.Print("CREATED BY XCR-1793", 0xfeb4, 5, 4,  25+ gy_value, 15 + round(gz_value));
        }
        case 4: {

        }
        default:
            break;
    }


    for(size_t i = 0; i < 128; i++){
        colourcycle = i + colourvelocity - az_value*5;
        for(size_t j = 0; j < 32; j++){
            if(Panels.Array[i+j*128]){Panels.DrawPixel(i,j+1, shader.HueReturnColour(colourcycle)); colourcycle++;}
        }
    }

    // process_pixels(Panels.Array, 32*128, rgb565_array, 100-Brightness1);

    if(!(cyclecounter%5)){colourvelocity++;}

    Panels.drive_HUB75();
    Panels.Clear();

    // End measuring time
    endTime = millis();

    // Calculate cycle time in milliseconds
    unsigned long cycleTime = endTime - startTime;
    totalTime += cycleTime;
    numLoops++;

    numIterations++;

    // Output progress report every 1000 cycles
    if (numLoops >= 1000) {
        // Calculate average cycle time and cycles per second
        float avgCycleTime = static_cast<float>(totalTime) / numLoops;
        float cyclesPerSecond = 1000.0 / avgCycleTime;

        // Output progress report
        Serial.print("Progress report - Completed ");
        Serial.print(numIterations);
        Serial.println(" iterations.");
        Serial.print("Average cycle time (ms): ");
        Serial.println(avgCycleTime);
        Serial.print("Cycles per second (Hz): ");
        Serial.println(cyclesPerSecond);

        // Reset counters for next progress report
        numLoops = 0;
        totalTime = 0;
    }

    // Uncomment to introduce a delay between iterations (if needed)
    // delay(100); // Adjust delay as necessary
    cyclecounter++;
}

