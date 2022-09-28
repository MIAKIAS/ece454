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
 * @param rotate_iteration - rotate object inside frame buffer clockwise by 90 degrees, <iteration> times
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note: You can assume the frame will always be square and you will be rotating the entire image
 **********************************************************************************************************************/
unsigned char *processRotateCW(unsigned char *buffer_frame, unsigned width, unsigned height,
                               int rotate_iteration) {
    return processRotateCWReference(buffer_frame, width, height, rotate_iteration);
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
 * WARNING: Do not modify the implementation_driver and team info prototype (name, parameter, return value) !!!
 *          Do not forget to modify the team_name and team member information !!!
 **********************************************************************************************************************/
void print_team_info(){
    // Please modify this field with something interesting
    char team_name[] = "打中文会变慢吗";

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
                // Move: left->up, down->left
                temp = move_up;
                move_up = move_left;
                move_left = -1 * temp;
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
                move_left *= -1;
                move_up *= -1;
                mirror_x = !mirror_x;
                mirror_y = !mirror_y;
            // Rotate CCW 90 degrees
            } else if (value == 3 || value == -1){
                value = -1;
                // Move: right->up, up->left
                temp = move_up;
                move_up = -1 * move_left;
                move_left = temp;
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
            // Move: up->down
            move_up *= -1;
        } else if (!strcmp(key, "MY")) {
            // Move: left->right, right->left
            mirror_y = !mirror_y;
            move_left *= -1;
        }

        // Perform one action every 25 frame
        if ((sensorValueIdx+1) % 25 == 0) {
            rotate_cw %= 4;
            if (rotate_cw) {
                //printf("Rotate CW %d degrees\n", rotate_cw);
                frame_buffer = processRotateCW(frame_buffer, width, height, rotate_cw);
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
