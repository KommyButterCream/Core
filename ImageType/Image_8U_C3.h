#pragma once

#include "ImageBase.h"

namespace Core
{
	namespace ImageType
	{
		class Image_8U_C3 : public ImageBase
		{
		public:
			Image_8U_C3();
			Image_8U_C3(int32_t width, int32_t height);
			Image_8U_C3(int32_t width, int32_t height, uint8_t* buffer, int32_t stride);
			Image_8U_C3(const ImageBase& src, const Core::ShapeType::Rect2i& roi);

		public:
			inline Pixel8UC3& At(int32_t x, int32_t y)
			{
				return reinterpret_cast<Pixel8UC3*>(Ptr(y))[x];
			}

			inline const Pixel8UC3& At(int32_t x, int32_t y) const
			{
				return reinterpret_cast<const Pixel8UC3*>(Ptr(y))[x];
			}

			inline Pixel8UC3* Row(int32_t y)
			{
				return reinterpret_cast<Pixel8UC3*>(Ptr(y));
			}

			inline const Pixel8UC3* Row(int32_t y) const
			{
				return reinterpret_cast<const Pixel8UC3*>(Ptr(y));
			}

			Image_8U_C3 ROI(const Core::ShapeType::Rect2i& roi) const
			{
				return Image_8U_C3(*this, roi);
			}
		};
	}
}
