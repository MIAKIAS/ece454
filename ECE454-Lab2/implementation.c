#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "utilities.h"  // DO NOT REMOVE this line
#include "implementation_reference.h"   // DO NOT REMOVE this line
#include <stdlib.h>

struct pixel {
    unsigned char R;
    unsigned char G;
    unsigned char B;
    int row;
    int col;
};

/***********************************************************************************************************************
 * WARNING: Do not modify the implementation_driver and team info prototype (name, parameter, return value) !!!
 *          Do not forget to modify the team_name and team member information !!!
 **********************************************************************************************************************/
void print_team_info(){
    // Please modify this field with something interesting
    char team_name[] = "aaa";

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
    int size = width * height * 3;
    int i;

    // A list stores the colored pixels' RGBs and coordinates
    struct pixel* color = (struct pixel*)malloc(width * height * sizeof(struct pixel));
    // // A list stores the colors of all colored pixels. [R, G, B, R, G, B,......]
    // unsigned char* color_buffer = (unsigned char*)malloc(3 * color_count * sizeof(char));
    // // A list stores the coordinates of all colored pixels. [Row, Col, Row, Col, Row, Col,......]
    // int* color_coordinate = (int*)malloc(2 * color_count * sizeof(int));

    // Store the values to the lists
    int position_frame_buffer;
    int row_index = 3 * width;
    // color_count = 0;
    for (i = 0; i < size; i += 3) {
        if (frame_buffer[i] != 255 || frame_buffer[i+1] != 255 || frame_buffer[i+2] != 255) {
            color[color_count].R = frame_buffer[i];
            color[color_count].G = frame_buffer[i+1];
            color[color_count].B = frame_buffer[i+2];
            color[color_count].row = i / row_index;
            color[color_count].col = (i / 3) % width;
            ++color_count;
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

    for (int sensorValueIdx = 0; sensorValueIdx < sensor_values_count; ++sensorValueIdx) {
        /* Here we accumulate the actions every 25 frames. Since the order of actions matters,
         * (e.g. A Rotation followed by A Moving Up does not equal to A Moving Up followed by A Rotation.)
         * we define that after each 25 frames, we first issue the rotation action, then mirroring,
         * and Horizontal and Vertical Moving in the end.
         * 
         * As a result, the later ones need to translate the corresponding actions according to
         * the former ones.
         */
        char* key = sensor_values[sensorValueIdx].key;
        int value = sensor_values[sensorValueIdx].value;
        if (!strcmp(key, "W")) {
            move_up += value;
        } else if (!strcmp(key, "A")) {
            move_left += value;
        } else if (!strcmp(key, "S")) {
            move_up -= value;
        } else if (!strcmp(key, "D")) {
            move_left -= value;
        } else if (!strcmp(key, "CW") || !strcmp(key, "CCW")) {
            if (!strcmp(key, "CCW")) {
                value *= -1;
            }
            value %= 4;
            // Rotate CW 90 degrees
            if (value == 1 || value == -3) {
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
            // Rotate 180 degrees
            } else if (value == 2 || value == -2) {
                // Same as MX + MY
                value = 0;
                // Translate the Moving actions to after the Rotation
                move_left *= -1;
                move_up *= -1;

                mirror_x = !mirror_x;
                mirror_y = !mirror_y;
            // Rotate CCW 90 degrees
            } else if (value == 3 || value == -1){
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
            }
            rotate_cw += value;
        } else if (!strcmp(key, "MX")) {
            mirror_x = !mirror_x;
            // Translate the Mirroring actions to after the Rotation
            // Move: up->down
            move_up *= -1;
        } else if (!strcmp(key, "MY")) {
            mirror_y = !mirror_y;
            // Translate the Mirroring actions to after the Rotation
            // Move: left->right, right->left
            move_left *= -1;
        }

        // Perform one action every 25 frame
        if ((sensorValueIdx+1) % 25 == 0) {
            // Clean up old colored pixles
            for (i = 0; i < color_count; ++i) {
                position_frame_buffer = (color[i].row * width + color[i].col) * 3;
                frame_buffer[position_frame_buffer] = 255;
                frame_buffer[position_frame_buffer+1] = 255;
                frame_buffer[position_frame_buffer+2] = 255;  
            }

            rotate_cw %= 4;
            for (i = 0; i < color_count; ++i) {
                if (rotate_cw) {
                    // printf("Rotate CW %d degrees", rotate_cw);
                    if (rotate_cw == 2 || rotate_cw == -2) {
                        color[i].row = height - color[i].row - 1;
                        color[i].col = width - color[i].col - 1;
                    } else if (rotate_cw == 1 || rotate_cw == -3) {
                        temp = color[i].row;
                        color[i].row = color[i].col;
                        color[i].col = width - 1 - temp;
                    } else {
                        temp = color[i].row;
                        color[i].row = height - 1 - color[i].col;
                        color[i].col = temp;
                    }
                }
                if (mirror_x) {
                    // printf("Mirror X\n");
                    color[i].row = height - color[i].row - 1;
                }
                if (mirror_y) {
                    // printf("Mirror Y\n");
                    color[i].col = width - color[i].col - 1;
                }
                if (move_up) {
                    // printf("Move up %d units\n", move_up);
                    color[i].row -= move_up;
                }
                if (move_left) {
                    // printf("Move left %d units\n\n", move_left);
                    color[i].col -= move_left;
                }

                // Write new colored pixels back to the frame
                position_frame_buffer = (color[i].row * width + color[i].col) * 3;
                frame_buffer[position_frame_buffer] = color[i].R;
                frame_buffer[position_frame_buffer+1] = color[i].G;
                frame_buffer[position_frame_buffer+2] = color[i].B;
            }

            verifyFrame(frame_buffer, width, height, grading_mode);

            // Clear them up for next iteration
            move_up = 0;
            move_left = 0;
            rotate_cw = 0;
            mirror_x= 0;
            mirror_y = 0;
        }
    }
    free(color);
    return;
}
