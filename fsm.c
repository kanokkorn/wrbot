#include "fsm.h"
#include <stdio.h>

/*
 * Initialize the Finite State Machine
 *
 * @param fsm: Pointer to the FSM structure
 */
void fsm_init(fsm_t *fsm) {
    if (fsm) {
        fsm->current_state = ROBOT_STATE_IDLE;
        printf("FSM initialized. Current state: IDLE\n");
    }
}

/*
 * Handle a state transition based on an event
 *
 * @param fsm:   Pointer to the FSM structure
 * @param event: The event that occurred
 */
void fsm_handle_event(fsm_t *fsm, robot_event_t event) {
    if (!fsm) return;

    printf("Handling event: %d in state: %d\n", event, fsm->current_state);

    switch (fsm->current_state) {
        case ROBOT_STATE_IDLE:
            if (event == ROBOT_EVENT_WAYPOINT_RECEIVED) {
                fsm->current_state = ROBOT_STATE_MOVING;
                printf("Transitioning from IDLE to MOVING\n");
                // TODO: Implement logic to start moving the robot
            }
            break;

        case ROBOT_STATE_MOVING:
            if (event == ROBOT_EVENT_WAYPOINT_REACHED) {
                fsm->current_state = ROBOT_STATE_WORKING;
                printf("Transitioning from MOVING to WORKING\n");
                // TODO: Implement logic to start the task
            }
            break;

        case ROBOT_STATE_WORKING:
            if (event == ROBOT_EVENT_TASK_COMPLETED) {
                fsm->current_state = ROBOT_STATE_IDLE;
                printf("Transitioning from WORKING to IDLE\n");
                // TODO: Implement logic for when task is done
            }
            break;
    }
}
