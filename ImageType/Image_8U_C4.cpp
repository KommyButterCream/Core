#include "Image_8U_C4.h"
#include "../ShapeType/Rect2i.h"

using Core::ImageType::ImageBase;
using Core::ImageType::Image_8U_C4;
using Core::ShapeType::Rect2i;

Image_8U_C4::Image_8U_C4()
{
	m_pixelType = PixelType::U8;
	m_channel = 4;
	m_bitsPerChannel = 8;
}

Image_8U_C4::Image_8U_C4(int32_t width, int32_t height)
	: Image_8U_C4()
{
	Allocate(width, height, 4, 8);
}

Image_8U_C4::Image_8U_C4(int32_t width, int32_t height, uint8_t* buffer, int32_t stride)
	: Image_8U_C4()
{
	Attach(buffer, width, height, stride, 4, 8);
}

Image_8U_C4::Image_8U_C4(const ImageBase& src, const Rect2i& roi)
	: ImageBase(src, roi)
{
	m_pixelType = PixelType::U8;
	m_channel = 4;
	m_bitsPerChannel = 8;
}
