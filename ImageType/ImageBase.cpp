#include "ImageBase.h"

using Core::ImageType::ImageBase;
using Core::ShapeType::Rect2i;

ImageBase::ImageBase(const ImageBase& rhs)
{
	m_data = rhs.m_data;
	m_width = rhs.m_width;
	m_height = rhs.m_height;
	m_stride = rhs.m_stride;
	m_channel = rhs.m_channel;
	m_pixelType = rhs.m_pixelType;
	m_bitsPerChannel = rhs.m_bitsPerChannel;
	m_ownBuffer = rhs.m_ownBuffer;
	m_referenceCount = rhs.m_referenceCount;

	AddRef();
}

ImageBase& ImageBase::operator=(const ImageBase& rhs)
{
	if (this == &rhs)
		return *this;

	ReleaseBuffer();

	m_data = rhs.m_data;
	m_width = rhs.m_width;
	m_height = rhs.m_height;
	m_stride = rhs.m_stride;
	m_channel = rhs.m_channel;
	m_pixelType = rhs.m_pixelType;
	m_bitsPerChannel = rhs.m_bitsPerChannel;
	m_ownBuffer = rhs.m_ownBuffer;
	m_referenceCount = rhs.m_referenceCount;

	AddRef();

	return *this;
}

ImageBase::ImageBase(ImageBase&& rhs) noexcept
{
	m_data = rhs.m_data;
	m_width = rhs.m_width;
	m_height = rhs.m_height;
	m_stride = rhs.m_stride;
	m_channel = rhs.m_channel;
	m_pixelType = rhs.m_pixelType;
	m_bitsPerChannel = rhs.m_bitsPerChannel;
	m_ownBuffer = rhs.m_ownBuffer;
	m_referenceCount = rhs.m_referenceCount;

	rhs.m_data = nullptr;
	rhs.m_referenceCount = nullptr;
	rhs.m_ownBuffer = false;
	rhs.m_width = rhs.m_height = rhs.m_stride = 0;
	rhs.m_channel = 0;
	rhs.m_bitsPerChannel = 0;
}

ImageBase& ImageBase::operator=(ImageBase&& rhs) noexcept
{
	if (this == &rhs)
		return *this;

	ReleaseBuffer();

	m_data = rhs.m_data;
	m_width = rhs.m_width;
	m_height = rhs.m_height;
	m_stride = rhs.m_stride;
	m_channel = rhs.m_channel;
	m_pixelType = rhs.m_pixelType;
	m_bitsPerChannel = rhs.m_bitsPerChannel;
	m_ownBuffer = rhs.m_ownBuffer;
	m_referenceCount = rhs.m_referenceCount;

	rhs.m_data = nullptr;
	rhs.m_referenceCount = nullptr;
	rhs.m_ownBuffer = false;
	rhs.m_width = rhs.m_height = rhs.m_stride = 0;
	rhs.m_channel = 0;
	rhs.m_bitsPerChannel = 0;

	return *this;
}

ImageBase::ImageBase(const ImageBase& src, const Rect2i& roi)
{
	if (roi.Empty())
		return;

	if (roi.left < 0 || roi.top < 0 ||
		roi.left + roi.Width()  > src.m_width ||
		roi.top + roi.Height() > src.m_height)
		return;

	const int32_t bytesPerPixel =
		(src.m_bitsPerChannel / 8) * src.m_channel;

	m_width = roi.Width();
	m_height = roi.Height();
	m_stride = src.m_stride;

	m_channel = src.m_channel;
	m_pixelType = src.m_pixelType;
	m_bitsPerChannel = src.m_bitsPerChannel;

	m_data = src.m_data +
		roi.top * src.m_stride +
		roi.left * bytesPerPixel;

	m_referenceCount = src.m_referenceCount;
	m_ownBuffer = false;

	AddRef();
}

ImageBase ImageBase::ROI(const Rect2i& roi) const
{
	return ImageBase(*this, roi);
}

void ImageBase::ReleaseBuffer()
{
	if (m_referenceCount)
	{
		Release();
	}

	m_data = nullptr;
	m_referenceCount = nullptr;
	m_ownBuffer = false;
	m_width = m_height = m_stride = 0;
}

bool ImageBase::Allocate(int32_t width, int32_t height, int32_t channel, int32_t bitsPerChannel, int32_t stride)
{
	ReleaseBuffer();

	m_width = width;
	m_height = height;
	m_bitsPerChannel = bitsPerChannel;

	const int32_t bytesPerPixel = (m_bitsPerChannel / 8) * channel;

	if (stride == 0)
		m_stride = AlignStride(width * bytesPerPixel);
	else
		m_stride = stride;

	m_channel = channel;

	if (bitsPerChannel == 8)
		m_pixelType = PixelType::U8;
	else if (bitsPerChannel == 16)
		m_pixelType = PixelType::U16;
	else if (bitsPerChannel == 32)
		m_pixelType = PixelType::F32;

	const size_t totalSize = static_cast<size_t>(m_stride) * height;

	m_data = static_cast<uint8_t*>(AlignedAlloc(totalSize));

	if (!m_data)
	{
		return false;
	}

	m_referenceCount = new int32_t(1);
	if (!m_referenceCount)
	{
		AlignedFree(m_data);
		m_data = nullptr;

		return false;
	}

	m_ownBuffer = true;

	memset(m_data, 0, totalSize);

	return true;
}

void ImageBase::Attach(void* externalBuffer, int32_t width, int32_t height, int32_t stride, int32_t channel, int32_t bitsPerChannel)
{
	ReleaseBuffer();

	m_data = static_cast<uint8_t*>(externalBuffer);

	m_width = width;
	m_height = height;
	m_channel = channel;
	m_bitsPerChannel = bitsPerChannel;
	m_stride = stride;

	if (bitsPerChannel == 8)
		m_pixelType = PixelType::U8;
	else if (bitsPerChannel == 16)
		m_pixelType = PixelType::U16;
	else if (bitsPerChannel == 32)
		m_pixelType = PixelType::F32;

	m_ownBuffer = false;
	m_referenceCount = new int32_t(1);
}

void ImageBase::AddRef()
{
	if (m_referenceCount)
		++(*m_referenceCount);
}

void ImageBase::Release()
{
	if (!m_referenceCount)
		return;

	if (--(*m_referenceCount) == 0)
	{
		if (m_ownBuffer && m_data)
		{
			AlignedFree(m_data);
		}

		delete m_referenceCount;
		m_referenceCount = nullptr;
	}
}


