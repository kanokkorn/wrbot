#ifndef FSM_H
#define FSM_H

typedef enum {
  ROBOT_STATE_IDLE,
  ROBOT_STATE_MOVING,
  ROBOT_STATE_WORKING
} robot_state_t;

typedef enum {
  ROBOT_EVENT_WAYPOINT_RECEIVED,
  ROBOT_EVENT_WAYPOINT_REACHED,
  ROBOT_EVENT_TASK_COMPLETED
} robot_event_t;

typedef struct {
  robot_state_t current_state;
} fsm_t;

void fsm_init(fsm_t *fsm);
void fsm_handle_event(fsm_t *fsm, robot_event_t event);

#endif /* FSM_H */
