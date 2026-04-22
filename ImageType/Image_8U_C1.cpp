#include "Image_8U_C1.h"
#include "../ShapeType/Rect2i.h"

using Core::ImageType::ImageBase;
using Core::ImageType::Image_8U_C1;
using Core::ShapeType::Rect2i;

Image_8U_C1::Image_8U_C1()
{
	m_pixelType = PixelType::U8;
	m_channel = 1;
	m_bitsPerChannel = 8;
}

Image_8U_C1::Image_8U_C1(int32_t width, int32_t height)
	: Image_8U_C1()
{
	Allocate(width, height, 1, 8);
}

Image_8U_C1::Image_8U_C1(int32_t width, int32_t height, uint8_t* buffer, int32_t stride)
	: Image_8U_C1()
{
	Attach(buffer, width, height, stride, 1, 8);
}

Image_8U_C1::Image_8U_C1(const ImageBase& src, const Rect2i& roi)
	: ImageBase(src, roi)
{
	m_pixelType = PixelType::U8;
	m_channel = 1;
	m_bitsPerChannel = 8;
}

