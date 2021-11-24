#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "bmp.h"
#include "transformations.h"

int main(){
    //bmp tests
    FILE * bmp = fopen("cj.bmp", "rb");

    FILE * writing = fopen("myshittywriting.bmp", "wb");
    struct bmp_image *read = read_bmp(bmp);

    struct bmp_image *edit1 = flip_horizontally(read);
    struct bmp_image *edit2 = flip_vertically(edit1);
    struct bmp_image *edit3 = rotate_left(edit2);
    struct bmp_image *edit4 = rotate_right(edit3);
    struct bmp_image *edit5 = crop(edit4,3,3,50,50);
    struct bmp_image *edit6 = scale(edit5, 2.3);
    struct bmp_image *edit7 = extract(edit6, "rg");
    
    bool answer = write_bmp(writing, edit5);
    printf ("%d", answer);

    free_bmp_image(read);
    free_bmp_image(edit1);
    free_bmp_image(edit2);
    free_bmp_image(edit3);
    free_bmp_image(edit4);
    free_bmp_image(edit5);
    free_bmp_image(edit6);
    free_bmp_image(edit7);
    

    fclose(bmp);
    fclose(writing);
    return 0;
}

// int main(){
    
//     FILE * bmp = fopen("/home/john/prog-2021/ps4/arena.bmp", "rb");

//     //fprintf (stderr,"lll\n");

//     FILE * writing = fopen("/home/john/prog-2021/ps4/assets/myshittywriting.bmp", "wb");
//     struct bmp_image *read = malloc(sizeof(struct bmp_image));
//     read = read_bmp(bmp);
//     //fprintf (stderr,"lll\n");

//     struct bmp_image *flipped = scale(read, 1.502948);

//     printf("size %d\n", flipped->header->size);
//     printf("image size %d\n", flipped->header->image_size);

//     bool answer = write_bmp(writing, flipped);
//     printf ("%d", answer);


//     free_bmp_image(read);
//     free_bmp_image(flipped);
    

//     fclose(bmp);
//     fclose(writing);
//     return 0;
// }