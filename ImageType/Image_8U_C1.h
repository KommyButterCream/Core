#pragma once

#include "ImageBase.h"

namespace Core
{
	namespace ImageType
	{
		class Image_8U_C1 : public ImageBase
		{
		public:
			Image_8U_C1();
			Image_8U_C1(int32_t width, int32_t height);
			Image_8U_C1(int32_t width, int32_t height, uint8_t* buffer, int32_t stride);
			Image_8U_C1(const ImageBase& src, const Core::ShapeType::Rect2i& roi);

		public:
			inline uint8_t& At(int32_t x, int32_t y)
			{
				return Ptr(y)[x];
			}

			inline const uint8_t& At(int32_t x, int32_t y) const
			{
				return Ptr(y)[x];
			}

			inline uint8_t* Row(int32_t y)
			{
				return Ptr(y);
			}

			inline const uint8_t* Row(int32_t y) const
			{
				return Ptr(y);
			}

			Image_8U_C1 ROI(const Core::ShapeType::Rect2i& roi) const
			{
				return Image_8U_C1(*this, roi);
			}
		};
	}
}

