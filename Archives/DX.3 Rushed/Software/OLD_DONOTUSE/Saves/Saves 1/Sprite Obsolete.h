#pragma once

int blendMode(int mainpixel, int compositepixel, int blendMode){//Add more blend modes later
    switch(blendMode){
        case 0:
            return compositepixel;
            break;
        case 1:
            if(!mainpixel){return compositepixel;}else{return mainpixel;}
            break;
        default:
        break;
    }
    return 0;
}

void BlendtoLayer(int previousLayer[], int prelayerwidth, int prelayerheight, int spriteArray[], int spritewidth, int spriteheight, int xoffset, int yoffset, int blendmode){
    int totaloffset = (yoffset*prelayerwidth)+xoffset;
    int loopover = prelayerwidth-(spritewidth);

    int spritecount = 0;
    for(size_t i = 1; i <= spriteheight; i++){
        for(size_t i = 1; i <= spritewidth; i++){
            previousLayer[totaloffset] = blendMode(previousLayer[totaloffset], spriteArray[spritecount], blendmode);
            spritecount++;
            totaloffset++;
        }
        totaloffset = totaloffset + loopover;
    }
}

void ClearDisplay(int arraysize){
    for(size_t i = 1; i <= arraysize; i++){
        compositelayer[0][i] = 0;
    }
}





















////////// Dont use anything beyond this point \\\\\\\\\\

/**
void DrawSprite(int mainheight, int spriteArray[], int spriteheight, int xoffset, int yoffset, int blendmode){
    //int mainwidth = sizeof(frame[0])/mainheight;
    //int mainwidth = 64;
    //int spritewidth = (sizeof(spriteArray))/spriteheight;
    //int spritewidth = 64/spriteheight;
    //int row = yoffset*mainwidth;
    //Serial.println(spritewidth);

    int looparound = 64-xoffset-8;
    int trueoffset = (64*yoffset)+xoffset;
    int spriteArrayPixel = 0;
    for(int i = 0; i <= 7; i++){
        for(int i = 0; i <= 7; i++){
            frame[0][trueoffset] = spriteArray[spriteArrayPixel];
            spriteArrayPixel++;
            trueoffset++;
        }
    trueoffset = trueoffset+looparound+xoffset;
    }
}
**/