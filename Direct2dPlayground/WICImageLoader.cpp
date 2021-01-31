#include "WICImageLoader.h"

Direct2dHelpers::WICImageLoader::WICImageLoader() {
	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(wicFactory.GetAddressOf()));
}

Microsoft::WRL::ComPtr<ID2D1Bitmap> Direct2dHelpers::WICImageLoader::LoadBitmapFromFile(ID2D1RenderTarget* renderTarget, std::wstring file, uint32_t width, uint32_t height) {
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
	HRESULT hr = wicFactory->CreateDecoderFromFilename(file.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, decoder.GetAddressOf());
	DX::ThrowIfFailed(hr);

	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frameDecoder;
	hr = decoder->GetFrame(0, frameDecoder.GetAddressOf());
	DX::ThrowIfFailed(hr);

	Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
	hr = wicFactory->CreateFormatConverter(converter.GetAddressOf());
	DX::ThrowIfFailed(hr);
	hr = converter->Initialize(frameDecoder.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeMedianCut);
	DX::ThrowIfFailed(hr);
	
	Microsoft::WRL::ComPtr<IWICBitmapScaler> scaler;
	hr = wicFactory->CreateBitmapScaler(scaler.GetAddressOf());
	DX::ThrowIfFailed(hr);
	hr = scaler->Initialize(converter.Get(), width, height, WICBitmapInterpolationModeFant);
	DX::ThrowIfFailed(hr);

	Microsoft::WRL::ComPtr<ID2D1Bitmap> resultBitmap;
	hr = renderTarget->CreateBitmapFromWicBitmap(scaler.Get(), resultBitmap.GetAddressOf());
	DX::ThrowIfFailed(hr);

	return resultBitmap;
}
