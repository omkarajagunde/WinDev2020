#include<windows.h>
#include "ClassFactoryDllServerWithRegFile.h"

class CSumSubtract :public ISum, ISubtract {
private:
	long m_cRef;
public:

	// constructor for CSumSubtract
	CSumSubtract(void);
	// destructor for CSumSubtract
	~CSumSubtract(void);

	// IUnknown specific methods declarations
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// ISum specific methods declarations
	HRESULT __stdcall SumOfTwoIntegers(int, int, int*);

	// ISubtract specific method declarations
	HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int*);

};


class CSumSubtractClassFactory :public IClassFactory {

private:
	long m_cRef;
public:

	// constructor of CSumSubtractClassFactory
	CSumSubtractClassFactory(void);

	// destructor of CSumSubtractClassFactory
	~CSumSubtractClassFactory(void);

	// IUnknown specific method declarations
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// IClassFactory specific method declarations
	HRESULT __stdcall CreateInstance(IUnknown*, REFIID, void**);
	HRESULT __stdcall LockServer(BOOL);
};

// global varaibles declarations
long glNumberOfActiveComponents = 0;	// number of active components
long glNumberOfServerLocks = 0;			// number of locks on this DLL

// DllMain
BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID Reserved) {

	//code
	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	
	}

	return (TRUE);
}

// CSumSubtract constructor() implementation
CSumSubtract::CSumSubtract(void) {
	//code
	m_cRef = 1;		// hardcoded initialization to anticipate the failure of QueryInterface()
	InterlockedIncrement(&glNumberOfActiveComponents);	// incrementing the global counter
}


// CSumSubtract destructor() implementation
CSumSubtract::~CSumSubtract(void) {

	//code
	InterlockedDecrement(&glNumberOfActiveComponents);	//decrementing global counter
}

// CSumSubtract's IUnknown implmentation
HRESULT CSumSubtract::QueryInterface(REFIID riid, void** ppv) {

	//code
	if (riid == IID_IUnknown)
		*ppv = static_cast<ISum*>(this);
	else if (riid == IID_ISum)
		*ppv = static_cast<ISum*>(this);
	else if (riid == IID_ISubtract)
		*ppv = static_cast<ISubtract*>(this);
	else {
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return(S_OK);
}

ULONG CSumSubtract::AddRef(void) {

	//code
	InterlockedIncrement(&m_cRef);
	return (m_cRef);
}

ULONG CSumSubtract::Release(void) {

	//code
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0) {
		delete(this);
		return 0;
	}
	return (m_cRef);
}


// Implementation of ISum methods
HRESULT CSumSubtract::SumOfTwoIntegers(int a, int b, int* pSum) {

	//code
	*pSum = a + b;
	return (S_OK);
}

// Implementation of ISubtract methods
HRESULT CSumSubtract::SubtractionOfTwoIntegers(int a, int b, int* pSubtract) {

	//code
	*pSubtract = a - b;		// assuming a is greater than b
	return(S_OK);
}

// Implementation of CSumSubtractClassFactory's constructor
CSumSubtractClassFactory::CSumSubtractClassFactory(void) {

	//code
	m_cRef = 1;				// hardcoded initialisation to anticipate possible failure of QueryInterface()
}

// Implementation of CSumSubtractClassFactory's destructor
CSumSubtractClassFactory::~CSumSubtractClassFactory(void) {

	//code
}

// Implementation of CSumSubtractClassFactory's IUnknown methods
HRESULT CSumSubtractClassFactory::QueryInterface(REFIID riid, void** ppv) {

	//code
	if (riid == IID_IUnknown)
		*ppv = static_cast<IClassFactory*>(this);
	else if (riid == IID_IClassFactory)
		*ppv = static_cast<IClassFactory*>(this);
	else {
		*ppv = NULL;
		return(E_NOINTERFACE);
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return(S_OK);
}


ULONG CSumSubtractClassFactory::AddRef(void) {

	//code
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CSumSubtractClassFactory::Release(void) {

	//code
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0) {
		delete(this);
		return (0);
	}
	return (m_cRef);
}

// Implementation of CSumSubtractClassFactory's IClassFactory's Methods
HRESULT CSumSubtractClassFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv) {

	// variable declarations
	CSumSubtract* pCSumSubtract = NULL;
	HRESULT hr;

	// code
	if (pUnkOuter != NULL) {
		return (CLASS_E_NOAGGREGATION);
	}

	// create instance of component i.e. CSumSubtract class
	pCSumSubtract = new CSumSubtract;
	if (pCSumSubtract == NULL)
		return (E_OUTOFMEMORY);

	// get requested interface
	hr = pCSumSubtract->QueryInterface(riid, ppv);
	pCSumSubtract->Release();
	return (hr);
}

HRESULT CSumSubtractClassFactory::LockServer(BOOL fLock) {

	//code
	if (fLock)
		InterlockedIncrement(&glNumberOfServerLocks);
	else
		InterlockedDecrement(&glNumberOfServerLocks);

	return S_OK;
}

// Implmentation of Exported Functions from DLL
extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv) {

	// variable declarations
	CSumSubtractClassFactory* pCSumSubtractClassFactory = NULL;
	HRESULT hr;

	//code
	if (rclsid != CLSID_SumSubtract)
		return (CLASS_E_CLASSNOTAVAILABLE);

	// create class factory
	pCSumSubtractClassFactory = new CSumSubtractClassFactory;
	if (pCSumSubtractClassFactory == NULL) {
		return (E_OUTOFMEMORY);
	}

	hr = pCSumSubtractClassFactory->QueryInterface(riid, ppv);
	pCSumSubtractClassFactory->Release();			// anticipate possible failure of QueryInterface()
	return (hr);

}

extern "C" HRESULT __stdcall DllCanUnloadNow(void) {

	//code
	if ((glNumberOfActiveComponents == 0) && (glNumberOfActiveComponents == 0))
		return (S_OK);
	else
		return (S_FALSE);
}
































































