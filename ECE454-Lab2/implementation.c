#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "utilities.h"  // DO NOT REMOVE this line
#include "implementation_reference.h"   // DO NOT REMOVE this line

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image up
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
unsigned char *processMoveUp(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
    return processMoveUpReference(buffer_frame, width, height, offset);
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image left
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
unsigned char *processMoveRight(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
    return processMoveRightReference(buffer_frame, width, height, offset);
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image up
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
unsigned char *processMoveDown(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
    return processMoveDownReference(buffer_frame, width, height, offset);
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image right
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
unsigned char *processMoveLeft(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
    return processMoveLeftReference(buffer_frame, width, height, offset);
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param rotate_iteration - rotate object inside frame buffer counter clockwise by 90 degrees, <iteration> times
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note: You can assume the frame will always be square and you will be rotating the entire image
 **********************************************************************************************************************/
unsigned char *processRotateCCW(unsigned char *buffer_frame, unsigned width, unsigned height,
                                int rotate_iteration) {
    return processRotateCCWReference(buffer_frame, width, height, rotate_iteration);
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param _unused - this field is unused
 * @return
 **********************************************************************************************************************/
unsigned char *processMirrorX(unsigned char *buffer_frame, unsigned int width, unsigned int height, int _unused) {
    return processMirrorXReference(buffer_frame, width, height, _unused);
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param _unused - this field is unused
 * @return
 **********************************************************************************************************************/
unsigned char *processMirrorY(unsigned char *buffer_frame, unsigned width, unsigned height, int _unused) {
    return processMirrorYReference(buffer_frame, width, height, _unused);
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param rotate_iteration - rotate object inside frame buffer clockwise by 90 degrees, <iteration> times
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note: You can assume the frame will always be square and you will be rotating the entire image
 **********************************************************************************************************************/
unsigned char *processRotate(unsigned char *buffer_frame, unsigned width, int rotate_iteration) {
    // Because of the accumulation, the iteration may still greater than 4
    rotate_iteration %= 4;

    // Rotate 180 degrees, same as MX + MY
    if (rotate_iteration == 2 || rotate_iteration == -2) {
        return processMirrorY(processMirrorX(buffer_frame, width, width, 0), width, width, 0);
    } else {
        // allocate memory for temporary image buffer
        unsigned char *rendered_frame = allocateFrame(width, width);
        // Rotate CW 90 degrees
        if (rotate_iteration == 1 || rotate_iteration == -3) { 
            // store shifted pixels to temporary buffer
            int render_column = width - 1;
            int render_row = 0;
            int position_frame_buffer;
            for (int row = 0; row < width; row++) {
                for (int column = 0; column < width; column++) {
                    position_frame_buffer = row * width * 3 + column * 3;
                    rendered_frame[render_row * width * 3 + render_column * 3] = buffer_frame[position_frame_buffer];
                    rendered_frame[render_row * width * 3 + render_column * 3 + 1] = buffer_frame[position_frame_buffer + 1];
                    rendered_frame[render_row * width * 3 + render_column * 3 + 2] = buffer_frame[position_frame_buffer + 2];
                    render_row += 1;
                }
                render_row = 0;
                render_column -= 1;
            }
        // Rotate CCW 90 degrees
        } else if (rotate_iteration == 3 || rotate_iteration == -1){
            // store shifted pixels to temporary buffer
            int render_column = 0;
            int render_row = width - 1;
            int position_frame_buffer;
            for (int row = 0; row < width; row++) {
                for (int column = 0; column < width; column++) {
                    position_frame_buffer = row * width * 3 + column * 3;
                    rendered_frame[render_row * width * 3 + render_column * 3] = buffer_frame[position_frame_buffer];
                    rendered_frame[render_row * width * 3 + render_column * 3 + 1] = buffer_frame[position_frame_buffer + 1];
                    rendered_frame[render_row * width * 3 + render_column * 3 + 2] = buffer_frame[position_frame_buffer + 2];
                    render_row -= 1;
                }
                render_row = width - 1;
                render_column += 1;
            }
        }
        // copy the temporary buffer back to original frame buffer
        buffer_frame = copyFrame(rendered_frame, buffer_frame, width, width);
        // free temporary image buffer
        deallocateFrame(rendered_frame);
        return buffer_frame;
    }
}

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
            rotate_cw %= 4;
            if (rotate_cw) {
                //printf("Rotate CW %d degrees\n", rotate_cw);
                frame_buffer = processRotate(frame_buffer, width, rotate_cw);
            }
            if (mirror_x) {
                //printf("Mirror X\n");
                frame_buffer = processMirrorX(frame_buffer, width, height, 0);
            }
            if (mirror_y) {
                //printf("Mirror Y\n");
                frame_buffer = processMirrorY(frame_buffer, width, height, 0);
            }
            if (move_up) {
                //printf("Move up %d units\n", move_up);
                frame_buffer = processMoveUp(frame_buffer, width, height, move_up);
            }
            if (move_left) {
                // printf("Move left %d units\n\n", move_left);
                frame_buffer = processMoveLeft(frame_buffer, width, height, move_left);
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
    return;
}
