class ISum :public IUnknown {

public:
	virtual HRESULT __stdcall SumOfTwoIntegers(int, int, int*) = 0;
};

class ISubtract : public IUnknown {

public:
	virtual HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int*) = 0;
};

// CLSID for SumSubtract Component {E9FBA5B5-D284-4130-838F-00D566B9CB8A}
static const CLSID CLSID_SumSubtract=
{ 0xe9fba5b5, 0xd284, 0x4130, 0x83, 0x8f, 0x0, 0xd5, 0x66, 0xb9, 0xcb, 0x8a};

// IID of ISum
static const IID IID_ISum=
{ 0x9ef06ee9, 0xd75c, 0x4041, 0xbd, 0x51, 0x7d, 0x38, 0x3f, 0x5a, 0x47, 0x6f };

// IID od ISubtract
static const IID IID_ISubtract =
{ 0x49e6411, 0x323f, 0x47c4, 0x87, 0x68, 0xd0, 0x28, 0x2e, 0x3f, 0xf1, 0x83 };