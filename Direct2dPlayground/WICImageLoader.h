#pragma once
#include <wincodec.h>
#include <wrl.h>
#include <d2d1_1.h>
#include <string>
#include "ThrowIfFailed.h"
#include "ComUser.h"

#pragma comment(lib,"windowscodecs")

namespace Direct2dHelpers
{
	class WICImageLoader {
	public:
		WICImageLoader();
		Microsoft::WRL::ComPtr<ID2D1Bitmap> LoadBitmapFromFile(ID2D1RenderTarget* renderTarget, std::wstring file, uint32_t width, uint32_t height);

	private:
		ComUser com;
		Microsoft::WRL::ComPtr<IWICImagingFactory> wicFactory;

	};
};

