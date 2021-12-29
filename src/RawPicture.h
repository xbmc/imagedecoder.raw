/*
 *  Copyright (C) 2005-2021 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include <kodi/addon-instance/ImageDecoder.h>
#include <libraw.h>

class ATTR_DLL_LOCAL RawPicture : public kodi::addon::CInstanceImageDecoder
{
public:
  RawPicture(const kodi::addon::IInstanceInfo& instance);
  ~RawPicture() override;

  bool SupportsFile(const std::string& file) override;
  bool ReadTag(const std::string& file, kodi::addon::ImageDecoderInfoTag& tag) override;
  bool LoadImageFromMemory(const std::string& mimetype,
                           const uint8_t* buffer,
                           size_t bufSize,
                           unsigned int& width,
                           unsigned int& height) override;
  bool Decode(uint8_t* pixels,
              unsigned int width,
              unsigned int height,
              unsigned int pitch,
              ADDON_IMG_FMT format) override;

private:
  libraw_data_t* m_raw_data;
  unsigned int m_width{0};
  unsigned int m_height{0};
};
