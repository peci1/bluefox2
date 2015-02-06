#include "bluefox2/mvimpact_helper.h"

#include <iostream>
#include <sensor_msgs/image_encodings.h>

namespace bluefox2 {

using namespace mvIMPACT::acquire;

void PrintDeviceDetails(const Device* device) noexcept {
  std::cout << "Serial: " << device->serial.read()
            << ", Version: " << device->deviceVersion.read()
            << ", Family: " << device->family.read()
            << ", Firmware: " << device->firmwareVersion.read()
            << ", Product: " << device->product.read() << std::endl;
}

std::string PixelFormatToEncoding(
    const TImageBufferPixelFormat& format) noexcept {
  using namespace sensor_msgs::image_encodings;
  switch (format) {
    case ibpfMono8:
      return MONO8;
    case ibpfMono16:
      return MONO16;
    case ibpfRGB888Packed:
      return BGR8;
    case ibpfRGB161616Packed:
      return BGR16;
    case ibpfRGBx888Packed:
      return BGRA8;
    case ibpfAuto:
      return "auto";
    default:
      return MONO8;
  }
}

std::string BayerPatternToEncoding(const TBayerMosaicParity& pattern,
                                   unsigned int bits) noexcept {
  using namespace sensor_msgs::image_encodings;
  if (bits == 8) {
    switch (pattern) {
      case bmpRG:
        return BAYER_RGGB8;
      case bmpGB:
        return BAYER_GBRG8;
      case bmpGR:
        return BAYER_GRBG8;
      case bmpBG:
        return BAYER_BGGR8;
      default:
        return MONO8;
    }
  } else if (bits == 16) {
    switch (pattern) {
      case bmpRG:
        return BAYER_RGGB16;
      case bmpGB:
        return BAYER_GBRG16;
      case bmpGR:
        return BAYER_GRBG16;
      case bmpBG:
        return BAYER_BGGR16;
      default:
        return MONO16;
    }
  }

  return "";
}

double PixelClockToFrameRate(int pclk_khz, double width, double height,
                             double expose_us) noexcept {
  static const double kTriggerPulseWidthUs = 200;
  double frame_time_us = (width + 94) * (height + 45) / pclk_khz * 1e3;
  return 1e6 / (frame_time_us + expose_us + kTriggerPulseWidthUs);
}

}  // namespace bluefox2
