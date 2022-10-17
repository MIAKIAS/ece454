#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "utilities.h"  // DO NOT REMOVE this line
#include "implementation_reference.h"   // DO NOT REMOVE this line
#include <stdlib.h>
#include "immintrin.h"
#pragma GCC target ("avx,avx2")
#define piece_ratio 15


/***********************************************************************************************************************
 * WARNING: Do not modify the implementation_driver and team info prototype (name, parameter, return value) !!!
 *          Do not forget to modify the team_name and team member information !!!
 **********************************************************************************************************************/
void print_team_info(){
    // Please modify this field with something interesting
    char team_name[] = "男模编程";

    // Please fill in your information
    char student_first_name[] = "Weizhou";
    char student_last_name[] = "Wang";
    char student_student_number[] = "1004421262";

    // Printing out team information
    printf("*******************************************************************************************************\n");
    printf("Team Information:\n");
    printf("\tteam_name: %s\n", team_name);
    printf("\tstudent_first_name: %s\n", student_first_name);
    printf("\tstudent_last_name: %s\n", student_last_name);
    printf("\tstudent_student_number: %s\n", student_student_number);
}

/***********************************************************************************************************************
 * WARNING: Do not modify the implementation_driver and team info prototype (name, parameter, return value) !!!
 *          You can modify anything else in this file
 ***********************************************************************************************************************
 * @param sensor_values - structure stores parsed key value pairs of program instructions
 * @param sensor_values_count - number of valid sensor values parsed from sensor log file or commandline console
 * @param frame_buffer - pointer pointing to a buffer storing the imported  24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param grading_mode - turns off verification and turn on instrumentation
 ***********************************************************************************************************************
 *
 **********************************************************************************************************************/
void implementation_driver(struct kv *sensor_values, int sensor_values_count, unsigned char *frame_buffer,
                           unsigned int width, unsigned int height, bool grading_mode) {
    /*******************************************************************************************************************
     * Store only the colored pixels. Furture actions will only change the states of these pixles.
     *******************************************************************************************************************/
    // Count the total number of colored pixels
    int color_count = 0;
    int size = width * width * 3;
    register int i = 0;

    // A list stores the colors of all colored pixels. [R, G, B, R, G, B,......]
    unsigned char* color_buffer = (unsigned char*)malloc(size);
    // A list stores the coordinates of all colored pixels. [Row, Col, Row, Col, Row, Col,......]
    int* color_coordinate_rows = (int*)malloc(4 * width * width);
    int* color_coordinate_cols = (int*)malloc(4 * width * width);
    

    // Store the values to the lists
    int position_frame_buffer;
    int row_index = 3 * width;
    int piece_width = width / piece_ratio;
    int piece_length = piece_width * 3;
    int offset_rest = 3 * (width % piece_ratio);
    unsigned char* piece_of_blank = (unsigned char*)malloc(piece_length);
    memset(piece_of_blank, 255, piece_length);
    // color_count = 0;
    for (register int row = 0; row < width; ++row) {
        register int col = 0;
        for (register int piece = 0; piece < piece_ratio; ++piece) {
            if (!memcmp(frame_buffer+i, piece_of_blank, piece_length)) {
                col += piece_width;
                i += piece_length;
            } else {
                for (register int j = 0; j < piece_length; j += 3) {
                    if (frame_buffer[i] != 255 || frame_buffer[i+1] != 255 || frame_buffer[i+2] != 255) {
                        // memcpy(color_buffer+color_count*3, frame_buffer+i, 3);
                        *(int*)(color_buffer+color_count*3) = *((int*)(frame_buffer+i))&0x00ffffff;
                        color_coordinate_rows[color_count] = row;
                        color_coordinate_cols[color_count] = col;
                        ++color_count;
                    }
                    i += 3;
                    ++col;
                }
            }
        }
        if (memcmp(frame_buffer+i, piece_of_blank, offset_rest)) {
            for (; col < width; ++col) {
                if (frame_buffer[i] != 255 || frame_buffer[i+1] != 255 || frame_buffer[i+2] != 255) {
                    // memcpy(color_buffer+color_count*3, frame_buffer+i, 3);
                    *(int*)(color_buffer+color_count*3) = *((int*)(frame_buffer+i))&0x00ffffff;
                    color_coordinate_rows[color_count] = row;
                    color_coordinate_cols[color_count] = col;
                    ++color_count;
                }
                i += 3;
            }
        } else {
            i += offset_rest;
        }
        
    }

    /*******************************************************************************************************************
     * Summarize the actions performed in 25 frames into one frame
     *******************************************************************************************************************/
    int move_up = 0;
    int move_left = 0;
    int rotate_cw = 0;
    bool mirror_x= 0;
    bool mirror_y = 0;
    int temp; // For swapping numbers

    char key_0;
    char key_1;
    int value;
    // struct pixel curr_color;
    struct kv sensor_value;
    register int param = width - 1;
    __m256i params =_mm256_set1_epi32(param);
    // We do not care about the last <25 ones
    sensor_values_count = sensor_values_count - (sensor_values_count % 25);
    register int unroll = color_count - 7;
    for (register int sensorValueIdx = 0; sensorValueIdx < sensor_values_count; ++sensorValueIdx) {
        /* Here we accumulate the actions every 25 frames. Since the order of actions matters,
         * (e.g. A Rotation followed by A Moving Up does not equal to A Moving Up followed by A Rotation.)
         * we define that after each 25 frames, we first issue the rotation action, then mirroring,
         * and Horizontal and Vertical Moving in the end.
         * 
         * As a result, the later ones need to translate the corresponding actions according to
         * the former ones.
         */
        sensor_value = sensor_values[sensorValueIdx];
        key_0 = *sensor_value.key;
        key_1 = *(sensor_value.key + 1);
        value = sensor_value.value;
        switch (key_0) {
            case 87: // W
                move_up += value;
                break;
            case 65: // A
                move_left += value;
                break;
            case 83: // S
                move_up -= value;
                break;
            case 68: // D
                move_left -= value;
                break;
            case 67: // C for CW AND CCW
                // CCW
                if (key_1 == 67) {
                    value *= -1;
                }
                value %= 4;
                switch (value) {
                    // Rotate CW 90 degrees
                    case 1: 
                    case -3:
                        value = 1;
                        // Translate the Moving actions to after the Rotation
                        // Move: left->up, down->left
                        temp = move_up;
                        move_up = move_left;
                        move_left = -1 * temp;
                        // Translate the Mirroring actions to after the Rotation
                        // Flip: X->Y, Y->X
                        temp = mirror_y;
                        if (mirror_x) {
                            mirror_x = !mirror_x;
                            mirror_y = !mirror_y;
                        }
                        if (temp) {
                            mirror_y = !mirror_y;
                            mirror_x = !mirror_x;
                        }
                        break;
                    // Rotate 180 degrees
                    case 2: 
                    case -2:
                        // Same as MX + MY
                        value = 0;
                        // Translate the Moving actions to after the Rotation
                        move_left *= -1;
                        move_up *= -1;

                        mirror_x = !mirror_x;
                        mirror_y = !mirror_y;
                        break;
                    // Rotate CCW 90 degrees
                    case 3:
                    case -1:
                        value = -1;
                        // Translate the Moving actions to after the Rotation
                        // Move: right->up, up->left
                        temp = move_up;
                        move_up = -1 * move_left;
                        move_left = temp;
                        // Translate the Mirroring actions to after the Rotation
                        // Flip: X->Y, Y->X
                        temp = mirror_y;
                        if (mirror_x) {
                            mirror_x = !mirror_x;
                            mirror_y = !mirror_y;
                        }
                        if (temp) {
                            mirror_y = !mirror_y;
                            mirror_x = !mirror_x;
                        }
                        break;
                }
                rotate_cw += value;
                break;
            default:
                if (key_1 == 88) { // MX
                    mirror_x = !mirror_x;
                    // Translate the Mirroring actions to after the Rotation
                    // Move: up->down
                    move_up *= -1;
                } else { // MY
                    mirror_y = !mirror_y;
                    // Translate the Mirroring actions to after the Rotation
                    // Move: left->right, right->left
                    move_left *= -1;
                }
        }

        // Perform one action every 25 frame
        if (!((sensorValueIdx+1) % 25)) {
            // Clean up old colored pixles
            memset(frame_buffer, 255, size);

            rotate_cw %= 4;
            for (i = 0; i < unroll; i+=8) {
                __m256i rows = _mm256_loadu_si256((const __m256i *)(color_coordinate_rows+i));
                __m256i cols = _mm256_loadu_si256((const __m256i *)(color_coordinate_cols+i));
                
                if (rotate_cw) {
                    // printf("Rotate CW %d degrees", rotate_cw);
                    if (rotate_cw == 2 || rotate_cw == -2) {
                        rows = _mm256_sub_epi32(params, rows);
                        cols = _mm256_sub_epi32(params, cols);
                    } else if (rotate_cw == 1 || rotate_cw == -3) {
                        __m256i temps = rows;
                        rows = cols;
                        cols = _mm256_sub_epi32(params, temps);
                    } else {
                        __m256i temps = rows;
                        rows = _mm256_sub_epi32(params, cols);
                        cols = temps;
                    }
                }
                if (mirror_x) {
                    // printf("Mirror X\n");
                    rows = _mm256_sub_epi32(params, rows);
                }
                if (mirror_y) {
                    // printf("Mirror Y\n");
                    cols = _mm256_sub_epi32(params, cols);
                }
                if (move_up) {
                    // printf("Move up %d units\n", move_up);
                    rows = _mm256_sub_epi32(rows, _mm256_set1_epi32(move_up));
                }
                if (move_left) {
                    // printf("Move left %d units\n\n", move_left);
                    cols = _mm256_sub_epi32(cols, _mm256_set1_epi32(move_left));
                }

                // memcpy(color_coordinate+index, &color_coordinate_col_3, 24);
                _mm256_storeu_si256((__m256i*)(color_coordinate_rows+i), rows);
                _mm256_storeu_si256((__m256i*)(color_coordinate_cols+i), cols);

                unsigned char* location = color_buffer + i*3;
                memcpy(frame_buffer + color_coordinate_rows[i] * row_index + color_coordinate_cols[i] * 3, location, 3);
                memcpy(frame_buffer + color_coordinate_rows[i+1] * row_index + color_coordinate_cols[i+1] * 3, location+3, 3);
                memcpy(frame_buffer + color_coordinate_rows[i+2] * row_index + color_coordinate_cols[i+2] * 3, location+6, 3);
                memcpy(frame_buffer + color_coordinate_rows[i+3] * row_index + color_coordinate_cols[i+3] * 3, location+9, 3);
                memcpy(frame_buffer + color_coordinate_rows[i+4] * row_index + color_coordinate_cols[i+4] * 3, location+12, 3);
                memcpy(frame_buffer + color_coordinate_rows[i+5] * row_index + color_coordinate_cols[i+5] * 3, location+15, 3);
                memcpy(frame_buffer + color_coordinate_rows[i+6] * row_index + color_coordinate_cols[i+6] * 3, location+18, 3);
                memcpy(frame_buffer + color_coordinate_rows[i+7] * row_index + color_coordinate_cols[i+7] * 3, location+21, 3);
            }
            for (; i < color_count; ++i) {
                register int color_coordinate_row = color_coordinate_rows[i];
                register int color_coordinate_col = color_coordinate_cols[i];
                if (rotate_cw) {
                    // printf("Rotate CW %d degrees", rotate_cw);
                    if (rotate_cw == 2 || rotate_cw == -2) {
                        color_coordinate_row = param - color_coordinate_row;
                        color_coordinate_col = param - color_coordinate_col;
                    } else if (rotate_cw == 1 || rotate_cw == -3) {
                        temp = color_coordinate_row;
                        color_coordinate_row = color_coordinate_col;
                        color_coordinate_col = param - temp;
                    } else {
                        temp = color_coordinate_row;
                        color_coordinate_row = param - color_coordinate_col;
                        color_coordinate_col = temp;
                    }
                }
                if (mirror_x) {
                    // printf("Mirror X\n");
                    color_coordinate_row = param - color_coordinate_row;
                }
                if (mirror_y) {
                    // printf("Mirror Y\n");
                    color_coordinate_col = param - color_coordinate_col;
                }
                if (move_up) {
                    // printf("Move up %d units\n", move_up);
                    color_coordinate_row -= move_up;
                }
                if (move_left) {
                    // printf("Move left %d units\n\n", move_left);
                    color_coordinate_col -= move_left;
                }

                color_coordinate_rows[i] = color_coordinate_row;
                color_coordinate_cols[i] = color_coordinate_col;

                // Write new colored pixels back to the frame
                memcpy(frame_buffer + color_coordinate_row * row_index + color_coordinate_col * 3, color_buffer + i*3, 3);
            }

            // Clear them up for next iteration
            move_up = 0;
            move_left = 0;
            rotate_cw = 0;
            mirror_x= 0;
            mirror_y = 0;

            verifyFrame(frame_buffer, width, width, grading_mode);
        }
    }
    return;
}
