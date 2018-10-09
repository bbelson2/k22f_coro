/*
 * scheduler.cpp
 *
 *  Created on: 8 Aug 2018
 *      Author: belsonba
 */

#include "scheduler.h"

namespace scheduling {

task_data_t task_control_block_t::getData() const {
	return data;
}

task_state_t task_control_block_t::getState() const {
	return state;
}

task_state_t task_control_block_t::setState(task_state_t newState) volatile {
	task_state_t oldState = state;
	state = newState;
	return oldState;
}

task_priority_t task_control_block_t::getPriority() const {
	return priority;
}

task_priority_t task_control_block_t::setPriority(task_priority_t newPriority) {
	// TODO: consider pushing old value onto a stack (change verb e.g. pushPriority)
	task_priority_t oldPriority = priority;
	priority = newPriority;
	return oldPriority;
}

void task_control_block_t::run() {
	taskfn(this);
}

void idle_task_fn(task_control_block_t*);

/* static */
task_control_block_t scheduler::m_idle_task = task_control_block_t(
	idle_task_fn,
	0,
	PRIORITY_LOWEST,
	task_state_t::TASK_READY
);

void idle_task_fn(task_control_block_t*) {
	__asm("wfi");
}

scheduler::scheduler() : m_taskCount(0),
			  m_taskMax(SCHEDULER_MAX_TASKS),
			  m_lastTaskIndex(SCHEDULER_MAX_TASKS)
{
	for (uint8_t i = 0; i < m_taskMax; i++) {
		m_tasks[i] = 0;
	}
	registerTask(&m_idle_task);
}

bool scheduler::registerTask(task_control_block_t* t) {
	if (m_taskCount >= m_taskMax) {
		return false;
	}
	m_tasks[m_taskCount++] = t;
	updateCounts();
	return true;
}

void scheduler::runTask(uint8_t taskIndex) {
	task_control_block_t* t = m_tasks[taskIndex];
	m_lastTaskIndex = taskIndex;
	t->setState(task_state_t::TASK_RUNNING);

	t->run();

	switch (t->getState()) {
	case task_state_t::TASK_RUNNING:
		t->setState(task_state_t::TASK_READY);
		break;
	case task_state_t::TASK_CLOSING:
		t->setState(task_state_t::TASK_CLOSED);
		updateCounts();
		break;
	default:
		break;
	}
}

int scheduler::run() {
	while (true) {
		uint8_t nextTaskIndex = getNextTask();
		if (nextTaskIndex == (uint8_t)-1) {
			return -1;
		}
		runTask(nextTaskIndex);
	}
}

uint8_t scheduler::getNextTask() const {
	uint8_t iLastTask = (m_lastTaskIndex >= m_taskCount)
			? (m_taskCount - 1) : m_lastTaskIndex;
	uint8_t nextTaskIndex = (uint8_t)-1;
	task_control_block_t* nextTask = 0;
	task_priority_t nextPriority = PRIORITY_LOWEST;

	for (uint8_t iTaskOffset = 1; iTaskOffset <= m_taskCount; iTaskOffset++) {
		uint8_t iTask = (iLastTask + iTaskOffset) % m_taskCount;
		task_control_block_t* t = m_tasks[iTask];

		if (t) {
			switch(t->getState()) {
			case task_state_t::TASK_UNINITIALISED:
				break;	// ???
			case task_state_t::TASK_READY:
				if ((nextTask == 0) || (t->priority > nextPriority)) {
					nextTask = t;
					nextTaskIndex = iTask;
					nextPriority = t->priority;
				}
				break;
			case task_state_t::TASK_WAITING:
				break;
			case task_state_t::TASK_RUNNING:
				//assert(false);
				break;
			case task_state_t::TASK_CLOSING:
				break;
			case task_state_t::TASK_CLOSED:
				break;
			}
		}
	}
	return nextTaskIndex;
}

void scheduler::updateCounts() {
	m_livingTaskCount = 0;
	for (uint8_t i = 0; i < m_taskCount; i++) {
		auto t = m_tasks[i];
		m_livingTaskCount += (t && (t->getState() != task_state_t::TASK_CLOSED)) ? 1 : 0;
	}
}

bool scheduler::hasAnyTasks() {
	return m_livingTaskCount > 0;
}

/* static */ scheduler& scheduler::instance() {
	static scheduler theInstance;
	return theInstance;
}


}
