/*
 * Copyright (C) 2001-2013 Jacek Sieka, arnetheduck on gmail point com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef DCPLUSPLUS_DCPP_W_H_
#define DCPLUSPLUS_DCPP_W_H_

#include "compiler.h"
#include <boost/atomic.hpp>

#include "w_flylinkdc.h"

#ifndef STRICT
#define STRICT 1
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX 1
#endif

#include <windows.h>
#include <mmsystem.h>
#include <tchar.h>

// http://msdn.microsoft.com/en-us/library/windows/desktop/ms644930(v=vs.85).aspx
// WM_USER through 0x7FFF
// Integer messages for use by private window classes.
// WM_APP through 0xBFFF
// Messages available for use by applications.

#define WM_SPEAKER (WM_APP + 500)
#define WM_SPEAKER_UPDATE_USER (WM_USER + 1)
#define WM_SPEAKER_CONNECTED (WM_USER + 2)
#define WM_SPEAKER_DISCONNECTED (WM_USER + 3)
#define WM_SPEAKER_ADD_CHAT_LINE  (WM_USER + 4)
#define WM_SPEAKER_CHEATING_USER  (WM_USER + 5)
#define WM_SPEAKER_USER_REPORT  (WM_USER + 6)

#ifdef FLYLINKDC_UPDATE_USER_JOIN_USE_WIN_MESSAGES_Q
#define WM_SPEAKER_UPDATE_USER_JOIN (WM_USER + x)
#endif

#ifdef FLYLINKDC_REMOVE_USER_WIN_MESSAGES_Q
#define WM_SPEAKER_REMOVE_USER (WM_USER + x)
#endif

#define WM_SPEAKER_BEGIN   WM_SPEAKER_UPDATE_USER
#define WM_SPEAKER_END     WM_SPEAKER_USER_REPORT

class CFlyTickDelta
{
	private:
		DWORD& m_tc;
	public:
		CFlyTickDelta(DWORD& p_tc): m_tc(p_tc)
		{
			m_tc = GetTickCount();
		}
		~CFlyTickDelta()
		{
			m_tc = GetTickCount() - m_tc;
		}
};

template <class T> class CFlySafeGuard
{
		T& m_counter;
	public:
		CFlySafeGuard(T& p_counter) : m_counter(p_counter)
		{
			++m_counter;
		}
		~CFlySafeGuard()
		{
			--m_counter;
		}
};

#endif /* W_H_ */
