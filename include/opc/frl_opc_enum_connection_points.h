#ifndef frl_opc_enum_connection_points_h_
#define frl_opc_enum_connection_points_h_
#include "frl_platform.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include <OCIdl.h>
#include <vector>
#include "opc/frl_opc_com_allocator.h"

namespace frl
{
	namespace opc
	{
		class ConnectionPoint;
		class EnumConnectionPoints : public IEnumConnectionPoints, public ComAllocator
		{
		private:
		private:
			volatile LONG refCount;
			size_t currentIndex;	//Current element
			std::vector< ConnectionPoint* > points;
		public:
			
			EnumConnectionPoints();
			EnumConnectionPoints( const std::vector< ConnectionPoint* > &pointsList );
			virtual ~EnumConnectionPoints();

			// IUnknown implementation
			STDMETHODIMP QueryInterface( REFIID iid, LPVOID* ppInterface);
			STDMETHODIMP_(ULONG) AddRef( void);
			STDMETHODIMP_(ULONG) Release( void);

			// IEnumConnectionPoints implementation
			HRESULT STDMETHODCALLTYPE Next( 
				/* [in] */ ULONG cConnections,
				/* [length_is][size_is][out] */ LPCONNECTIONPOINT *ppCP,
				/* [out] */ ULONG *pcFetched);

			HRESULT STDMETHODCALLTYPE Skip( 
				/* [in] */ ULONG cConnections);

			HRESULT STDMETHODCALLTYPE Reset( void);

			HRESULT STDMETHODCALLTYPE Clone( 
				/* [out] */ IEnumConnectionPoints **ppEnum);
		};
	} // namespace opc
} // namespace FatRat Library

#endif // FRL_PLATFORM_WIN32
#endif // frl_opc_enum_connection_points_h_
