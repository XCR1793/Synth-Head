#include <Arduino.h>
#include <ImageTest.h>
#include <Display.h>

HUB75 Panels;
Compose compose;

#define RX 16
#define TX 17
HardwareSerial fromSensors(2);

void setup(){
    Panels.PanelCount(2);
    Panels.Buffer(5);
    Panels.SetPins(26, 27, 25, 12, 5, 14, 2, 4, 19, 18, 13, 21, 23, 22);
    Panels.Initialise();
    Panels.Clear();
    Panels.drive_HUB75();

    Serial.begin(115200);
    fromSensors.begin(921600, SERIAL_8N1, RX, TX);
}

double AccelX, AccelY, AccelZ, GyroX, GyroY, GyroZ, ToF1, ToF2, HGS, PResist1, PResist2, PResist3, PResist4, Status;

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



#define WIDTH 128
#define HEIGHT 32

// Simple swap function for integers
void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

uint16_t canvas[WIDTH * HEIGHT]; // Static allocation of canvas array

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
        sscanf(fromSensors.readStringUntil('\n').c_str(), "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &AccelX, &AccelY, &AccelZ, &GyroX, &GyroY, &GyroZ, &ToF1, &ToF2, &HGS, &PResist1, &PResist2, &PResist3, &PResist4, &Status);
    }

    // Example points defining a triangle
    int points[][2] = {{0, 0}, {10, 10}, {15, 5}, {64,16}, {128,0}};
    int num_points = sizeof(points) / sizeof(points[0]);
    
    // Example RGB565 color (red)
    uint16_t rgb565_color = 0xF800;

    // Rasterize the shape using Wu's antialiased algorithm with color parameter
    rasterizeShape(canvas, WIDTH, HEIGHT, points, num_points, true, rgb565_color); // true for filled shape, false for outline

    // Now you can copy `canvas` data to your Panels.Array for display
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        Panels.Array[i] = canvas[i];
    }

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
}

