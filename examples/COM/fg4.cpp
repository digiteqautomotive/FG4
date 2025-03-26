#include <Windows.h>
#include <uuids.h>
#include <strmif.h>
#include <stdio.h>
#include <stdlib.h>
#include "fg4.h"

#define FG4_INPUTS  2
#define FG4_OUTPUTS 2

/*
	Iterate all DirectShow devices and return id-th FG4 input device handle
*/
static IFG4InputConfig* inputConfig(int id)
{
	IFG4InputConfig* piConfig = NULL;
	HRESULT hr;
	ICreateDevEnum* piCreateDevEnum;

	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, __uuidof(piCreateDevEnum), reinterpret_cast<void**>(&piCreateDevEnum));
	if (SUCCEEDED(hr)) {
		int Xorder = id;
		IEnumMoniker* piEnumMoniker;

		hr = piCreateDevEnum->CreateClassEnumerator(AM_KSCATEGORY_CAPTURE, &piEnumMoniker, 0);
		piCreateDevEnum->Release();

		if (SUCCEEDED(hr))
			hr = piEnumMoniker->Reset();

		if (SUCCEEDED(hr)) {
			ULONG cFetched;
			IMoniker* piMoniker;

			while ((hr = piEnumMoniker->Next(1, &piMoniker, &cFetched)) == S_OK) {
				IBaseFilter* piFilter;

				hr = piMoniker->BindToObject(NULL, NULL, __uuidof(piFilter), reinterpret_cast<void**>(&piFilter));
				if (SUCCEEDED(hr)) {
					hr = piFilter->QueryInterface(__uuidof(piConfig), reinterpret_cast<void**>(&piConfig));
					if (FAILED(hr))
						piFilter->Release();
					else {
						if (Xorder > 0) {
							Xorder--;
							piFilter->Release();
							piConfig->Release();
						}
						else {
							piFilter->Release();
							piMoniker->Release();
							piEnumMoniker->Release();

							return piConfig;
						}
					}
				}
				piMoniker->Release();
			}
		}
		piEnumMoniker->Release();
	}

	return 0;
}

/*
	Iterate all DirectShow devices and return id-th FG4 output device handle
*/
static IFG4OutputConfig* outputConfig(int id)
{
	IFG4OutputConfig* piConfig = NULL;
	HRESULT hr;
	ICreateDevEnum* piCreateDevEnum;

	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, __uuidof(piCreateDevEnum), reinterpret_cast<void**>(&piCreateDevEnum));
	if (SUCCEEDED(hr)) {
		int Xorder = id;
		IEnumMoniker* piEnumMoniker;

		hr = piCreateDevEnum->CreateClassEnumerator(AM_KSCATEGORY_RENDER, &piEnumMoniker, 0);
		piCreateDevEnum->Release();

		if (SUCCEEDED(hr))
			hr = piEnumMoniker->Reset();

		if (SUCCEEDED(hr)) {
			ULONG cFetched;
			IMoniker* piMoniker;

			while ((hr = piEnumMoniker->Next(1, &piMoniker, &cFetched)) == S_OK) {
				IBaseFilter* piFilter;

				hr = piMoniker->BindToObject(NULL, NULL, __uuidof(piFilter), reinterpret_cast<void**>(&piFilter));
				if (SUCCEEDED(hr)) {
					hr = piFilter->QueryInterface(__uuidof(piConfig), reinterpret_cast<void**>(&piConfig));
					if (FAILED(hr))
						piFilter->Release();
					else {
						if (Xorder > 0) {
							Xorder--;
							piFilter->Release();
							piConfig->Release();
						}
						else {
							piFilter->Release();
							piMoniker->Release();
							piEnumMoniker->Release();

							return piConfig;
						}
					}
				}
				piMoniker->Release();
			}
		}
		piEnumMoniker->Release();
	}

	return 0;
}

int main(int argc, char* argv[])
{
	long resolution, id;
	IFG4InputConfig* incfg;
	IFG4OutputConfig* outcfg;

	// Initialize COM
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// Get input resolutions
	for (int i = 0; i < FG4_INPUTS; i++) {
		if ((incfg = inputConfig(i))) {
			incfg->GetDetectedResolution(&resolution);
			incfg->GetChannel(&id);
			incfg->Release();
			printf("[%d]  IN: %dx%d\n", id, resolution >> 16, resolution & 0xFFFF);
		}
	}

	// Get output resolutions
	for (int i = 0; i < FG4_OUTPUTS; i++) {
		if ((outcfg = outputConfig(i))) {
			outcfg->GetResolution(&resolution);
			outcfg->GetChannel(&id);
			outcfg->Release();
			printf("[%d] OUT: %dx%d\n", id, resolution >> 16, resolution & 0xFFFF);
		}
	}

	CoUninitialize();

	return 0;
}
