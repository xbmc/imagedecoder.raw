/*
 *  Copyright (C) 2005-2020 Team Kodi
 *  https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include <kodi/addon-instance/ImageDecoder.h>
#include <libraw.h>

class ATTRIBUTE_HIDDEN RawPicture : public kodi::addon::CInstanceImageDecoder
{
public:
  RawPicture(KODI_HANDLE instance, const std::string& version)
    : CInstanceImageDecoder(instance, version), m_raw_data(libraw_init(0))
  {
  }

  ~RawPicture() override { libraw_close(m_raw_data); }

  bool LoadImageFromMemory(unsigned char* buffer,
                           unsigned int bufSize,
                           unsigned int& width,
                           unsigned int& height) override
  {
    if (m_raw_data == nullptr)
      return false;

    if (libraw_open_buffer(m_raw_data, buffer, bufSize) != LIBRAW_SUCCESS)
    {
      kodi::Log(ADDON_LOG_ERROR,
                "Texture manager unable to load image from memory (libraw_open_buffer)");
      return false;
    }
    int err = 0;
    if ((err = libraw_unpack(m_raw_data)) != LIBRAW_SUCCESS)
    {
      kodi::Log(ADDON_LOG_ERROR,
                "Texture manager unable to load image from memory (libraw_unpack)");
      return false;
    }

    if ((err = libraw_dcraw_process(m_raw_data)) != LIBRAW_SUCCESS)
    {
      kodi::Log(ADDON_LOG_ERROR,
                "Texture manager unable to load image from memory (libraw_dcraw_process)");
      return false;
    }

    m_width = m_raw_data->sizes.width;
    m_height = m_raw_data->sizes.height;

    width = m_width;
    height = m_height;
    return true;
  }

  bool Decode(unsigned char* pixels,
              unsigned int width,
              unsigned int height,
              unsigned int pitch,
              ImageFormat format) override
  {
    if (!m_raw_data || m_raw_data->sizes.width == 0 || m_raw_data->sizes.height == 0)
      return false;

    int err = 0;
    m_raw_data->sizes.flip = 3;
    libraw_processed_image_t* image = libraw_dcraw_make_mem_image(m_raw_data, &err);

    unsigned int dstPitch = pitch;
    unsigned int srcPitch = 3 * m_width;

    unsigned char* dst = (unsigned char*)pixels;
    unsigned char* src = (unsigned char*)image->data + srcPitch * m_height;

    for (unsigned int y = 0; y < m_height; y++)
    {
      unsigned char* dst2 = dst;
      unsigned char* src2 = src;
      for (unsigned int x = 0; x < m_width;
           x++, dst2 += (format == ADDON_IMG_FMT_RGB8 ? 3 : 4), src2 -= 3)
      {
        dst2[0] = src2[2];
        dst2[1] = src2[1];
        dst2[2] = src2[0];
        if (format == ADDON_IMG_FMT_A8R8G8B8)
          dst2[3] = 0xff;
      }
      src -= srcPitch;
      dst += dstPitch;
    }

    libraw_dcraw_clear_mem(image);
    return true;
  }

private:
  libraw_data_t* m_raw_data;
  unsigned int m_width;
  unsigned int m_height;
};

class ATTRIBUTE_HIDDEN CMyAddon : public kodi::addon::CAddonBase
{
public:
  CMyAddon() = default;
  ADDON_STATUS CreateInstance(int instanceType,
                              const std::string& instanceID,
                              KODI_HANDLE instance,
                              const std::string& version,
                              KODI_HANDLE& addonInstance) override
  {
    addonInstance = new RawPicture(instance, version);
    return ADDON_STATUS_OK;
  }
};

ADDONCREATOR(CMyAddon)
