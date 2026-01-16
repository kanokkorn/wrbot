/*
 * fsm.h - Finite State Machine for the robot
 */

#ifndef FSM_H
#define FSM_H

/* Robot States */
typedef enum {
    ROBOT_STATE_IDLE,
    ROBOT_STATE_MOVING,
    ROBOT_STATE_WORKING
} robot_state_t;

/* Robot Events */
typedef enum {
    ROBOT_EVENT_WAYPOINT_RECEIVED,
    ROBOT_EVENT_WAYPOINT_REACHED,
    ROBOT_EVENT_TASK_COMPLETED
} robot_event_t;

/* FSM structure */
typedef struct {
    robot_state_t current_state;
} fsm_t;

/* Function Prototypes */
void fsm_init(fsm_t *fsm);
void fsm_handle_event(fsm_t *fsm, robot_event_t event);

#endif /* FSM_H */
