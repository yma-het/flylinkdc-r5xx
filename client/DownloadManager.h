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

#ifndef DCPLUSPLUS_DCPP_DOWNLOAD_MANAGER_H
#define DCPLUSPLUS_DCPP_DOWNLOAD_MANAGER_H

#include "forward.h"

#include "DownloadManagerListener.h"
#include "UserConnectionListener.h"
#include "QueueItem.h"
#include "TimerManager.h"
#include "Singleton.h"
#include "UserConnection.h"
#include "ZUtils.h"
#include "FilteredFile.h"

/**
 * Singleton. Use its listener interface to update the download list
 * in the user interface.
 */
class DownloadManager : public Speaker<DownloadManagerListener>,
	private UserConnectionListener, private TimerManagerListener,
	public Singleton<DownloadManager>
{
	public:
	
		/** @internal */
		void addConnection(UserConnection* p_conn);
		void checkIdle(const UserPtr& user);
		
		/** @internal */
		void abortDownload(const string& aTarget);
		
		/** @return Running average download speed in Bytes/s */
		int64_t getRunningAverage() const
		{
			return runningAverage;//[+] IRainman refactoring transfer mechanism
		}
		
		/** @return Number of downloads. */
		size_t getDownloadCount() const
		{
			webrtc::ReadLockScoped l(*g_csDownload);
			return m_download_map.size();
		}
		
		bool startDownload(QueueItem::Priority prio);
		bool checkFileDownload(const UserPtr& aUser); //[+]
		
	private:
	
		static std::unique_ptr<webrtc::RWLockWrapper> g_csDownload;
		DownloadMap m_download_map;
		std::unordered_map<UserPtr, UserConnection*, User::Hash> m_idlers;
		void remove_idlers(UserConnection* aSource)
		{
			webrtc::WriteLockScoped l(*g_csDownload);
			dcassert(aSource->getUser());
			// ����� ���� �� �������.
			// ������ �������� dcassert(m_idlers.find(aSource->getUser()) != m_idlers.end());
			m_idlers.erase(aSource->getUser());
		}
		int64_t runningAverage;//[+] IRainman refactoring transfer mechanism
		
		void removeConnection(UserConnection* p_conn);
		void removeDownload(Download* aDown);
		void fileNotAvailable(UserConnection* aSource);
		void noSlots(UserConnection* aSource, const string& param = Util::emptyString);
		
		int64_t getResumePos(const string& file, const TigerTree& tt, int64_t startPos);
		
		void failDownload(UserConnection* aSource, const string& reason);
		
		friend class Singleton<DownloadManager>;
		
		DownloadManager();
		~DownloadManager();
		
		void checkDownloads(UserConnection* aConn);
		void startData(UserConnection* aSource, int64_t start, int64_t newSize, bool z);
		void endData(UserConnection* aSource);
		
		void onFailed(UserConnection* aSource, const string& aError);
		
		// UserConnectionListener
		void on(Data, UserConnection*, const uint8_t*, size_t) noexcept;
		void on(Failed, UserConnection* aSource, const string& aError) noexcept
		{
			onFailed(aSource, aError);
		}
		void on(ProtocolError, UserConnection* aSource, const string& aError) noexcept
		{
			onFailed(aSource, aError);
		}
		void on(MaxedOut, UserConnection*, const string& param) noexcept;
		void on(FileNotAvailable, UserConnection*) noexcept;
		void on(ListLength, UserConnection* aSource, const string& aListLength);
		void on(Updated, UserConnection*) noexcept;
		
		void on(AdcCommand::SND, UserConnection*, const AdcCommand&) noexcept;
		void on(AdcCommand::STA, UserConnection*, const AdcCommand&) noexcept;
		
		// TimerManagerListener
		void on(TimerManagerListener::Second, uint64_t aTick) noexcept;
		/*#ifdef IRAINMAN_ENABLE_AUTO_BAN
		        void on(BanMessage, UserConnection*, const string& aMessage) noexcept; // !SMT!-B
		#endif*/
		void on(CheckUserIP, UserConnection*); // [+] SSA
};

#endif // !defined(DOWNLOAD_MANAGER_H)

/**
 * @file
 * $Id: DownloadManager.h 568 2011-07-24 18:28:43Z bigmuscle $
 */
