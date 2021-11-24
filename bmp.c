#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

// int main(){
//     FILE* stream = fopen("/home/john/prog-2021/ps4/assets/cherry.bmp", "rb");
//     //struct bmp_header *header = read_bmp_header(bmp);
//     //read_data(bmp, header);
//     //read_bmp(bmp);

//     FILE * writing = fopen("/home/john/prog-2021/ps4/assets/myshittywriting.bmp", "wb");
//     struct bmp_image *read = read_bmp(stream);

//     bool answer = write_bmp(writing, read);
//     printf ("%d", answer);
//     fclose(writing);

//     //FILE * written = fopen("/home/john/prog-2021/ps4/assets/myshittywriting.bmp", "rb");

//     free (read);
//     //read_bmp(written);

//     fclose(stream);
//     //fclose(written);
//     return 0;
// }

bool write_bmp(FILE* stream, const struct bmp_image* image){
    //fprintf (stderr,"write bmp start\n");
    if (stream == NULL || image == NULL || image->data == NULL || image->header == NULL) return false;
    uint32_t height = image->header->height, width = image->header->width;

    //int n = 0;

    fwrite(image->header, sizeof(struct bmp_header), 1, stream);
    // if (n!= 1){
    //     printf("header failed");
    //     free (stream);
    //     return false;
    // }

    //n = 0;

    const unsigned char padding[3] = {'\0', '\0', '\0'};
    
    for (int y = 0; y <height; y++){
		for(uint32_t z = 0;z<width;z++){
            fwrite(&image->data[(y*width)+z],sizeof(struct pixel),1,stream);
        }
        fwrite(padding, sizeof(unsigned char), width%4,stream);
    }

    // if (n!=1){
    //     printf("data failed");
    //     free(stream);
    //     return false;
    // }
    //fprintf (stderr,"write bmp\n");

    return true;
}

struct bmp_image* read_bmp(FILE* stream){
    if (stream == NULL) return NULL;

    struct bmp_image *result = malloc(sizeof(struct bmp_image));
    result->header = read_bmp_header(stream);
    if (result->header == NULL) {
        fprintf(stderr, "Error: This is not a BMP file.\n");
        free(result);
        return NULL;
    }
    //fprintf (stderr,"lll\n");
    //result->header = header;
    result->data = read_data(stream, result->header);
    if (result->data == NULL){
        fprintf(stderr, "Error: Corrupted BMP file.\n");
        free_bmp_image(result);
        return NULL;
    } 
    //result->data = pixel;
    //fprintf (stderr,"read bmp\n");
    return result;
}

struct bmp_header* read_bmp_header(FILE* stream){
    if (stream == NULL || ftell(stream) < 0) return NULL;
    
    struct bmp_header *Header = malloc(sizeof(struct bmp_header));

    fseek (stream, 0, SEEK_SET);
    
    // fread(Header, sizeof(struct bmp_header), 1, stream);

    // fread(Header->type, 2, 1, stream);
    // fread(Header->size, 4, 1, stream);
    // fread(Header->reserved1, 2, 1, stream);
    // fread(Header->reserved2, 2, 1, stream);
    // fread(Header->offset, 4, 1, stream);
    // fread(Header->dib_size, 4, 1, stream);
    // fread(Header->width, 4, 1, stream);
    // fread(Header->height, 4, 1, stream);
    // fread(Header->planes, 2, 1, stream);
    // fread(Header->bpp, 2, 1, stream);
    // fread(Header->compression, 4, 1, stream);
    // fread(Header->image_size, 4, 1, stream);
    // fread(Header->x_ppm, 4, 1, stream);
    // fread(Header->y_ppm, 4, 1, stream);
    // fread(Header->num_colors, 4, 1, stream);
    // fread(Header->important_colors, 4, 1, stream);

    if (fread(Header, sizeof(struct bmp_header), 1, stream) != 1 || Header->type != 0x4d42) {
        free(Header);
        return NULL;
    }
    if (Header->height<=0 || Header->width<=0){
        free(Header);
        return NULL;
    }
    // fprintf (stderr,"read header\n");
    // printf("%d\n", Header->width);
    return Header;
}

struct pixel* read_data(FILE* stream, const struct bmp_header* header){
    if (stream == NULL || ftell(stream) < 0 || header == NULL) return NULL;
    //fprintf (stderr,"lll\n");
    uint32_t height = header->height, width = header->width;

    //uint32_t padding = width%4;
    //fprintf (stderr,"lll\n");
	struct pixel* pixels = calloc(1, header->image_size);;
    fseek(stream, sizeof(struct bmp_header), 0);
    //fprintf (stderr,"lll\n");
	for (uint32_t i = 0; i <height; i++){
		for(uint32_t j = 0;j<width;j++){
			fread (&pixels[(i*width)+j], sizeof (struct pixel), 1, stream);	
            //fprintf (stderr,"lll\n");	
		}
		if (i!=height-1) fseek (stream, width%4, SEEK_CUR);	
	}
    //fprintf (stderr,"read data\n");
    return pixels;
}

void free_bmp_image(struct bmp_image* image){
    if ( image != NULL){
        if (image->header != NULL) free (image->header);
        if (image->data != NULL) free (image->data);
        free (image);
    }
}
