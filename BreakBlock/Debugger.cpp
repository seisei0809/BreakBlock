#include "AllSetting.h"
#include "Debugger.h"

void Debugger::EnableDebugLayer() {

	try
	{
		ID3D12Debug* debugLayer = nullptr;
		auto result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer));
		debugLayer->EnableDebugLayer(); // デバッグレイヤーを有効化する
		debugLayer->Release();
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("デバッグレイヤーの有効化で異常エラー:") + STR(e.what()) + STR(":"));
	}
}

void Debugger::ShowHRESULT(HRESULT hr, const WSTR& caption)
{
	std::wstringstream ss;
	ss << L"HRESULT = 0x" << std::hex << std::uppercase << hr;
	MessageBoxW(nullptr, ss.str().c_str(), caption.c_str(), MB_OK | MB_ICONINFORMATION);
}

void Debugger::CheckHR(HRESULT hr, STR msg)
{
	if (FAILED(hr))
	{
		std::stringstream ss;
		ss << msg << ":HRESULT = 0x"
			<< std::hex << std::uppercase << hr;
		throw std::runtime_error(ss.str());
	}
};
