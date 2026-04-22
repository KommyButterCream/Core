#pragma once

/*
 * @brief DirectX 객체 안전 해제 유틸리티
 * @param instance: 해제할 COM 객체 포인터의 참조
 */

namespace Core
{
	namespace DirectX
	{
		template <typename T>
		inline void SafeRelease(T*& instance)
		{
			if (instance != nullptr)
			{
				instance->Release();
				instance = nullptr;
			}
		}
	}
}