#include "Image_8U_C3.h"
#include "../ShapeType/Rect2i.h"

using Core::ImageType::ImageBase;
using Core::ImageType::Image_8U_C3;
using Core::ShapeType::Rect2i;

Image_8U_C3::Image_8U_C3()
{
	m_pixelType = PixelType::U8;
	m_channel = 3;
	m_bitsPerChannel = 8;
}

Image_8U_C3::Image_8U_C3(int32_t width, int32_t height)
	: Image_8U_C3()
{
	Allocate(width, height, 3, 8);
}

Image_8U_C3::Image_8U_C3(int32_t width, int32_t height, uint8_t* buffer, int32_t stride)
	: Image_8U_C3()
{
	Attach(buffer, width, height, stride, 3, 8);
}

Image_8U_C3::Image_8U_C3(const ImageBase& src, const Rect2i& roi)
	: ImageBase(src, roi)
{
	m_pixelType = PixelType::U8;
	m_channel = 3;
	m_bitsPerChannel = 8;
}
