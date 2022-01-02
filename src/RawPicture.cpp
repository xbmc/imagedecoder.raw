
/*
 *  Copyright (C) 2005-2021 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "RawPicture.h"

#include <kodi/Filesystem.h>

RawPicture::RawPicture(const kodi::addon::IInstanceInfo& instance)
  : CInstanceImageDecoder(instance), m_raw_data(libraw_init(0))
{
}

RawPicture::~RawPicture()
{
  libraw_close(m_raw_data);
}

bool RawPicture::SupportsFile(const std::string& file)
{
  if (m_raw_data == nullptr)
    return false;

  kodi::vfs::CFile fileData;
  if (!fileData.OpenFile(file))
    return false;

  size_t length = fileData.GetLength();

  std::vector<uint8_t> buffer(length);
  fileData.Read(buffer.data(), length);

  if (libraw_open_buffer(m_raw_data, buffer.data(), buffer.size()) != LIBRAW_SUCCESS)
  {
    kodi::Log(ADDON_LOG_ERROR,
              "Texture manager unable to load image from memory (libraw_open_buffer)");
    return false;
  }

  return true;
}

bool RawPicture::ReadTag(const std::string& file, kodi::addon::ImageDecoderInfoTag& tag)
{
  if (m_raw_data == nullptr)
    return false;

  kodi::vfs::CFile fileData;
  if (!fileData.OpenFile(file))
    return false;

  size_t length = fileData.GetLength();

  std::vector<uint8_t> buffer(length);
  fileData.Read(buffer.data(), length);

  if (libraw_open_buffer(m_raw_data, buffer.data(), buffer.size()) != LIBRAW_SUCCESS)
  {
    kodi::Log(ADDON_LOG_ERROR,
              "Texture manager unable to load image from memory (libraw_open_buffer)");
    return false;
  }

  if (libraw_unpack(m_raw_data) != LIBRAW_SUCCESS)
  {
    kodi::Log(ADDON_LOG_ERROR, "Texture manager unable to load image from memory (libraw_unpack)");
    return false;
  }

  tag.SetWidth(m_raw_data->sizes.width);
  tag.SetHeight(m_raw_data->sizes.height);
  tag.SetFlashUsed(m_raw_data->color.flash_used > 0.0f ? ADDON_IMG_FLASH_TYPE_FIRED
                                                       : ADDON_IMG_FLASH_TYPE_NO_FLASH);
  tag.SetMeteringMode(static_cast<ADDON_IMG_METERING_MODE>(m_raw_data->shootinginfo.MeteringMode));
  tag.SetExposureProgram(
      static_cast<ADDON_IMG_EXPOSURE_PROGRAM>(m_raw_data->shootinginfo.ExposureProgram));
  tag.SetExposureMode(static_cast<ADDON_IMG_EXPOSURE_MODE>(m_raw_data->shootinginfo.ExposureMode));
  switch (m_raw_data->sizes.flip)
  {
    case 3:
      tag.SetOrientation(ADDON_IMG_ORIENTATION_ROTATE_180_CCW);
      break;
    case 5:
      tag.SetOrientation(ADDON_IMG_ORIENTATION_ROTATE_90_CCW);
      break;
    case 6:
      tag.SetOrientation(ADDON_IMG_ORIENTATION_ROTATE_270_CCW);
      break;
    default:
      tag.SetOrientation(ADDON_IMG_ORIENTATION_NONE);
      break;
  }

  std::string str;
  libraw_iparams_t* param = libraw_get_iparams(m_raw_data);

  str = param->make;
  kodi::tools::StringUtils::Trim(str);
  tag.SetCameraManufacturer(str);

  str = param->model;
  kodi::tools::StringUtils::Trim(str);
  tag.SetCameraModel(str);

  libraw_lensinfo_t* lensinfo = libraw_get_lensinfo(m_raw_data);

  tag.SetFocalLengthIn35mmFormat(lensinfo->FocalLengthIn35mmFormat);

  libraw_imgother_t* imgother = libraw_get_imgother(m_raw_data);

  str = imgother->artist;
  kodi::tools::StringUtils::Trim(str);
  tag.SetAuthor(str);

  str = imgother->desc;
  kodi::tools::StringUtils::Trim(str);
  tag.SetDescription(str);

  tag.SetTimeCreated(imgother->timestamp);
  tag.SetISOSpeed(imgother->iso_speed);
  tag.SetFocalLength(imgother->focal_len);
  tag.SetApertureFNumber(imgother->aperture);

  if (imgother->parsed_gps.gpsparsed && imgother->parsed_gps.latref != 0 &&
      imgother->parsed_gps.longref != 0)
  {

    tag.SetGPSInfo(true, imgother->parsed_gps.latref, imgother->parsed_gps.latitude,
                   imgother->parsed_gps.longref, imgother->parsed_gps.longitude,
                   imgother->parsed_gps.altref, imgother->parsed_gps.altitude);
  }

  return true;
}

bool RawPicture::LoadImageFromMemory(const std::string& mimetype,
                                     const uint8_t* buffer,
                                     size_t bufSize,
                                     unsigned int& width,
                                     unsigned int& height)
{
  if (m_raw_data == nullptr)
    return false;

  if (libraw_open_buffer(m_raw_data, const_cast<uint8_t*>(buffer), bufSize) != LIBRAW_SUCCESS)
  {
    kodi::Log(ADDON_LOG_ERROR,
              "Texture manager unable to load image from memory (libraw_open_buffer)");
    return false;
  }
  int err = 0;
  if ((err = libraw_unpack(m_raw_data)) != LIBRAW_SUCCESS)
  {
    kodi::Log(ADDON_LOG_ERROR, "Texture manager unable to load image from memory (libraw_unpack)");
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

bool RawPicture::Decode(uint8_t* pixels,
                        unsigned int width,
                        unsigned int height,
                        unsigned int pitch,
                        ADDON_IMG_FMT format)
{
  if (!m_raw_data || m_raw_data->sizes.width == 0 || m_raw_data->sizes.height == 0)
    return false;

  int err = 0;
  m_raw_data->sizes.flip = 3;
  libraw_processed_image_t* image = libraw_dcraw_make_mem_image(m_raw_data, &err);

  unsigned int dstPitch = pitch;
  unsigned int srcPitch = 3 * m_width;

  uint8_t* dst = pixels;
  uint8_t* src = image->data + srcPitch * m_height;

  for (unsigned int y = 0; y < m_height; y++)
  {
    uint8_t* dst2 = dst;
    uint8_t* src2 = src;
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

class ATTR_DLL_LOCAL CMyAddon : public kodi::addon::CAddonBase
{
public:
  CMyAddon() = default;
  ADDON_STATUS CreateInstance(const kodi::addon::IInstanceInfo& instance,
                              KODI_ADDON_INSTANCE_HDL& hdl) override
  {
    hdl = new RawPicture(instance);
    return ADDON_STATUS_OK;
  }
};

ADDONCREATOR(CMyAddon)
