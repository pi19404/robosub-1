#include "state/state_machine.h"
#include <cv.h>
#include <highgui.h>
#include <vector>
#include <string>

// TODO(LPE) Things should eventually work without including mock_dlm.h
#include "decision/mock_dlm.h"
#include "decision/tollbooth_dlm.h"
#include "vision/base_eye.h"
#include "movement/fuzzy_sets.h"
#include "utility/DebugLog.hpp"

using ::std::vector;
using ::std::map;
using ::std::string;

namespace state {
  StateMachine::StateMachine(
      cv::VideoCapture forward_eye, cv::VideoCapture downward_eye) {
    DEBUG_METHOD();
    m_fuzzy_sub_state = new movement::FuzzySets();

    // TODO(LPE) All of the MockDLM objects should be replaced with
    // the appropriate DLM.
    m_task_to_dlm[START] = new decision::MockDLM(
        m_fuzzy_sub_state, forward_eye, downward_eye);
    m_task_to_dlm[PATH] = new decision::MockDLM(
        m_fuzzy_sub_state, forward_eye, downward_eye);
    m_task_to_dlm[TRAFFIC_LIGHT] = new decision::MockDLM(
        m_fuzzy_sub_state, forward_eye, downward_eye);
    m_task_to_dlm[PARKING] = new decision::MockDLM(
        m_fuzzy_sub_state, forward_eye, downward_eye);
    m_task_to_dlm[SPEED_TRAP] = new decision::MockDLM(
        m_fuzzy_sub_state, forward_eye, downward_eye);
    m_task_to_dlm[TOLLBOOTH] = new decision::TollboothDLM(
        m_fuzzy_sub_state, forward_eye, downward_eye);
    m_task_to_dlm[DRIVING] = new decision::MockDLM(
        m_fuzzy_sub_state, forward_eye, downward_eye);
    m_task_to_dlm[PIZZA] = new decision::MockDLM(
        m_fuzzy_sub_state, forward_eye, downward_eye);

    for (int from_dex = 0; from_dex < NUM_TASKS; from_dex++) {
      for (int to_dex = 0; to_dex < NUM_TASKS; to_dex++) {
        m_can_transition[from_dex][to_dex] = false;
      }
    }
    // This is ugly. Is there a better way to do this?
    m_can_transition[START][PATH] = true;
    m_can_transition[PATH][TRAFFIC_LIGHT] = true;
    m_can_transition[PATH][PARKING] = true;
    m_can_transition[PATH][SPEED_TRAP] = true;
    m_can_transition[PATH][TOLLBOOTH] = true;
    m_can_transition[PATH][DRIVING] = true;
    m_can_transition[PATH][PIZZA] = true;
    m_can_transition[TRAFFIC_LIGHT][PATH] = true;
    m_can_transition[PARKING][PATH] = true;
    m_can_transition[SPEED_TRAP][PATH] = true;
    m_can_transition[TOLLBOOTH][PATH] = true;
    m_can_transition[DRIVING][PATH] = true;
    m_can_transition[PIZZA][PATH] = true;
  }

  StateMachine::~StateMachine() {
  }

  void StateMachine::activate() {
    DEBUG_METHOD();
    m_task_to_dlm[START]->activate();

    task_t previous_task = START;
    map<task_t, decision::DecisionLogicModule*>::iterator it;
    bool can_continue = true;
    while (can_continue) {
      for (it = m_task_to_dlm.begin(); it != m_task_to_dlm.end(); ++it) {
        // PATH can transition to most everything, most everything can
        // only transition to PATH. If this assumption becomes invalid,
        // this if statement will need to be overhauled.
        if (!it->second->get_mission_accomplished() &&
            m_can_transition[previous_task][it->first]) {
          it->second->activate();
          previous_task = it->first;
          break;
        }
      }

      // Check to see if we are done.
      can_continue = false;
      for (it = m_task_to_dlm.begin(); it != m_task_to_dlm.end(); ++it) {
        if (m_can_transition[previous_task][it->first] &&
            !it->second->get_mission_accomplished()) {
          can_continue = true;
          break;
        }
      }
    }
  }
}  // namespace state

