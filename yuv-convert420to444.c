// YUV Conversion:

// The problem is to convert a 3-plane, 4:2:0, 8-bit YUV buffer into a 2-Plane, 4:4:4, 8-bit YUV buffer, where:

// - for the input 4:2:0 YUV, the first plane contains the luma pixels, the second plane contains the chroma U pixels, and the third plane contains the Chroma V pixels.
// - for the output 4:4:4 YUV, the first plane contains the luma pixels and the second plane contains the chroma U and V pixels, interleaved.

uint8_t * convert420to444(uint8_t *input, uint32_t w, uint32_t h)
{
    uint8_t *output = (uint8_t*)malloc(3*h*w*sizeof(uint8_t));
    uint32_t y_height = uv_height = h;
    uint32_t y_width = w;
    uint32_t uv_width = 2*w;

    //Copy Y plane from input to output
    for (uint32_t i=0; i<y_height; i++) {
        for (uint32_t j=0; j<y_width; j++) {
            output[i*y_width+j] = input[i*w+j];
        }
    }

    //Upsample (simple copy) and interleave U and V planes
    //input u0 | v0 -> output u0 v0 u0 v0
    //                        v0 u0 v0 u0
    for (uint32_t i=y_height; i<(y_height+uv_height); i+=2) {
        for (uint32_t j=0; j<uv_width; j+=4) {
            uint8_t u_input = input[i*w/2+j/4];
            uint8_t v_input = input[(i+y_height)*w/2+j/4];
            output[i*uv_width+j] = u_input;
            output[i*uv_width+j+2] = u_input;
            output[(i+1)*uv_width+j+1] = u_input;
            output[(i+1)*uv_width+j+3] = u_input;
            output[i*uv_width+j+1] = v_input;
            output[i*uv_width+j+3] = v_input;
            output[(i+1)*uv_width+j] = v_input;
            output[(i+1)*uv_width+j+2] = v_input;
        }
    }

    return output;
}
