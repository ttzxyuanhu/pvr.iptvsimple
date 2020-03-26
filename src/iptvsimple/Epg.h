/*
 *  Copyright (C) 2005-2020 Team Kodi
 *  https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "Channels.h"
#include "Settings.h"
#include "data/ChannelEpg.h"
#include "data/EpgGenre.h"

#include <string>
#include <vector>

#include <kodi/libXBMC_pvr.h>

namespace iptvsimple
{
  static const int SECONDS_IN_DAY = 86400;
  static const std::string GENRES_MAP_FILENAME = "genres.xml";
  static const std::string GENRE_DIR = "/genres";
  static const std::string GENRE_ADDON_DATA_BASE_DIR = ADDON_DATA_BASE_DIR + GENRE_DIR;

  enum class XmltvFileFormat
  {
    NORMAL,
    TAR_ARCHIVE,
    INVALID
  };

  class Epg
  {
  public:
    Epg(iptvsimple::Channels& channels);

    PVR_ERROR GetEPGForChannel(ADDON_HANDLE handle, int iChannelUid, time_t start, time_t end);
    void Clear();
    void ReloadEPG();

    data::EpgEntry* GetLiveEPGEntry(const data::Channel& myChannel) const;
    data::EpgEntry* GetEPGEntry(const data::Channel& myChannel, time_t lookupTime) const;

  private:
    static const XmltvFileFormat GetXMLTVFileFormat(const char* buffer);
    static void MoveOldGenresXMLFileToNewLocation();

    bool LoadEPG(time_t iStart, time_t iEnd);
    bool GetXMLTVFileWithRetries(std::string& data);
    char* FillBufferFromXMLTVData(std::string& data, std::string& decompressedData);
    bool LoadChannelEpgs(const pugi::xml_node& rootElement);
    void LoadEpgEntries(const pugi::xml_node& rootElement, int start, int end);
    bool LoadGenres();

    data::ChannelEpg* FindEpgForChannel(const std::string& id) const;
    data::ChannelEpg* FindEpgForChannel(const data::Channel& channel) const;
    void ApplyChannelsLogosFromEPG();

    std::string m_xmltvLocation;
    int m_epgTimeShift;
    bool m_tsOverride;
    int m_lastStart;
    int m_lastEnd;

    iptvsimple::Channels& m_channels;
    std::vector<data::ChannelEpg> m_channelEpgs;
    std::vector<iptvsimple::data::EpgGenre> m_genreMappings;
  };
} //namespace iptvsimple