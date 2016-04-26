#version 440

#define TILE_SIZE_X 1
#define TILE_SIZE_Y 1
#define TOTAL_SIZE (TILE_SIZE_X * TILE_SIZE_Y)

#define N_STEPS 1

layout(local_size_x = 1,
       local_size_y = 1) in;

// Input
// ----------------------------------------------------------------------------
layout (binding = 0, r16ui) uniform readonly uimage2D k_tex;

// Output
// ----------------------------------------------------------------------------
/*! Map containing cluster index per tile. */
layout (binding = 1, r16ui) uniform writeonly uimage2D cluster_index_tex;

/*! compacted unique clusters */
layout (binding = 2, r16ui) uniform writeonly uimage2D unique_clusters_tex;

/*! specifies the number of clusters per tile. */
layout (binding = 3, r16ui) uniform writeonly uimage2D n_clusters_tex;

// TODO: optimise by packing properly
// ----------------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------------
void main() {
    uint sort_buffer_a[TOTAL_SIZE];
    uint sort_buffer_b[TOTAL_SIZE];

    uvec2 offset = gl_GlobalInvocationID.xy * uvec2(TILE_SIZE_X, TILE_SIZE_Y);

    // First Step
    uint index;
    uvec2 coord_i;

    uint val_1;
    uint val_2;

    for (uint y_i = 0; y_i < TILE_SIZE_Y; y_i++) {
        for (uint x_i = 0; x_i < TILE_SIZE_X / 2; x_i++) {
            coord_i = uvec2(x_i * 2, y_i);
            index = coord_i.x + coord_i.y * TILE_SIZE_X;

            val_1 = (imageLoad(k_tex, offset + coord_i) << 16) + index;
            val_2 = (imageLoad(k_tex, offset + coord_i + uvec(1, 0)) << 16) + index + 1;

            if (val_1 < val_2) {
                sort_buffer_a[index] = val_1;
                sort_buffer_a[index + 1] = val_2;
            } else {
                sort_buffer_a[index] = val_2;
                sort_buffer_a[index + 1] = val_1;
            }
            
        }
    }

    
    // Intermediate steps
    bool write_to_a = false;
    uint next_sub_array_size;

    uint sub_array_cursor_1;
    uint sub_array_cursor_2;
    uint offset_array_i;

    for (uint sort_step_i = 1; sort_step_i < N_STEPS - 1; sort_step_i++) {
        next_sub_array_size = 2 << sort_step_i;
        
        // Loop over each element in the read buffer and write to write buffer
        // --> loop over each sub array in the read buffer
        for ( uint array_i = 0; array_i < (TOTAL_SIZE / next_sub_array_size); array_i++) {
            offset_array_i = array_i * next_sub_array_size;
            sub_array_cursor_1 = 0;
            sub_array_cursor_2 = next_sub_array_size / 2;

            // --> loop over each element in the next sub array
            for (uint array_cursor = 0; array_cursor < next_sub_array_size; array_cursor++) {
                if (write_to_a) {
                    // sub_array_cursor_1 is done   
                    if (sub_array_cursor_1 >= next_sub_array_size / 2) {
                        sort_buffer_a[offset_array_i + array_cursor] = sort_buffer_b[offset_array_i + sub_array_cursor_2];
                        sub_array_cursor_2++;
                    // sub_array_cursor_2 is done    
                    } else if (sub_array_cursor_2 >= next_sub_array_size) {
                        sort_buffer_a[offset_array_i + array_cursor] = sort_buffer_b[offset_array_i + sub_array_cursor_1];
                        sub_array_cursor_1++;
                    // both are still in range
                    } else {
                        val_1 = sort_buffer_b[offset_array_i + sub_array_cursor_1];
                        val_2 = sort_buffer_b[offset_array_i + sub_array_cursor_2];
                        
                        if (val_1 < val_2) {
                            sort_buffer_a[offset_array_i + array_cursor] = val_1;
                            sub_array_cursor_1++;
                        } else {
                            sort_buffer_a[offset_array_i + array_cursor] = val_2;
                            sub_array_cursor_2++;
                        }
                    }
                } else {
                    // sub_array_cursor_1 is done   
                    if (sub_array_cursor_1 >= next_sub_array_size / 2) {
                        sort_buffer_b[offset_array_i + array_cursor] = sort_buffer_a[offset_array_i + sub_array_cursor_2];
                        sub_array_cursor_2++;
                    // sub_array_cursor_2 is done    
                    } else if (sub_array_cursor_2 >= next_sub_array_size) {
                        sort_buffer_b[offset_array_i + array_cursor] = sort_buffer_a[offset_array_i + sub_array_cursor_1];
                        sub_array_cursor_1++;
                    // both are still in range
                    } else {
                        val_1 = sort_buffer_a[offset_array_i + sub_array_cursor_1];
                        val_2 = sort_buffer_a[offset_array_i + sub_array_cursor_2];
                        
                        if (val_1 < val_2) {
                            sort_buffer_b[offset_array_i + array_cursor] = val_1;
                            sub_array_cursor_1++;
                        } else {
                            sort_buffer_b[offset_array_i + array_cursor] = val_2;
                            sub_array_cursor_2++;
                        }
                    }
                }
            }
        }
        write_to_a = !write_to_a;
    }

    // Final step
    int cursor_index = -1;
    int value_unique = -1;

    uint cursor_1 = 0;
    uint cursor_2 = TOTAL_SIZE / 2;

    uint output_val;
    uint output_k_val;
    uint output_pixel_index;

    uint output_x_coord;
    uint output_y_coord;

    uint x_unique;
    uint y_unique;
    for (uint index = 0; index < TOTAL_SIZE; index++) {
        // cursor_1 is done
        if (cursor_1 >= (TOTAL_SIZE / 2)) {
            if (write_to_a) {
                output_val = sort_buffer_b[cursor_2];
            } else {
                output_val = sort_buffer_a[cursor_2];
            }
            cursor_2++;
        // cursor_2 is done
        } else if (cursor_2 >= TOTAL_SIZE) {
            if (write_to_a) {
                output_val = sort_buffer_b[cursor_1];
            } else {
                output_val = sort_buffer_a[cursor_1];
            }
            cursor_1++;
        // both cursors are still in range
        } else {
            if (write_to_a) {
                val_1 = sort_buffer_b[cursor_1];
                val_2 = sort_buffer_b[cursor_2];
            } else {
                val_1 = sort_buffer_a[cursor_1];
                val_2 = sort_buffer_a[cursor_2];
            }

            if (val_1 < val_2) {
                output_val = val_1;
                cursor_1++;
            } else {
                output_val = val_2;
                cursor_2++;
            }
        }
        output_k_val = output_val >> 16;
        output_pixel_index = output_val & (~((output_val >> 16) << 16));
        
        output_x_coord = output_pixel_index % TILE_SIZE_X;
        output_y_coord = (output_pixel_index - output_x_coord ) / TILE_SIZE_X;

        // new cluster has been found
        if (output_k_val > value_unique) {
            cursor_index++;
            
            x_unique = cursor_index % TILE_SIZE_X;
            y_unique = (cursor_index - x_unique) / TILE_SIZE_X;

            imageStore(unique_clusters_tex, uvec2(x_unique, y_unique) + offset, output_k_val);
            value_unique = output_k_val;
        }

        imageStore(cluster_index_tex, uvec2(output_x_coord, output_y_coord) + offset, cursor_index);
    }

    // write away the total number of unique clusters found
    imageStore(n_clusters_tex, gl_GlobalInvocationID.xy, cursor_index + 1);
}