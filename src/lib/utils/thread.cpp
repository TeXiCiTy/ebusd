/*
 * Copyright (C) Roland Jax 2012-2014 <ebusd@liwest.at>
 *
 * This file is part of ebusd.
 *
 * ebusd is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ebusd is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ebusd. If not, see http://www.gnu.org/licenses/.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "thread.h"

void* Thread::runThread(void* arg)
{
	((Thread*)arg)->enter();
	return NULL;
}

Thread::~Thread()
{
	if (m_started == true && m_detached == false)
		pthread_detach(m_threadid);

	if (m_started == true)
		pthread_cancel(m_threadid);
}

bool Thread::start(const char* name)
{

	int result = pthread_create(&m_threadid, NULL, runThread, this);

	if (result == 0) {

#ifdef HAVE_PTHREAD_SETNAME_NP
		pthread_setname_np(m_threadid, name);
#endif

		m_started = true;

		return true;
	}

	return false;
}

bool Thread::join()
{
	int result = -1;

	if (m_started == true) {
		m_stopped = true;
		result = pthread_join(m_threadid, NULL);

		if (result == 0) {
			m_detached = false;
			m_started = false;
		}
	}

	return result == 0;
}

bool Thread::detach()
{
	int result = -1;

	if (m_started == true && m_detached == false) {
		result = pthread_detach(m_threadid);

		if (result == 0)
			m_detached = true;

	}

	return result == 0;
}

void Thread::enter() {
	m_running = true;
	run();
	m_running = false;
}