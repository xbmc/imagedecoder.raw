/*
 *      Copyright (C) 2005-2013 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include <kodi/addon-instance/ImageDecoder.h>
#include <libraw.h>

class RawPicture : public kodi::addon::CInstanceImageDecoder
{
public:
  RawPicture(KODI_HANDLE instance)
    : CInstanceImageDecoder(instance),
      m_raw_data(libraw_init(0))
  {
  }

  virtual ~RawPicture()
  {
    libraw_close(m_raw_data);
  }

  virtual bool LoadImageFromMemory(unsigned char* buffer, unsigned int bufSize, unsigned int& width, unsigned int& height) override
  {
    if (m_raw_data == nullptr)
      return false;

    if (libraw_open_buffer(m_raw_data, buffer, bufSize) != LIBRAW_SUCCESS)
    {
      kodi::Log(ADDON_LOG_ERROR, "Texture manager unable to load image from memory (libraw_open_buffer)");
      return false;
    }
    int err = 0;
    if ( (err = libraw_unpack(m_raw_data)) != LIBRAW_SUCCESS)
    {
      kodi::Log(ADDON_LOG_ERROR, "Texture manager unable to load image from memory (libraw_unpack)");
      return false;
    }

    if ( (err = libraw_dcraw_process(m_raw_data)) != LIBRAW_SUCCESS)
    {
      kodi::Log(ADDON_LOG_ERROR, "Texture manager unable to load image from memory (libraw_dcraw_process)");
      return false;
    }

    m_width = m_raw_data->sizes.width;
    m_height = m_raw_data->sizes.height;

    width = m_width;
    height = m_height;
    return true;
  }

  virtual bool Decode(unsigned char *pixels,
                      unsigned int width, unsigned int height,
                      unsigned int pitch, ImageFormat format) override
  {
    if (!m_raw_data || m_raw_data->sizes.width == 0 || m_raw_data->sizes.height == 0)
      return false;

    int err = 0;
    m_raw_data->sizes.flip = 3;
    libraw_processed_image_t * image = libraw_dcraw_make_mem_image(m_raw_data, &err);

    unsigned int dstPitch = pitch;
    unsigned int srcPitch = 3*m_width;

    unsigned char *dst = (unsigned char*)pixels;
    unsigned char *src = (unsigned char*)image->data + srcPitch*m_height;

    for (unsigned int y = 0; y < m_height; y++)
    {
      unsigned char *dst2 = dst;
      unsigned char *src2 = src;
      for (unsigned int x = 0; x < m_width; x++, dst2 += (format==ADDON_IMG_FMT_RGB8?3:4), src2 -= 3)
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

class CMyAddon : public kodi::addon::CAddonBase
{
public:
  CMyAddon() { }
  virtual ADDON_STATUS CreateInstance(int instanceType, std::string instanceID, KODI_HANDLE instance, KODI_HANDLE& addonInstance) override
  {
    addonInstance = new RawPicture(instance);
    return ADDON_STATUS_OK;
  }
};

ADDONCREATOR(CMyAddon)
