#include "image.h"

void fill_image(image i, uint16_t colour){
    rectangle r;
    if(i.left>=WIDTH || i.top>=HEIGHT || i.left+i.width<=0 || i.top + i.height<=0)
        return;
        
    r.left=i.left<0 ? 0:i.left;
    r.right=i.left+i.width-1;
    r.right=r.right>=WIDTH ? WIDTH : r.right;
    
    r.top=i.top<0 ? 0:i.top;
    r.bottom=i.top+i.height-1;
    r.bottom=r.bottom>=HEIGHT ? HEIGHT : r.bottom;
    fill_rectangle(r,colour);
}

void draw_image_indexed(image i, uint16_t* colour){
    rectangle r;
    
    r.left=i.left;
    r.right=i.left+i.width-1;
    
    r.top=i.top;
    r.bottom=i.top+i.height-1;
    r.bottom=r.bottom>=HEIGHT ? HEIGHT : r.bottom;
    fill_rectangle_indexed(r,colour);
}