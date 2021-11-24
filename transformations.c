#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "transformations.h"
#include "bmp.h"

// int main(){
    
//     FILE * bmp = fopen("/home/john/prog-2021/ps4/assets/lenna.bmp", "rb");

//     fprintf (stderr,"lll\n");

//     FILE * writing = fopen("/home/john/prog-2021/ps4/assets/myshittywriting.bmp", "wb");
//     struct bmp_image *read = read_bmp(bmp);
//     fprintf (stderr,"lll\n");

//     //struct bmp_image *flipped = flip_vertically(read);

//     bool answer = write_bmp(writing, read);
//     printf ("%d", answer);


//     free_bmp_image(read);
//     //free_bmp_image(flipped);
    

//     fclose(bmp);
//     fclose(writing);
//     return 0;
// }

struct bmp_image* extract(const struct bmp_image* image, const char* colors_to_keep){
    if (image == NULL || image->data==NULL || image->header == NULL
    || colors_to_keep == NULL) return NULL;

    bool red = false, green = false, blue = false;

    fprintf (stderr,"extr\n");

    while (*colors_to_keep){
        if (*colors_to_keep!='r' && *colors_to_keep!='g' && *colors_to_keep!= 'b') return NULL;
        if (*colors_to_keep == 'r') {
            //if(red == true) return NULL;
            red = true;
                fprintf (stderr,"R\n");

        }
        if(*colors_to_keep == 'g') {
            //if (green == true) return NULL;
            green = true;
                fprintf (stderr,"G\n");
                fprintf(stderr, "%d", green);

        }
        if (*colors_to_keep == 'b') {
            //if(blue == true) return NULL;
            blue = true;
        }
        colors_to_keep++;
    }
    fprintf (stderr,"extr\n");

    uint32_t height = image->header->height, width = image->header->width;

    struct bmp_image* new = malloc(sizeof(struct bmp_image));

    new->header = malloc(sizeof(struct bmp_header));
    *new->header = *image->header;
    new->data = calloc(1,image->header->image_size);
    for (int i =0; i<height; i++){
        for (int j = 0; j < width; j++){
            new->data[(i*width)+j] = image->data[(i*width)+j];
        }
    }
    printf("%d %d %d", red, green, blue);
    fprintf (stderr,"extr\n");
    for (int i =0; i<height; i++){
        for (int j = 0; j < width; j++){
            if (red == false) new->data[(i*width)+j].red = 0;
            if (green == false) new->data[(i*width)+j].green = 0;
            if (blue == false) new->data[(i*width)+j].blue = 0;
        }
    }

    return new;
}


struct bmp_image* scale(const struct bmp_image* image, float factor){
    if (image == NULL || image->data==NULL || image->header == NULL) return NULL;
    if (factor<=0) return NULL;

    uint32_t width = round(factor*image->header->width);
    uint32_t height = round(factor*image->header->height);

    uint32_t old_w = image->header->width, /*bpp = image ->header->bpp/8,*/ old_h = image->header->height;

    struct bmp_image* new = malloc(sizeof(struct bmp_image));
    new->header = malloc(sizeof(struct bmp_header));
    *new->header = *image->header;
    new->header->height = height;
    new->header->width = width;
    fprintf (stderr,"header\n");
    uint32_t padding = (4 -(  width * sizeof(struct pixel)) % 4) % 4;

    fprintf (stderr,"%d %d\n", width, height);

    new->header->image_size = height*(padding+width*sizeof(struct pixel));

    new->header->size = new->header->image_size +  new->header->offset;

    new->data = calloc(height*width, sizeof(struct pixel));

    for (int i =0; i<height; i++){
        for (int j = 0; j < width; j++){
            int tmpw = (int)(j*old_w)/width;
            //fprintf (stderr,"%d old x ", tmpw);
            int tmph = (int)(i*old_h)/height;
            //fprintf (stderr,"%d old y\n", tmph);
            new->data[(i*width)+j] = image->data[(tmph*old_w)+tmpw];
        }
    }
    return new;
}

struct bmp_image* crop(const struct bmp_image* image, const uint32_t start_y, const uint32_t start_x, const uint32_t height, const uint32_t width){
    if (image == NULL) return NULL;

    if (height+start_y>image->header->height) return NULL;
    if (width+start_x>image->header->width) return NULL;

    if (height<=0 || width <= 0 || start_y < 0 || start_x < 0) return NULL;

    uint32_t widthold = image->header->width;//, bpp = image ->header->bpp/8;
    struct bmp_image* new = malloc(sizeof(struct bmp_image));
    new->header = malloc(sizeof(struct bmp_header));
    *new->header = *image->header;

    uint32_t padding = (4 -(  width * sizeof(struct pixel)) % 4) % 4;

    new->header->height = height;
    new->header->width = width;

    new->header->image_size = height*(padding+width*sizeof(struct pixel));
    
    
    // if(bpp*width%4==0){
    //     new->header->image_size = bpp*width*height;
    // } 
    // else {
    //     new->header->image_size = (4-(bpp*height)%4 + width*bpp)*height;
    // }

    new->header->size = new->header->image_size + new->header->offset;

    struct bmp_image *imageflipped = flip_vertically(image); 

    new->data = calloc(height*width, sizeof(struct pixel));

    for (int i = 0; i<height; i++){
        for (int j = 0; j<width; j++){
            new->data[(i*width)+j] = imageflipped->data[(start_y+i)*widthold+start_x+j];
        }
    }

    struct bmp_image *newfl = flip_vertically(new);
    free_bmp_image(imageflipped);
    free_bmp_image(new);

    //printf("image size in crop %d\n", newfl->header->image_size);
    return newfl;
}

struct bmp_image* rotate_right(const struct bmp_image* image){
    if (image == NULL || image->header == NULL || image->data == NULL) return NULL;

    uint32_t height = image->header->height, width = image->header->width;

    struct bmp_image* new = malloc(sizeof(struct bmp_image));

    new->header = malloc(sizeof(struct bmp_header));
    *new->header = *image->header;

    new->header->height = width;
    new->header->width = height;

    uint32_t padding = (4 -(  new->header->width * sizeof(struct pixel)) % 4) % 4;

    new->header->image_size = new->header->height*(padding+height*sizeof(struct pixel));
    new->header->size = new->header->offset + new->header->image_size;

    new->data = calloc(height*width, sizeof(struct pixel));
        
    for (int i = 0; i<height; i++){
        for (int j=0; j<width; j++){
            new->data[(new->header->height-j-1)*new->header->width+i] = image->data[i*image->header->width+j];
            //*(new->data+(i*width)+j) = *(image->data+(j*width)+(width-1-i));
        }
    }
    fprintf (stderr,"rotate right\n");
    return new;
}

struct bmp_image* rotate_left(const struct bmp_image* image){
    if (image == NULL || image->header == NULL || image->data == NULL) return NULL;
    
    uint32_t height = image->header->height, width = image->header->width;

    struct bmp_image* new = malloc(sizeof(struct bmp_image));

    new->header = malloc(sizeof(struct bmp_header));
    *new->header = *image->header;

    
    new->header->height = width;
    new->header->width = height;

    uint32_t padding = (4 -(  new->header->width * sizeof(struct pixel)) % 4) % 4;

    new->header->image_size = new->header->height*(padding+height*sizeof(struct pixel));
    new->header->size = new->header->offset + new->header->image_size;

    new->data = calloc(height*width, sizeof(struct pixel));
    fprintf (stderr,"rotate left \n");
    for (int i = 0; i<height; i++){
        for (int j=0; j<width; j++){
             new->data[(j*height)+(height-i-1)] = image->data[(i*width)+j];
            //new->data[(j*height)+width-i-1] = image->data[(i*width)+j];
        }
    }
    fprintf (stderr,"rotate left \n");
    return new;   
}

struct bmp_image* flip_vertically(const struct bmp_image* image){
    if (image == NULL) return NULL;

    uint32_t height = image->header->height, width = image->header->width;

    struct bmp_image* new = malloc(sizeof(struct bmp_image));

    new->header = malloc(sizeof(struct bmp_header));
    *new->header = *image->header;
    new->data = calloc(1,image->header->image_size);

    for (int i = 0; i<height; i++){
        for (int j=0; j<width; j++){
            new->data[(i*width)+j] = image->data[((height-1-i)*width)+j];
        }
    }
    return new;

}

struct bmp_image* flip_horizontally(const struct bmp_image* image){
    //fprintf (stderr,"flip horizontally\n");
    if (image == NULL) return NULL;

    uint32_t height = image->header->height, width = image->header->width;

    struct bmp_image* new = malloc(sizeof(struct bmp_image));

    new->header = malloc(sizeof(struct bmp_header));
    *new->header = *image->header;

    
    //printf("%d", new->header->height);
    //fprintf (stderr,"copying header\n");
    new->data = calloc(1,image->header->image_size);

    for (int i = 0; i<height; i++){
        for (int j=0; j<width; j++){
            new->data[(i*width)+j] = image->data[(i*width)+width-1-j];
        }
    }
    //fprintf (stderr,"flip done\n");
    return new;
}
